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
        <translation>Id izgradnje:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Finščina</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Brazilščina</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Različica Scribusa %1
 %2 %3</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation>%1. %2 %3 </translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galščina</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Češčina</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Valižanščina:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Pomagali so:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Poljščina:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ukrajinščina</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Baskovščina:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Slovaščina:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italijanščina:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Danščina:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Litvanščina:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Različica za Windows:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Turščina:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Ruščina:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Slovenščina:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Madžarščina:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Francoščina:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Bolgarščina:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norveščina:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Nemščina:</translation>
    </message>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">O Scribusu%1%2</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Tu lahko vidite različico, datum priprave in\n
v Scribus vključene knjižnice\n
C-C-T pomeni podporo za: C=CUPS C=littlecms T=TIFF.\n
Manjkajoča knjižnica je označena z zvezdico ( * )</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;O programu</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;vtorji</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Prevajalci</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Na spletu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
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
        <translation type="unfinished">Spletna pomoč</translation>
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
        <translation>Napredne možnosti</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Ustvari Postscript 3. ravni</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Ustvari Postscript 2. ravni; pazite,\n
to so lahko ogromne datoteke</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Ustvari Postscript 1. ravni; pazite,\n
to so lahko ogromne datoteke</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>&amp;Vodoravno prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>&amp;Navpično prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Uveljavi profile &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>Postscript &amp;1. ravni</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>Postscript &amp;2. ravni</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>Postscript &amp;3. ravni</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Uveljavi &amp;odstranjevanje barve pod</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation> pal</translation>
    </message>
    <message>
        <source> mm</source>
        <translation> mm</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Poravnaj</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodoravno</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Z vrha</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Po sredi</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Z desne</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Z leve</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Od spodaj</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Distribute/Align</source>
        <translation>Razporedi/Poravnaj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Uveljavi</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Med:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>Po&amp;ravnaj</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>Raz&amp;postavitev</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>&amp;Enakomerno razporedi</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>V&amp;mes:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>&amp;Ne spreminjaj</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Po&amp;ravnaj</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Raz&amp;postavitev</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>&amp;Enakomerno razporedi</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;Ne spreminjaj</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Besedilo poravnaj po levi</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Besedilo vsiljeno poravnaj enakomerno</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Besedilo poravnaj sredinsko</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Besedilo poravnaj enakomerno</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Besedilo poravnaj po desni</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>vsota</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cilj</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Potisni</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Zvezda</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tanek</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Čas</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Široko</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Dodaj simbol valute</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Pošlji obrazec</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Omejitev</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Preveri</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Križ</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Ob pozornosti</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Pojdi na</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikone</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Umik</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Ime:</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Stran:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Drugo</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Enobarven</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Vrsta:</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Seznamsko polje</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Uporabi simbol valute</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Oblika števil</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Vrsta preverjanja:</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>V datoteko:</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ne tiskaj</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Pošlji na URL:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Izračunaj</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Prehod kazalca</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Vrednost ni preverjena</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>največ</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Oblikovanje</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinirano polje</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Posebna skripta za preverbe:</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>najmanj</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Dejanje</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Rob</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Oblika datuma</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Gumb</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Krog</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation>Lastnosti polja</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Črtkano</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Prikrojeno</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Oblika časa</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Dogodek:</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Vrednost je</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Oblika</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Skrito</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Posebna skripta za izračunavanje</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Tu vnesite spisek polj, ločenih z vejicami</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Posebne skripte</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Zaobrni barve</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Brez predogleda</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Število</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstrani</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Kvadrat</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Slog:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Polje je oblikovano kot:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Java Script</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Ĺširina:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Namig orodja:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Ob razpustu</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Ne izvozi vrednosti</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Slike (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Pošlji podatke kot HTML</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Polje besedila</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Postavitev ikon...</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Kazalec spusti</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Izberi...</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Kazalec vstopi</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Uporabljaj ikone</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Samo za branje</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pritisnjeno</translation>
    </message>
    <message>
        <source>product</source>
        <translation>zmnožek</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Besedilo ob prehodu kazalca</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Potrdi</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Spremeni...</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Oblika odstotka</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Znaki</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Geslo</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Vidnost:</translation>
    </message>
    <message>
        <source>average</source>
        <translation>povprečje</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Privzdignjeno</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Uvozi podatke</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Kazalec izstopi</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Kazalec pritisne</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skripta:</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Odstotek</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimalke:</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>iz naslednjih polj:</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Uvozi podatke iz:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>in manjša ali enaka kot:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Natipk:</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Več-vrstično</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčrtano</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Karo</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Uredljiv</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Videz</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Potrditveno polje</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Vrednost ni izračunana</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Uredi...</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Obnovi obrazec</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Zahtevano</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ne preverjaj pravopisa</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Obrobljeno</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Viden</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Vrednost mora biti večja ali enakakot :</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Oblika izpisa:</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Poudari</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Privzeto izbrano</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Primer:</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ne drsaj</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Besedilo ob pritisku gumba</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Če želiš uporabljati ikone na gumbih, potrebuješ vsaj ikono za Običajno</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Spremeni izbiro</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Pisava za k PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Možnost se ne upošteva pri PDF 1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF datoteke (*.pdf);;Vse datoteke (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source> pt</source>
        <translation>pik</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cilj</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Povezava</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Povezava na splet</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation>Lastnosti opomb</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Dokumenti (*.pdf);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Povezava navzven</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">&amp;Spremeni...</translation>
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
        <translation type="unfinished">&amp;Y:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Normal</source>
        <translation>Običajo</translation>
    </message>
    <message>
        <source>Apply Template</source>
        <translation>Uveljavi predlogo</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Predloga:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Uveljavi na &amp;trenutni strani</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Uveljavi s &amp;strani:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Za:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Uveljavi na &amp;sodih straneh</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Uveljavi na &amp;lihih straneh</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Delete</source>
        <translation>Izbriši</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Predmet</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Odložišča (*.scs);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Odložišče</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nov vnos</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Preimenuj</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Pozor</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Naloži...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Majhen</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Srednji</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Velik</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Predogled</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Premakni zaznamek</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Zaznamki</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Vstavi zaznamek</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Zaznamki</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikona</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nikoli</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Ponastavi</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Napis prekrije ikono</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Kako umeriti:</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Postavitev:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Samo napis</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Napis pod ikono</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Vedno</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Merilo:</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Samo ikona</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Če je ikona premajhna</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Napis levo od ikone</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Naslov desno od ikone</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Če je ikona prevelika</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Nesorazmerno</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Sorazmerno</translation>
    </message>
    <message>
        <source>Icon Placement</source>
        <translation>Postavitev ikon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Napis nad ikono</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Barvni profil, ki ste ga pripravili sami ali dobili od proizvajalca. 
Ta naj bi bil prirejen vašemu modelu zaslona in ne kak splošen profil.</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Barvni profil vašega tiskalnika. To naj bi bil poseben profil za vaš model,
priložen s strani proizvajalca.</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Sistemski profili</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Privzet profil pri uvažanju slik.</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Namen izrisa</translation>
    </message>
    <message>
        <source>Color Management Settings</source>
        <translation>Nastavitve upravljanje barv</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Nadomeščanje po črnini je način za izboljšanje kontrasta slike. 
Če v dokumentu uporabljate slike je to priporočljivo.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Privzet namen izrisovanja strani. Če niste gotovi,
izberite Relative-Colorimetric ali &quot;Samo pogled&quot;.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Omogoči &quot;mehko nastavljanje&quot; barv dokumenta pri tisku,
glede na izbran tiskalniški profil.</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Samo ze predogled</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Privzet namen izrisovanja strani. Če niste gotovi,
izberite Relative-Colorimetric ali &quot;Samo pogled&quot;.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Način prikazovanja barv na zaslonu, ki se morda ne bodo natisnile pravilno. 
To zahteva zelo natančno izbiro vseh profilov.</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Privzet profil za barvne površine na strani.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Vključi upravljanje barv</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Slike:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Polne barve:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Zaslon:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Ti&amp;skalnik:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>Z&amp;aslon:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;Tiskalnik:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Pri izrisu &amp;posnemaj tiskalnik</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Označi barve izven &amp;spektra</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Uporabljaj kopenzacijo &amp;črnine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source> %</source>
        <translation>V redu</translation>
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
        <translation>V redu</translation>
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
        <translation>Novo</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Staro</translation>
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
        <source>Name of the Color is not unique</source>
        <translation>Ime barve ni edinstveno</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation>Spletne RGB</translation>
    </message>
    <message>
        <source>Edit Color</source>
        <translation>Nastavi barvo</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Stabilni izbirniki barv</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Spremenljivi izbirniki barv</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Paleta HSV</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Barvni &amp;model</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Brez</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Krožen preliv</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Način prelivanja</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Senca:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Prosojnost:</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Navpičen preliv</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Nastavi prosojnost izbrane barve</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Barva izbranega predmeta</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Križem postrani preliv</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Uredi lastnosti barve polnila</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Nasičenost barve</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Preliv postrani</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Uredi lastnosti barvanja črte</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Vodoraven preliv</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Prost enosmeren preliv</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Prost krožen preliv</translation>
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
        <translation>točk</translation>
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
        <translation>pik</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Možnosti CVS uvoza</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Ločilo polj:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Ločilo vrednosti:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Prva vrstica so naslovi</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Samo sode strani</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vrednost</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Stran na list</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Zrcali</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Možnost</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Usmerjenost</translation>
    </message>
    <message>
        <source>Printer Options</source>
        <translation>Možnosti tiskanja</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Samo lihe strani</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Nabor strani</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Strani na list</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Tiskanje n-gor</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Vse strani</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Tu vidite razne CUPSove možnosti pri tiskanju.\n
Natančne nastavitve so odvisne od vaših gonilnikov tiskalnika.\n
Podporo CUPSa lahko preverite pod Pomoč -&gt; O Scribusu.\n
Poiščite naslednje: C-C-T pomeni podporo za: C=CUPS C=littlecms T=TIFF.\n
Manjkajoča knjižnica je označena z zvezdico ( * )</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Premakne v mapo dokumenta.
To lahko nastavite v Nastavitvah.</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Razpored znakov:</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Stiskaj datoteko</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;vključi pisave</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Zamenjaj z:</translation>
    </message>
    <message>
        <source>Delete Color</source>
        <translation>Izbriši barvo</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Izbriši barvo:</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Brez</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>to:</source>
        <translation>za:</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Odstrani iz:</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation>Odstrani strani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>namesto</translation>
    </message>
    <message>
        <source>Missing Font</source>
        <translation>Manjka pisava</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Pisava %1 ni nameščena.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Uporabi</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Podatki o dokumentu</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Naslov:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Avtor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Ključne besede:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Opis:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Založnik:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Prispevki:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datum:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Vrsta:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Oblika:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Označevalec:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Vir:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Odnos:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Pokritje:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Pravice:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Nadaljne &amp;informacije</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Oseba ali organizacije, ki je odgovorna za vsebino dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Ime dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Opis vsebine dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Tema vsebine dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Oseba ali organizacija, ki je odgovorna za distribucijo dokumenta</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Oseba ali organizacija odgovorna za prispevke k vsebini dokumenta</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Datum povezan z dogodkov v življenskem ciklu dokumenta. Po standardu ISO 8601 v obliki: LETO-MM-DD</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Vrsta ali žanr vsebine dokumenta</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Fizično ali elektronsko udejanjenje dokumenta. Navedite vrsto nosilca in velikosti.
Lahko tudi RFC2045,RFC2046 kot MIME vrste</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Referenca do dokumenta v danem kontekstu, na primer ISBN ali URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Referenca do dokumenta iz katere je dokument izveden, na primer ISBN ali URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Jezik v katerem je napisana vsebina dokumenta, običajno jezikovna koda ISO-639 
z dodano okrajšavo in ISO-3166 državno kodo, na primer sl-SI, en-GB</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Referenca do povezanega dokumenta, po možnosti s formalnim označevalcem kot sta ISBN in URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Razširitev ali umestitev vsebine dokumenta, verjetno vključujoč mesto, čas in pravni doseg</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Podatki o pravicah uporabljenih in zadržavnih v dokumentu, na primer avtorske pravice,
patenti, blagovne znamke</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cijan</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Postscript datoteke (*.ps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Rumena</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Setup Printer</source>
        <translation>Nastavi tiskalnik</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Črna</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Cilj tiskanja</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Možnosti...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Datoteka:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Nadomesten ukaz za tiskanje</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Ukaz:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Obseg</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Natisni &amp;vse</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Natisni &amp;trenutno stran</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Natisni &amp;obseg</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Število tiskov:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Tiskaj &amp;običajno</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Tiskaj &amp;separacije</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Če je mogoče tiskaj v &amp;barvah</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Tiskaj &amp;črnobelo</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>&amp;Napredne možnosti...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Natisni</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Uporabite drug upravljalnik tiskanja, na primer kprinter ali 
gtklp, za dodatne možnosti tiskanja</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Uvoz datoteke:\n
%1\n
ni uspel!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Usodna napaka</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
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
        <translation> točk</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Ime sloga ni edinstveno</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Razmik nad odstavkom</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorji in zamikanje</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Ime vašega sloga odstavka</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave:</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Nastavi celotno višino ozaljšanih začetnic</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Pisava za izbrano besedilo ali predmet</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Poseben učinek, ki poveča prvo črko odstavka</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Uredi slog</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva črt besedila</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Navpični presledki</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Poravnaj besedilo na osnovno mrežo</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva polnenja besedila:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Razmik pod odstavkom</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Razmik vrstic</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Učinek:</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>&amp;Pisava:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>P&amp;oravnava:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Ozaljšana začetnica</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Črte:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>Barva po&amp;teze:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Prilagodi osnovni &amp;mreži</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>&amp;Razmik vrstic:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>&amp;Nad:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Pod:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Urejevalnik</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Java skripti (*.js);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Odpri...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Shrani in končaj</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Končaj brez shranjevanja</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Obnovi</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Dobi imena polj</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>Vse str&amp;ani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>V red&amp;u</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Spremeni končno mapo</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Mapa v katero shranjujete slike.\n
Ime izvozne datoteke bo &apos;imedokumenta-številkastrani.vrstadatoteke&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Izvozi samo trenutno stran</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Dostopne oblike za izvažanje</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Izberite mapo za izvažanje</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Izvozi v imenik:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Vrsta slike:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kakovost:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Izvozi kot sliko(e)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Resolucija:</translation>
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
        <translation>Obseg</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Trenutna stran</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Obseg</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Izvozi več strani</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Izvozi vse strani</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Resolucija slik. Za uporabo na zaslonu
uporabite 72 dpi</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Kakovost slik - 100% je najboljša, 1% je najslabša</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Velikost:</translation>
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
        <translation>Velikost:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Brez naslova</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Naziv:</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Avtor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznano</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Skribusov dokument</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>V trenutni nabor dodaj barve iz obstoječega dokumenta</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Ustvari novo barvo znotraj trenutnega nabora</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Izberi nabor barve</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Odstrani neuporabljene barve iz nabora barv trenutnega dokumenta</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Izbriši izbrano barvo</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Nabori barv</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Trenuten nabor barve</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Shrani trenuten nabor barve</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nova barva</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija %1</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Uredi izbrano barvo</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Izberi ime</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Napravi kopijo trenutno izbrane barve</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Izberi trenuten nabor barv kot privzet</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Odstrani neuporabljene</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Shrani nabor barv</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Brez</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Ime pisave</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Nadomestljive barve</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Podnabor</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Zamenjava</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Dodatne &amp;poti</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Vključi v:</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Uporabi pisavo</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberi mapo</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Nadome&amp;stljive barve</translation>
    </message>
    <message>
        <source>Global Font Settings</source>
        <translation>Globalne nastavitve pisav</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Pot do datoteke s pisavo</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Dodatne poti</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Dosegljive pis&amp;ave</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Dosegljive pisave</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Dodaj...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstrani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Piškur molče grabi fižol iz dna cezijeve hoste</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Predogled pisav</translation>
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
        <translation>Položaj:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Tu lahko dodajate, spreminjate ali odstranite barvne postanke.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
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
        <translation> točk</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Vodoravna vodila</translation>
    </message>
    <message>
        <source>Manage Guides</source>
        <translation>Nastavi vodila</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Navpična vodila</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Izbriši</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Izbriši</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Zakleni vodila</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
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
        <translation type="unfinished">Povezava</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Accept</source>
        <translation>Sprejmi</translation>
    </message>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Možno deljenje</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Preskoči</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Nastavitve deljenja besed</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Največ zaporednih deljenj besed.
0 pomeni neomejeno.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Dolžina najkrajše besede, ki se jo še deli.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Najmanjša beseda:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
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
        <translation>Običajno</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>pred stranjo</translation>
    </message>
    <message>
        <source>Insert Page</source>
        <translation>Vstavi stran</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Vstavljanje</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stranjo</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Predloga (desna stran)</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Vstavljanje</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Stran(i)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>&amp;Predloga (leva stran):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Predloga:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Vstavi tabelo</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Število stolpcev:</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Število vrstic:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Nova skripta</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Pozor</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Uredi Javaskripte</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Uredi...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Dodaj...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nova skripta:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
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
        <translation>Dejanje</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Shift+</translation>
    </message>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Uredi bližnjice na tipkovnici</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Izberi tipko za to dejanje</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Trenutna tipka</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Tipka &quot;ne&quot;</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Uporabnikova tipka</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Nastavi &amp;tipko</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Ta kombinacija tipk je že uporabljena</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Odstrani sloj</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Spusti plast</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Sloji</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Dvigni plast</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nova plast</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Ali res želite izbrisati tudi vse predmete tega sloja?</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Dodaj sloj</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Edit Line Styles</source>
        <translation>Uredi sloge črt</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nov slog</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
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
        <translation>točk</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>točk</translation>
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
        <translation>pik</translation>
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
        <translation>pik</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Večkratno podvojevanje</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pal</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Število kopij:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vodoravni zamik:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Navpični zamik:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Razdalje</translation>
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
        <translation>Kot:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Dolžina:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pik</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pal</translation>
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
        <translation>Napaka skripte</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Če ste pognali uradno skipto, prosimo sporočite napako na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Pokaži k&amp;onzolo</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>Skrij k&amp;onzolo</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Tudi to sporočilo je v odložišču. Uporabite Ctrl+V da ga prilepite v slednika hroščev.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Uvozi</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Spremeni...</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation>Uvozi predlogo</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Uvozi stran(i)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Iz dokumenta:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Uvozi stran(i):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>iz 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Ustvari stran(i)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>pred stranjo</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stranjo</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>if %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>to:</source>
        <translation>Za:</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Premakni stran(i)</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source>Move Pages</source>
        <translation>Premakni strani</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopiraj stran</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>Pred stranjo</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stranjo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> p</source>
        <translation> pik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
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
        <translation> točk</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Premakni na dno </translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Stopnja</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Širina:</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Obdrži razmerje raztega po X in Y enako</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Izberi spodnji levi kot za izhodišče</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Lastnosti besedila po poti</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Spremeni nastavitve levih ali končnih točk</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Obrni vodoravno </translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Obrni navpično</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Izberi zgornji desni kot za izhodišče</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Namen upodobitve slike:</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Razdalja med besedilom in dnom okvirja</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Raven konec</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Uredi nastavitve tabulatorja v okvirju z besedilom...</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Razdalja do krivulje</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Točka od katere se merijo razdalje ali koti vrtenja</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Navpični zamik slike znotraj okvirja</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Razdalja med stolpci</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Izvorni profil slike</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Spremeni velikost slike navpično</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Zaobljen spoj</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Namen upodobitve:</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Črta na dnu</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Širina stolpcev</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Višina:</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Ime izbranega predmeta</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Premakni na vrh </translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Nasičenost barve črte besedila</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Končne točke</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Senca:</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Oblika:</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Navpični položaj trenutnega izhodišča</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Pisava za izbrano besedilo ali predmet</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Spremeni velikost slike vodoravno</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Nasičenost barve polnila besedila</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Leva točka</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Vodoravni zamik slike znotraj okvirja</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Razdalja med besedilom in vrhom okvirja</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Posebna razdalja</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Črta na vrhu</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Vrste spojev</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Debelina črte</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Velikost črk za razpenjanje</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Slog trenutnega odstavka</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Izberi spodnji desni kot za izhodišče</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Kvadraten konec</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Vrtenje predmeta okrog trenutnega izhodišča</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Črta na desni</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Črta na levi</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Vrste zaključkov</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Prikaži krivuljo</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Ročno zgoščevanje</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Zakleni ali odkleni predmet </translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Samo za predogled</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Zamenja med presledkom in širino stolpca</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva črte besedila</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Nakaže raven na kateri je predmet, 0 pomeni da je predmet na dnu</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Naj slika ustreza velikosti okvirja</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Zakleni ali odkleni velikost predmeta</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Naj besedilo iz nižjih okvirjev obliva obliko predmeta</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Izberi zgornji levi kot za izhodišče</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Uporbi raje razmerja slike, kot pa razmerja okvirja</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Premakni raven navzgor </translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Nagnjen spoj</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Slog črte trenutnega predmeta</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Premakni raven navzdol </translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spoj pod kotom</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Zaobljen konec</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometrija</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Razdalja med besedilom in levim robom okvirja</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Izhodišče</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Vključi ali izključi možnost tiskanja predmeta</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva polnila besedila</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Uredi obliko okvirja...</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Ohrani razmerja </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Razdalja med besedilom in desnim robom okvirja</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Profil vnosa:</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Vzorec črte</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Izberi središče za izhodišče</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Črte celic</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Vodoravni položaj trenutnega izhodišča</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Dovoli, da je slika lahko druge velikosti kot okvir</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Nastavi zaobljenost roba</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Izberi obliko okvirja...</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Razdalja besedila</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Število stolpcev v okvirju besedila</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Razmik vrstic</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Začetni zamik:</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Oblika</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Besedilo</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Slika</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Črta</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Barve</translation>
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
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Vrtenje:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Uredi obliko...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>&amp;Zaobljeni
robovi:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Razpoka:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulatorji...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Besedilo &amp;poteka okrog okvirja</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Uporabi &amp;okvir</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Uporabi obrobno črto</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Velikost pisave:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation>&amp;Zgoščevanje:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>&amp;Razmik vrstic:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>S&amp;log:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Prosto raztegovanje</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;Razteg X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Raz&amp;teg Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Raztegni na &amp;velikost okvirja</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Sorazmerno</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Izhodišče:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Vrsta črte:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Širina črte:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Robovi:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Konci:</translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation>&amp;X1:</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation>&amp;X2:</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation>&amp;Y1:</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation>&amp;Y2:</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Pri oblivanju besedila uporabi pravokotno obrobo namesto oblike okvirja</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Pri oblivanju besedila uporabi dodatno črto na osnovi oblike okvirja</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Jezik za deljenje besed v tem okvirju</translation>
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
        <translation>V redu</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Raven konec</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Črta pika pika črta</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Zaobljen spoj</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Širina črte:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni edinstveno.
Prosim izberite drugo.</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Uredi slog</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Kvadratast konec</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Črtkana črta</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Pikčasta črta</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Nagnjen spoj</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spoj pod kotom</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Zaobljen konec</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Črta pia črta</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Polna črta</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>točk</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Name:</source>
        <translation>Ime:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopija #%1 </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Nova predloga</translation>
    </message>
    <message>
        <source>Edit Templates</source>
        <translation>Uredi predloge</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation>palcev</translation>
    </message>
    <message>
        <source> mm</source>
        <translation> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation>Točk (pts)</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Velikost strani v dokumentu; lahko standardna ali posebna</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nov dokument</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dolžina strani v dokumentu; nastavljivo, če ste izbrali posebno velikost</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Usmerjenost strani dokumenta</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Prikrojeno</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Pokončno</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Razdalja med samodejno ustvarjenimi stolpci</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila robov</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Naj bo prva stran leva</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Pik</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Širina strani v dokumentu; nastavljivo, če ste izbrali posebno velikost</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Ob dodajanju strani samodejno ustvari okvirje za besedila.</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Palcev (in)</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Omogoči enojno ali mnogotero postavitev</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Število stolpcev v samodejno ustvarjenih okvirjih za besedilo</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Številka prve strani:</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Privzeta enota mer pri urejanju dokumenta</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Vodila stolpcev</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milimetrov (mm)</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Pravna</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Pismo</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloidna</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Najprej &amp;leva stran</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Številka &amp;prve strani:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Privzeta merska enota:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Samodejni besedilni okvirji</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Razpoka:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Znotraj:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Z&amp;unaj:</translation>
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
        <translation>Leva stran</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Desna stran</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Vozlišča</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Če označeno, uporabljaj koordinate glede na stran, 
sicer glede na predmet.</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Ponastavi to kontrolno točko</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Zbriši vozlišča</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Dodaj vozlišča</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Premakni vozlišča</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Premakni kontrolne točke</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Ponastavi kontrolne točke</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolutne koordinate</translation>
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
        <translation>&amp;Uredi obrobno črto</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Ponastavi obrobno črto</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Končaj urejanje</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Kontrolne točke premikaj neodvisno</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Kontrolne točke premikaj simetrično</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Odpri mnogokotnik ali odreži bezier krivuljo</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Zapri to bezier krivuljo</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Vodoravno prezrcali pot</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Navpično prezrcali pot</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Pot razreži vodoravno proti levi</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Pot razreži navpično navzgor</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Pot razreži navpično navzdol</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Pot zavrti proti smeri ure</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Pot zavrti v smeri ure</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Zmanjšaj velikost poti za prikazanih %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Povečaj velikost poti za prikazanih %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Kot vrtenja</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>Povečaj ali zmanjšaj za %</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Vključi način za urejanje obrobnih črt</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Ponastavi obrobno črto na osnovno obliko okvirja</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Pot razreži vodoravno proti desni</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Konzola skripte</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
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
        <translation> točk</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Okvir</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Nizka</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation>Zip</translation>
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
        <source>High</source>
        <translation>Visoka</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation>JPEG</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Učinek zavzema</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Slike:</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Razdeli</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Razpusti</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Začetek učinkov škatlja in razcep.</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Izvozni profil za tiskanje. Če je mogoče izberite profil v povezavi z vašim tiskalnikom.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Ustvari PDF prispevke, kar je koristno pri pregledovanju povezanih člankov v PDFju.</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Polno barvanje</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodoravno</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Vključi barvne profile za čiste barve</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Izvozna resolucija za besedilo in vektorsko grafiko.
To ne vpliva na resolucijo slik.</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Velikost odmika od fizičnega levega robu strani</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Izberite glavno geslo, ki bo o(ne)mogočilo vse varnostne možnosti v izvoženem PDFju</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Pokaži predoglede strani</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Omogoči varnostne zmožnosti v izvoznem PDFju.
Če izberete Acrobat 4, bo PDF zaščiten s 40 bitno enkripcijo.
Če izberete Acrobat 5, bo PDF zaščiten s 128 bitno enkripcijo.
Pozor: PDF enkripcija ni tako zanesljiva kot GPG ali PGP in ima nekatere omejitve.</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Obseg izvoza</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Možnosti datoteke</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>Pisave</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Vključi povezave iz dokumenta.
To je uporabno za pregledovanje dolgih PDFjev.</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Desni rob</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Vrsta stiskanja slik.
Samodejno dovoli Scribusu, da sam izbere najboljši način.
ZIP je uporabna za slike z barvnimi ploskvami.
JPEG je boljši pri ustvarjanju majhnih PDFjev z mnogo slikami.
Če ne potrebujete posebnih nastavitev stiskanja, raje pustite samodejno.</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Vključi pisave:</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Velikost odmika od fizičnega desnega robu strani</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Pretvori slike na izbrano DPI resolucijo.
Če tega ne izberete, bodo slike risane pri izvirni resoluciji.</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Največja</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Ključna beseda za PDF/X-3, sicer izvoz ne uspe pravilno.
Predlagamo, da uporabite naslov dokumenta.</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Z leve proti desni</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Z desne proti levi</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Najmanjša</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Raven stiskanja: najmanj (25%), malo (50%), srednje (75%), močno (85%), najmočneje (95%)</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Polne barve</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Namen upodobitve:</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Zavese</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Brez učinkov</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Ne uporabi priloženih ICC profilov</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Dostopne pisave:</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Srednja</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Smer učinkov bleščice in metla.</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Levi rob</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Zaslon / Splet</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Zunaj</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Izvozi obseg strani v PDF</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Od zgoraj levo do spodaj desno</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Vključi barne profile za slike</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Barvni model za izvoz vašega PDFja.
Izberite Zaslon/splet za PDFje ki bodo brani na ekranu ali tiskani na običajnih tiskalnikih.
Izberite Tiskalnik, za tiskanje s CMYK tiskalniki.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Omogoči učinke za predstavitve, kadar uporabljate Acrobat Reader v celozaslonskem načinu.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Velikost odmika od fizičnega vrha strani</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Vse strani</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Tiskalnik</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Vključujem</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Nastavljač</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Stiskanje besedila in slik.
Če nimate dobrega razloga, raje pustite to vključeno, ker zmanjša velikost PDFja.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Smer gibajočih črt pri učinkih razcep in zavesa.</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Samo za predogled</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Namen upodobitve:</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Izvozi vse strani v PDF</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Čas kazanja strani, preden se prezentacija prične na izbrani strani</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Dodaj učinek na vse strani</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Pripravi pomanjšane sličice vsake strani v PDFju.
Nekateri pregledovalniki jih lahko uporabijo za pregledovanje.</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Gesla</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Samodejno:</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Namen upodobitve PDF/X-3</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Učinki</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Velikost odmika od fizičnega dna strani</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Dots Per Inch) pri izvozu slik.</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Nastavitve</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Ne uporabi barvnih profilov iz izvirnih slik</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Vrsta učinka ob izrisu</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Določi združljivost PDF. Privzeta je naširše združljiva oblika Acrobat 4.
Izberite Acrobat 5, če vaš dokument uproablja zmožnosti PDF-1.4, kot na
primer prosojnosti, ali če potrebujete enkripcijo.
PDF/X-3 se uporablja za pošiljanje v tiskarno, in je omogočeno le, če 
uporabljate tudi upravljanje barv.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Dolžina učinka. Krajši čas učinek pospeši, daljši upočasni</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Splošno</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>Splošno</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Osvetli</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Namen upodobitve:</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Določi povezovanje strani PDF. Če niste prepričani
pustite na privzeti vrednosti - levo.</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Od spodaj navzgor</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Vključi pisave v PDF. To bo omogočilo, da se pri
prenosu izgled in postavitev dokumenta ne spremenita.</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Nastavitve slik</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Od zgoraj navzdol</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Znotraj</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Izberite geslo, s katerim bodo uporabniki lahko brali vaš PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Dovoli tiskanje PDFja. ČE ne izberete, bo tiskanje onemogočeno.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Dovoli spreminjanje PDFja. Če ne izberete, bo spreminjanje onemogočeno.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Dovoli kopiranje besedila in slik iz PDFja.\n
Če ne izberete, se besedila in slik ne bo dalo presneti.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation>Dovolite dodajanje opomb in drugih polj v PDF.\n
Če ne izberete, bo dodajanje polj onemogočeno.</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation>Ustvari datoteko PDF </translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Izvozi v datoteko:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>Vse str&amp;ani</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Izberi strani</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Združljivost:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Povezanost:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Pripravi &amp;sličice</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>&amp;Povezane besedilne okvirje shrani kot članke PDF </translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Vključi opombe</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Resolucija:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Metoda:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kakovost:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>&amp;Slike reduciraj na:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Vključi vse pisave</translation>
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
        <translation>Pokaži pred&amp;oglede strani</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Dolžina &amp;prikazovanja:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Trajanje &amp;učinka:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Vrsta učinka:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Premikanje črt:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Iz:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Smer:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Dodaj učinek na vse strani</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Uporabi enkripcijo</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Uporabnik:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Lastnik:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Dovoli &amp;tiskanje dokumenta</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Dovoli &amp;spreminjanje dokumenta</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Dovoli &amp;kopiranje besedila in slik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Dovoli &amp;dodajanje opomb in polj</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Varnost</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>&amp;Namen izhoda:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Uporabi posebne nastavitve upodabljanja</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Nastavitve upodabljanja</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Frekvenca:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Kot:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Funkcija &amp;spot:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Enostavna točka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Črta</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Okroglo</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipsa</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Uporabi ICC profil</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Podatki:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Profil &amp;izhoda:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF datoteke (*.pdf);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>To je napredna nastavitev, ki ni privzeta. Vključite jo le, če jo potrebujete zaradi tiskalnika ali
navodil iz tiskarne. Sicer se vaš PDF morda ne bo pravilno natisnil in ne prav prikazal na drugih
sistemih.</translation>
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
        <translation type="unfinished">&amp;Vrtenje:</translation>
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
        <translation>Vse</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Predogled tiskanja</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Vključi/izključi barvilo M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Vključi/izključi barvilo K (Črno)</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Vključi/izključi barvilo C (cyan)</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Prikazuj prosojnosti in prosojne predmete v dokumentu. To zahteva vsaj Ghostscript 7.07.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Pisave vrste True-Type in Open-Type, ter EPS, PDF in vektorske grafike izgledajo prijetneje
na račun malce počasnejšega izrisovanja.</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Prikaže predogled tiskanja s simulacijo splošnih CMYK barvil namesto RGB barv.</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Vključi/izključi barvilo Y (Yellow)</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Omogoča lepši izgled besedil, na račun malce počasnejšega \n
izrisovanja. Vpliva samo na pisave vrste Type 1</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Mehčaj &amp;besedilo</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Mehčaj &amp;slike</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Prikaži &amp;prosojnost</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Prikaži CMYK</translation>
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
        <translation>&amp;Odstranjevanje barve pod</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Izključeno</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>manjka!</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Datoteka: </translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Okvir besedila</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Slika</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Natisni: </translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Program</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaki: </translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopija</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Besedilo po poti</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Povezano besedilo</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Vključeno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Pozor</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odstavki: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Besede: </translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Uredi besedilo...</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopiraj sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Premakni sem</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Pokaži &amp;meje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Pokaži &amp;okvirje</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Pokaži &amp;slike</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Kaži &amp;mrežo</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Prikaži &amp;vodila</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Kaži &amp;osnovno mrežo</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Preskakuj do mreže</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Preskakuj do vodil</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Izvorna PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Dejanska PPI:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Podatki</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>&amp;Dodaj sliko...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>&amp;Slika je vidna</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>&amp;Osveži sliko</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Uredi sliko</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>Okvir &amp;prilagodi sliki</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Najdi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Dodaj besedilo...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>&amp;Uredi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>&amp;Vstavi naključno besedilo</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Je PDF &amp;beležka</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>je PDF &amp;opomba</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Lastnosti opombe</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Lastnosti &amp;polja</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Možnosti &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Zakleni</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Od&amp;kleni</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Zakleni &amp;velikost predmeta</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Odkleni &amp;velikost predmeta</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Pošlji na &amp;odložišče</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Pošlji na &amp;sloj</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Združi</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>&amp;Razdruži</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>&amp;Raven</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Pošlji na &amp;dno</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Prinesi na &amp;vrh</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Spusti</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Dvigni</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation>&amp;Okvir slike</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>&amp;Mnogokotnik</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Obrobe</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Okvir besedila</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier krivulja</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Pretvori v</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;Počisti vsebino</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Prikaži &amp;lastnosti...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Skrij &amp;lastnosti...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Ali zares želite počistiti vso besedilo?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Line</source>
        <translation>Črta</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Slika</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomljena črta</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Poligon</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Besedilo po poti</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Stran </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation> od %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Result</source>
        <translation>Rezultat</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Izberi</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Najdi rezultate za: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Predogled</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Pojdi v</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Pot</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Natisni</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Manjkajoče</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Iskanje</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Stanje</translation>
    </message>
    <message>
        <source>Pictures</source>
        <translation>Slike</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
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
        <translation>Lastnosti poligona</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Koti:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Vrtenje:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Število kotov mnogokotnika</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Stopinje zasuka mnogokotnika</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Primer poligona</translation>
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
        <translation> pik</translation>
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
        <translation> točk</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Grafični vmesnik</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation>Položaj urejevalnika slik na sistemu</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Uporabi mehčanje pisav pri izrisu za EPS in PDF</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Meniji</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Razno</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Poti</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Drugo</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Majhen</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Orodja</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Enote</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Naj bo prva stran dokumenta leva</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Druge možnosti</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Lastnosti risanja poligonov</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Izberi velikost predogleda v paleti odložišča</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Razpored pod vodilom pisave na črti</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Razdalja med malimi črtami mreže</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Razdalja med glavnimi črtami mreže</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Dodatna orodja</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Nasičenost barve polnila</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Nasičenost barve črte</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Male črke</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Naj bo mreža pod predmeti na strani</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Kadar uporabljaš nasproti postavljene strani, jih kaži dve po dve</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Velikost pisave v novih okvirjih z besedilom</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativna velikost podpisave v primerjavi z običajno pisavo</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Barva polnila okvirjev s slikami</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Lastnosti risanja oblik</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Razpored nad vodilom pisave na črti</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Uporabi mehčanje slik pri izrisu za EPS in PDF</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Slog črt</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Postscript upodobljevalnik</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Prikrojeno</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Barva črte pri oblikah</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Barva polnila pri oblikah</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Naj bosta vodoraven in navpičen razteg ista</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Vodila</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Barve mreže</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Število kotov poligona</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Srednje</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Čas med samodejnimi shranjevanji</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Postavitev mreže</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Če izberete to, bo Scribus shranil varnostno kopijo
vašega dokumenta (s končnico .bak), vsako izbrano enoto časa</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Pisava</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Število vrstic, ki jih preskoči premik kolešča na miški</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Pisava v novih okvirjih z besedilom</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Najmanjša dovoljena povečava</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Največja dovoljena povečava</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation>Vključi osnovno mrežo</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Širina strani dokumenta; to lahko nastavite, če ste označili posebno velikost strani</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Lastnosti risanja črt</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Orodje za upravljanje s slikami</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Privzeta povečava</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Samodejno razpete slike ohranijo svoja razmerja</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Barva papirja</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Postavitev</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Stopinje zasuka poligona</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Barva vodil robu</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila meje</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Predogled</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Vodoraven razteg slik</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Širina črt</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Slog črte pri oblikah</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Privzeta velikost menijev in oken</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Piškur molče grabi fižol iz dna cezijeve hoste.</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Večjost razmaka črt v primerjavi z velikostjo pisave v odstotkih</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Navpičen razteg slik</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Odložišče</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Za prilagoditev prikaza povlecite spodnje vodilo.</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Lastnosti okvija za sliko</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Tiskanje</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Privzete vrednosti povečav</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Privzeta mapa s Scripterjevimi skriptami</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Pike</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Ne kaži predmet, ki so zunaj robov</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Podpisano</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Nadpisano</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Število stolpcev v okvirjih z besedilom</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Naj bo mreža pred predmeti na strani</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativna velikost napisaneve v primerjavi z običajno pisavo</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativna velikost pomanjšanih vleikih črk v primerjav iz običajno velikostjo pisave</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Okvir s sliko lahko razpne sliko na kakršnokoli velikost</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation>Izključi osnovno mrežo</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Barva malih črt mreže:</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Barva večjih črt mreže:</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation>Privzeta mapa z ICC profili</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Lastnosti okvirja z besedilom</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Privzeta mapa z dokumenti</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Privzeta usmeritev strani dokumenta</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Barva vodilnih črt</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Pike (pt)</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Prikaz</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Število nedavno uporabljenih dokumentov, ki naj se prikažejo v meniju Datoteka</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Nasičenost barve</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Palcev (in)</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Vključi enotno ali razpršeno postavitev</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Sprememba povečave v vsaki zoom operaciji</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Razmak med stolpci okvirja z besedilom</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Privzeta velikost strani; lahko je običajna ali posebna</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Debelina črte pri oblikah</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Primer pisave</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Samodejno shranjevanje</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Nastavitve</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Osnovna črta mreže</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Položaj upodobljevalnika Ghostscript na sistemu</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Izberite privzet izgled in okraske oken.
 Scribus uporabi vse dostopne KDE ali QT teme.</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Slike v okvirjih s slikami naj bodo razpete do elikosti okvirja</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Označi prostor robu z barvo robu</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Splošne</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Uporabi sposobnosti prosojnosti pri izvozu v PDF-1.4</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Doseg oprijemanja predmetov</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Shrani vsebino odložišča ob vsaki spremembi</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Privzeta merska enota ob urejanju dokumenta</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Barva črt</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Višina strani dokumenta; to lahko nastavite, če ste označili posebno velikost strani</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Primer poligona</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milimetri (mm)</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Razdalja do vodila, na kateri bo predmet preskočil na vodilo</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Barva pisave</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Velikost pisave:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Nastavitve miške</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Preskok koleščka:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>&amp;Radij dosega:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nedavni dokumenti:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenti:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profili:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skripte:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>&amp;Spremeni...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Najprej &amp;leva stran</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>&amp;Vključeno</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Presledek:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>Razmik &amp;malih mrežnih črt:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Razmik &amp;glavnih mrežnih črt:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>Doseg &amp;preskakovanja do vodil:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>Barva &amp;malih mrežnih črt:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Barva &amp;glavnih mrežnih črt:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Barva &amp;uporabnikovih vodil:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>&amp;Barva osnovne mreže:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>V &amp;ozadju</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>V o&amp;spredju</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>&amp;Vključeno</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>&amp;Izključeno</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Raz&amp;postavitev:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Raztegovanje:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>&amp;Raz&amp;postavitev:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>R&amp;aztegovanje:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ra&amp;ztegovanje:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>&amp;Osnova mreže:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>&amp;Zamik osnove mreže:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>&amp;Samodejni razmiki &amp;vrstic:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>Privzeta &amp;pisava:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Privzeta &amp;velikost:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Barva &amp;besedila:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Razpoka:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Barva &amp;črte:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Senčenje:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>S&amp;enčenje:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation>&amp;Vrsta črte:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Širina črte:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;Najmanj:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Na&amp;jveč:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Po korakih:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Prosto raztegovanje</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Vodoravno raztegovanje:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Navpično raztegovanje:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Raztegni sliko na velikost okvirja</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Ohrani &amp;razmerja</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Koti:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Vrtenje:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>&amp;Shrani vsebino ob spremembah</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Velik</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Prikazuj strani &amp;drugo ob drugi</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Barve strani</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Ozadje:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Meje:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Prikazuj &amp;nenatisljivo območje v barvi meje</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Uporabi PDF 1.4 &amp;prosojnosti</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Prilagodi velikost prikaza</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Ime zagonske datoteke:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Mehčaj &amp;besedilo</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Mehčaj &amp;slike</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Ime &amp;zagonske datoteke:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>&amp;Reži po mejah strani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Znotraj:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Z&amp;unaj:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Uveljavi &amp;odstranjevanje barve pod</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">&amp;Spremeni...</translation>
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
        <translation type="unfinished">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
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
        <translation type="unfinished">Pravna</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Pismo</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished">Tabloidna</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>Na&amp;s:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Vred:</translation>
    </message>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Odt&amp;enek:</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>Nastavi barve &gt;&gt;</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Izberi barvo</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Dodaj lastnim barvam</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Modra:</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>Zelena:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Osnovne &amp;barve</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Barve po meri</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Prosojnost</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rdeča:</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Mapa</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Nazaj</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Shrani</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Razvrsti</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Odpri</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Napaka</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Pisanje: %1</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Uredi po veliko&amp;sti</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Uredi po &amp;datumih</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nova mapa 1</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>Mapa</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Vrs&amp;ta datoteke:</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Ime datoteke:</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>Znova naloži</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nova mapa</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Nerazvrščeno</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Poglej v:</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Poglej vsebino datoteke</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nova mapa %1</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Za pisanje in branje</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Samo za branje</translation>
    </message>
    <message>
        <source>Copy or Move a File</source>
        <translation>Prepiši ali premakni datoteko</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Poišči mapo</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Kaži skrite datoteke</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Nedostopno</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Ne najdem datoteke.
Preverite ime in pot do nje.</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Seznamski pogled</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Posebno</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Samo za pisanje</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>povezava</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Mape</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Povezava do posebnega predmeta</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Vse datoteke (*.*)</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Branje: %1</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>P&amp;reimenuj</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Mapa:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Mapo navzgor</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Poglej podatke datoteke</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>datoteka</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Ustvari novo mapo</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Povezava do datoteke</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Povezava do mape</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Podroben pogled</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Uredi po ime&amp;nih</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Izbriši %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ali zares želite izbrisati %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Pisava</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>S&amp;log pisave</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Učinki</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Pre&amp;črtano</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Podčrtano</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Primer</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>S&amp;kripta</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Uporabi</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Izberi pisavo</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Obnovi</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Počisti</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Izberi vse</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>V vrsto</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Prilagodi...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;O Qt&lt;/h3&gt;&lt;p&gt;Ta program uporablja QT različico %1.&lt;/p&gt;&lt;p&gt;Qt je C++ orodje za razvoj grafičnih vmesnikov in programov za več okolij. .&lt;/p&gt;&lt;p&gt;Qt omogoča prenos iste kode v okolja MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, in vse pomembnejše Unixe.&lt;br&gt;Qt obstaja tudi za vključene pripomočke.&lt;/p&gt;&lt;p&gt;Qt ije izdelek Trolltecha. Več na &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Predogled tiskanja</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Joj! Poskušate ubesediti predmet ki ne obstaja!</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG Slike (*.svg *.svgz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Joj! Kličete predmet ki ne obstaja!</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Skripte (*.py);; Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>Skripta</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Joj! Poskušate zbrisati predmet ki ne obstaja!</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Ali ste prepričani, da želite prepisati datoteko:
%1 ?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Spletna pomoč</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG Slike (*.svg);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Pripravljam...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Shrani kot sliko</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Napaka pri zapisovanju izhodnih datotek.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Napaka pri zapisovanju izhodnih datotek.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Izvoz je bil uspešen.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Datoteka že obstaja. Jo prepišem?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>že obstaja. Jo prepišem?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Da, vedno</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Predogled &amp;pisav</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Vstavi posebej</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nova &amp;iz predloge...</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Novice</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brošure</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalogi</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Letaki</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Znaki</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kartice</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Glave pisem</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Ovojnice</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Vizitke</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Koledarji</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Oglasi</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Nalepke</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Meniji</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programi</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF forme</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF prezentacije</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Revije</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plakati</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Napovedniki</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Besedilne datoteke</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Zgibanke</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Lastne predloge</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Shrani kot &amp;sliko...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Pred&amp;ogled tiskanja</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Uvozi &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Vse podprte oblike (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Shrani kot &amp;predlogo...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Priročnik za s&amp;kripte...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Skribusove skripte</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Poženi skripto...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Nedavne skripte</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Pokaži &amp;konzolo</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Shrani stran kot &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Uvozi &amp;SVG...</translation>
    </message>
    <message>
        <source>Oook! Wrong arguments! Call: </source>
        <translation type="obsolete">Aaaa! Napačni argumenti! Kličite:</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Aaaa! Sliko poskušate vklučiti v predmet ki ne obstaja ali ni izbran!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Aaaa! Poskušate (od)kleniti predmet ki ne obstaja! In tudi noben drug ni izbran.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Aaaa! Povprašujete po predmetu, ki ne obstaja! In tudi noben ni izbran.</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Uvažam besedilo</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Vse podprte oblike</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Datoteke HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Datoteke z besedilom</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Datoteke z vrednostmi, ločene z vejicami</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>podatki CVS</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>glava CVS</translation>
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
        <source>OO.o Writer Documents</source>
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
        <translation type="unfinished">Katalonščina</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Češčina</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Danščina</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Nizozemščina</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Angleščina</translation>
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
        <translation type="unfinished">Nemščina</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Finščina</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Francoščina</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Grščina</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">MadĹžarščina</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Italijanščina</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Litvanščina</translation>
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
        <translation type="unfinished">Poljščina</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Ruščina</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Švedščina</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Španščina</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Slovaščina</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Slovenščina</translation>
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
        <source>Import &amp;Open Office Draw...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Office Draw (*.sxd);;All Files (*)</source>
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
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Cu&amp;t</source>
        <translation>Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Obnovi</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Počisti</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Izberi vse</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Pomanjšaj</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Razpni</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Obnovi</translation>
    </message>
    <message>
        <source>System Menu</source>
        <translation>Sistemski meni</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Odsenči</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Premakni</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Pomanjšaj</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Ostani na &amp;vrhu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Obnovi</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>Odsenči</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Pomanjšaj</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Razpni</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Obnovi navzdol</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation> pal</translation>
    </message>
    <message>
        <source> mm</source>
        <translation> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Nastavitev dokumenta</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila robov</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Naj bo prva stran leva</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Omogoči enojno ali mnogotero postavitev</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Najprej &amp;leva stran</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Znotraj:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>Z&amp;unaj:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Velikost strani</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Prikrojeno</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="unfinished">Usmerjenost:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Ležeče</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Ĺširina:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished">Številka &amp;prve strani:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Slog trenutnega odstavka</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Nastavitve sloga</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva polnila besedila</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Nasičenost barve polnila besedila</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Nastavitve barve polnila</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva črte besedila</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Nasičenost barve črte besedila</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Nastavitve barve črte</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source> pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Pisava izbranega besedila</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Velikost črk za razpenjanje</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Nastavitve pisave</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation>Spodsekavanje:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Ročno spodsekavanje</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Nastavitve znakov</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopija #%1 izvirnika </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>p</source>
        <translation>pik</translation>
    </message>
    <message>
        <source> p</source>
        <translation>pik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>in</source>
        <translation>pal</translation>
    </message>
    <message>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pal</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>točk</translation>
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
        <translation>Vse</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Izreži</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nova</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopiraj</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Pisava</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Zakleni</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Končaj</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Shrani</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Končujem</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Razveljavi</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Poravnaj desno</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Zapri trenuten dokument</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Prečrtano</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Poravnaj na sredino</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>Pomoč</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>Predmet</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>Pogled</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Slogi...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Razporedi/Poravnaj...</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Počisti</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Češčina</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Nizozemščina</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grščina</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Združi</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irščina</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Spusti</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Prilepi</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Dvigni</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Pripravljen</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Slog</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Orodja</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Pošlji na dno</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italijanščina</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Preskakuj do mreže</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Vključujem dodatke</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Razdruži</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Pametno deljenje besed</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS datoteke (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Orodja</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Zbriši...</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Pomanjšane velike črke</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Večkrat podvoji</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Prikaži paleto strani</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Barve...</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Sličice</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Iščem pisave</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Podatki o dokumentu...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Premakni...</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Skrij okvirje</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danščina</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Zbriši</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Nedeljiv presledek</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Skrij slike</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>MadĹžarščina</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francoščina</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Nemščina</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Zakleni/Odkleni</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Preobrni</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation>Na vašem sistemu ni Postscriptnih pisav</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Prikaži sloje</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Poljščina</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Nalagam:</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Odkleni vse</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Zakleni vse</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovaščina</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
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
        <source>Unlock</source>
        <translation>Odkleni</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Odpri...</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Prikaži mrežo</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Obroba</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovenščina</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Podvoji</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Skrij meje</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Shrani kot...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Dodaj sliko...</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>Dodatno</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Besedilo pripni na pot</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Nekateri predmeti so zaklenjeni.</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Vstavi številko strani</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Skrij osnovno mrežo</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Prinesi na vrh</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Prilagajam barve</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Upravljaj vodila...</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukrajinščina</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Odložišče</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Berem odložišče</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Priprava dokumenta</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Java skripti...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Na velikost okna</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Natisni...</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Prikaži osnovno mrežo</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>O QT-ju</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Dodaj besedilo/sliko</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalonščina</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Uveljavi predlogo...</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Podpisano</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Odpri dokument</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Nadpisano</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Vstavi...</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruščina</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Tiskanje ni uspelo!</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Shrani trenutno odprt dokument kot PDF</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Opravi deljenje besed</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Španščina</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus je razpadel zaradi napake #%1</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Natisni trenutno odprt dokument</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Dodaj besedilo...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Tiskam...</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčrtano</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Švedščina</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Shrani trenuten dokument</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Nalagam...</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Besedilne datoteke (*.txt);;Vse datoteke(*)</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberi mapo</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Ustvari nov dokument</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Pomoč na spletu</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Izberi vse</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugalščina</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Upravljaj s slikami</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Angleščina</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Obrisano</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus je razpadel</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Predloge...</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Shranjujem...</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finščina</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Nastavitve branja</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>NAstavljam bližnjice</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Usodna napaka</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Poravnaj levo</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Okna</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Izberi novo pisavo</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>O Scribusu</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Vključujem deljenje</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litvanščina</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Priročnik za Scribus</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Pisave...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Na&amp;stavitve</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Vse podprte oblike</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Ne morem pisati v datoteko:\n
%1</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Manjkajo sledeči programi:</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Upravljanje barv...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Odpri...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Odpri &amp;nedavno</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Povrni do shranjene</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Zberi za &amp;izvoz...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>&amp;Dodaj besedilo/sliko...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Dodaj &amp;besedilo...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Uvozi</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Shrani &amp;besedilo...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Shrani stran kot &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Shrani kot &amp;PDF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Izvoz</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Nastavitve dokumenta...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Tiskaj...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Končaj</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Izberi &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Po&amp;išči in zamenjaj...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Barve...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Slogi odstavka...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Slogi &amp;črt...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Predloge...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Java skripti...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Večkratno podvojevanje</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Združi</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Razdruži</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Zakleni</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Pošlji na &amp;dno</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Prinesi na &amp;vrh</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Spusti</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Dvigni</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Razporedi/&amp;Poravnaj...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Oblika</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Besedilo &amp;pripni na pot</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Besedilo &amp;snemi s poti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Sestavi poligone</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Razdeli &amp;poligone</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>Pr&amp;etvori v črte</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Vstavi...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Zbriši...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Premakni...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Uveljavi predlogo...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Na velikost okna</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Sličice</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Kaži &amp;mrežo</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Preskakuj do vodil</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Kaži &amp;osnovno mrežo</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Lastnosti</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Obroba</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Odložišče</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Sloji</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>P&amp;aleta strani</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Zaznamki</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Upravljaj s slikami</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>Opravi &amp;deljenje besed</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Namigi orodja</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Orodja za &amp;PDF</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Namigi orodja</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>&amp;Pisave...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Deljenje besed...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>&amp;Bližnjice tipkovnice...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;O Scribusu</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>O &amp;QT-ju</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Pr&amp;iročnik za Scribusa...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>S&amp;log</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Levo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Sredinsko</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Desno</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blok</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Vsiljeno</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Drugo...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskadno</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Tlakovano</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>Pre&amp;obrni</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Vstavi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Pisava</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Učinki</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>P&amp;oravnava</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Senca</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorji...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Od&amp;kleni</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Pokaži &amp;slike</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Pokaži &amp;meje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Pokaži &amp;okvirje</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Prikaži &amp;vodila</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript: Ne morete uporabiti EPS slik</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Uvozi &amp;stran(i)...</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Preskakuj do mreže</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Nastavitve...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Uvažam strani...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Uvozi stran(i)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Poskušate uvoziti več strani, kot jih je na voljo v dokumentu od trenutno aktivne dalje.&lt;/p&gt;Izberite eno od možnosti:&lt;br&gt;
&lt;ul&gt;&lt;li&gt;&lt;b&gt;Ustvari&lt;/b&gt; manjkajoče strani&lt;/li&gt;&lt;li&gt;&lt;b&gt;Uvozi&lt;/&gt; strani do zadnje strani&lt;/li&gt;&lt;/li&gt;&lt;b&gt;Prekliči&lt;/b&gt;&lt;/li&gt;/&lt;ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Ustvari</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Uvozi</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Uvoz končan</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nič za uvoziti</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Iščem ICC profile</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Upravljaj &amp;vodila...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Senca:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>&amp;Podatki o dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>&amp;Prekliči brisanje predmeta</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>&amp;Prekliči premikanje predmeta</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>&amp;Prekliči spreminjanje predmeta</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Uredi obliko</translation>
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
        <source>p</source>
        <translation>pik</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>in</source>
        <translation>pal</translation>
    </message>
    <message>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Sloj</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>has been changed since the last save.</source>
        <translation>je bil od zadnjega shranjevanja še spremenjen.</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>Vseeno &amp;zapusti</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Vseeno &amp;zapri</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Shrani sedaj</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Pokaži predoglede strani</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Pokaži imena predlog</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Pisava</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Levo</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blok</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Desno</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Slog odstavka</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Barva poteze</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Senca poteze</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Poišči in zamenjaj</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Sredina</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Vsiljeno</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Iskanje končano</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Išči:</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Barva polnila</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Senca polnila</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Učinki pisave</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Zamenjaj z:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Cela beseda</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignoriraj velikost</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Išči</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Zamenjaj</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Zamenjaj &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Počisti</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Tu so vse vaše predloge. Da sutvarite novo stran
povlecite predlogo na delovno področje spodaj.</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Dostopne predloge: </translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Najprej leva stran</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Za izbris povlecite strani ali predloge v koš.</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Strani dokumenta:</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Uredi strani</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Predogled vseh strani v dokumentu.</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Available Fields</source>
        <translation>Polja na voljo</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Izbrana polja</translation>
    </message>
    <message>
        <source>Select Fields</source>
        <translation>Izberite polja</translation>
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
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Shade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>Other...</source>
        <translation>Drugo...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Senca:</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Edit Styles</source>
        <translation>Uredi sloge</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nov slog</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija od %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
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
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Ali zares želite izgubiti vse spremembe?</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Urejevalnik zgodbe</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Ali zares želite počistiti vso besedilo?</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Trenuten odstavek</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Ali želite shraniti spremembe?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaki: </translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Besedilne datoteke (*.txt);;Vse datoteke(*)</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Skupno:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odstavki: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Besede: </translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Posodobi okvir z besedilom in končaj</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Končaj brez posodabljanja besedila</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Posodobi okvir z besedilom</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Vstavi posebej</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Osveži besedilo iz okvirja</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Shrani v datoteko...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Naloži iz datoteke...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Shrani &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Posodobi okvir z besedilom in končaj</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Končaj brez posodabljanja besedila</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>&amp;Vstavi posebej...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Posodobi okvir z besedilom</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Izberi &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Uredi sloge...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Naloži besedilo iz datoteke</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Shrani besedilo v datoteko</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Osveži besedilo iz okvirja</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Po&amp;išči in zamenjaj...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Predogled &amp;pisav...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Ozadje...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>Pisava za &amp;prikaz...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Na&amp;stavitve</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Poišči in zamenjaj</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Predogled &amp;pisav</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Počisti vse besedilo</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Outline Text</source>
        <translation>Besedilo orisa</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Pomanjšane velike črke</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Podpisano</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Nadpisano</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčrtano</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Prečrtano</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OO.o Writer Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
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
        <source>Should the importer always use currently
set value when importing OO.o document and
never ask your confirmation again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">V redu</translation>
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
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Nastavi tabulator</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source> p</source>
        <translation> pik</translation>
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
        <translation> točk</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Levo</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Vejica</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Desno</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Pika</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Sredina</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Zamik prve vrstice odstavka</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Zamik celotnega odstavka v levo</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Pobriši tabulatorje</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Izbriši vse</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Položaj:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>Prva &amp;črta:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">&amp;Zamik:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
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
        <source>Line</source>
        <translation>Črta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Pisava:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Slika</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomljena črta</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Obroba</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Poligon</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Besedilo po poti</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Podatki</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Pozor</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Združi</translation>
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
        <translation type="unfinished">Alt+O</translation>
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
        <translation>Orodja</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Uredi besedilo z urejevalnikom zgodbe</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Poveča ali pomanjša </translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Vstavi črte</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Vstavi tabelo</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Vstavi sliko</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Izberi predmete</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Uredi vsebino okvirja</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Riše različne oblike </translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Poveži okvirje besedila</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Vstavi Bezier krivulje</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Zavrti predmet</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Razveži okvire besedila</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Vstavi prostoročno črto</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Lastnosti...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Vstavi poligone</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Vstavi okvir besedila</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Izmeri</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Link</source>
        <translation>Povezava</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Seznamsko polje</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinirano polje</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Gumb</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Polje besedila</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Potrditveno polje</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>Orodja za PDF</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Vstavi polje PDF</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Vstavi opombo PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Izberi znak:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Vstavi črke v besedilo pod kazalec</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Izbriši trenutno izbiro.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Zapri to pogovorno okno in se vrni k urejanju besedila.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vstavi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Izberite uvoznika</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Samodejno</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation>Najdi samo besedilo</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Uvozi besedilo brez oblikovanja</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Uvoznik:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Razpored znakov:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation type="unfinished">Izberite uvoznika</translation>
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
        <translation type="unfinished">V redu</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nova iz predloge</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Opis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uporaba</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Narejeno z</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Avtor</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstrani</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Odpri</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Nalagam predloge</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Nameščam predloge</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Izvleci paket v imenik s predlogami: za trenutnega uporabnika ~/.scribus/templates, za vse uporabnike sistema pa 
PREDPONA/share/scribus/templates.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Pripravljam predlogo</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Odstranujem predlogo</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Prevajam template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Predloge dokumentov lahko najdete na http://www.scribus.net/, v rubriki &quot;Downloads&quot;.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Prepričajte se, da uporabljate prosto dostopne slike in pisave. Če pisave niso za prosto uporabo jih ne shranjujte v predlogo.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Kdor ustvarja predlogo mora poskrbeti, da se gornji razdelek &quot;Nalagam predloge&quot; nanaša tudi na njegove predloge. Tako bodo uporabniki lahko uporabili paket s predlogo v svojih imenikih za predloge.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Odstranjevanje predloge preko pogovornega okna &quot;Nova iz predloge&quot; odstrani le vnos v nastavitveni datoteki template.xmln ne pa tudi dejanske datoteke. Meni za brisanje se prikaže le, le imate pravico pisanja v datoteko template.xml.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Skopiraj obstoječo datoteko template.xml v datoteko z imenom template.jezik_DRŽAVA.xml (uporabite isto jezikovno oznako kot za .qm datoteko vašega jezika), na primer template.sl_SI.xml za slovensko datoteko s predlogami. Kopija mora biti v istem imeniku kot izvirna datoteka template.xml.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Shrani kot predlogo</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Opis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uporaba</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Avtor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-pošta</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Podrobnosti</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Manj podrobnosti</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Pravna</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Pismo</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloidna</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>ležeče</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>pokončno</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>posebno</translation>
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
        <translation type="unfinished">&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Zbriši</translation>
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
        <translation type="unfinished">Prekli&amp;či</translation>
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
        <translation type="unfinished">Odstrani</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation type="unfinished">Zamenjaj</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Uporabi</translation>
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
