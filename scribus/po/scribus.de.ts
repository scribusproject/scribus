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
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Über Scribus%1%2</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation>%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Scribus Version %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Build-ID:</translation>
    </message>
    <message>
        <source>About</source>
        <translation type="obsolete">Über</translation>
    </message>
    <message>
        <source>Programming:</source>
        <translation type="obsolete">Programmierung:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Beiträge von:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Windows-Portierung:</translation>
    </message>
    <message>
        <source>Documentation:</source>
        <translation type="obsolete">Dokumentation:</translation>
    </message>
    <message>
        <source>Authors</source>
        <translation type="obsolete">Autoren</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Deutsch:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Französisch:</translation>
    </message>
    <message>
        <source>Spanish and Catalan:</source>
        <translation type="obsolete">Spanisch und Katalanisch:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italienisch:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Ungarisch:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ukrainisch:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Bulgarisch:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galizisch:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Türkisch:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Litauisch:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Polnisch:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Tschechisch:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Slowakisch:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Dänisch:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norwegisch:</translation>
    </message>
    <message>
        <source>English:</source>
        <translation type="obsolete">Englisch:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Walisisch:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Russisch:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Brasilianisch:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Finnisch:</translation>
    </message>
    <message>
        <source>Translations</source>
        <translation type="obsolete">Übersetzungen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>March</source>
        <translation type="obsolete">März</translation>
    </message>
    <message>
        <source>Homepage and online reference</source>
        <translation type="obsolete">Homepage und Online Referenz</translation>
    </message>
    <message>
        <source>Mailing list</source>
        <translation type="obsolete">Mailing Liste</translation>
    </message>
    <message>
        <source>Bugs and feature requests</source>
        <translation type="obsolete">Fehlerberichte und Wünsche</translation>
    </message>
    <message>
        <source>Online</source>
        <translation type="obsolete">Internet</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Slovenisch:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Baskisch:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Dieser Dialog zeigt die Version, das Kompilierungsdatum und
die unterstützten Bibliotheken in Scribus an
Die Zeichen stehen für C=CUPS, C=LittleCMS und T=TIFF.
Fehlende Unterstützung wird duch ein Sternchen angezeigt</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>Ü&amp;ber</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utoren</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>Über&amp;setzungen</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Online</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation>Schwedisch:</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Entwickler-Team:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Offizielle Dokumentation:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Andere Dokumentationen:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation>Englisch (Britisch):</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Homepage</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Onlinereferenz</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Fehler und Featurewünsche</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Mailingliste</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation>Katalanisch:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>Koreanisch:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>Spanisch:</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Offizielle Übersetzungen und Übersetzer:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>Serbisch:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Ehemalige Übersetzer:</translation>
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
        <translation>Weitere Optionen</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontal</source>
        <translation type="obsolete">Seite(n) horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertical</source>
        <translation type="obsolete">Seite(n) vertikal spiegeln</translation>
    </message>
    <message>
        <source>Apply ICC-Profiles</source>
        <translation type="obsolete">ICC-Profile anwenden</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation type="obsolete">PostScript-Level 3</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation type="obsolete">PostScript-Level 2</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation type="obsolete">PostScript-Level 1</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Erstellt eine PostScript-Level 3 Datei</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Erstellt eine PostScript-Level 2 Datei. Vorsicht:
hierbei können sehr große Dateien entstehen</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Erstellt eine PostScript-Level 1 Datei. Vorsicht:
hierbei können sehr große Dateien entstehen</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Seiten &amp;horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Seiten &amp;vertikal spiegeln</translation>
    </message>
    <message>
        <source>Appy Under Color &amp;Removal</source>
        <translation type="obsolete">Unterfarben&amp;reduktion anwenden</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>&amp;ICC-Profile anwenden</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript-Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript-Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript-Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Unterfarben&amp;reduktion durchführen</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Mit dieser Option werden ein paar Grautöne, die entstehen,
wenn Black aus Cyan, Magenta und Yellow gemischt wird, durch Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr nahe stehen. Diese Option kann zu besseren Bildern führen,
allerdings müssen Sie von Fall zu Fall entscheiden, wie Sie bessere Ergebnisse
bekommen. Außerdem reduziert UFR die Gefahr einer Übersättigung mit CMY.
</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Abstand/Ausrichtung</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Ausrichten</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Waagrecht</translation>
    </message>
    <message>
        <source>between:</source>
        <translation type="obsolete">zwischen:</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Linken Kanten</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Mitten</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Rechten Kanten</translation>
    </message>
    <message>
        <source>Don&apos;t change</source>
        <translation type="obsolete">Nicht ändern</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation type="obsolete">Versatz</translation>
    </message>
    <message>
        <source>Distribute evenly</source>
        <translation type="obsolete">Gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Oberen Kanten</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Unteren Kanten</translation>
    </message>
    <message>
        <source> pts</source>
        <translation type="obsolete">pts</translation>
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
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="obsolete">Anwenden</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>An&amp;wenden</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;zwischen:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>&amp;Ausrichten</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>&amp;Versatz</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>&amp;Gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>zwi&amp;schen:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>&amp;Nicht verändern</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>A&amp;usrichten</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Ve&amp;rsatz</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Gleich&amp;mäßig verteilen</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>N&amp;icht verändern</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Linksbündig</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Rechtsbündig</translation>
    </message>
    <message>
        <source>Align Text Block</source>
        <translation type="obsolete">Blocksatz</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Zentriert</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Blocksatz</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Erzwungener Blocksatz</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Feldeigenschaften</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Hilfstext:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Font for use with PDF-1.3:</source>
        <translation type="obsolete">Schriftart für PDF-1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Rand</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farbe:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Breite:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Schmal</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Breit</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Durchgehend</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Gestrichelt</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Hervorgehoben</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Vertieft</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andere</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Nur Lesen</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Erforderlich</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Wert nicht exportieren</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Darstellung:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Sichtbar</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Versteckt</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Nicht Drucken</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Nicht Sichtbar</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Aussehen</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Text für Button Down</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Text für Roll Over</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Icons</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Icons benutzen</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Gedrückt</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Roll Over</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Anordnung...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Hervorhebung</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertieren</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Umrandung</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Gedrückt</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Mehrzeilig</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Maximum von</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Nicht scrollen</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Nicht in Rechtschreibprüfung einbeziehen</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Art des Häkchens:</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Voreinstellung ist angekreuzt</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Änderbar</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Ereignis:</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Bearbeiten...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Sende an URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Sende Daten als HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importiere Daten von:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Ziel</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>In Datei:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Ändern...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Seite:</translation>
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
        <translation>Aktion</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Feld ist formatiert als:</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Zahlenformat</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Dezimalstellen:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Währungssymbol benutzen</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Währungssymbol voranstellen</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatierung</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Prozent-Format</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datums-Format</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Zeit-Format</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Tastendruck:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Wert wird nicht überprüft</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Wert muss größer oder gleich sein als:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>und kleiner oder gleich als:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Eigenes Überprüfungsscript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Überprüfen</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Wert wird nicht berechnet</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Wert ist</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>die Summe</translation>
    </message>
    <message>
        <source>product</source>
        <translation>das Produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation>der Durchschnitt</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>das Minimum</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>das Maximum</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>der folgenden Felder:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Auswählen...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Eigenes Berechnungs-Script:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Berechnen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF-1.3</source>
        <translation type="obsolete">In PDF-1.3 nicht benutzbar</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Hier eine komma-separierte Liste der Felder eintragen</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Sie benötigen mindestens das Icon für Normal um Icons zu benutzen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Bilder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Beispiel:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Auswahl geändert</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF-Dateien (*.pdf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Schaltfläche</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textfeld</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kontrollkästchen</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinationsfeld</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listenfeld</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Haken</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kreuz</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Raute</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Kreis</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Stern</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Quadrat</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gehe zu</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Formular senden</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Formular zurücksetzen</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Daten importieren</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Maustaste loslassen</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Maustaste drücken</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mauszeiger berührt Feld</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mauszeiger verlässt Feld</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Feld hat Fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Feld verliert Fokus</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Nichts</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Zahl</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Prozent</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Zeit</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Schriftart für PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Wird von PDF 1.3 ignoriert</translation>
    </message>
    <message>
        <source>PDF Documents (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF-Dateien (*.pdf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*.*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Eigenschaften der Anmerkung</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="obsolete">Typ:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Verknüpfung</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Externe Verknüpfung</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Externe Web-Verknüpfung</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Ziel</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ändern...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="obsolete">Seite:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Pos:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-Pos:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Seite:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Position</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Musterseite anwenden</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation type="obsolete">Musterseite:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Apply to Current Page</source>
        <translation type="obsolete">Auf aktuelle Seite anwenden</translation>
    </message>
    <message>
        <source>Apply from Page:</source>
        <translation type="obsolete">Anwenden von Seite:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">bis:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Vorlage:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Auf &amp;aktuelle Seite anwenden</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Anwenden von &amp;Seite:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Auf alle &amp;geraden Seiten anwenden</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Auf alle &amp;ungeraden Seiten anwenden</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Bibliothek</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Speichern</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Speichern unter...</translation>
    </message>
    <message>
        <source>Load...</source>
        <translation type="obsolete">Laden...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">Klein</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Mittel</translation>
    </message>
    <message>
        <source>Big</source>
        <translation type="obsolete">Groß</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Datei</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Vorschau</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Bibliotheken (*.scs);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Name:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Neuer Eintrag</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Umbenennen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Laden...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Klein</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Mittel</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Groß</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>Vor&amp;schau</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Lesezeichen</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Lesezeichen verschieben</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Lesezeichen einfügen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Lesezeichen</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Anordnung:</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skaliere:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Immer</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>bei zu kleinem Icon</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>bei zu großem Icon</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skalierungsart:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proportional</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Unproportional</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Icon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Zurücksetzen</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Nur Text</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Nur Icon</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Text unter dem Icon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Text über dem Icon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Text rechts vom Icon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Text links vom Icon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Text überlagert Icon</translation>
    </message>
    <message>
        <source>When Icon it too small</source>
        <translation type="obsolete">bei zu kleinem Icon</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation>Farbmanagement Einstellungen</translation>
    </message>
    <message>
        <source>Activate Color Management</source>
        <translation type="obsolete">Farbmanagement aktivieren</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Farbprofile</translation>
    </message>
    <message>
        <source>Pictures:</source>
        <translation type="obsolete">Bilder:</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Farben:</translation>
    </message>
    <message>
        <source>Monitor:</source>
        <translation type="obsolete">Monitor:</translation>
    </message>
    <message>
        <source>Printer:</source>
        <translation type="obsolete">Drucker:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Render-Prioritäten</translation>
    </message>
    <message>
        <source>Simulate Printer on the Screen</source>
        <translation type="obsolete">Drucker auf Bildschirm simulieren</translation>
    </message>
    <message>
        <source>Mark Colors out of Gamut</source>
        <translation type="obsolete">Farben außerhalb des Farbbereichs markieren</translation>
    </message>
    <message>
        <source>Use Blackpoint Compensation</source>
        <translation type="obsolete">Tiefenkompensierung benutzen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ Farbmetrisch</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Farbsättigung</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut farbmetrisch</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Farbprofil für Bilder</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Farbprofil für Objektfarben</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Farbprofil für den Monitor.</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Farbprofil für den Drucker.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Tiefenkompensierung ist eine Methode zur Verbesserung des Kontrasts.
Diese Option sollte aktiviert sein, wenn Sie Fotos im Dokument haben.</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard-Rendering Methode für Ihren Monitor. Ohne einen Grund zur Veränderung
sollte entweder relativ farbmetrisch oder Wahrnehmung aktiviert sein.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard-Rendering Methode für den Drucker. Ohne einen Grund zur Veränderung
sollte entweder relativ farbmetrisch oder Wahrnehmung aktiviert sein.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Aktiviert die Darstellung der Farben basierend auf dem 
gewählten Druckerprofil.
</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Aktiviert die Darstellung der Farben, die mit dem aktuellen Profil ungenau wiedergegeben werden.
Das erfordert sehr genaue Profile und dient nur als Anhaltspunkt.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Farb-Management &amp;aktivieren</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Bilder:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Füllfarben:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>Moni&amp;tor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Drucker:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>Moni&amp;tor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;Drucker:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Druckerfarben auf dem Bildschirm &amp;simulieren</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Farben außerhalb des Farbbereichs &amp;markieren</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>&amp;Tiefenkompensierung benutzen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Farbe bearbeiten</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Name:</translation>
    </message>
    <message>
        <source>Color Model</source>
        <translation type="obsolete">Farbmodell</translation>
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
        <translation>Web Farben</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Neu</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Alt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
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
        <translation>Dynamische Farbregler</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statische Farbregler</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Der Name der Farbe ist nicht eindeutig</translation>
    </message>
    <message>
        <source>HSB-Colormap</source>
        <translation type="obsolete">HSB-Farbwähler</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-Farbwähler</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Farb&amp;modell</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Sie können keine Farbe mit dem Namen &quot;%1&quot;.
Dieser Name ist für die transparente Farbe reserviert</translation>
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
        <translation>Waagrechter Verlauf</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Senkrechter Verlauf</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonaler Verlauf</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Umgekehrt Diagonaler Verlauf</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Kreisförmiger Verlauf</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Deckkraft:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Tonwert:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Linienfarbe bearbeiten</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Füllfarbe bearbeiten</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Tonwert der Farbe</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Füllmethode</translation>
    </message>
    <message>
        <source>Edit the first color of object</source>
        <translation type="obsolete">Erste Verlaufsfarbe auswählen</translation>
    </message>
    <message>
        <source>Edit the second color of object</source>
        <translation type="obsolete">Zweite Verlaufsfarbe auswählen</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Transparenz für Farbe auswählen</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Farbe des selektierten Objekts</translation>
    </message>
    <message>
        <source>Special</source>
        <translation type="obsolete">Spezialattribut</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Benutzerdefinierter linearer Verlauf</translation>
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
    <message>
        <source>Free radial Gradient</source>
        <translation>Benutzerdefinierter radialer Verlauf</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Optionen für CSV-Importer</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Feldtrenner:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Werttrenner:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keiner</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Erste Zeile ist Kopfzeile</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Druckereinstellungen (CUPS)</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Einstellung</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Wert</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Welche Seiten</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle Seiten</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Nur gerade Seiten</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Nur ungerade Seiten</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Spiegeln</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Ausrichtung</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Seiten zusammenfassen</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Seite pro Blatt</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Seiten pro Blatt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Dieser Dialog zeigt verschiedene CUPS-Optionen zum Drucken an.
Die verfügbaren Einstellungen sind von Ihrem Drucker anhängig.
Hlife -&gt; Über zeigt Ihnen den CUPS-Support an.
Dabei stehen die Zeichen für C=CUPS, C-LittleCMS und T=TIFF.
Fehlende Bibliotheken werden durch ein Sternchen angezeigt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Compress File</source>
        <translation type="obsolete">Datei komprimieren</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodierung:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Geht zum Dokumenten Verzeichnis.
Wird in den Voreinstellungen eingestellt.</translation>
    </message>
    <message>
        <source>Include Fonts</source>
        <translation type="obsolete">Schriftarten einbetten</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>Datei &amp;komprimieren</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>Schriftarten ein&amp;betten</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Farbe löschen</translation>
    </message>
    <message>
        <source>OK to delete Color:</source>
        <translation type="obsolete">Löschen der Farbe:</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Ersetzen durch:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Farbe löschen:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Seiten löschen</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Löschen von:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation>Fehlende Schrift</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Die Schrift %1 ist nicht installiert.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Benutze</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>anstatt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Info</source>
        <translation type="obsolete">Information</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation type="obsolete">Autor:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation type="obsolete">Titel:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="obsolete">Beschreibung:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <translation type="obsolete">Schlüsselwörter:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Dieses Feld kann als Referenz in das Dokument eingebettet werden oder in den Meta-Daten der PDF&apos;s enthalten sein</translation>
    </message>
    <message>
        <source>This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Dieses Feld ist für eine kurze Beschreibung oder Zusammenfassung des Dokuments gedacht, sie wird beim PDF-Export mit in die PDF-Datei eingebunden</translation>
    </message>
    <message>
        <source>This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">In dieses Feld können Sie Stichworte eingeben, die sie in die PDF-Datei übernehmen wollen - das erleichtert das Suchen und Indizieren der PDF-Dateien</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformationen</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Stichworte:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Beschrei&amp;bung:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Herausgeber:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Mitarbeiter:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datum:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Format:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Identifikation:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Quelle:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Beziehung:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Gültigkeitsbereich:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Rechte:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Weitere &amp;Informationen</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Die Person oder Organisation, die vorrangig verantwortlich für den Inhalt des Dokuments ist.
Diese Feld kann sowohl in das Scribus-Dokument als auch in die Meta-Daten einer PDF-Datei eingebettet werden</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Ein Name für das Dokment
Diese Feld kann sowohl in das Scribus-Dokument als auch in die Meta-Daten einer PDF-Datei eingebettet werden</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Ein Abriss des Inhalts.
Hier kann eine kurze Beschreibung oder Zusammenfassung stehen. Sie wird in die PDF-Datei exportiert</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Worte, die den Inhalt des Dokuments beschreiben.
Der Inhalt dieses Feldes wird in die PDF-Datei exportiert und hilft Ihnen, PDF-Dateien zu indizieren und und wiederzufinden</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Eine Person oder Organisation, die für die Veröffentlichung des Dokuments verantwortlich ist</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Eine Person oder Organisation, die bei dem Dokument mitgearbeitet hat</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Ein Datum, das mit der Entstehung des Dokuments verbunden ist, nach ISO 8601 im Format YYYY-MM-DD </translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Die Gattung oder der Typ des Dokuments, z.B. Kategorien, Funktionen, Arten usw</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Die physische oder digitale Veröffentlichungsform des Dokuments. Abmessung und Medien-Typ können hier notiert werden.
Für MIME-Typen sind auch RFC2045 und RFC2046 gebräuchlich</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Eine eindeutige Referenz zu dem Dokument in einem gegebenen Kontext wie ISBN oder URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISDN or URI</source>
        <translation type="obsolete">Eine Referenz zu einem Dokument, von dem sich das aktuelle Dokument ableitet, z.B. ISBN oder URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Die Sprache , in der der Inhalt des Dokuments geschrieben ist, normalerweise ein ISO-639 Sprachcode,
optional ergänzt durch ein Bindestrich und den ISO-3166 Ländercode, wie z.B. en-GB oder fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Eine Referenz zu einem ähnlichen Dokument, wenn möglich eine formale Identifikation wie ISBN oder URI benutzen</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Der Ausbreitungsbereich des Dokuments, wenn möglich mit Ort, Zeit und Gerichtsbarkeit</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informationen über Rechte in dem oder über das Dokument, z.B. Copyright, Patente oder Handelsmarken</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Eine Referenz zu einem Dokument, von dem sich das aktuelle Dokument ableitet, z.B. ISBN oder URI</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Drucker einrichten</translation>
    </message>
    <message>
        <source>Print destination</source>
        <translation type="obsolete">Ziel</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Options...</source>
        <translation type="obsolete">Optionen...</translation>
    </message>
    <message>
        <source>File:</source>
        <translation type="obsolete">Datei:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ändern...</translation>
    </message>
    <message>
        <source>Alternative Printer Command</source>
        <translation type="obsolete">Anderer Druckbefehl</translation>
    </message>
    <message>
        <source>Command:</source>
        <translation type="obsolete">Befehl:</translation>
    </message>
    <message>
        <source>Range:</source>
        <translation type="obsolete">Bereich:</translation>
    </message>
    <message>
        <source>To page:</source>
        <translation type="obsolete">bis Seite:</translation>
    </message>
    <message>
        <source>From page:</source>
        <translation type="obsolete">Von Seite:</translation>
    </message>
    <message>
        <source>Print range</source>
        <translation type="obsolete">Druckbereich</translation>
    </message>
    <message>
        <source>Print all</source>
        <translation type="obsolete">Alles drucken</translation>
    </message>
    <message>
        <source>Print last page first</source>
        <translation type="obsolete">Drucke letzte Seite zuerst</translation>
    </message>
    <message>
        <source>Print first page first</source>
        <translation type="obsolete">Drucke erste Seite zuerst</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation type="obsolete">Anzahl der Kopien:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation type="obsolete">Normal drucken</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation type="obsolete">Farbauszüge drucken</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Print in color if available</source>
        <translation type="obsolete">In Farbe drucken, wenn verfügbar</translation>
    </message>
    <message>
        <source>Print in grayscale</source>
        <translation type="obsolete">In Graustufen drucken</translation>
    </message>
    <message>
        <source>Advanced Options...</source>
        <translation type="obsolete">Weitere Optionen...</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Postscript-Dateien (*.ps);;Alle Dateien (*)</translation>
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
        <translation>Gelb</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Schwarz</translation>
    </message>
    <message>
        <source>Print</source>
        <translation type="obsolete">Drucken</translation>
    </message>
    <message>
        <source>Print current page</source>
        <translation type="obsolete">Aktuelle Seite drucken</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Geben sie durch Kommata getrennt ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Druckerwahl</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Optionen...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Datei:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Al&amp;ternativer Druckbefehl</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Befehl:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Bereich</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>&amp;Alles drucken</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Aktue&amp;lle Seite drucken</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Be&amp;reich drucken</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Anzahl der &amp;Kopien:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>&amp;Normal drucken</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>&amp;Farbauszüge drucken</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>In Fa&amp;rbe drucken, falls verfügbar</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>In &amp;Graustufen drucken</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Er&amp;weiterte Optionen...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Drucken</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Einen anderen Druckmanager benutzen, zum Beispiel kprinter oder gtklp,
um zusätzliche Optionen einstellen zu können</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Fehler beim Importieren
der Datei
%1!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fataler Fehler</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Stilvorlage bearbeiten</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Name:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Face:</source>
        <translation type="obsolete">Schriftart:</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Größe:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Effekt:</translation>
    </message>
    <message>
        <source>Alignment:</source>
        <translation type="obsolete">Ausrichtung:</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="obsolete">Füllfarbe:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation type="obsolete">Initialien</translation>
    </message>
    <message>
        <source>Lines:</source>
        <translation type="obsolete">Zeilen:</translation>
    </message>
    <message>
        <source>Indentation</source>
        <translation type="obsolete">Einrückungen</translation>
    </message>
    <message>
        <source>Left Indent:</source>
        <translation type="obsolete">Links:</translation>
    </message>
    <message>
        <source>First Line:</source>
        <translation type="obsolete">Erste Zeile:</translation>
    </message>
    <message>
        <source>Tabulators...</source>
        <translation type="obsolete">Tabulatoren...</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Senkrechte Abstände</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Zeilenabstand</translation>
    </message>
    <message>
        <source>Above:</source>
        <translation type="obsolete">Oben:</translation>
    </message>
    <message>
        <source>Below:</source>
        <translation type="obsolete">Unten:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Der Name der Stilvorlage ist nicht eindeutig</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation type="obsolete">Randfarbe:</translation>
    </message>
    <message>
        <source>Adjust to Baseline Grid</source>
        <translation type="obsolete">Ausrichten am Grundlinienraster</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Name des Absatz-Stils</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Schriftart des Objekts</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textfarbe</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Textumrissfarbe</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Stilistischer Effekt: Erzeugt einen übergroßen ersten Buchstaben in einem Absatz</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Legt die gesamte Höhe des Initials in Zeilennummern fest</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Text am Grundlinienraster ausrichten</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Abstand über dem Absatz</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Abstand unter dem Absatz</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation type="obsolete">Einzug für die erste Zeile des Absatzes</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation type="obsolete">Einzug von links für den ganzen Absatz</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation type="obsolete">Tabulatoren setzen/bearbeiten...</translation>
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
        <translation>Tabulatoren und Einzüge</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation type="obsolete">Zeilenabstand:</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>Schrift&amp;art:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Ausrichtung:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Initialien</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Zeilen:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Füll&amp;farbe:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>&amp;Randfarbe:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Text am &amp;Grundlinienraster ausrichten</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>Zeilenab&amp;stand:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>&amp;Oberhalb:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Unterhalb:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Editor</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Speichern unter...</translation>
    </message>
    <message>
        <source>Load...</source>
        <translation type="obsolete">Laden...</translation>
    </message>
    <message>
        <source>Save and Exit</source>
        <translation type="obsolete">Speichern und Beenden</translation>
    </message>
    <message>
        <source>Exit without Saving</source>
        <translation type="obsolete">Beenden ohne Speichern</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Rückgängig</translation>
    </message>
    <message>
        <source>Redo</source>
        <translation type="obsolete">Wiederherstellen</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Ausschneiden</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopieren</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Einfügen</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Get Field Names</source>
        <translation type="obsolete">Feldnamen auswählen</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Datei</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascripte (*.js);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>Ö&amp;ffnen...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>Speicher&amp;n und beenden</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>Nicht speichern und &amp;beenden</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Feldnamen auslesen</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Export Bitmap - Save As Image</source>
        <translation type="obsolete">Bitmap exportieren - Als Bild speichern</translation>
    </message>
    <message>
        <source>Export to directory:</source>
        <translation type="obsolete">Exportieren nach:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="obsolete">&amp;Wechseln...</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Bitmap type:</source>
        <translation type="obsolete">Dateiformat:</translation>
    </message>
    <message>
        <source>Quality :</source>
        <translation type="obsolete">Qualität:</translation>
    </message>
    <message>
        <source>%</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Größe:</translation>
    </message>
    <message>
        <source>Export </source>
        <translation type="obsolete">Export-Einstellungen</translation>
    </message>
    <message>
        <source>&amp;One page</source>
        <translation type="obsolete">Eine &amp;Seite</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Alle &amp;Seiten</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Interval from:</source>
        <translation type="obsolete">Seiten &amp;von:</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">bis:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Ausgabeverzeichnis wechseln</translation>
    </message>
    <message>
        <source>The output directory - the place to store your bitmaps.
Name of the eport file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation type="obsolete">Das Ausgabeverzeichnis - der Ort, an dem die Bilder gespeichert werden.
Name der Datei: &apos;NamedesDokuments-Seitenzahl.Dateiformat&apos;</translation>
    </message>
    <message>
        <source>Export only the actual page</source>
        <translation type="obsolete">Nur die aktuelle Seite exportieren</translation>
    </message>
    <message>
        <source>Export all pages of your document</source>
        <translation type="obsolete">Alles Seiten des Dokuments exportieren</translation>
    </message>
    <message>
        <source>Export only specified pages</source>
        <translation type="obsolete">Nur bestimmte Seiten exportieren</translation>
    </message>
    <message>
        <source>The beginning of the export</source>
        <translation type="obsolete">Bereich exportieren: erste Seite</translation>
    </message>
    <message>
        <source>The end of the export</source>
        <translation type="obsolete">Bereich exportieren: letzte Seite</translation>
    </message>
    <message>
        <source>The height of the output bitmap in pixels</source>
        <translation type="obsolete">Die Höhe des Bildes in Pixeln</translation>
    </message>
    <message>
        <source>The quality of your bitmaps - 100% is the best, 1% the biggest compression</source>
        <translation type="obsolete">DIe Qualität des Bildes - 100% ist die schwächste, 1% die stärkste Komprimierung</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Verfügbare Export-Formate</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Wählen Sie ein Ausgabe-Verzeichnis</translation>
    </message>
    <message>
        <source>Export as Image</source>
        <translation type="obsolete">Als Bild speichern</translation>
    </message>
    <message>
        <source>Image type:</source>
        <translation type="obsolete">Exportformat:</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Das Ausgabeverzeichnis - dort werden Ihre Bilder gespeichert.
Dateinamen der Bilder haben das Format &apos;NamedesDokuments-Seite.Dateiformat</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Nur die aktuelle Seite exportieren</translation>
    </message>
    <message>
        <source>Export all pages to images</source>
        <translation type="obsolete">Alle Seiten exportieren</translation>
    </message>
    <message>
        <source>Export a range of pages to images</source>
        <translation type="obsolete">Einen Bereich des Dokumentes exportieren</translation>
    </message>
    <message>
        <source>First page to export when exporting a range</source>
        <translation type="obsolete">Erste Seite</translation>
    </message>
    <message>
        <source>Last page to export when exporting a range</source>
        <translation type="obsolete">Letzte Seite</translation>
    </message>
    <message>
        <source>The height of the output images in pixels</source>
        <translation type="obsolete">Höhe der Bilder in Pixeln</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the highest compression</source>
        <translation type="obsolete">Bildqualität: 100% bedeutet keine, 1% höchste Komprimierung</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>In &amp;Verzeichnis exportieren:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Dateityp:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualität:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Größe:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Als Bild speichern</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Auf&amp;lösung:</translation>
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
        <translation>Bereich</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>A&amp;ktuelle Seite</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Bereich</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Eine Seitenbereich exportieren</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Geben sie durch Kommata getrennt ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Alle Seiten exportieren</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Gibt die Auflösung der Bilder an.
72 dpi sind optimal, wenn Sie die Seiten nur auf dem 
Bildschirm betrachten wollen</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Gibt die Qualität der Bilder an - 100% beste Qualität...1% schlechteste Qualität</translation>
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
        <translation>Größe:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titel:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Kein Titel</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <source>Scribus-Document</source>
        <translation type="obsolete">Scribus-Dokument</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus-Dokument</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>Append</source>
        <translation type="obsolete">Anfügen</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplizieren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Speichern</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Farbpaletten</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktive Palette:</translation>
    </message>
    <message>
        <source>Save Color Set</source>
        <translation type="obsolete">Palette speichern</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Name:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Namen wählen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>New Color:</source>
        <translation type="obsolete">Neue Farbe:</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Neue Farbe</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>Remove Unused</source>
        <translation type="obsolete">Unbenutzte löschen</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Farbpalette auswählen</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Farbpalette speichern</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Unbenutzte Farben löschen</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Farben aus anderem Dokument nachladen</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Neue Farbe anlegen</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Farbe bearbeiten</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Kopie der ausgewählen Farbe anlegen</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Ausgewählte Farbe löschen</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Aktuelle Palette zur Voreinstellung machen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>An&amp;fügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Unbenutzte &amp;entfernen</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Farbset &amp;speichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation>Globale Schrifteinstellungen</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Verfügbare Schriften</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Schrift Ersetzungen</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Zusätzliche Pfade</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Schrift Name</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Ersatz</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ändern...</translation>
    </message>
    <message>
        <source>Add...</source>
        <translation type="obsolete">Hinzufügen...</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="obsolete">Entfernen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Benutzen</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Einbetten in:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Unterteilen</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Pfad zur Schriftdatei</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Verfügbare &amp;Schriftarten</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Schriftarten-&amp;Ersetzung</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Zusätzliche &amp;Pfade</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Hinzufügen...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Font Preview</source>
        <translation type="obsolete">Vorschau für Schriftarten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>The quick brown fox jumps over the lazy dog</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Schriftartenvorschau</translation>
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
        <translation>Position:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Hier können Sie Farben hinzufügen, ändern oder löschen.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Hilfslinien bearbeiten</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Waagerechte Linien</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-Position:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Hinzufügen</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Senkrechte Linien</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Position:</translation>
    </message>
    <message>
        <source>Lock Guides</source>
        <translation type="obsolete">Linien sperren</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
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
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Position:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Hin&amp;zufügen</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>Lö&amp;schen</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>Hilfslinien &amp;sperren</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available!</source>
        <translation type="obsolete">Leider kein Handbuch verfügbar!</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://scribus.net for updated docs and downloads.</source>
        <translation type="obsolete">Leider kein Handbuch verfügbar! Bitte besuchen Sie: http://scribus.net für Updates.</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Leider ist kein Handbuch verfügbar! Auf http://docs.scribus.net finden Sie aktualisierte Handbücher und auf http://www.scribus.net finden Sie weitere Downloads.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="unfinished">Verknüpfung</translation>
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
        <translation>Trennungsvorschlag</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Überspringen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <source>Fully Automatic</source>
        <translation type="obsolete">Vollautomatisch</translation>
    </message>
    <message>
        <source>If you uncheck this you will get a dialog
everytime a possible Hyphenation is found.</source>
        <translation type="obsolete">Falls nicht Vollautomatisch gewählt ist,
erscheint bei jeder möglichen Trennung eine\nDialogbox.</translation>
    </message>
    <message>
        <source>Check during Typing</source>
        <translation type="obsolete">Während Texteingabe überprüfen</translation>
    </message>
    <message>
        <source>Enables automatic checking of your text while typing.</source>
        <translation type="obsolete">Schaltet die automatische Überprüfung während der Eingabe ein.</translation>
    </message>
    <message>
        <source>Language:</source>
        <translation type="obsolete">Sprache:</translation>
    </message>
    <message>
        <source>Smallest Word:</source>
        <translation type="obsolete">Kürzestes Wort:</translation>
    </message>
    <message>
        <source>This is the length of the smallest word to be hyphenated.</source>
        <translation type="obsolete">Dies ist die Länge des kleinsten trennbaren Wortes.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Länge des kürzesten Wortes, das getrennt werden soll.</translation>
    </message>
    <message>
        <source>Number of Hypenations allowed:</source>
        <translation type="obsolete">Anzahl der möglichen Trennungen:</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maximale Anzahl aufeinanderfolgender Trennungen.
Null bedeutet unbegrenzt.</translation>
    </message>
    <message>
        <source>&amp;Fully Automatic</source>
        <translation type="obsolete">&amp;Vollautomatisch</translation>
    </message>
    <message>
        <source>Check &amp;During Typing</source>
        <translation type="obsolete">Während Texteingabe über&amp;prüfen</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Kürzestes Wort:</translation>
    </message>
    <message>
        <source>&amp;Number of Hypenations allowed:</source>
        <translation type="obsolete">Anzahl der &amp;möglichen Trennungen:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Trenn-&amp;Vorschläge</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Text automatisch während der Ein&amp;gabe trennen</translation>
    </message>
    <message>
        <source>Consecutive Hypenations &amp;Allowed:</source>
        <translation type="obsolete">Maximal aufeinander &amp;folgende Trennungen:</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Wenn Sie die Funktion Extras - Text trennen benutzen, können Sie aus einer Dialogbox aus allen möglichen Trennungen für jedes Wort wählen.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Aktiviert automatische Trennung während Ihrer Eingabe.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Aufeinanderfolgende &amp;Trennungen:</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Seite einfügen</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Einfügen von</translation>
    </message>
    <message>
        <source>page(s)</source>
        <translation type="obsolete">Seite(n)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>vor Seite</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>nach Seite</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>am Dokumentende</translation>
    </message>
    <message>
        <source>Template (Left Page):</source>
        <translation type="obsolete">Linke Seite basiert auf:</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation type="obsolete">Musterseite:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Rechte Seite basiert auf:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Einfügen von</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Seite(n)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>&amp;Musterseite (Links):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Musterseite:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Tabelle einfügen</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Anzahl der Zeilen:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Anzahl der Spalten:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Global JavaScripts</source>
        <translation type="obsolete">Globale Java-Scripte</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation type="obsolete">Bearbeiten...</translation>
    </message>
    <message>
        <source>Add...</source>
        <translation type="obsolete">Hinzufügen...</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>New Script:</source>
        <translation type="obsolete">Neues Script:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Neues Script</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>JavaScripts bearbeiten</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want do delete this Script?</source>
        <translation type="obsolete">Wollen Sie dieses Script wirklich löschen?</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Hinzufügen...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Neues Script:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Wollen Sie dieses Script wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Tastenkombinationen einstellen</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Aktion</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Aktuelle Taste</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Tastenkombination für die ausgewählte Aktion</translation>
    </message>
    <message>
        <source>No Key</source>
        <translation type="obsolete">Keine Taste</translation>
    </message>
    <message>
        <source>User Defined Key</source>
        <translation type="obsolete">Benutzerdefinierte Taste</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set Key</source>
        <translation type="obsolete">Definiere Taste</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation>Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation>Strg</translation>
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
        <translation>Strg+</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>This Key-Sequence is already in use</source>
        <translation type="obsolete">Diese Tastenkombination wird bereits verwendet</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Keine Taste</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Benutzerdefinierte Taste</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Taste &amp;definieren</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Diese Tastenkombination wird bereits verwendet</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Ebenen</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Neue Ebene hinzufügen</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Ebene löschen</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Ebene nach oben verschieben</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Ebene nach unten verschieben</translation>
    </message>
    <message>
        <source>Closes this Dialog</source>
        <translation type="obsolete">Schließt diese Palette</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Neue Ebene</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Wollen Sie auch alle Objekte dieser Ebene löschen?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Linienstile bearbeiten</translation>
    </message>
    <message>
        <source>Append</source>
        <translation type="obsolete">Anfügen</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplizieren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Speichern</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Neuer Stil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want do delete this Style?</source>
        <translation type="obsolete">Wollen Sie diesen Stil wirklich löschen?</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anfügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Wollen Sie diesen Absatzstil wirklich löschen?</translation>
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
        <translation></translation>
    </message>
    <message>
        <source> in</source>
        <translation></translation>
    </message>
    <message>
        <source> p</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Mehrfach Duplizieren</translation>
    </message>
    <message>
        <source>Number of Copies:</source>
        <translation type="obsolete">Anzahl der Kopien:</translation>
    </message>
    <message>
        <source>Horizontal Shift:</source>
        <translation type="obsolete">Horizontaler Versatz:</translation>
    </message>
    <message>
        <source>Vertical Shift:</source>
        <translation type="obsolete">Vertikaler Versatz:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
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
        <translation>An&amp;zahl der Kopien:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horizontaler Versatz:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikaler Versatz:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Abstände</translation>
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
        <translation>Winkel:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Länge:</translation>
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
    <name>MenuTest</name>
    <message>
        <source>Show Console</source>
        <translation type="obsolete">Konsole zeigen</translation>
    </message>
    <message>
        <source>Hide Console</source>
        <translation type="obsolete">Konsole verbergen</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation>Fehler bei der Ausführung eines Scripts</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Falls das Script ein offizelles Scribus-Script ist, den Fehler bitte auf &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; melden.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Konsole zeigen</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>&amp;Konsole verbergen</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Diese Meldung befindet sich auch in Ihrer Zwischenablage. Benutzen Sie Strg+V, um sie in den Bugtracker zu kopieren.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import a Page</source>
        <translation type="obsolete">Seite einfügen</translation>
    </message>
    <message>
        <source>Document to load:</source>
        <translation type="obsolete">Aus Dokument:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Ändern...</translation>
    </message>
    <message>
        <source>Document contains: 0 Page(s)</source>
        <translation type="obsolete">Das Dokument enthält 0 Seiten</translation>
    </message>
    <message>
        <source>Import Page Nr:</source>
        <translation type="obsolete">Importiere Seite:</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Document contains: %1 Page(s)</source>
        <translation type="obsolete">Dokument enthält %1 Seite(n)</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation>Vorlage importieren</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Seiten importieren</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Vom Dokument:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Seiten:</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Geben sie durch Kommata getrennt ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>von 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Seiten erstellen</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>vor Seite</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>nach Seite</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>am Ende des Dokuments</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>von %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Seiten verschieben</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Seite kopieren</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Seiten verschieben von:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>vor Seite</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>nach Seite</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>ans Dokumentende</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>X, Y, Z</source>
        <translation type="obsolete">X, Y, Z</translation>
    </message>
    <message>
        <source>Shape</source>
        <translation type="obsolete">Form</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Text</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Bild</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Linie</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="obsolete">Farben</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometrie</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Position:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-Position:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Breite:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Höhe:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Rotation:</source>
        <translation type="obsolete">Drehung:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Ursprung:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <source>Text flows around Box</source>
        <translation type="obsolete">Text umfließt Objekt</translation>
    </message>
    <message>
        <source>Use Bounding Box</source>
        <translation type="obsolete">Umgebendes Rechteck benutzen</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Form:</translation>
    </message>
    <message>
        <source>Edit Shape...</source>
        <translation type="obsolete">Form bearbeiten...</translation>
    </message>
    <message>
        <source>Round
Corners:</source>
        <translation type="obsolete">Eckrundung:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Abstand des Textes</translation>
    </message>
    <message>
        <source>Columns:</source>
        <translation type="obsolete">Spalten:</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="obsolete">Abstand:</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="obsolete">Oben:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="obsolete">Unten:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="obsolete">Links:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="obsolete">Rechts:</translation>
    </message>
    <message>
        <source>Tabulators...</source>
        <translation type="obsolete">Tabulatoren...</translation>
    </message>
    <message>
        <source>Path Text Properites</source>
        <translation type="obsolete">Eigenschaften des Text-Pfads</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Kurve zeigen</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Anfangsposition:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Abstand zur Kurve:</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Größe:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Abstände</translation>
    </message>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Kerning:</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation type="obsolete">Zeilenabstand:</translation>
    </message>
    <message>
        <source>Styles</source>
        <translation type="obsolete">Stile</translation>
    </message>
    <message>
        <source>Lang.:</source>
        <translation type="obsolete">Sprache:</translation>
    </message>
    <message>
        <source>Free Scaling</source>
        <translation type="obsolete">Freie Skalierung</translation>
    </message>
    <message>
        <source>X-Pos.:</source>
        <translation type="obsolete">X-Pos.:</translation>
    </message>
    <message>
        <source>Y-Pos.:</source>
        <translation type="obsolete">Y-Pos.:</translation>
    </message>
    <message>
        <source>X-Scale:</source>
        <translation type="obsolete">X-Skalierung:</translation>
    </message>
    <message>
        <source>Y-Scale:</source>
        <translation type="obsolete">Y-Skalierung:</translation>
    </message>
    <message>
        <source>Scale to Frame Size</source>
        <translation type="obsolete">An Rahmen anpassen</translation>
    </message>
    <message>
        <source>Keep Aspect Ratio</source>
        <translation type="obsolete">Seitenverhältnis beibehalten</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation type="obsolete">Proportional</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Farbprofil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Render Priorität:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ Farbmetrisch</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Farbsättigung</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut farbmetrisch</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Linker Punkt</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Endpunkte</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="obsolete">Linienstil:</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation type="obsolete">Linienbreite:</translation>
    </message>
    <message>
        <source>Edges:</source>
        <translation type="obsolete">Ecken:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spitz</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Gefast</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Endings:</source>
        <translation type="obsolete">Enden:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flach</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Quadratisch</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Abgerundet</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Zeilenabstand</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Manuelles Kerning</translation>
    </message>
    <message>
        <source>Reverse Writing</source>
        <translation type="obsolete">Gespiegelt</translation>
    </message>
    <message>
        <source>Flip horizontal</source>
        <translation type="obsolete">Horizontal spiegeln</translation>
    </message>
    <message>
        <source>Flip vertical</source>
        <translation type="obsolete">Vertikal spiegeln</translation>
    </message>
    <message>
        <source>Move one Level up</source>
        <translation type="obsolete">Eine Ebene höher</translation>
    </message>
    <message>
        <source>Move one Level down</source>
        <translation type="obsolete">Eine Ebene tiefer</translation>
    </message>
    <message>
        <source>Move to Front</source>
        <translation type="obsolete">In den Vordergrund</translation>
    </message>
    <message>
        <source>Move to Back</source>
        <translation type="obsolete">In den Hintergrund</translation>
    </message>
    <message>
        <source>Locks or unlocks the Object</source>
        <translation type="obsolete">Sperrt oder entsperrt das Objekt</translation>
    </message>
    <message>
        <source>Enables or disables printing of the Object</source>
        <translation type="obsolete">Schaltet die Druckbarkeit des Objekts ein oder aus</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation type="obsolete">X1:</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation type="obsolete">X2:</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation type="obsolete">Y1:</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation type="obsolete">Y2:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Text flows around frame</source>
        <translation type="obsolete">Text umfließt Rahmen</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Tonwert:</translation>
    </message>
    <message>
        <source>Language:</source>
        <translation type="obsolete">Sprache:</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Name des selektierten Objekts</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horizontale Position des aktuellen Ursprungs</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikale Position des aktuellen Ursprungs</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Breite</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Höhe</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Drehung des Objekts um den aktuellen Ursprung</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Ursprungspunkt</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Links-Oben als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Rechts-Oben als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Links-Unten als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Rechts-Unten als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Mittelpunkt als Ursprung setzen</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Horizontal spiegeln</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Vertikal spiegeln</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Weiter nach vorn verschieben</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Weiter nach hinten verschieben</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>In den Vordergrund bringen</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>In den Hintergrund bringen</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Objekt sperren oder entsperren</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Größenänderung sperren oder entsperren</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Drucken aktivieren oder deaktivieren</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Schriftart des Objekts</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Zeichenbreiten skalieren</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Textumrissfarbe</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Tonwert der Textumrissfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Tonwert der Textfarbe</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilvorlage für aktuellen Absatz</translation>
    </message>
    <message>
        <source>Language of object</source>
        <translation type="obsolete">Sprache des Objekts</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Ändert die Einstellungen für die linken und End-Punkte</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Muster der Line</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Linienbreite</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Art der Ecken</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Art der Enden</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Linienstil des Objekts</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Rahmenform auswählen...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Rahmenform bearbeiten...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Radius der Eckrundung setzen</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Anzahl der Spalten im Textrahmen</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Spaltenabstand</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Abstand des Textes vom oberen Rahmenrand</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Abstand des Textes vom unteren Rahmenrand</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Abstand des Textes vom linken Rahmenrand</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Abstand des Textes vom rechten Rahmenrand</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Tabulatoren setzen/bearbeiten...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Erlaube verschiedene Größen für Bild und Rahmen</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Horizontaler Versatz des Bildes innerhalb des Rahmens</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Vertikaler Versatz des Bildes innerhalb des Rahmens</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Bild horizontal skalieren</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Bild vertikal skalieren</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Seitenverhältnis beibehalten</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Bild füllt den Rahmen aus</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Benutzt die Bildproportionen an Stelle der Rahmenproportionen</translation>
    </message>
    <message>
        <source>Select the source profile of the image</source>
        <translation type="obsolete">Farbprofil für Bild auswählen</translation>
    </message>
    <message>
        <source>Select the rendering intent for the image</source>
        <translation type="obsolete">Rendering Intent für Bild auswählen</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Zellumrandungen</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Oben</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Unten</translation>
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
        <source>Style:</source>
        <translation type="obsolete">Stil:</translation>
    </message>
    <message>
        <source>Make text in lower objects flow around this frame</source>
        <translation type="obsolete">Lässt den Text in untergeordneten Objekten um diesen Rahmen fließen</translation>
    </message>
    <message>
        <source>Use the imaginary square border around your object, rather than its actual shape</source>
        <translation type="obsolete">Ein Rechteck an Stelle der wirklichen Form als Vorlage für umfließenden Text benutzen</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Seitenverhältnisse beibehalten</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Farbprofil des Bildes</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Render-Methode für das Bild</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation></translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Spaltenbreite</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Eigenschaften des Pfadtextes</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Text in untergeordneten Textrahmen um die Objektform fließen lassen</translation>
    </message>
    <message>
        <source>Use the frame shape instead of the object one</source>
        <translation type="obsolete">Form des Rahmens anstatt des Objektes benutzen</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation></translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="obsolete">Schriftgröße:</translation>
    </message>
    <message>
        <source>Type of Line:</source>
        <translation type="obsolete">Linienart:</translation>
    </message>
    <message>
        <source>Use Contour Line</source>
        <translation type="obsolete">Umgebende Form benutzen</translation>
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
        <translation>&amp;Text</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Bild</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linien</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>Fa&amp;rben</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Position:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Text umfließt Ra&amp;hmen</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>&amp;Bounding Box benutzen</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Objektumriss benutzen</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Form bearbeiten...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Ecken
ab&amp;runden:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Spalten:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Abstand:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulatoren...</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Schrift&amp;größe:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation>&amp;Kerning:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>&amp;Zeilenabstand:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>S&amp;til:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Freie Skalierung</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;X-Größe:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Y-Größe:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>An Rahmen an&amp;passen</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proportional</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Ursprung:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Linien&amp;art:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linien&amp;breite:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Ecken:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>En&amp;den:</translation>
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
        <translation>Für den Textfluss wird das umgebende Rechteck an Stelle der Form genutzt</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Für den Textfluss wird eine zweite Linie benutzt, die auf der originalen Form basiert</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Spracheinstellung für die Silbentrennung</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Rechts-nach-Links-Text</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Stil bearbeiten</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flach</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Quadratisch</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Abgerundet</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spitz</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Gefast</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linienbreite:</translation>
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
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Durchgehend</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Gestrichelt</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Gepunktet</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Strich-Punkt</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Strich-Punkt-Punkt</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation>Musterseiten bearbeiten</translation>
    </message>
    <message>
        <source>Append</source>
        <translation type="obsolete">Anfügen</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplizieren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Exit</source>
        <translation type="obsolete">Verlassen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want do delete this Template?</source>
        <translation type="obsolete">Wollen Sie wirklich diese Musterseite löschen?</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Neue Musterseite</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie #%1 von</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anfügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation>Wollen Sie diese Vorlage wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Neues Dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Größe:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Ausrichtung:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Breite:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Höhe:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Doppelseiten</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Linke Seite zuerst</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="obsolete">Links:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="obsolete">Rechts:</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="obsolete">Oben:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="obsolete">Unten:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>First Page Number:</source>
        <translation type="obsolete">Erste Seiten Nummer:</translation>
    </message>
    <message>
        <source>Default Unit:</source>
        <translation type="obsolete">Maßeinheit:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation>Punkte (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Zoll (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Autom. Text Frames</source>
        <translation type="obsolete">Autom. Textrahmen</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Spalteneinteilung</translation>
    </message>
    <message>
        <source>Gutter Width:</source>
        <translation type="obsolete">-abstand:</translation>
    </message>
    <message>
        <source>Columns:</source>
        <translation type="obsolete">Spalten:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation type="obsolete">Innen:</translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation type="obsolete">Außen:</translation>
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
        <source>Automatic Text Frames</source>
        <translation type="obsolete">Automatisch Textrahmen erzeugen</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="obsolete">Abstand:</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Seitengröße des Dokuments, entweder eine Standardgröße oder benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Ausrichtung der Dokument-Seite</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Breite der Seite, veränderbar, wenn &quot;Benutzerdefiniert&quot; ausgewählt ist</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Höhe der Seite , veränderbar, wenn &quot;Benutzerdefiniert&quot; ausgewählt ist</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Aktiviert doppelseitiges Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Macht die erste Seite zu der linken Seite des Dokuments</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der Abstand gewählt werden, um eine genaue Bindung zu ermöglichen</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der Abstand gewählt werden, um eine genaue Bindung zu ermöglichen</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Erste Seitennummer im Dokument</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard-Maßeinheit für das Dokument</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Textrahmen automatisch erstellen, wenn neue Seiten hinzugefügt werden</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Anzahl der Spalten in den automatisch erzeugten Textrahmen</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Abstand zwischen den Spalten</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>US-Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Au&amp;srichtung:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Lin&amp;ks:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Nummer der &amp;ersten Seite:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Standard&amp;maßeinheit:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Automatische &amp;Textrahmen</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Ab&amp;stand:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Spa&amp;lten:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>A&amp;ußen:</translation>
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
        <translation>Linke Seite</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Rechte Seite</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Pfade</translation>
    </message>
    <message>
        <source>Absolute Coordinates</source>
        <translation type="obsolete">Absolute Koordinaten</translation>
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
        <source>End Editing</source>
        <translation type="obsolete">Beenden</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Punkte verschieben</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Kontrollpunkte verschieben</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Punkte hinzufügen</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Punkte löschen</translation>
    </message>
    <message>
        <source>Move Control Points independently</source>
        <translation type="obsolete">Kontrollpunkte unabhängig verschieben</translation>
    </message>
    <message>
        <source>Move Control Points symmetrical</source>
        <translation type="obsolete">Kontrollpunkte symmetrisch verschieben</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Kontrollpunkte zurücksetzen</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Diesen Kontrollpunkt zurücksetzen</translation>
    </message>
    <message>
        <source>Opens a Polygon or cuts a Bezier Curve</source>
        <translation type="obsolete">Öffnet ein Polygon oder trennt eine Bezier Kurve auf</translation>
    </message>
    <message>
        <source>Closes this Bezier Curve</source>
        <translation type="obsolete">Schließt diese Bezier Kurve</translation>
    </message>
    <message>
        <source>Mirrors the Path Horizontally</source>
        <translation type="obsolete">Spiegelt den Pfad horizontal</translation>
    </message>
    <message>
        <source>Mirrors the Path Vertically</source>
        <translation type="obsolete">Spiegelt den Pfad vertikal</translation>
    </message>
    <message>
        <source>Shears the Path horizontal to the right</source>
        <translation type="obsolete">Scherung nach rechts</translation>
    </message>
    <message>
        <source>Shears the Path horizontal to the left</source>
        <translation type="obsolete">Scherung nach links</translation>
    </message>
    <message>
        <source>Shears the Path vertical up</source>
        <translation type="obsolete">Scherung nach open</translation>
    </message>
    <message>
        <source>Shears the Path vertical down</source>
        <translation type="obsolete">Scherung nach unten</translation>
    </message>
    <message>
        <source>Rotates the Path counter-clockwise</source>
        <translation type="obsolete">Rotiert den Pfad um 1° gegen den Uhrzeigersinn</translation>
    </message>
    <message>
        <source>Rotates the Path clockwise</source>
        <translation type="obsolete">Rotiert den Pfad um 1° im Uhrzeigersinn</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by 10%</source>
        <translation type="obsolete">Verkleinert den Pfad um 10%</translation>
    </message>
    <message>
        <source>Enlarges the Size of the Path by 10%</source>
        <translation type="obsolete">Vergrößert den Pfad um 10%</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Aktiviert bedeutet Koordinaten relativ zur Seite,
andernfalls relativ zum Objekt.</translation>
    </message>
    <message>
        <source>Edit Contour Line</source>
        <translation type="obsolete">Umgebende Form bearbeiten</translation>
    </message>
    <message>
        <source>Reset Contour Line</source>
        <translation type="obsolete">Kontrollpunkte zurücksetzen</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path</source>
        <translation type="obsolete">Verkleinert den Pfad</translation>
    </message>
    <message>
        <source>Enlarges the Size of the Path</source>
        <translation type="obsolete">Vergrößert den Pfad</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolute Koordinaten</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Position:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>&amp;Objektumriss bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Objektumriss &amp;zurücksetzen</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Beabeitung &amp;beenden</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Kontrollpunkte unabhängig verschieben</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Kontrollpunkte symmetrisch verschieben</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Bezierkurve oder Polygon öffnen</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Bezierkurve schließen</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Pfad horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Pfad vertikal spiegeln</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Rright</source>
        <translation type="obsolete">Pfad horizontal nach rechts verschieben</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Pfad horizontal nach links verschieben</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Pfad nach oben scheren</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Pfad nach unten scheren</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Pfad gegen den Uhrzeigersinn drehen</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Pfad mit dem Uhrzeigersinn drehen</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Pfad verkleinern um x %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Pfad vergrößern um x %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Grad der Drehung</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% vergrößern oder verkleinern</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Konturmodus aktivieren</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Kontur auf die originale Form zurücksetzen</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Pfad horizontal nach rechts verschieben</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Script Konsole</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF-File</source>
        <translation type="obsolete">PDF erzeugen</translation>
    </message>
    <message>
        <source>Output to File:</source>
        <translation type="obsolete">In Datei ausgeben:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ändern...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Bereich</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="obsolete">Alle Seiten</translation>
    </message>
    <message>
        <source>From:</source>
        <translation type="obsolete">Von:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">bis:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Datei Optionen</translation>
    </message>
    <message>
        <source>Compatibility:</source>
        <translation type="obsolete">Dateiformat:</translation>
    </message>
    <message>
        <source>Binding:</source>
        <translation type="obsolete">Bund:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Linker Rand</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Rechter Rand</translation>
    </message>
    <message>
        <source>Generate Thumbnails</source>
        <translation type="obsolete">Vorschauen erzeugen</translation>
    </message>
    <message>
        <source>Save linked Text Frames as PDF-Articles</source>
        <translation type="obsolete">Textrahmenketten als PDF-Artikel speichern</translation>
    </message>
    <message>
        <source>Include Bookmarks</source>
        <translation type="obsolete">Lesezeichen einfügen</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="obsolete">Auflösung:</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Compression</source>
        <translation type="obsolete">Komprimierung</translation>
    </message>
    <message>
        <source>Downsample Images to:</source>
        <translation type="obsolete">Auflösung von Bildern:</translation>
    </message>
    <message>
        <source>Compress Text and Vector Graphics</source>
        <translation type="obsolete">Text und Vektorgrafiken komprimieren</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <source>Embed all Fonts</source>
        <translation type="obsolete">Alle Schriften einbetten</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Einbetten</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Verfügbare Schriften:</translation>
    </message>
    <message>
        <source>&gt;&gt;</source>
        <translation type="obsolete">&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation type="obsolete">&lt;&lt;</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Einzubettende Schriften:</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="obsolete">Schriften</translation>
    </message>
    <message>
        <source>Enable Presentation Effects</source>
        <translation type="obsolete">Präsentations Effekte verwenden</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Show Page Previews</source>
        <translation type="obsolete">Zeige Seiten Vorschau</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>Display Duration:</source>
        <translation type="obsolete">Anzeigedauer:</translation>
    </message>
    <message>
        <source>Effect Duration:</source>
        <translation type="obsolete">Effektdauer:</translation>
    </message>
    <message>
        <source>Effect Type:</source>
        <translation type="obsolete">Art des Effekts:</translation>
    </message>
    <message>
        <source>Moving Lines:</source>
        <translation type="obsolete">Bewegte Linien:</translation>
    </message>
    <message>
        <source>from the:</source>
        <translation type="obsolete">von:</translation>
    </message>
    <message>
        <source>Direction:</source>
        <translation type="obsolete">Richtung:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sek</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Waagrecht</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Innen</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Außen</translation>
    </message>
    <message>
        <source>Apply Effect on all Pages</source>
        <translation type="obsolete">Effekt auf alle Seiten anwenden</translation>
    </message>
    <message>
        <source>Extras</source>
        <translation type="obsolete">Extras</translation>
    </message>
    <message>
        <source>Use Encryption</source>
        <translation type="obsolete">Verschlüsseln</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Passwörter</translation>
    </message>
    <message>
        <source>User:</source>
        <translation type="obsolete">Benutzer:</translation>
    </message>
    <message>
        <source>Owner:</source>
        <translation type="obsolete">Eigentümer:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Berechtigungen</translation>
    </message>
    <message>
        <source>Allow Printing the Document</source>
        <translation type="obsolete">Dokument drucken erlaubt</translation>
    </message>
    <message>
        <source>Allow Changing the Document</source>
        <translation type="obsolete">Dokument ändern erlaubt</translation>
    </message>
    <message>
        <source>Copying Text and Graphics is allowed</source>
        <translation type="obsolete">Texte und Bilder kopieren erlaubt</translation>
    </message>
    <message>
        <source>Adding Annotations and Fields is allowed</source>
        <translation type="obsolete">Notizen und Formularfelder hinzufügen erlaubt</translation>
    </message>
    <message>
        <source>Security</source>
        <translation type="obsolete">Sicherheit</translation>
    </message>
    <message>
        <source>Output intended for:</source>
        <translation type="obsolete">Ausgabe vorgesehen für:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Monitor / Internet</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Drucker</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Farben:</translation>
    </message>
    <message>
        <source>Use ICC-Profile</source>
        <translation type="obsolete">ICC-Profile einbetten</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Render-Priorität:</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Eingebettete Profile nicht benutzen</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Farbe</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 Ausgabebedingung</translation>
    </message>
    <message>
        <source>Info String:</source>
        <translation type="obsolete">Info Kommentar:</translation>
    </message>
    <message>
        <source>Output Profile:</source>
        <translation type="obsolete">Ausgabeprofil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Trim Box</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>PDF-Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF-Dateien (*.pdf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Kein Effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Jalusie</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Einblenden</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Auflösen</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Schachbrett</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Teilen</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Rollen</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Links nach Rechts</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Oben nach Unten</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Unten nach Oben</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Rechts nach Links</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Schräg von Links Oben</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ Farbmetrisch</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Farbsättigung</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut farbmetrisch</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Bildeinstellungen</translation>
    </message>
    <message>
        <source>Method:</source>
        <translation type="obsolete">Methode:</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisch</translation>
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
        <translation>Keine</translation>
    </message>
    <message>
        <source>Quality:</source>
        <translation type="obsolete">Qualität:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maximal</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Hoch</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mittel</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Niedrig</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimal</translation>
    </message>
    <message>
        <source>From the:</source>
        <translation type="obsolete">Vom:</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Alle Seiten als PDF exportieren</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Eine Seitenbereich als PDF exportieren</translation>
    </message>
    <message>
        <source>First page to export when exporting a range</source>
        <translation type="obsolete">Erste Seite</translation>
    </message>
    <message>
        <source>Last page to export when exporting a range</source>
        <translation type="obsolete">Letzte Seite</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Zeit bis die nächste Seite angezeigt wird.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Länge des Effekts in Sekunden.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Gewählten Effekt auf alle Seiten anwenden.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Wählen Sie ein Masterpasswort, dass alle Sicherheitsfunktionen
in der exportierten Datei aktiviert</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF</source>
        <translation type="obsolete">Wählen Sie ein Passwort, dass es Benutzern erlaubt, die PDF-Datei zu lesen</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Farbprofil für Dokumentfarben einbetten</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Farbprofil für Bilder einbetten</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Farbprofile der Quell-Bilder nicht benutzen</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Abzug für Beschnitt oben</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Abzug für Beschnitt unten</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Abzug für Beschnitt links</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Abzug für Beschnitt rechts</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Allgemein</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>Schrift&amp;arten</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Security</source>
        <translation type="obsolete">&amp;Sicherheit</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Farben</translation>
    </message>
    <message>
        <source>&amp;PDF/X-3</source>
        <translation type="obsolete">&amp;PDF/X-3</translation>
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
        <source>Save</source>
        <translation type="obsolete">Speichern</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Legt die PDF-Kompatiblität fest. Standard ist Acrobat 4.0, weil das die größte Kompatiblität ermöglicht.
Wählen Sie Acrobat 5.0, wenn Sie PDF 1.4-Funktionen wie Transparenz oder 128 bit-Verschlüsselung benötigen.
PDF/X-3 ist gedacht für gewerbliche Druckereien und nur möglich, wenn Sie das Farb-Management aktiviert haben.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Legt die Art des Bundes in der PDF-Datei fest. Lassen die Standard-
Einstellung (Links), wenn Sie das nicht verändern müssen.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Erzeugt Vorschau-Bilder für jede Seite der PDF-Datei.
Einige Programme nutzen diese Bilder zur Navigation.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Erzeugt PDF-Artikel, die für die Navigation in verketteten Textrahmen nützlich sind.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Lesezeichen im Dokument in die PDF-Datei einbetten.
Nützlich zur Navigation in langen PDF-Dateien.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Ausgabe-Auflösung für Text und Vektor-Grafiken.
Das beeinflusst nicht die Auflösung der Bitmap-Grafiken.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Kompression von Text und Grafiken reduziert die Dateigröße.
Nicht ohne Grund deaktivieren.
</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Kompressions-Methode für Bitmap-Bilder.
Automatisch - Scribus wählt die beste Methode.
ZIP - Gut für Bilder mit Farbflächen.
JPEG - erzeugt kleinere PDF&apos;s bei Dokumenten mit viel Bildern ohne große Kompressionsverluste.
Braucht normalerweise nicht geändert werden.</translation>
    </message>
    <message>
        <source>Compression level: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Kompressions-Stärke: Minimum (25%), Niedrig (50%), Mittel (75%), Hoch (85%), Maximum (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Bitmap-Bilder neu mit gewählten DPI rendern.
Ist die Option deaktiviert, wird die natürliche Auflösung der Bilder übernommen.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Punkte pro Zoll) für den Export von Bildern.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Schriftarten in PDF&apos;s einbetten. Das schützt das
Layout und Erscheinungsbild des Dokuments vor Veränderungen.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Aktiviert die Präsentationseffekte, wenn der Acrobat Reader im Vollbild-Modus ausgeführt wird.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Vorschau für aufgeführte Seiten anzeigen.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Auswahl des Effekts.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Richtung für die Effekte Rollen und Schachbrett.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Start-Position für die Effekte Einblenden und Teilen.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Richtung für die Effekte Jalusie und Teilen.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Aktiviert die Sicherheits-Funktionen in der exportierten PDF-Datei.
Wenn Acrobat 4.0 ausgewählt ist, wird die PDF-Datei mit 40 bit verschlüsselt.
Wenn Acrobat 5.0 ausgewählt ist, wird die PDF-Datei mit 128 bit verschlüsselt.
Hinweis: Die Verschlüsselung ist nicht so zuverlässig wie GPG oder PGP und hat einige Begrenzungen.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF</source>
        <translation type="obsolete">Erlaubt das Drucken der PDF-Datei</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF</source>
        <translation type="obsolete">Erlaubt das Verändern der PDF-Datei</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF</source>
        <translation type="obsolete">Erlaubt das Kopieren von Bildern oder Texten aus der PDF-Datei</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF</source>
        <translation type="obsolete">Erlaubt das Anfügen von Anmerkungen zu der PDF-Datei</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Farbmodell für die Ausgabe der PDF-Datei.
Wählen Sie Monitor/Internet für PDF-Dateien, die auf dem Monitor angezeigt oder mit herkömmlichen Druckern gedruckt werden sollen.
Wählen Sie Drucker, um die PDF-Datei auf einem CMYK-Drucker auszugeben.</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Farbprofil für Farben</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Render-Methode für Farben</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Farbprofil für Bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Render-Methode für Bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Ausgabe-Profil zum Drucken. Wenn möglich, benutzen Sie das Profil Ihres Druckers.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Erforderliche Eingabe für PDF/X-3, ohne die das Dokument nicht PDF/X-3-konform ist. Benutzen Sie z.B. den Titel des Dokuments.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Kompressionsstärke: Minimal (25%), Niedrig (50%), Mittel (75%), Hoch (80%), Maximal (95%)</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Wählen Sie ein Password für Benutzer, um die PDF-Datei ansehen zu können.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Drucken erlauben. Wenn nicht ausgewählt, ist das Drucken verboten.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Veränderung der PDF-Datei gestatten. Wenn nicht ausgewählt, wird die Veränderung verhindert.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Kopieren von Text oder Bildern gestatten.
Wenn nicht ausgewählt, können weder Text von Bilder kopiert werden.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation>Hinzufügen von Anmerkungen gestatten.
Wenn nicht ausgewählt, wird das Hinzufügen von Anmerkungen verhindert.</translation>
    </message>
    <message>
        <source>Allow Copying Text and Graphics</source>
        <translation type="obsolete">Kopieren von Text und Bildern gestatten</translation>
    </message>
    <message>
        <source>Allow Adding Annotations and Fields</source>
        <translation type="obsolete">Hinzufügen von Anmerkungen gestatten</translation>
    </message>
    <message>
        <source>Choose Pages</source>
        <translation type="obsolete">Seiten auswählen</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Geben sie durch Kommata getrennt ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation>PDF-Datei erstellen</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Ausgabe in &amp;Datei:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Alle Seiten</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Seiten &amp;wählen</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Kompatibilität:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Einband:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>&amp;Vorschaubilder erzeugen</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Verknüpfte Textrahmen als PDF-Arti&amp;kel speichern</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Lesezeichen integrieren</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Auf&amp;lösung:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="obsolete">&amp;Text und Vektorbilder komprimieren</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Methode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualität:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>Auflösung der Bilder &amp;reduzieren auf:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Alle Schriftarten ein&amp;betten</translation>
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
        <source>Enable &amp;Presentation Effects</source>
        <translation type="obsolete">&amp;Präsentationseffekte aktivieren</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Seiten&amp;vorschau anzeigen</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Anzeigedauer:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Dauer des &amp;Effekts:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Effekt&amp;typ:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Bewegte Linien:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;von:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Richtung:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Effekt auf alle Seiten an&amp;wenden</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Verschlüsselung benutzen</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Benutzer:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Besitzer:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Dokument &amp;drucken erlaubt</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Än&amp;derungen am Dokument erlaubt</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Kopieren von &amp;Text und Bildern gestatten</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Hinzufügen von &amp;Anmerkungen gestatten</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Sicherheit</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Ausgabe &amp;vorgesehen für:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Benutzerdefinierte Rendering-Einstellungen benutzen</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Rendering-Einstellungen</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Häufigkeit:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Winkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Punktaufbau:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Runden</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Ellipse</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>ICC-Profile benutzen</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Info-Text:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Ausgabe&amp;profil:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Diese erweiterte Optio ist normalerweise deaktiviert und sollte nur aktiviert werden,
wenn Ihre Druckerei es fordert und sie die notwendigen Informationen haben.
Andernfalls könnte ihre PDF-Datei fehlerhaft gedruckt werden und sie kann nicht plattformübergreifend verwendet werden.</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Seiten &amp;horizontal spiegeln</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Text und Vektor&amp;grafiken komprimieren</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Von allen Fonts nur &amp;benutzte Schriftzeichen einbetten</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Nur benutzte Schriftzeichen einbetten:</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>Präsentations&amp;effekte aktivieren</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Präsentation</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Seite(n) horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Seite(n) vertikal spiegeln</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Druckvorschau</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="obsolete">Seite:</translation>
    </message>
    <message>
        <source>Anti-alias Text</source>
        <translation type="obsolete">Text glätten</translation>
    </message>
    <message>
        <source>Anti-alias Graphics</source>
        <translation type="obsolete">Grafik glätten</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Display Transparency</source>
        <translation type="obsolete">Transparenz anzeigen</translation>
    </message>
    <message>
        <source>Display CMYK</source>
        <translation type="obsolete">In CMYK anzeigen</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">C</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="obsolete">M</translation>
    </message>
    <message>
        <source>Y</source>
        <translation type="obsolete">Y</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">K</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Stellt den Text auf Kosten der Render-Geschwindigkeit glatter dar.
Funktioniert nur mit Type-1-Schriftarten</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Stellt True Type Fonts, Open Type Fonts, EPS, PDF und Vektor-Grafiken 
auf Kosten der Render-Geschwindigkeit glatter dar</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Aktiviert Transparenz und transparente Objekt im Dokument. Erfordert Ghostscript 7.07 oder höher</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Benutzt zur Vorschau CMYK an Stelle der RGB-Farben</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Aktiviert den Cyan-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Aktiviert den Magenta-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Aktiviert den Yellow-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Aktiviert den Black-Auszug</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Text &amp;weichzeichnen</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Bil&amp;der weichzeichnen</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>&amp;Transparenz aktivieren</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;CMYK anzeigen</translation>
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
        <translation>&amp;Unterfarbenreduktion</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Mit dieser Option werden ein paar Grautöne, die entstehen,
wenn Black aus Cyan, Magenta und Yellow gemischt wird, durch Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr nahe stehen. Diese Option kann zu besseren Bildern führen,
allerdings müssen Sie von Fall zu Fall entscheiden, wie Sie bessere Ergebnisse
bekommen. Außerdem reduziert UFR die Gefahr einer Übersättigung mit CMY.
</translation>
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
        <translation>An diese Stelle kopieren</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>An diese Stelle verschieben</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Einfügen</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Ränder verbergen</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation type="obsolete">Ränder zeigen</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Rahmen verbergen</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Rahmen zeigen</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Bilder verbergen</translation>
    </message>
    <message>
        <source>Show Images</source>
        <translation type="obsolete">Bilder zeigen</translation>
    </message>
    <message>
        <source>Hide Grid</source>
        <translation type="obsolete">Raster verbergen</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Raster zeigen</translation>
    </message>
    <message>
        <source>Hide Guides</source>
        <translation type="obsolete">Hilfslinien verbergen</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="obsolete">Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Am Raster ausrichten</translation>
    </message>
    <message>
        <source>Snap to Guides</source>
        <translation type="obsolete">Hilfslinen sind magnetisch</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Datei:</translation>
    </message>
    <message>
        <source>Original ppi: </source>
        <translation type="obsolete">DPI original: </translation>
    </message>
    <message>
        <source>Actual ppi: </source>
        <translation type="obsolete">DPI aktuell: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Textkette</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Textrahmen</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text auf einem Pfad</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Absätze:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Wörter:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Zeichen:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Drucken:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Ein</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Aus</translation>
    </message>
    <message>
        <source>Info</source>
        <translation type="obsolete">Info</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Bild laden...</translation>
    </message>
    <message>
        <source>Image Visible</source>
        <translation type="obsolete">Bild anzeigen</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Text laden...</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Text bearbeiten...</translation>
    </message>
    <message>
        <source>Is PDF-Bookmark</source>
        <translation type="obsolete">Ist PDF-Lesezeichen</translation>
    </message>
    <message>
        <source>Is PDF-Annotation</source>
        <translation type="obsolete">Ist PDF-Anmerkung</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation type="obsolete">Eigenschaften der Anmerkungen</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation type="obsolete">Eigenschaften der Felder</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Gruppieren</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Gruppe auflösen</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Sperren</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">In den Hintergrund</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">In den Vordergrund</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Weiter nach hinten</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Weiter nach vorn</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Entsperren</translation>
    </message>
    <message>
        <source>Send to Scrapbook</source>
        <translation type="obsolete">In die Bibliothek kopieren</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="obsolete">Kopiere auf die Ebene</translation>
    </message>
    <message>
        <source>Picture Frame</source>
        <translation type="obsolete">Bildrahmen</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Polygon</translation>
    </message>
    <message>
        <source>Outlines</source>
        <translation type="obsolete">Pfade</translation>
    </message>
    <message>
        <source>Bezier Curve</source>
        <translation type="obsolete">Bezierkurve</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation type="obsolete">Umwandeln in</translation>
    </message>
    <message>
        <source>Show Properties...</source>
        <translation type="obsolete">Eigenschaften zeigen...</translation>
    </message>
    <message>
        <source>Hide Properties...</source>
        <translation type="obsolete">Eigenschaften verbergen...</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Ausschneiden</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopieren</translation>
    </message>
    <message>
        <source>Clear Contents</source>
        <translation type="obsolete">Inhalt löschen</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Update Picture</source>
        <translation type="obsolete">Bild neu laden</translation>
    </message>
    <message>
        <source>Edit Picture</source>
        <translation type="obsolete">Bild bearbeiten</translation>
    </message>
    <message>
        <source>Append Text...</source>
        <translation type="obsolete">Text anfügen...</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Das Programm</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>fehlt!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopie von</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Grundlinien-Raster verstecken</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Adjust Frame to Picture</source>
        <translation type="obsolete">Rahmen am Bild ausrichten</translation>
    </message>
    <message>
        <source>Insert LoremIpsum</source>
        <translation type="obsolete">Blindtext einfügen</translation>
    </message>
    <message>
        <source>Insert Sample Text</source>
        <translation type="obsolete">Beispiel-Text einfügen</translation>
    </message>
    <message>
        <source>Lock object size</source>
        <translation type="obsolete">Objektgröße sperren</translation>
    </message>
    <message>
        <source>Unlock object size</source>
        <translation type="obsolete">Objektgröße entsperren</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Anordnung</translation>
    </message>
    <message>
        <source>PDF-Options</source>
        <translation type="obsolete">PDF-Optionen</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;Ränder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Ra&amp;hmen zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>&amp;Bilder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Ra&amp;ster zeigen</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Magnetisches Ra&amp;ster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Magnetische Hilfs&amp;linien</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Original-PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Aktuelle PPI:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Information</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>Bild &amp;laden...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Bild &amp;sichtbar</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>Bilder ak&amp;tualisieren</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>Bild &amp;bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>Rahmen am Bild aus&amp;richten</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Text &amp;laden...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Text anfügen...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>Text &amp;bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>Beispielte&amp;xt einfügen</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Ist ein PDF-&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Ist eine PDF-&amp;Anmerkung</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Eigenschaften der An&amp;merkung</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Feld&amp;eigenschaften</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-Optionen</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Sperren</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Ent&amp;sperren</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Objekt&amp;größe sperren</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Objektgröße &amp;freigeben</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>In &amp;Bibliothek speichern</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Auf andere &amp;Ebene verschieben</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Gruppieren</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Gruppe auf&amp;lösen</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>An&amp;ordnung</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>In den &amp;Vordergrund</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Weiter nach hin&amp;ten</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Weiter nach vor&amp;n</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation>&amp;Bildrahmen</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Poly&amp;gon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Umrisse</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textrahmen</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezierkurve</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Umwandeln in</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>Inhalt lösc&amp;hen</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Eigenschaften an&amp;zeigen...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Eigenschaften &amp;verbergen...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Wollen Sie wirklich den ganzen Text löschen?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polyline</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Pfadtext</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Seite </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation> von %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Ergebnis</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Suchergebnis für: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Auswählen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation>Bilder</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Gehe zu</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Nicht vorhanden</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Pfad</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Drucken</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygon-Eigenschaften</translation>
    </message>
    <message>
        <source>Corners:</source>
        <translation type="obsolete">Ecken:</translation>
    </message>
    <message>
        <source>Rotation:</source>
        <translation type="obsolete">Drehung:</translation>
    </message>
    <message>
        <source>Convex Polygon</source>
        <translation type="obsolete">Konvexes Polygon</translation>
    </message>
    <message>
        <source>Factor:</source>
        <translation type="obsolete">Faktor:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Ec&amp;ken:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Conve&amp;x Polygon</source>
        <translation type="obsolete">Konve&amp;xes Polygon</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fak&amp;tor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Anzahl der Ecken des Polygons</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grad der Drehung des Polygons</translation>
    </message>
    <message>
        <source>Polygons will be convex rather than concave</source>
        <translation type="obsolete">Zeichnet ein konvexes Polygon</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Vorschau der gewählten Eigenschaften</translation>
    </message>
    <message>
        <source>Change the angles at which lines of the polygon join</source>
        <translation type="obsolete">Legt den Winkel fest, bei dem sich die Linien des Polygons verbinden</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Faktor über&amp;nehmen</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Hier stellen Sie ein, wie stark konvex oder konkav das Polygon gezeichnet wird</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Ein negativer Wert steht für eine konkave (sternförmige) Figur,
ein positiver Wert für eine konkave Form</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Voreinstellungen</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Bibliothek</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Oberfläche</translation>
    </message>
    <message>
        <source>Theme</source>
        <translation type="obsolete">Thema</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="obsolete">Schriftgröße:</translation>
    </message>
    <message>
        <source> pts</source>
        <translation type="obsolete">pts</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Einheiten</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Punkte (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Zoll (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Mouse-Settings</source>
        <translation type="obsolete">Maus-Einstellungen</translation>
    </message>
    <message>
        <source>Wheel-Jump</source>
        <translation type="obsolete">Rad-Vorschub</translation>
    </message>
    <message>
        <source>Grab-Radius</source>
        <translation type="obsolete">Fangradius</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menüs</translation>
    </message>
    <message>
        <source>Recent Documents:</source>
        <translation type="obsolete">Letzte Dokumente:</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts...</source>
        <translation type="obsolete">Tastaturkürzel...</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Pfade</translation>
    </message>
    <message>
        <source>Documents:</source>
        <translation type="obsolete">Dokumente:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ändern...</translation>
    </message>
    <message>
        <source>ICC-Profiles:</source>
        <translation type="obsolete">ICC-Profile:</translation>
    </message>
    <message>
        <source>Scripts:</source>
        <translation type="obsolete">Skripte:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Ausrichtung:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Größe:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Breite:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Höhe:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Doppelseiten</translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="obsolete">Linke Seite zuerst</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="obsolete">Unten:</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="obsolete">Oben:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="obsolete">Rechts:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="obsolete">Links:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autom. Speichern</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Eingeschaltet</translation>
    </message>
    <message>
        <source>Interval:</source>
        <translation type="obsolete">alle:</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Hilfslinien Einrichtung</translation>
    </message>
    <message>
        <source>Minor Grid Spacing:</source>
        <translation type="obsolete">Abstand Teillinien:</translation>
    </message>
    <message>
        <source>Major Grid Spacing:</source>
        <translation type="obsolete">Abstand Hauptlinien:</translation>
    </message>
    <message>
        <source>Guide Snap Distance:</source>
        <translation type="obsolete">Fangabstand der Hilfslinien:</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Hilfslinienfarben</translation>
    </message>
    <message>
        <source>Minor Grid Color:</source>
        <translation type="obsolete">Farbe Teillinien:</translation>
    </message>
    <message>
        <source>Major Grid Color:</source>
        <translation type="obsolete">Farbe Hauptlinien:</translation>
    </message>
    <message>
        <source>User Guides Color:</source>
        <translation type="obsolete">Farbe Hilfslinien:</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Platzierung</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation type="obsolete">Im Hintergrund</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation type="obsolete">Im Vordergrund</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation type="obsolete">Skalierung:</translation>
    </message>
    <message>
        <source>Disp.:</source>
        <translation type="obsolete">Versatz:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitälchen</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andere</translation>
    </message>
    <message>
        <source>Automatic Line Spacing:</source>
        <translation type="obsolete">Automatischer Zeilenabstand:</translation>
    </message>
    <message>
        <source>Default Size:</source>
        <translation type="obsolete">Größenvorgabe:</translation>
    </message>
    <message>
        <source>Default Font:</source>
        <translation type="obsolete">Standard Schrift:</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation type="obsolete">Textfarbe:</translation>
    </message>
    <message>
        <source>Columns:</source>
        <translation type="obsolete">Spalten:</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="obsolete">Abstand:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>The quick brown fox jumps over the lazy dog</translation>
    </message>
    <message>
        <source>Line Color:</source>
        <translation type="obsolete">Linienfarbe:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation type="obsolete">Tonwert:</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="obsolete">Füllfarbe:</translation>
    </message>
    <message>
        <source>Linestyle:</source>
        <translation type="obsolete">Linienstil:</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation type="obsolete">Linienbreite:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="obsolete">Linienstil:</translation>
    </message>
    <message>
        <source>Minimum:</source>
        <translation type="obsolete">Minimum:</translation>
    </message>
    <message>
        <source>Maximum:</source>
        <translation type="obsolete">Maximum:</translation>
    </message>
    <message>
        <source>Stepping:</source>
        <translation type="obsolete">Schrittweite:</translation>
    </message>
    <message>
        <source>Free Scaling</source>
        <translation type="obsolete">Freies Skalieren</translation>
    </message>
    <message>
        <source>Horizontal Scaling</source>
        <translation type="obsolete">Horiz. Skalierung</translation>
    </message>
    <message>
        <source>Vertical Scaling</source>
        <translation type="obsolete">Vertik. Skalierung</translation>
    </message>
    <message>
        <source>Scale Picture to Frame Size</source>
        <translation type="obsolete">Bild an Rahmen anpassen</translation>
    </message>
    <message>
        <source>Keep Aspect Ratio</source>
        <translation type="obsolete">Seitenverhältnis beibehalten</translation>
    </message>
    <message>
        <source>Corners:</source>
        <translation type="obsolete">Ecken:</translation>
    </message>
    <message>
        <source>Rotation:</source>
        <translation type="obsolete">Drehung:</translation>
    </message>
    <message>
        <source>Convex Polygon</source>
        <translation type="obsolete">Konvexes Polygon</translation>
    </message>
    <message>
        <source>Factor:</source>
        <translation type="obsolete">Faktor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Andere Optionen</translation>
    </message>
    <message>
        <source>Save Contents on Exit</source>
        <translation type="obsolete">Inhalt bei Programmende speichern</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Klein</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mittel</translation>
    </message>
    <message>
        <source>Big</source>
        <translation type="obsolete">Groß</translation>
    </message>
    <message>
        <source>Display Frames</source>
        <translation type="obsolete">Zeige Rahmen</translation>
    </message>
    <message>
        <source>Display Pages Side by Side</source>
        <translation type="obsolete">Seiten nebeneinander</translation>
    </message>
    <message>
        <source>Page-Colors</source>
        <translation type="obsolete">Seiten-Farben</translation>
    </message>
    <message>
        <source>Background:</source>
        <translation type="obsolete">Hintergrund:</translation>
    </message>
    <message>
        <source>Margins:</source>
        <translation type="obsolete">Ränder:</translation>
    </message>
    <message>
        <source>Display Unprintable Area in Margin Color</source>
        <translation type="obsolete">Nicht druckbaren Bereich in Randfarbe zeigen</translation>
    </message>
    <message>
        <source>Use PDF-1.4 Transparency Features</source>
        <translation type="obsolete">PDF-1.4 Tranzparenzen benutzen</translation>
    </message>
    <message>
        <source>Adjust Display Size</source>
        <translation type="obsolete">Größenanpassung der Anzeige</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Zum Einstellen der Anzeigegröße das Lineal mit dem Regler anpassen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation type="obsolete">Innen:</translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation type="obsolete">Außen:</translation>
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
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hilfslinien</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Externe Hilfsprogramme</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Versch.</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation type="obsolete">Versatz:</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Postscript Interpreter</translation>
    </message>
    <message>
        <source>Name of executable:</source>
        <translation type="obsolete">Programmname:</translation>
    </message>
    <message>
        <source>Antialias Text</source>
        <translation type="obsolete">Text weichzeichnen</translation>
    </message>
    <message>
        <source>Antialias Graphics</source>
        <translation type="obsolete">Graphiken weichzeichnen</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Bildbearbeitungsprogramm</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Drucken</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="obsolete">Auf Seitenränder beschneiden</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Auswahl der Standard-Fenster-Dekoration.
Scribus stellt alle verfügbaren KDE und QT-Themen zur Verfügung</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Schriftgröße für Menüs und Dialoge</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Maßeinheit für Dokumente</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Anzahl der Zeilen, die Scribus bei der Bewegung des Mausrades scrollen soll</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radius des Bereichs, in dem es möglich ist, die Objektbegrenzungen zu aktivieren</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Anzahl der zuletzt geöffneten Dokumenten im Datei-Menü</translation>
    </message>
    <message>
        <source>Change the keyboard shortcuts to your preference</source>
        <translation type="obsolete">Tastaturkürzel einstellen</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standard-Verzeichnis für Dokumente</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation>Standard-Verzeichnis für ICC-Profile</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standard-Verzeichnis für Scripte</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standard-Größe der Seite</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standard-Ausrichtung der Seite in neuen Dokumente</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Breite der Seite, veränderbar, wenn Benutzerdefiniert ausgewählt ist</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Höhe der Seite, veränderbar, wenn Benutzerdefiniert ausgewählt ist</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Aktiviert doppelseitiges Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Die erste Seite im Dokument soll eine linke Seite sein</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Scribus speichert im gegebenen Intervall selbstständig eine Backup-Kopie 
des Dokuments mit der Erweiterung .bak</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Abstand zwischen zwei automatischen Sicherungen</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Abstand zwischen den Teillinien</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Abstand zwischen den Hauptlinien</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Abstand, in dem ein Objekt an die Hilfslinien einrastet</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Farbe der Teillinien</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Farbe der Hauptlinien</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Farbe der Hilfslinien</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Gitter im Hintergrund platzieren</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Gitter im Vordergrund platzieren</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Versatz über der Grundlinie auf einer Zeile</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relative Größe der tiefgestellen Buchstaben im Vergleich zur normalen Schriftgröße</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Versatz unter der Grundlinie auf einer Zeile</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relative Größe der hochgestellten Buchstaben im Vergleich zur normalen Schriftgröße</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relative Größe der Kapitälchen im Vergleich zur normalen Schriftgröße</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Prozentuale Vergrößerung des Zeilenabstandes zur Schriftgröße</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Eigenschaften der Text-Rahmen</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Eigenschaften der Bilder-Rahmen</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Eigenschaften der Formen</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Eigenschaften der Zoom-Level</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Eigenschaften des Linien-Werkzeuges</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Eigenschaften des Polygon-Werkzeuges</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Schriftart für einen neuen Text-Rahmen</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Schriftgröße für einen neuen Textrahmen</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Schriftfarbe</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Anzahl der Spalten in einem neuen Text-Rahmen</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Abstand zwischen den Spalten</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Vorschau der gewählten Schriftart</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Bild-Rahmen ermöglicht das Skalieren auf eine beliebige Größe</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Horizontale Skalierung des Bildes</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Vertikale Skalierung des Bildes</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Seitenverhältnisse beibehalten</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Bilder werden auf die Größe des Rahmens skaliert</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Bilder behalten automatisch ihre originalen Seitenverhältnisse</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Füllfarbe für Bild-Rahmen</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Intensität der Füllfarbe</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Linienfarbe der Form</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Intensität der Linienfarbe</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Füllfarbe der Form</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Linien-Stil der Form</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Linien-Breite der Form</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Kleinstmögliche Vergrößerungsstufe</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Größtmögliche Vergrößerungstufe</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Abstand zwischen zwei Zoom-Stufen</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Farbe der Linien</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Tonwert der Farbe</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Linien-Stil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Breite der Linien</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Anzahl der Ecken in Polygonen</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grad der Drehung des Polygons</translation>
    </message>
    <message>
        <source>Polygons will be convex rather than concave</source>
        <translation type="obsolete">Zeichnet ein konvexes Polygon</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Vorschau der gewählten Eigenschaften</translation>
    </message>
    <message>
        <source>Change the angles at which lines of the polygon join</source>
        <translation type="obsolete">Legt den Winkel fest, bei dem sich die Linien des Polygons verbinden</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Ändert die Größe der Vorschau im Bibliotheks-Fenster</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Wenn Doppelseiten aktiviert sind, werden zwei Seiten nebeneinander gezeigt</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Farbe des Seiten-Hintergrundes</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farbe für die Seiten-Ränder</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Zeigt den nicht-druckbaren Bereich in der Farbe des Seiten-Randes</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Aktiviert die Transparenz-Funktion mit dem PDF 1.4-Export</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Legt das Standard-Zoom-Einstellung fest</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Pfad zum Ghostscript-Interpreter</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Text bei EPS- und PDF-Anzeige weichzeichnen</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Grafiken ebi EPS- und PDF-Anzeige weichzeichnen</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Objekte außerhalb der Seitenränder auf den gedruckten oder exportierten Seiten nicht zeigen</translation>
    </message>
    <message>
        <source>Baseline Grid:</source>
        <translation type="obsolete">Grundlinien-Raster:</translation>
    </message>
    <message>
        <source>Baseline Offset:</source>
        <translation type="obsolete">Grundlinien-Versatz:</translation>
    </message>
    <message>
        <source>Save Contents on Changes</source>
        <translation type="obsolete">Inhalt bei Veränderungen sichern</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Bibliothek nach jeder Veränderung erneut speichern</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation>Pfad zum Bildbearbeitungsprogramm</translation>
    </message>
    <message>
        <source>Theme:</source>
        <translation type="obsolete">Thema:</translation>
    </message>
    <message>
        <source>Wheel-Jump:</source>
        <translation type="obsolete">Scroll-Intensität:</translation>
    </message>
    <message>
        <source>Grab-Radius:</source>
        <translation type="obsolete">Grab-Radius:</translation>
    </message>
    <message>
        <source>Horizontal Scaling:</source>
        <translation type="obsolete">Horizontale Größe:</translation>
    </message>
    <message>
        <source>Vertical Scaling:</source>
        <translation type="obsolete">Vertikale Größe:</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Grundlinien-Raster</translation>
    </message>
    <message>
        <source>On</source>
        <translation type="obsolete">Ein</translation>
    </message>
    <message>
        <source>Off</source>
        <translation type="obsolete">Aus</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation>Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation>Grundlinien-Raster nicht anzeigen</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>Type of Line:</source>
        <translation type="obsolete">Linienart:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Thema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Schrift&amp;größe:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Einstellungen für die Maus</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Rad-Vorschub:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>Fang&amp;radius:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Zuletzt verwendete Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Wechseln...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-Profile:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skripte:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Aus&amp;richtung:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>&amp;Aktiviert</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>&amp;Abstand der Teillinien:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Ab&amp;stand der Hauptlinien:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>&amp;Fangabstand der Hilfslinien:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>&amp;Farbe der Teillinien:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Fa&amp;rbe der Hauptlinien:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Farbe der &amp;Hilfslinien:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>&amp;Farbe des Grundlinienrasters:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>In den &amp;Vordergrund</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>E&amp;in</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>A&amp;us</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Versatz:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>&amp;Versatz:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Grundlinen&amp;raster:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Grundlinien-&amp;Versatz:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>Automatischer &amp;Zeilenabstand:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>&amp;Schriftartvorgabe:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Größen&amp;vorgabe:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Text&amp;farbe:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Spalten:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Abstand:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Linien&amp;farbe:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Tonwert:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Füll&amp;farbe:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>&amp;Tonwert:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation>Linien&amp;art:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linien&amp;breite:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mini&amp;mal:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ximal:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>Schritt&amp;weite:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Freie Skalierung</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Horizontale Skalierung:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Vertikale Skalierung:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Bild an &amp;Rahmen anpassen</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Seitenverhältnisse beibe&amp;halten</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Füll&amp;farbe:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Ec&amp;ken:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Conve&amp;x Polygon</source>
        <translation type="obsolete">Konve&amp;xes Polygon</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fa&amp;ktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Veränderungen beim &amp;Verlassen speichern</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Groß</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Seiten &amp;nebeneinander anzeigen</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Seitenfarben</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Hintergrund:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Ränder:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Nicht druck&amp;baren Rand in der Rahmenfarbe zeigen</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>PDF 1.4 &amp;Transparenzen aktivieren</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Größe des Displays an&amp;passen</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>Name der &amp;ausführbaren Datei:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>&amp;Text weichzeichnen</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>&amp;Bilder weichzeichnen</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Name der &amp;ausführbaren Datei:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Auf Sei&amp;tenränder beschneiden</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>A&amp;ußen:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Unterfarben&amp;reduktion anwenden</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>&amp;Vorlagen:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Zusätzliches Verzeichnis für Dokument-Vorlagen</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Faktor über&amp;nehmen</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Hier stellen Sie ein, wie stark konvex oder konkav das Polygon gezeichnet wird</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Ein negativer Wert steht für eine konkave (sternförmige) Figur,
ein positiver Wert für eine konkave Form</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Mit dieser Option werden ein paar Grautöne, die entstehen,
wenn Black aus Cyan, Magenta und Yellow gemischt wird, durch Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr nahe stehen. Diese Option kann zu besseren Bildern führen,
allerdings müssen Sie von Fall zu Fall entscheiden, wie Sie bessere Ergebnisse
bekommen. Außerdem reduziert UFR die Gefahr einer Übersättigung mit CMY-Farben.</translation>
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
        <translation type="unfinished">US-Letter</translation>
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
        <translation>Farb&amp;ton:</translation>
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
        <translation>&amp;Rot:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Grün:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bla&amp;u:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lphakanal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Grundfar&amp;ben</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Benutzerdefinierte Farben</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>Eigene Farben &amp;definieren &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>Zu benutzerdefinierten Farben &amp;hinzufügen</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Farbauswahl</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Datei kopieren oder verschieben</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Lesen: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Schreiben: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Dateiname:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Datei&amp;typ:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Eine Ebene nach oben</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Attribute</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Su&amp;chen in:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Zurück</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Neuen Ordner erstellen</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Liste</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Ausführlich</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Voransicht der Datei-Info</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Voransicht des Datei-Inhalts</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lesen/Schreiben</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Nur Lesen</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Nur Schreiben</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Gesperrt</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Link auf Datei</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Link auf Verzeichnis</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Link auf Spezialdatei</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Verzeichnis</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Spezialattribut</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Öffnen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Umbenennen</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>Erne&amp;ut laden</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Nach &amp;Name sortieren</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Nach &amp;Größe sortieren</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Nach &amp;Datum sortieren</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Unsortiert</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sortieren</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>&amp;Versteckte Dateien anzeigen</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>die Datei</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>das Verzeichnis</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>den symbolischen Link</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>%1 löschen</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Sind Sie sicher, dass Sie %1 &quot;%2&quot; löschen möchten?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Neues Verzeichnis 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Neues Verzeichnis</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Neues Verzeichnis %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Verzeichnis suchen</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Verzeichnisse</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Verzeichnis:</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Datei wurde nicht gefunden.
Überprüfen Sie Pfad und Dateinamen.</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>Schrift&amp;art</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Formatierung</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Du&amp;rchgestrichen</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Unterstrichen</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>&amp;Zeichensatz</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Schriftart auswählen</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Ausrichten</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Anpassen...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Über Qt&lt;/h3&gt;&lt;p&gt;Dieses Programm verwendet Qt Version %1&lt;/p&gt;&lt;p&gt;Qt ist ein multi-platform Framework zum Erstellen von GUI-Programmen in C++.&lt;/p&gt;&lt;p&gt;Qt bietet Portierungsmöglichkeiten mit nur einer Quellcode-Basis auf MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux und allen anderen großen kommerziellen Unix-Versionen.&lt;br&gt;Qt ist auch für eingebettete Systeme erhältlich.&lt;/p&gt;&lt;p&gt;Qt ist ein Produkt von Trolltech. Weitere Informationen finden Sie unter &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Initialisierung...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Wollen sie die Datei
%1
wirklich überschreiben?</translation>
    </message>
    <message>
        <source>Insert Special</source>
        <translation type="obsolete">Sonderzeichen einfügen</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Druckvorschau</translation>
    </message>
    <message>
        <source>Scripter Manual...</source>
        <translation type="obsolete">Scripter-Handbuch...</translation>
    </message>
    <message>
        <source>Execute Script...</source>
        <translation type="obsolete">Script ausführen...</translation>
    </message>
    <message>
        <source>Recent Scripts</source>
        <translation type="obsolete">Zuletzt verwendete Scripte</translation>
    </message>
    <message>
        <source>Show Console</source>
        <translation type="obsolete">Konsole zeigen</translation>
    </message>
    <message>
        <source>Script</source>
        <translation type="obsolete">Script</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Onlinereferenz</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Scripte (*.py);; Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Save Page as SVG...</source>
        <translation type="obsolete">Seite als SVG speichern...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-Bilder (*.svg *.svgz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-Bilder (*.svg);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>Import SVG-Image</source>
        <translation type="obsolete">Importiere SVG-Bild</translation>
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
        <source>Scribus Scripts</source>
        <translation type="obsolete">Scribus Scripte</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Fehler! Sie versuchen ein Objekt zu aktivieren, das nicht existiert!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Fehler! Sie versuchen, ein Objekt zu löschen, das nicht existiert!</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Fehler! Sie versuchen, bei einem nicht existierenden Objekt die Textfluss-Eigenschaften zu ändern!</translation>
    </message>
    <message>
        <source>Import SVG-Image...</source>
        <translation type="obsolete">SVG-Bild importieren...</translation>
    </message>
    <message>
        <source>Save As Image...</source>
        <translation type="obsolete">Als Bild speichern...</translation>
    </message>
    <message>
        <source>Export to bitmap</source>
        <translation type="obsolete">Als Bitmap exportieren</translation>
    </message>
    <message>
        <source>Error writting the ouptut file(s).</source>
        <translation type="obsolete">Fehler beim Exportieren.</translation>
    </message>
    <message>
        <source>Error writing the ouptut file(s).</source>
        <translation type="obsolete">Fehler beim Exportieren.</translation>
    </message>
    <message>
        <source>Hooray! Export successfull.</source>
        <translation type="obsolete">Export erfolgreich abgeschlossen.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Datei existiert bereits. Soll sie überschrieben werden?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>existiert bereits. Soll die Datei überschrieben werden?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Save as Image...</source>
        <translation type="obsolete">Als Bild speichern...</translation>
    </message>
    <message>
        <source>Export to image</source>
        <translation type="obsolete">Als Bild exportieren</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Fehler beim Schreiben der Datei(en).</translation>
    </message>
    <message>
        <source>Export to Image successful</source>
        <translation type="obsolete">Export erfolgreich durchgeführt</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Als Bild speichern</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Export erfolgreich.</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Fehler beim Schreiben der Datei(en).</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Schriftartenvorschau</translation>
    </message>
    <message>
        <source>New From Template...</source>
        <translation type="obsolete">Neu von Vorlage...</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Newsletter</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Broschüren</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Kataloge</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Flyer</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Karten</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Briefköpfe</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Briefumschläge</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visitenkarten</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalender</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Werbung</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Beschriftungen</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menüs</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programme</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF-Formulare</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Magazine</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Poster</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Ankündigungen</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Textdokumente</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Hefter</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Eigene Vorlagen</translation>
    </message>
    <message>
        <source>Save as Template...</source>
        <translation type="obsolete">Als Vorlage speichern...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Speichern unter...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Drucken...</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Neu von &amp;Vorlage...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Neu</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-Präsentationen</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>&amp;Als Vorlage speichern...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Sonderzeichen einfügen</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Als &amp;Bild speichern...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Schriftarten&amp;vorschau</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>D&amp;ruckvorschau</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Drucken...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Handbuch für den &amp;Scripter...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scripts für Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>Script &amp;ausführen...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Zuletzt verwendete Scripts</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Konsole &amp;zeigen</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Seite als &amp;SVG speichern...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>&amp;SVG importieren...</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>&amp;EPS/PS importieren...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alle unterstützten Formate (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Oook! Wrong arguments! Call: </source>
        <translation type="obsolete">Fehler: Falsches Argument! Aufruf:</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Autsch! Sie versuchen ein Bild in ein Objekt zu laden, das nicht existiert oder nicht ausgewählt ist!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Fehler: Sie versuchen ein Objekt zu (ent)sperren, das gar nicht existiert. Außerdem ist gar keins ausgewählt.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Fehler: Sie versuchen ein Objekt zu aktivieren, das gar nicht exisitiert! Außerdem ist nichts ausgewählt.</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Text importieren</translation>
    </message>
    <message>
        <source>Importer</source>
        <translation type="obsolete">Importfilter</translation>
    </message>
    <message>
        <source>Choose the importer to use</source>
        <translation type="obsolete">Wählen Sie den Importfilter aus</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle unterstützten Formate</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML-Dateien</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Textdateien</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Kommaseparierte Dateien</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CVS_data</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_header</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation>Vorlage:</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
externe Links</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Die Schriftart %1 ist defekt und wird ab sofort ignoriert</translation>
    </message>
    <message>
        <source>OO.o Writer Documents</source>
        <translation type="obsolete">OO.o Textverarbeitung</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Textfilter</translation>
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
        <translation type="unfinished">Katalanisch</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Tschechisch</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Dänisch</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Holländisch</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Englisch</translation>
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
        <translation type="unfinished">Deutsch</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Finnisch</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Französisch</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Griechisch</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Ungarisch</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Italienisch</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Litauisch</translation>
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
        <translation type="unfinished">Polnisch</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Russisch</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Schwedisch</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Spanisch</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Slowakisch</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Slovenisch</translation>
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
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>System-Menü</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Aufrollen</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Herabrollen</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Wiederherstellen</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimieren</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximieren</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>Ver&amp;schieben</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe ändern</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>M&amp;inimieren</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximieren</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Im &amp;Vordergrund halten</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimieren</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Wiederherstellen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Aufrollen</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Herabrollen</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Dokument einrichten</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="obsolete">Oben:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="obsolete">Links:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="obsolete">Unten:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="obsolete">Rechts:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Doppelseiten</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Linke Seite zuerst</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation type="obsolete">Innen:</translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation type="obsolete">Außen:</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Aktiviert das doppelseitige Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Macht die erste Seite des Dokuments zu einer linken Seite</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
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
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>A&amp;ußen:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Seitenformat</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Größe:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="unfinished">Ausrichtung:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Querformat</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Breite:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished">Höhe:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished">Nummer der &amp;ersten Seite:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilvorlage für aktuellen Absatz</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Stil verändern</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Tonwert der Textfarbe</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Füllfarbe verändern</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Textumrissfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Tonwert der Textumrissfarbe</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Umrissfarbe verändern</translation>
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
        <translation>Schriftart des Objekts</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Zeichenbreiten skalieren</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Schriftart verändern</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation>Kerning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Manuelles Kerning</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Zeicheneinstellungen bearbeiten</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie #%1 von</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Creates a new Document</source>
        <translation type="obsolete">Erzeugt ein neues Dokument</translation>
    </message>
    <message>
        <source>Opens a Document</source>
        <translation type="obsolete">Öffnet ein Dokument</translation>
    </message>
    <message>
        <source>Saves the Current Document</source>
        <translation type="obsolete">Speichert das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Closes the Current Document</source>
        <translation type="obsolete">Schließt das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Prints the Current Document</source>
        <translation type="obsolete">Druckt das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Saves the Current Document as PDF</source>
        <translation type="obsolete">Speichert das aktuelle Dokument als PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Suche nach Schriftarten</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation>Auf Ihrem System sind keine Postscript-Fonts installiert</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Programm wird jetzt beendet</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fataler Fehler</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Weiche Trennung</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Links ausrichten</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Rechts ausrichten</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Zentrieren</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Seitennummer einfügen</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Text auf Pfad setzen</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Zeige Ebenen</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Javascripte...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Rückgängig</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Zeige Seitenpalette</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Sperren/Entsperren</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Fester Zwischenraum</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Voreinstellungen werden geladen</translation>
    </message>
    <message>
        <source>Getting ICC-Profiles</source>
        <translation type="obsolete">ICC-Profile werden geladen</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Silbentrennung wird initialisiert</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Tastaturkürzel werden gelesen</translation>
    </message>
    <message>
        <source>Color Management...</source>
        <translation type="obsolete">Farbmanagement...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Bibliothek wird geladen</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Plugins werden geladen</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Neu</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Öffnen...</translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="obsolete">Zuletzt benutzte Dateien</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Speichern unter...</translation>
    </message>
    <message>
        <source>Revert to Saved</source>
        <translation type="obsolete">Alte Fassung laden</translation>
    </message>
    <message>
        <source>Collect for Output...</source>
        <translation type="obsolete">Für Ausgabe sammeln...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Bild/Text laden...</translation>
    </message>
    <message>
        <source>Insert Page...</source>
        <translation type="obsolete">Seite einfügen...</translation>
    </message>
    <message>
        <source>Import...</source>
        <translation type="obsolete">Importieren...</translation>
    </message>
    <message>
        <source>Save Text...</source>
        <translation type="obsolete">Text speichern...</translation>
    </message>
    <message>
        <source>Save Page as EPS...</source>
        <translation type="obsolete">Seite als EPS sichern...</translation>
    </message>
    <message>
        <source>Save as PDF...</source>
        <translation type="obsolete">Als PDF sichern...</translation>
    </message>
    <message>
        <source>Export...</source>
        <translation type="obsolete">Exportieren...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Information...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Dokument einrichten...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Drucken...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Ausschneiden</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Einfügen</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>Search/Replace...</source>
        <translation type="obsolete">Suchen/Ersetzen...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Farben...</translation>
    </message>
    <message>
        <source>Paragraph Styles...</source>
        <translation type="obsolete">Stilvorlagen...</translation>
    </message>
    <message>
        <source>Line Styles...</source>
        <translation type="obsolete">Linienstile...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Stilvorlagen...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Musterseiten...</translation>
    </message>
    <message>
        <source>General...</source>
        <translation type="obsolete">Allgemein...</translation>
    </message>
    <message>
        <source>Preferences...</source>
        <translation type="obsolete">Voreinstellungen...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Schriften...</translation>
    </message>
    <message>
        <source>Hyphenator...</source>
        <translation type="obsolete">Silbentrennung...</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation type="obsolete">Voreinstellungen</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Neue Schriftart auswählen</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Duplizieren</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Mehrfach Duplizieren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Gruppieren</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Gruppe auflösen</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Sperren</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>In den Hintergrund</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>In den Vordergrund</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Weiter nach unten</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Weiter nach oben</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Abstand/Ausrichtung...</translation>
    </message>
    <message>
        <source>Edit Frame</source>
        <translation type="obsolete">Rahmen bearbeiten</translation>
    </message>
    <message>
        <source>Shape</source>
        <translation type="obsolete">Form</translation>
    </message>
    <message>
        <source>Detach Text from Path</source>
        <translation type="obsolete">Text vom Pfad lösen</translation>
    </message>
    <message>
        <source>Combine Polygons</source>
        <translation type="obsolete">Polygone vereinen</translation>
    </message>
    <message>
        <source>Split Polygons</source>
        <translation type="obsolete">Polygone teilen</translation>
    </message>
    <message>
        <source>Convert to Outlines</source>
        <translation type="obsolete">In Polygone verwandeln</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Einfügen...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Löschen...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Verschieben...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Musterseite anwenden...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Hilfslinien bearbeiten...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Ganze Seite</translation>
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
        <source>Actual Size</source>
        <translation type="obsolete">Originalgröße</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Miniaturen</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Ränder verbergen</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Rahmen verbergen</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Bilder verbergen</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Raster zeigen</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Am Raster ausrichten</translation>
    </message>
    <message>
        <source>Hide Guides</source>
        <translation type="obsolete">Hilfslinien verbergen</translation>
    </message>
    <message>
        <source>Snap to Guides</source>
        <translation type="obsolete">Hilfslinen magnetisch</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>PDF-Tools</source>
        <translation type="obsolete">PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Übersicht</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Bibliothek</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="obsolete">Ebenen</translation>
    </message>
    <message>
        <source>Page Palette</source>
        <translation type="obsolete">Seitenpalette</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation type="obsolete">Lesezeichen</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Bilder verwalten</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Silbentrennung</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Über Scribus</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Online-Hilfe...</translation>
    </message>
    <message>
        <source>Tool-Tips</source>
        <translation type="obsolete">Tool-Tips</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <source>Item</source>
        <translation type="obsolete">Objekt</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Seite</translation>
    </message>
    <message>
        <source>View</source>
        <translation type="obsolete">Ansicht</translation>
    </message>
    <message>
        <source>Extras</source>
        <translation type="obsolete">Extras</translation>
    </message>
    <message>
        <source>Windows</source>
        <translation type="obsolete">Fenster</translation>
    </message>
    <message>
        <source>Help</source>
        <translation type="obsolete">Hilfe</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="obsolete">Links</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="obsolete">Zentriert</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="obsolete">Rechts</translation>
    </message>
    <message>
        <source>Block</source>
        <translation type="obsolete">Blocksatz</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation type="obsolete">Erzwungener Blocksatz</translation>
    </message>
    <message>
        <source>Other...</source>
        <translation type="obsolete">Andere...</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Durchgestrichen</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitälchen</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Umrandung</translation>
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
        <source>Ready</source>
        <translation>Fertig</translation>
    </message>
    <message>
        <source>Cascade</source>
        <translation type="obsolete">Überlappen</translation>
    </message>
    <message>
        <source>Tile</source>
        <translation type="obsolete">Kacheln</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Rahmen anzeigen</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Bild laden...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertieren</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Text laden...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Schrift</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Effekte</translation>
    </message>
    <message>
        <source>Tabulators...</source>
        <translation type="obsolete">Tabulatoren...</translation>
    </message>
    <message>
        <source>Alignment</source>
        <translation type="obsolete">Ausrichtung</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Entsperren</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Lade...</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textdateien (*.txt);;Alle Dateien(*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Kann die Datei
%1
nicht speichern</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Speichere...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Drucke...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Drucken fehlgeschlagen!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus Handbuch</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation type="obsolete">Ränder anzeigen</translation>
    </message>
    <message>
        <source>Show Images</source>
        <translation type="obsolete">Bilder anzeigen</translation>
    </message>
    <message>
        <source>Hide Grid</source>
        <translation type="obsolete">Raster verbergen</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="obsolete">Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Größe:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Tonwert:</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Die folgenden Programme fehlen:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS-Images</source>
        <translation type="obsolete">Ghostscript: Sie können keine EPS-Bilder benutzen</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS-Bilder (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Lade:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Passe Farben an</translation>
    </message>
    <message>
        <source>Undo Delete Object</source>
        <translation type="obsolete">Rückgängig: Objekt löschen</translation>
    </message>
    <message>
        <source>Undo Object Move</source>
        <translation type="obsolete">Rückgängig: Objekt bewegen</translation>
    </message>
    <message>
        <source>Undo Object Change</source>
        <translation type="obsolete">Rückgängig: Objekt ändern</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Englisch</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Deutsch</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spanisch</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italienisch</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Französisch</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisch</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dänisch</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slowakisch</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungarisch</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tschechisch</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holländisch</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugiesisch</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukrainisch</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polnisch</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griechisch</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalanisch</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finnisch</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus ist abgestürzt</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Absturz durch Signal #%1</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle unterstützten Formate</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irisch</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Neues Dokument anlegen</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Vorhandenes Dokument öffnen</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Aktives Dokument speichern</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Schließt das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Druckt das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Speichert das aktuelle Dokument als PDF</translation>
    </message>
    <message>
        <source>Append Text...</source>
        <translation type="obsolete">Text anfügen...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Style</source>
        <translation type="obsolete">S&amp;til</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Seite</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Ansicht</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Werkzeuge</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Fenster</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hilfe</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Tool&amp;bars</source>
        <translation type="obsolete">Tool&amp;bars</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Grundlinien-Raster verbergen</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Einige Objekte sind gesperrt.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Alle sperren</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Alle entsperren</translation>
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
        <translation>Litauisch</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Schwedisch</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovenisch</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts...</source>
        <translation type="obsolete">Tastaturkürzel...</translation>
    </message>
    <message>
        <source>Scribus Manual...</source>
        <translation type="obsolete">Handbuch für Scribus...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Farbmanagement...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>Ö&amp;ffnen...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>&amp;Zuletzt verwendete öffnen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Neu la&amp;den</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>&amp;Für Ausgabe sammeln...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>Text/Bild &amp;laden...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Text anfügen...</translation>
    </message>
    <message>
        <source>Insert &amp;Page...</source>
        <translation type="obsolete">&amp;Seite einfügen...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>&amp;Text speichern...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Seite als &amp;EPS speichern...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Als &amp;PDF speichern...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>E&amp;xportieren</translation>
    </message>
    <message>
        <source>Document &amp;Info...</source>
        <translation type="obsolete">&amp;Information...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokument &amp;einrichten...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Drucken...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Beenden</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig machen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Einfügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Alles aus&amp;wählen</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Suchen &amp; Ersetzen...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Farben...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Absatzstile...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Linien-Stile...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Musterseiten...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Javascripts...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Mehrfach Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Gruppieren</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Gruppe auf&amp;lösen</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Sperren</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>In den &amp;Vordergrund</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Weiter nach hin&amp;ten</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Weiter nach vor&amp;n</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Verteilen &amp;&amp; &amp;Ausrichten...</translation>
    </message>
    <message>
        <source>&amp;Edit Frame</source>
        <translation type="obsolete">Form &amp;bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Text an Pfad aus&amp;richten</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Text von Pfad l&amp;ösen</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Polygone &amp;kombinieren</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Polygone &amp;teilen</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>In &amp;Pfade umwandeln</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>Ein&amp;fügen...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Löschen...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>Ver&amp;schieben...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Musterseite anwenden...</translation>
    </message>
    <message>
        <source>&amp;Manage Guides...</source>
        <translation type="obsolete">&amp;Hilfslinien bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Ganze Seite</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Miniaturen</translation>
    </message>
    <message>
        <source>Hide &amp;Margins</source>
        <translation type="obsolete">&amp;Ränder verbergen</translation>
    </message>
    <message>
        <source>Hide &amp;Frames</source>
        <translation type="obsolete">Ra&amp;hmen verbergen</translation>
    </message>
    <message>
        <source>Hide &amp;Images</source>
        <translation type="obsolete">&amp;Bilder verbergen</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Ra&amp;ster zeigen</translation>
    </message>
    <message>
        <source>Snap to &amp;Grid</source>
        <translation type="obsolete">Am Raster &amp;ausrichten</translation>
    </message>
    <message>
        <source>Hide G&amp;uides</source>
        <translation type="obsolete">&amp;Hilfslinien verbergen</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Magnetische Hilfslinien</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Eigenschaften</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>Über&amp;sicht</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>Biblio&amp;thek</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>E&amp;benen</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>Seiten&amp;palette</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Bilder &amp;verwalten</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Silbentrennung anwenden</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Tool&amp;tipps</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>&amp;PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Tooltipps</translation>
    </message>
    <message>
        <source>&amp;Preferences...</source>
        <translation type="obsolete">&amp;Voreinstellungen...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>Schrift&amp;arten...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Silbentrennung...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>Tasten&amp;kürzel...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>Ü&amp;ber Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Über &amp;Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scri&amp;bus Handbuch...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Stil</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Links</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Zentriert</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Rechts</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blocksatz</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Erzwungener Blocksatz</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Andere...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Hintereinander</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Nebeneinander</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farben</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Invertieren</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Text &amp;laden...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>Schrift&amp;art</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekte</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Ausrichtung</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Ton&amp;wert</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatoren...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Ent&amp;sperren</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>&amp;Bilder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;Ränder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Ra&amp;hmen zeigen</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Hide &amp;Grid</source>
        <translation type="obsolete">Ra&amp;ster verbergen</translation>
    </message>
    <message>
        <source>Hide &amp;Baseline Grid</source>
        <translation type="obsolete">&amp;Grundlinien-Raster verstecken</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript: Sie können keine EPS-Dateien einfügen</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Seiten &amp;importieren...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Seiten werden importiert...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Seiten importieren</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Sie versuchen, mehr Seiten zu importieren als im aktiven Dokument vorhanden sind.&lt;/p&gt;Wählen Sie bitte aus:&lt;br&gt;&lt;ul&gt;&lt;li&gt;fehlende Seiten &lt;b&gt;erstellen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;Bis zur letzten vorhandenen Seite &lt;b&gt;einfügen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;&lt;b&gt;Abbrechen&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Erstellen</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importvorgang erfolgreich abgeschlossen</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nichts zum Importieren gefunden</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Ma&amp;gnetisches Raster</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Voreinstellungen...</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>ICC-Profile werden geladen</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Hilfslinien bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Ton&amp;wert:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>Dokument&amp;informationen...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>&amp;Rückgängig: Objekt löschen</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>&amp;Rückgängig: Objekt verschieben</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>&amp;Rückgängig: Objekt ändern</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Form bearbeiten</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>Die Datei %1 wurde nicht mit Scribus erstellt</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="obsolete">Seite </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> von %1</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Ebene</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
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
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Leave Anyway</source>
        <translation type="obsolete">Verlassen</translation>
    </message>
    <message>
        <source>Close Anyway</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Datei:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>wurde seit dem letzten Speichern verändert.</translation>
    </message>
    <message>
        <source>Save Now</source>
        <translation type="obsolete">Jetzt sichern</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>&amp;Ignorieren</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Trotzdem sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Jetzt speichern</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Seitenvorschau anzeigen</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Bezeichnungen der Vorlagen anzeigen</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Suchen/Ersetzen</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Suchen nach:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Absatzstil</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Schriftart</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Füllfarbe</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Randfarbe</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Ersetzen durch:</translation>
    </message>
    <message>
        <source>Whole Word</source>
        <translation type="obsolete">Ganzes Wort</translation>
    </message>
    <message>
        <source>Ignore Case</source>
        <translation type="obsolete">Groß-/Kleinschreibung ignorieren</translation>
    </message>
    <message>
        <source>Search</source>
        <translation type="obsolete">Suchen</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation type="obsolete">Ersetzen</translation>
    </message>
    <message>
        <source>Replace All</source>
        <translation type="obsolete">Alles ersetzen</translation>
    </message>
    <message>
        <source>Leave</source>
        <translation type="obsolete">Verlassen</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Suche beendet</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Zentriert</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blocksatz</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Erzw. Blocksatz</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Ganzes Wort</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Groß-/&amp;Kleinschreibung ignorieren</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Suchen</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Ersetzen</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>&amp;Alles ersetzen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Löschen</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Seiten anordnen</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Verfügbare Vorlagen:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Seiten des Dokuments:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Doppelseiten</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Linke Seite zuerst</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Ziehen Sie die Seiten oder Vorlagen auf den Mülleimer, um sie zu löschen.</translation>
    </message>
    <message>
        <source>This previews all the pages of your Document.</source>
        <translation type="obsolete">Hier sind alle Seiten Ihres Dokuments dargestellt.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Hier sind Ihre Vorlagen, um eine neue Seite zu erzeugen
ziehen Sie eine Vorlage auf die Seitenansicht unten.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Vorschau aller Seiten des Dokuments.</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Felder auswählen</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Verfügbare Felder</translation>
    </message>
    <message>
        <source>&gt;&gt;</source>
        <translation type="obsolete">&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation type="obsolete">&lt;&lt;</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Ausgewählte Felder</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Andere...</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Tonwert:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Ton&amp;wert:</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Stilvorlagen bearbeiten</translation>
    </message>
    <message>
        <source>Append</source>
        <translation type="obsolete">Anfügen</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplizieren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Speichern</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Neue Stilvorlage</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want do delete this Style?</source>
        <translation type="obsolete">Wollen Sie diese Stilvorlage wirklich löschen?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>An&amp;fügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Wollen Sie diesen Absatzstil wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Textbearbeitung</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Neu</translation>
    </message>
    <message>
        <source>Save to File...</source>
        <translation type="obsolete">In Datei speichern...</translation>
    </message>
    <message>
        <source>Load from File...</source>
        <translation type="obsolete">Von Datei laden...</translation>
    </message>
    <message>
        <source>Save and Exit</source>
        <translation type="obsolete">Speichern und Beenden</translation>
    </message>
    <message>
        <source>Exit without Saving</source>
        <translation type="obsolete">Beenden ohne Speichern</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Rückgängig</translation>
    </message>
    <message>
        <source>Redo</source>
        <translation type="obsolete">Wiederherstellen</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Ausschneiden</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopieren</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Einfügen</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation type="obsolete">Stilvorlagen bearbeiten...</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Stil</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Text</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Aktueller Absatz:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Wörter:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Zeichen:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Gesamt:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Absätze:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Wollen Sie wirklich alle Änderungen verlieren?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Wollen Sie wirklich den ganzen Text löschen?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textdateien (*.txt);;Alle Dateien(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="obsolete">Links</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="obsolete">Zentriert</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="obsolete">Rechts</translation>
    </message>
    <message>
        <source>Block</source>
        <translation type="obsolete">Blocksatz</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation type="obsolete">Erzw. Blocksatz</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Textrahmen auffrischen</translation>
    </message>
    <message>
        <source>Search/Replace...</source>
        <translation type="obsolete">Suchen/Ersetzen...</translation>
    </message>
    <message>
        <source>Do you want save your changes?</source>
        <translation type="obsolete">Wollen Sie Ihre Änderungen sichern?</translation>
    </message>
    <message>
        <source>Insert Special...</source>
        <translation type="obsolete">Sonderzeichen einfügen...</translation>
    </message>
    <message>
        <source>Insert Special</source>
        <translation type="obsolete">Sonderzeichen einfügen</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Wollen Sie die Änderungen speichern?</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Änderungen übernehmen</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Änderungen ignorieren</translation>
    </message>
    <message>
        <source>Save Document</source>
        <translation type="obsolete">Dokument speichern</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">Sonder&amp;zeichen einfügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Text neu vom Text-Rahmen</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>In Datei &amp;speichern...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>Von Datei &amp;laden...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>&amp;Dokument speichern</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>Änderungen &amp;übernehmen</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Änderungen &amp;ignorieren</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>&amp;Sonderzeichen einfügen...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Textrahmen auffrischen</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Alles aus&amp;wählen</translation>
    </message>
    <message>
        <source>Clears all Text</source>
        <translation type="obsolete">Text löschen</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Text von Datei laden</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Text in Datei speichern</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Text aus Textrahmen neu laden</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Suchen &amp; Ersetzen...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Stil&amp;vorlagen bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Schriftarten&amp;vorschau...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Hintergrund...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Schriftart...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Kompletten Text löschen</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Suchen und Ersetzen</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Schriftarten&amp;vorschau</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Intelligente Textmarkierung</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitälchen</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>Strikeout</source>
        <translation type="obsolete">Durchgestrichen</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation>Umrandet</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Durchgestrichen</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OO.o Writer Importer Options</source>
        <translation type="obsolete">Optionen für den OO.o-Importer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation>Absatzstile aktualisieren</translation>
    </message>
    <message>
        <source>If there already exists a paragraph style with the same
name than current OO.o document&apos;s paragraph should the style
in Scribus be edited to match the one being imported
or left untouched</source>
        <translation type="obsolete">Wenn in Ihrem Dokument bereits ein Absatzstil mit gleichem Namen existiert
wie in dem OO.o-Dokument, können Sie hier einstellen ob der Stil in Scribus mit dem von OO.o
aktualisiert wird oder unverändert bleibt</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Name der Datei vor jeden Absatzstil anfügen</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scirubs</source>
        <translation type="obsolete">Aktivieren Sie diese Option, wenn jeder Absatzstil auch den
Namen der Ursprungsdatei tragen soll</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Einstellungen merken</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OO.o document and
never ask your confirmation again</source>
        <translation type="obsolete">Aktivieren Sie diese Option, wenn Sie nicht mehr nach diesen Einstellungen gefragt werden möchten</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation>Wenn ein Absatzstil mit dem gleichen Namen bereits in Scribus existiert,
wird er entweder an den Stil aus dem OpenOffice.org Dokument angepasst
oder unverändert gelassen</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation>Name des Dokuments als Suffix an die Namen der
Absatzstile anhängen</translation>
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
        <translation>Tabulatoren bearbeiten</translation>
    </message>
    <message>
        <source>Position:</source>
        <translation type="obsolete">Position:</translation>
    </message>
    <message>
        <source>Alignment:</source>
        <translation type="obsolete">Ausrichtung:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="obsolete">Entfernen</translation>
    </message>
    <message>
        <source>Add</source>
        <translation type="obsolete">Hinzufügen</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="obsolete">Links</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="obsolete">Rechts</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation type="obsolete">Punkt</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation type="obsolete">Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="obsolete">Zentriert</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Zentriert</translation>
    </message>
    <message>
        <source>Position:</source>
        <translation type="obsolete">Position:</translation>
    </message>
    <message>
        <source>First Line:</source>
        <translation type="obsolete">Erste Zeile:</translation>
    </message>
    <message>
        <source>Indent:</source>
        <translation type="obsolete">Einzug:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Alle löschen</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Einzug für die erste Zeile des Absatzes</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Einzug von links für den ganzen Absatz</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Alle Tabulatoren löschen</translation>
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
        <translation>&amp;Position:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>&amp;Erste Zeile:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">Ein&amp;zug:</translation>
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
        <translation>Übersicht</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
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
        <translation>Schrift:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polyline</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Pfadtext</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Gruppe</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Wert einfügen</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Geben Sie einen Wert ein und klicken Sie auf OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Geben Sie einen Wert ein und klicken Sie auf OK</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Wert an das Script senden</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Objekte auswählen</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Textrahmen einfügen</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Bild einfügen</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Eigenschaften...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Polygone einfügen</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Linien einfügen</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Bezierlinien einfügen</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Freihand Linie einfügen</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Objekt drehen</translation>
    </message>
    <message>
        <source>Zooms in or out</source>
        <translation type="obsolete">Vergrößern oder verkleinern</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Rahmeninhalt bearbeiten</translation>
    </message>
    <message>
        <source>Edit the Text with the Story Editor</source>
        <translation type="obsolete">Text mit dem Story Editor bearbeiten</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Textrahmen verbinden</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Textrahmenverbindung lösen</translation>
    </message>
    <message>
        <source>Draws various Shapes</source>
        <translation type="obsolete">Zeichnet verschiedene Formen</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Vergrößern oder verkleinern</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Text mit dem Story Editor bearbeiten</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Zeichnet verschiedene Formen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Tabelle einfügen</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Entfernungen abmessen</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF-Tools</source>
        <translation type="obsolete">PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Insert PDF-Fields</source>
        <translation type="obsolete">PDF-Felder einfügen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Verknüpfung</translation>
    </message>
    <message>
        <source>Insert PDF-Annotations</source>
        <translation type="obsolete">PDF-Anmerkungen einfügen</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Schaltfläche</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textfeld</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kontrollkästchen</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinationsfeld</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listenfeld</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>PDF-Felder einfügen</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>PDF Anmerkungen einfügen</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Zeichen auswählen:</translation>
    </message>
    <message>
        <source>Insert</source>
        <translation type="obsolete">Einfügen</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Löschen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Schließen</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Zeichen an der Cursorposition einfügen</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Gewählte Markierung(en) löschen.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Diesen Dialog schließen und zur Textbearbeitung zurückkehren.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>EIn&amp;fügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Wählen Sie den Importfilter aus</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisch</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation>Nur Text importieren</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Text ohne Formatierung importieren</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importfilter:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodierung:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Wählen Sie den Importfilter aus</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Verknüpfung mit Importfilter speichern</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Speichert die Verknüpfung zwischen Dateierweiterung und
Importfilter. Sie werden nicht mehr nach einem Importer
für diesen Dateityp gefragt.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Neu von Vorlage</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Verwendungszweck</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Erstellt mit</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="obsolete">Entfernen</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Öffnen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Vorlagen herunterladen</translation>
    </message>
    <message>
        <source>Document templates can be found in http://www.scribus.net/ Downloads section.</source>
        <translation type="obsolete">Sie finden weitere Vorlagen im Downloadbereich von http://www.scribus.net.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Vorlagen installieren</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Bitte entpacken Sie die Vorlage nach ~/.scribus/templates für den aktuellen Benutzer oder nach PREFIX/share/scribus/templates für alle Benutzer.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Vorlage vorbereiten</translation>
    </message>
    <message>
        <source>Make sure images you use can be used anywhere. Fonts must be checked for this as well. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="obsolete">Die von Ihnen verwendeten Bilder müssen überall benutzt werden können. Auch Fonts müssen ausgewählt sein. Wenn Sie Schriftarten nicht mit verbreiten können, dann lassen Sie sie beim Speichern aus.</translation>
    </message>
    <message>
        <source>Template creator should also make sure that the Installing Templates applies to her/his templates as well. Which means a user should download a template package and be able to extract them to the template directory and start using them.</source>
        <translation type="obsolete">Wenn Sie selber Vorlagen bereitstellen, sollten Sie vorher ausprobieren, ob sie auch korrekt funktionieren. Das heißt, wenn Sie die Vorlage ins angegebene Verzeichnis kopieren, sollte sie sofort einsatzbereit sein.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Vorlage entfernen</translation>
    </message>
    <message>
        <source>Removing a template from the NFT dialog will only remove the entry from the template.xml. It will not delete the document files. Popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation type="obsolete">Wenn Sie eine Vorlage hier entfernen, wird nur der Eintrag in template.xml gelöscht, die Vorlagendateien an sich bleiben erhalten. Außerdem erscheint das Popup-Menü mit Entfernen nur, wenn sie Schreibrechte auf template.xml besitzen.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>template.xml übersetzen</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that&apos;s present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. Copy must locate in the same directory than the original template.xml for NFT being able to load it.</source>
        <translation type="obsolete">Kopieren Sie eine bestehende template.xml nach template.lang_COUNTRY.xml, wobei Sie denselben Ländercode benutzen sollten wie die qm-Datei für Ihre Sprache auch hat. Außerdem muss die Kopie im selben Verzeichnis wie das Original liegen.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Weitere Dokumentvorlagen finden Sie unter http://www.scribus.net/ im Download-Bereich.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Sie müssen sicherstellen, dass die Bilder und Fonts frei benutzt werden können. Wenn Sie Fonts nicht verbreiten dürfen, dann binden Sie diese beim Export als Vorlage nicht ein.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Wenn Sie Vorlagen erstellen, sollten Sie darauf achten, dass Sie die Informationen in dem Abschnitt &quot;Vorlagen installieren&quot; beachten. Das bedeutet, der Benutzer sollte in der Lage sein, die Vorlage einfach in das entsprechende Verzeichnis zu kopieren und zu benutzen.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Wenn Sie eine Vorlage von dieser Liste entfernen, wird nur der entsprechende Eintrag in der template.xml gelöscht, nicht die Vorlage selber. Sie können Vorlagen nur löschen, wenn Sie Schreibrechte auf template.xml besitzen.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopieren Sie die existierende template.xml nach template.lang_COUNTRY.xml (selber Ländercode wie bei den qm-Dateien), zum Beispiel template.fi.xml für Finnisch. Diese Kopie muss sich im selben Verzeichnis wie template.xml befinden.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Als Vorlage speichern</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategorie</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Verwendungszweck</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Mehr Details</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Weniger Details</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>US-Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Filter erstellen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Gespeicherten Filter auswählen</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Geben Sie einen Namen für diesen Filter ein</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Name des Filters</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Diese Zeile aktivieren oder deaktivieren</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Diese Zeile entfernen</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Neue Zeile hinzufügen</translation>
    </message>
    <message>
        <source>to</source>
        <translation>bis</translation>
    </message>
    <message>
        <source>and</source>
        <translation>und</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>Ergebnisse entfernen</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>Ergebnisse nicht entfernen</translation>
    </message>
    <message>
        <source>words</source>
        <translation>Worte</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Ersetzen</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Linker Wert ist ein regulärer Ausdruck</translation>
    </message>
    <message>
        <source>with</source>
        <translation>mit</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>Absatzstil</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>Alle Instanzen von</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>Alle Absätze</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>Absätze beginnen mit</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>Absätze mit weniger als</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>Absätze mit mehr als</translation>
    </message>
</context>
</TS>
