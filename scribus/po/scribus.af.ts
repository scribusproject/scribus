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
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
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
</context>
<context>
    <name>About</name>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Scribus weergawe %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Bou-ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Oor</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Bydraes vanaf:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Windows-weergawe:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>O&amp;uteurs</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Duits:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Frans:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italiaans:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Hongaars:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukraïnies:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bulgaars:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galisiaans:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turks:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Litous:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Pools:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Tsjegies:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovaaks:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Deens:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Noors:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Wallies:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Russies:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Brasiliaans:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Fins:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Sloveens:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baskies:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Sweeds:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>Ver&amp;talings</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Aanlyn</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Sluit</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Ontwikkelingspan:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Offisiële dokumentasie:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Ander dokumentasie:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Engels (Brits):</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Tuisblad</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Aanlyn handleiding</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Foute en funksiewense</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Poslys</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Oor Scribus %1</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Amptelike vertalings en vertalers:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Katelaans:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Koreaans:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Servies:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Spaans:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Vorige vertalingsbydraers:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Die paneel wys die weergawe, vertaaldatum en die invertaalde biblioteke in Scribus 
Die C-C-T verwys na C=littlecms C=CUPS T=TIFF. 
Indien &apos;n biblioteek nie daar is nie, verskyn &apos;n *</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Maak oop...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Stoor &amp;as...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished">Gaa&amp;n terug na gestoorde</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished">Versamel vir &amp;uitvoer...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="unfinished">Teks verkrygen...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished">Voeg teks &amp;toe...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished">Stoor &amp;teks...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished">Bladsy stoor als &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished">Stoor as P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished">Documentin&amp;stellingen...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">Afdru&amp;kken...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished">A&amp;fsluiten</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished">Maak Inhoud &amp;leeg</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">&amp;Alles selecteren</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Zoeken/vervangen...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished">Kle&amp;uren...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished">&amp;Alineastylen...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished">&amp;Lynstylen...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="unfinished">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished">&amp;Voorkeuren...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished">&amp;Overig...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished">&amp;Links</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished">&amp;Midden</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished">&amp;Regs</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished">&amp;Blok</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished">Ge&amp;forceerd</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="unfinished">&amp;Onderstreep</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="unfinished">&amp;Outline</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="unfinished">&amp;Tabulators...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished">&amp;Meervoudig dupliceren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished">&amp;Groep losmaken</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
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
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished">Stuur na &amp;kladblok</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished">P&amp;rent sigbaar</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="unfinished">Dit is &apos;n PDF-&amp;boekmerk</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished">Dit is &apos;n PDF-a&amp;nnotasie</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished">&amp;Eienskappe van annotasie</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished">&amp;Veldeienskappe</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished">&amp;Bewerk Vorm...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished">Teks &amp;aan pad toevoegen</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished">Teks van &amp;pad losmaken</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished">Polygonen &amp;samenvoegen</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished">&amp;Polygonen opsplitsen</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished">&amp;Bezierkurwe</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished">&amp;Buitelyne</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">&amp;Teksraam</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="unfinished">&amp;Invoegen...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished">Verwy&amp;deren...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished">&amp;Verplaatsen...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished">&amp;Hulplynen beheren...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;50%</source>
        <translation type="unfinished">&amp;50%</translation>
    </message>
    <message>
        <source>&amp;75%</source>
        <translation type="unfinished">&amp;75%</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="unfinished">&amp;100%</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation type="unfinished">&amp;200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="unfinished">Minia&amp;turen</translation>
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
        <source>Show &amp;Text Chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
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
        <translation type="unfinished">&amp;Eienskappe</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished">&amp;Kladblok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished">&amp;Lagen</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Bladwyzers</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="unfinished">&amp;Gereedschappen</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished">P&amp;DF-gereedschappen</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Vorm</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished">&amp;Lyn</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="unfinished">Item roteren</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="unfinished">In- of uitzoomen</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="unfinished">Inhoud van raam bewerken</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="unfinished">Bewerk Teks...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="unfinished">Teksrame koppelen</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="unfinished">Teksrame loskoppelen</translation>
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
        <translation type="unfinished">Teks met de story-editor bewerken</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="unfinished">Teksraam invoegen</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="unfinished">Vryehand-curve invoegen</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished">Prente &amp;beheren</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished">Teks a&amp;fbreken</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="unfinished">&amp;Info over Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished">Info over &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished">Hul&amp;pballonnen</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished">Scribus &amp;handboek...</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Line</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>Double Turned Comma</source>
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
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Gevorderde opsies</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Bladsy(e) &amp;horisontaal spielbeeld</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Bladsy(e) &amp;verticaal spielbeeld</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Onde&amp;rliggende kleurverwydering</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>&amp;ICC-profiel toepassing</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript Vlak &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript vlak &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript vlak &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Maak PostScript vlak 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Maak PostScript vlak 2, dit kan baie 
groot lêers tot gevolg hê.</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Maak PostScript vlak 1, dit kan baie 
groot lêers tot gevolg hê.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>n&apos; Methode om grys skadu&apos;s saamgestel uit cyan, geel en magenta
te vervang met swart gebaseerde grys.
UCR (Under Color Removal) het meeste invloed op beeld dele waar neutrale en/of donker tone naby aan grys is. Die gebruik hiervan kan sekere beelde beter laat druk, maar eksperimenteer daarmee. UCR verminder die kans vir CMY ink oorbodigheid.</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Verdeel/Skouer</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Skouer</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horisontaal</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Links</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Middel</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Regs</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Tussen:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">Moenie veran&amp;der nie</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">&amp;Skouer</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">Verpla&amp;sing</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">Verdeel g&amp;elyk</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikaal</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Bokant</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Onderkant</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">T&amp;ussen:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">Moe&amp;nie verander nie</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">&amp;Skouer</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">Ver&amp;plasing</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">&amp;Verdeel gelyk</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">Toe&amp;pas</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Skouer</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Bladsy</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y: %1%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X: %1%2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Teks links skouer</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Teks regs skouer</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Teks sentreer</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Teks dubbelskouer</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Teks geforseerd dubbelskouer</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Veldeienskappe</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tipe:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knop</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Teksveld</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Keusevakkie</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Komboboks</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lys</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Eienskappe</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Naam:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Hulpballon:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Teks</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Lettertipe vir PDF 1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Rand</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Kleur:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Breedte:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Smal</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Breed</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Styl:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Solied</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Gestreep</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Onderstreep</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Afgeskuins</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Inlêend</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Ander</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Lees-alleen</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Vereis</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Waarde nie uitvoer nie</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Sigbaarheid:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Sigbaar</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Onsigbaar</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Nie druk</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Nie weergee nie</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Voorkoms</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Teks vir gedrukte knop</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Teks vir muis-oor</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikone</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Gebruik Ikone</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Verwyder</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Ingedruk</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Muis-oor</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Plaatsing van ikoon...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Beklemtoon</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Keerom</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Buitelyn</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Druk</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Meerlynig</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Wagwoord</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Limiet van</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Karakters</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Moenie skuif nie</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Geen spellingkontrole</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Kontroleer styl:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Kontroleer</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kruis</translation>
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
        <translation>Ster</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Vierkant</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Verstek is gekeis</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Bewerkbaar</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opsies</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Javascript</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gaan na</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Stuur vorm</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Maak vorm skoon</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Voer data in</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Gebeurtenis:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Muisknop hoog</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Muisknop laag</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Muis kom oor</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Muis gaan weg</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Met fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Met verlaat van fokus</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Bewerk...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Stuur na URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Stuur data as HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Voer data in vanaf:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Bestemming</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Na lêer:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Wysig...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Bladsy:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-posisie:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-posisie:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Aksie</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Veld is geformatteer as:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Eenvoudig</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Nommer</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Persentasie</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Tyd</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Aangepas</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Nommerformaat</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Desimale:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Gebruik geldeenheid-simbool</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>plaas geldeenheid simbool vooraan</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formattering</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Persentasieformaat</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datumformaat</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Tydformaat</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Eie scripts</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formaat:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Sleuteldruk:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formaat</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Waarde is nie gevalideer nie</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Waarde moet groter of gelyk aan wees:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>en kleiner of gelyk aan:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Eie validasiescript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Valideer</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Waarde is niee berekend nie</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Waarde is</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>die som</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produk</translation>
    </message>
    <message>
        <source>average</source>
        <translation>gemiddeld</translation>
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
        <translation>van die volgende velde:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Kies...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Eie berekeningscript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Bereken</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Hierdie vlag word genegeer in PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Voer &apos;n komma geskeide lys van velde hier in</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>U het tenminste &apos;n ikoon vir Normaal nodig om ikone vir knoppe te kan gebruik</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Beelde (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Voorbeeld:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Keuse verandering</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-lêers (*.pdf);;Alle lêers (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Annotasie-eienskappe</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Teks</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Skakel</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Eksterne skakel</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Eksterne webskakel</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Bestemming</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-lêers (*.pdf);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tipe:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>V&amp;erander...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Bladsy:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Pos</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-pos:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normaal</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished">Alle &amp;bladsy(e)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>to</source>
        <translation type="unfinished">aan</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Atl+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Pas templaat toe</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Templaat:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normaal</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Pas op &amp;huidige bladsy toe</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Pas op &amp;ewe bladsye toe</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Pas op &amp;onewe bladsye toe</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Pas toe vanaf &amp;bladsy:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">Tot:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Kladblok</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuut</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Laai...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Stoor</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Stoor &amp;as...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Sluit</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Klein</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Middel</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Groot</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Lêer</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Voorbeeld</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Kladbloklêers (*.scs);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Herbenoem</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Verwyder</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Die naam &quot;%1&quot; is nie uniek nie.
Kies &apos;n ander naam.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objek</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Naam:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nuwe inskrywing</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bladwysers</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Skuif Bladwyser</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Voeg Bladwyser in</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bladwysers</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Plasing van ikone</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Uitleg:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Slegs opskrif</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Slegs ikoon</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Opskrif onder ikoon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Opskrif bo ikoon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Opskrif regs van ikoon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Opskrif links van ikoon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Opskrif dwarsoor ikoon</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skaal:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Altyd</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>As ikoon te klein is</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>As ikoon te groot is</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nooit</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skaalmetode:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporsioneel</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Nie proporsioneel nie</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikoon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Begin oor</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Instelling van kleurbeheer</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Kleurbeheer &amp;aktiveer</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Stelselprofiele</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Prentjies:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Soliede kleure:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Skerm:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Drukker:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Rendering intents</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perseptueel</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relatief Kleurmeetries</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Versadiging</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absoluut kleurmeetries</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>S&amp;kerm:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>D&amp;rukker:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;uleer drukker op skerm</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Merk kleure buite &amp;gamut</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Gebruiken &amp;Swartpuntkompensasie</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Standaard kleurprofiel vir geïmporteerde prente</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Standaard kleurprofiel vir effen kleuren op de bladsy</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Door uzelf aangemaakt of via fabrikant verkregen kleurprofiel.
Dit profiel is specifiek vir uw beeldscherm en niet algemeen (zoals byv. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Kleurprofiel vir uw printer van de printerfabrikant.
Dit profiel is specifiek vir uw printer en niet algemeen (zoals byv. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standaard rendering intent vir uw beeldscherm. Kies, tenzy u precies weet
wat u doet, vir Relatief colorimetrisch of Perceptueel.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standaard rendering intent vir uw printer. Kies, tenzy u precies weet
wat u doet, vir Relatief colorimetrisch of Perceptueel.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Laat een weergave op uw scherm zien, gebaseerd
op het gekozen printerprofiel.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Hiermee kunnen kleurgebieden gemarkeerd worden die niet correct zouden worden
afgedrukt. Dit vereist zeer accurate profielen en dient enkel als waarschuwing.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Zwartpuntcompensatie is een methode om het contrast in foto&apos;s te verbeteren.
Als uw dokument foto&apos;s bevat wordt deze instelling aangeraden.</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for imported cmyk images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for imported rgb images</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Bewerk kleur</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Naam:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Kleur&amp;model</translation>
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
        <translation>RGB vir web</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nuut</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Oud</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-kleurkaart</translation>
    </message>
    <message>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
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
        <translation>Dinamiese kleurbalke</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statiese kleurbalke</translation>
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
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Kleurnaam is nie uniek nie</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Jy kan nie &apos;n kleur &quot;%1&quot; noem nie.
Dit dien as plekhouer vir deursigtig.</translation>
    </message>
    <message>
        <source>Is Spot-Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Registration-Color</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation type="unfinished">Karakter selecteren:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Lettertype:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Invoegen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished">Voegt de tekens in op de huidige cursorpositie</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished">Verwydert de huidige selectie(s).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="unfinished">Sluit deze dialoog om verder te werken aan de teks.</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Grieks</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Problems</source>
        <translation type="unfinished"></translation>
    </message>
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
        <translation type="unfinished"></translation>
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
        <source>Object is a placed PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Bladsy</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckerPrefsList</name>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
    <message>
        <source>PDF 1.3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Kanselleer</translation>
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
        <source>Normal Vision</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Protanopy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deuteranopy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Kleur</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Naam</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select Method:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>Difference between selected value and counted ones. See documentation for more info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click the wheel to get base color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here you have the sample color schema</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select one of the method to create color schema. See documentation for more info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here you have the color of your chosen color schema</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You can simulate common vision defects here. Just select type of the defect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">Toe&amp;pas</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Skadu:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Ondeursigtigheid:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Horisontale gradiënt</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Vertikale gradiënt</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonale gradiënt</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Kruisdiagonale gradiënt</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Straalgradiënt</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Vry liniêre gradiënt</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Vry straalgradiënt</translation>
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
        <source>Edit Line Color Properties</source>
        <translation>Kies lynkleur</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Kies vulkleur</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Kleurversadiging</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Vulmetode: normaal of gradiënt</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Kies deursigtigheid van gekose kleur</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Kleur van gekiesde objek</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation type="unfinished"></translation>
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
        <translation>Opsies vir CSV invoerder</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Veldskeidingsteken:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Waardeskeidingsteken:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Eerste lyn is &apos;n opskrif</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>REG</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Drukkeropsies</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Bladsy Bereik</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle bladsye</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Ewe bladsye</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Onewe bladsye</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Spieëlbeeld</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nee</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Oriëntasie</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Staande</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Lêend</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Op Druk</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Bladsy per vel</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Bladsye per vel</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Opsie</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Waarde</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Dit paneel toont verschillende CUPS-opties vir het afdrukken.
Welke instellingen precies mogelyk zyn hangt van uw printerdriver af.
Onder &quot;Help &gt; Info over&quot; kunt u kyken of CUPS-ondersteuning is ingebouwd.
Kyk vir de tekens C-C-T, welke staan vir C=CUPS, C=littlecms, T=TIFF-ondersteuning.
Een * geeft aan welke bibliotheken ontbreken</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Krimp lêer</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Sluit lettertipes in</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Enkodering:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Gaat na uw dokumentenmap.
Deze kan worden ingesteld in de voorkeuren.</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Verwyder kleur</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Verwyder kleur:</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Vervang met:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Verwyder bladsye</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Verwyder vorm:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>tot:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer
</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Ontbrekende lettertipe</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Die lettertipe %1 is nie geinstalleer nie.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Gebruik</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">in plaas daarvan</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">REG</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformasie</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Outeur:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Sleutelwoorde:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Beskrywing:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Uitgewer:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Medewerkers:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datum:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ipe:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formaat:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Identifiseerder:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>Br&amp;on:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>T&amp;aal:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>R&amp;elasie:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Dekking:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Reg&amp;te:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">Doku&amp;ment</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Meer &amp;informasie</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">De persoon of organisatie die de hoofdverantwoordelyke is vir het aanmaken van de inhoud van dit dokument.
Dit veld kan worden ingebed in het Scribus dokument, maar ook in de metadata van een PDF-lêer</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Een naam vir dit dokument.
Dit veld kan worden ingebed in het Scribus dokument, maar ook in de metadata van een PDF-lêer</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Een korte weergave van de inhoud van het dokument.
Dit wordt ook ingebed in PDF-lêers by exporteren</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">De onderwerpen van het dokument.
Deze sleutelwoorden worden ingebed in de PDF om zoeken en indexeren van PDF-lêers mogelyk te maken</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>De persoon of organisatie die dit dokument beschikbaar maakt</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Personen of organisaties die hebben bygedragen aan dit dokument</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Datum die by het dokument hoort, byv. de aanmaakdatum, in JJJJ-MM-DD formaat, zoals in ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Genre, categorie of type van dit dokument</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">De fysieke of digitale gestalte van het dokument, zoals mediatype en afmetingen.
RFC2045, RFC2046 vir MIME-lêerstypen zyn ook bruikbaar hier</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Een exacte referentie na dit dokument, zoals een ISBN-nummer of een URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Een verwyzing na een dokument waar dit dokument van is afgelyk, byv. ISBN of een URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">De taal waarin het dokument is geschreven, normaal gesproken een ISO-639 taalcode,
optioneel gevolgd door een koppelteken en een ISO-3166 landcode, byv. en-GB, nl-BE</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Een verwyzing na een gerelateerd dokument, mogelyk via een ISBN-nummer of URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Het bereik dat door het dokument wordt omvat, byvoorbeeld in plaats, tyd of jurisdictie</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informatie over rechten die vir dit dokument gelden, zoals copyright, patenten of handelsmerken</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation type="unfinished"></translation>
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
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Integer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>String</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Naam</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Waarde</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Toevoeg</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Drukker instelling</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Druk na</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Lêer</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Opsies...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Lêer:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Wysiging...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>A&amp;lternatiewe drukopdrag</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Gebruik een alternatief printcommando, zoals kprinter of gtklp,
om extra printopties te kunnen gebruiken</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>O&amp;pdrag:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Bereik</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Druk &amp;Alles</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Druk huidige bla&amp;dsy</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Druk Be&amp;reik</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Kies bladsy nommers. 1,3,4,6 of 1-4 vir 1 tot 4 
of * vir almal.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Aantal &amp;kopieë:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opsies</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Druk &amp;Normaal</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Druk &amp;skydings</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alles</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyaan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Geel</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Swart</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Druk &amp;in kleur indien beskikbaar</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Druk in &amp;grys</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Ge&amp;vorderde opsies...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Druk</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Stoor as</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscriptlêers (*.ps);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Postscript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Invoer van lêer
%1
het misluk!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatale fout</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Atl+O</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Bewerk styl</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Naam:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Karakter</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Lettertipe:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">&amp;Grootte:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Effek:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">&amp;Skouering:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">&amp;GrootHoof</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Lyne:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">&amp;Vulkleur:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">&amp;Lynkleur:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Vertikale tussenruimtes</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">Aanpas by basislyn &amp;rooster</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Lyn&amp;spasiëring:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">&amp;Bo:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">On&amp;der:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabs en induiking</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Naam van uw paragraafstyl</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Lettertype van object of geselecteerde teks</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tekengrootte</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Kleur van de teksvulling</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Kleur van de tekslynen</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Maakt de eerste letter van een paragraaf groter aan de bovenkant uitgelynd met het vervolg van een regel</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Geeft de hoogte van de kapitaal aan in het aantal regels dat deze beslaat</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Teks aan de basislyn van het raster uitlynen</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Witruimte boven de paragraaf</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Witruimte onder de paragraaf</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Tussenruimte tussen die reëls</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Die stylnaam is nie uniek nie</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distances</source>
        <translation type="unfinished">Afstande</translation>
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
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Bewerker</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuwe</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Maak oop...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Stoor &amp;as...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Stoor en gaan uit</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Gaan uit sonder om te stoor</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Ongedaanmaak</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Weer gedaanmaak</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kni&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiër</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>P&amp;lak</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>W&amp;is</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Kry Veldname</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Lêer</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Be&amp;werk</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascripts (*.js);;Alle lêers (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Kleur:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Skadu:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effects in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Kanselleer</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Kies uitvoer gids</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Voer as beeld(e) uit</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Voer na gids uit</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Wyzigen...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opties</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Afbeeldings&amp;type:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kwaliteit:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Resolusie:</translation>
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
        <translation>Bereik</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Huidige bl&amp;adsy</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Alle &amp;bladsy(e)</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Bereik</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Voer bereik van bladsye uit</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Kies bladsy nommers. 1,3,4,6 of 1-4 vir 1 tot 4 
of * vir almal.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Voer alle bladsye uit</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Voer net huidige bladsy uit</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Resolusie van beelde. 
Gebruik 72dpi vir beelde bedoel vir skerms</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Beeldkwaliteit: 100% vir beste, 1% vir swakste kwaliteit</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Beskikbare uitvoerformate</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Uitvoergids - die plek om beelde te stoor. 
Uitvoerlêernaam gaan &apos;dokumentnaam-bladsynommer.lêertipe&apos; wees</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Wysig uitvoergids</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Grootte:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Beeldgrootte. 100% geen verandering, 200% twee maal groter.</translation>
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
        <translation type="unfinished">Normaal</translation>
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
        <translation type="unfinished">Versadiging</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Kleur</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Oplos</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Ondeursigtigheid:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Naam</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Agtergrond</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="unfinished">Lae</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Paaie</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Grootte:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titel:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Geen titel</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Outeur:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Onbekend</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Kleure</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">Toe&amp;voeg</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuwe</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Be&amp;werk</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;upliseer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Verwy&amp;der</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Ve&amp;rwyder ongebruikte</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Kleurstelle</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Huidige kleurstel:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Stoor kleurstel</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Kies kleurstel om te laai</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Stoor huidige kleurstel</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Verwyder ongebruikte kleure uit dokument se kleurstel</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Voeg van &apos;n ander dokument se kleure by huidige dokument sin</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Maak &apos;n nuwe kleur in huidige kleurstel</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Verander gekiesde kleur</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Maak &apos;n kopie van huidiglik gekiesde kleur</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Verwyder huidig gekiesde kleur</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Maak huidige kleurstel die verstek</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Naam:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Kies &apos;n naam</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie van %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nuwe kleur</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Voer in</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Programwye lettertipe-instellings</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Beskikbare lettertipes</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Lettertipe-vervangings</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Additionele lokasies</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Lettertipe Naam</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Gebruik Lettertipe</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Inbed in:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Substel</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tipe</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Pad na lettertipelêer</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Beskikbare &amp;lettertipes</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Vervanging</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Verwy&amp;der</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Lettertipe-&amp;vervanging</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Wysig...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Toevoeg...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Ve&amp;rwyder</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Additionele &amp;paaie</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Kies &apos;n gids</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Lettertipe Naam</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation type="unfinished">Gebruik Lettertipe</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="unfinished">Inbed in:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Substel</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished">Pad na lettertipelêer</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Lettertipe voorbeeld</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Atl+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Filmquiz bracht knappe ex-yogi van de wys</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Voeg gekose lettertipe in styl, Lettertipe kieskaart in</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Verlaat voorbeeld</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Lettertipe Naam</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Substel</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Lettertipe voorbeeld</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="unfinished">Atl+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished">Voeg gekose lettertipe in styl, Lettertipe kieskaart in</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Verlaat voorbeeld</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished">Filmquiz bracht knappe ex-yogi van de wys</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Zoeken</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
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
        <source>This document contains some fonts that are not installed on your system,
please choose a suitable replacement for them.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Posisie:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Hier kan u kleur-stops toevoeg, bewerk of verwyder.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Bestuur hulplyne</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Horisontale hulplyne</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-pos:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Toevoeg</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>V&amp;erwyder</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Vertikale hulplyne</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-pos:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Toe&amp;voeg</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>Ve&amp;rwyder</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>S&amp;luit Hulplyne</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Bladsy</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Helaas, geen handleiding beskikbaar! Kyk op http://docs.scribus.net/ vir bygewerkte dokumentasie en op http://www.scribus.net/ vir downloads.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Inhoud</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Skakel</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus aanlyn hulp</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Zoeken</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">Afdru&amp;kken...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
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
    <message>
        <source>New Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Moontlike afkapping</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aanvaar</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Oorslaan</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Afkapinstellings</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Taal:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Kleinste woord:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Lengte van korste afkapbare woord.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maksimum aantal opeenvolgende afkappings.
0 beteken onbeperk.</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>A&amp;fkapvoorstelle</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Kap woorde ty&amp;dens tik af</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Met opsie Ekstra, Teksafkap kry u n dialoogvenster met alle afkapmoontlikhede vir elke woord.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Sit intydse -terwyl jy tik- afkapping aan.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Opeenvolgende &amp;afkappings toegelaat:</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Voeg Bladsy in</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Invoeging</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Bladsy(e)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>voor bladsy</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>na bladsy</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>aan die einde</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Invoeging</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Templaat (linker bladsy):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Templaat:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaal</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Templaat (regter bladsy):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Invoegen</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Bladsygrootte</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Grootte:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orië&amp;ntasie:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Breedte:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Hoogte:</translation>
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
        <translation>Voeg Tabel in</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Aantal rye:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Aantal kolomme:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Kanselleer</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Verander JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>B&amp;ewerk...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Toevoeg...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Verwy&amp;der</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Maak toe</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nuwe script:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nuwe script</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nee</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Wil u die script werklik verwyder?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Bestuur sleutelbord kortpaaie</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Aksie</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Huidige sleutel</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Kies sleutel vir die aksie</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Geen sleutel</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Gebr&amp;uikersgedefiniëerd</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>Alt+Shift+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Stel &amp;sleutel</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
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
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Die sleutel word reeds gebruik</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Lae</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Voeg Nuwe laag by</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Verwyder Laag</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Verhoog Laag</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Laatsak Laag</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Nuwe laag</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Wil jy alle objekte in hierdie laag ook uitvee?</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Naam</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Verander Lynstyl</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Toevoeg</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuwe</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>B&amp;ewerk</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;upliseer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Verwy&amp;der</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>Op&amp;slaan</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie van %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuwe styl</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nee</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Wil U werklik hierdie styl verwyder?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Voer in</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author:</source>
        <translation type="unfinished">Outeur:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Atl+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
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
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Atl+O</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Omschryving</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">B&amp;ewerk...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Voer in</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Bladsygrootte</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Grootte:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orië&amp;ntasie:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Breedte:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Hoogte:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="unfinished">Tipe:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">On&amp;der:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Bo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Regs:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Links:</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">B&amp;innenkant:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">B&amp;uitenkant:</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Duplicates the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Nee</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Naam:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopie van %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Naam:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopie #%1 van</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normaal</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Meervoudig dupliseer</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>Aa&amp;ntal kopieë:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horisontale verplasing:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikale verplasing:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Afstande</translation>
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
        <translation>Hoek:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Lengte:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Scriptfout</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">As dit &apos;n amptelike script is, gee hierdie fout asb. in by &lt;a href=&quot;http://bugs.scribus.net&quot;&gt; bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Die is in die knip-en-plakbord. Met Ctrl+V kan u die fout in bugtracker gaan plak.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Wys &amp;Konsole</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Versteek &amp;Konsole</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Voer templaat in</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Voer bladsy(e) in</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Vanaf dokument:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Wysig...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Voer Bladsy(e) in:</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Kies bladsy nommers. 1,3,4,6 of 1-4 vir 1 tot 4 
of * vir almal.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> vanaf 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Skep Bladsy(e)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">voor bladsy</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">na bladsy</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">aan die einde</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Invoer</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Kanselleer</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> vanaf %1</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>After Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>At End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Voer in</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation type="unfinished">Ontbrekende lettertipe</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished">Die lettertipe %1 is nie geinstalleer nie.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished">Gebruik</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished">in plaas daarvan</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>skuif Bladsye</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopiër Bladsy</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Skuif Bladsy(e):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>na:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">voor bladsy</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">na bladsy</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">aan die einde</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>After Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>At End</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Eienskappe</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Vorm</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Teks</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Prent</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Lyn</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Kleure</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Naam</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Plasing</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-pos:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-pos:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breedte:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hoogte:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotasie:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Basispunt:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Vlak</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Vorm:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Bewerk Vorm...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>R&amp;onde
hoeke:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Afstand van teks</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Kolo&amp;mme:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Tussen&amp;ruimte:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Bo:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>On&amp;der:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Regs:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>T&amp;abuleerders...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Tekspad-eienskappe</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Wys kurwe</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Beginafstand:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Afstand vanaf kurwe:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Teks &amp;loop om raam</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Gebruiken &amp;Bounding Box</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Gebr&amp;uik Kontourlyn</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Lettertipe grootte:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Skadu:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Aangepaste spasiëring</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Letterspasiëring</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">&amp;Lynspasiëring:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;yl:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Taal:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Skaleer &amp;Vrylik</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;X-skaal:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Y-skaal:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Skaal na &amp;raamgrootte</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proporsioneel</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Voer profiel in:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Rendering intent:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perseptueel</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relatief kleurmetries</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Versadiging</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absoluut kleurmetries</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Linker punt</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Eindpunt</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Basispunt:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Lynt&amp;ipe:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Lyndi&amp;kte:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Miter koppel</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Bevel koppel</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Ronde koppel</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Hoeke:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plat</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Vierkant</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Rond</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Eindpunte:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Geen styl</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Sellyne</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Bolyn</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linkerlyn</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Regterlyn</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Onderlyn</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Naam van gekose objek</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horisontale posisie van huidige basispunt</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikale posisie van het huidige basispunt</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Breedte</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Hoogte</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotasie van objek op huidige basispunt</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punt waarvandaan die rotasiehoek bereken word</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Kies linksbo as basispunt</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Kies regtsbo as basispunt</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Kies linksonder as basispunt</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Kies regtsonder as basispunt</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Kies middel as basispunt</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Spieël Horisontaal</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Spieël Vertikaal</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Skuif een vlak op</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Skuif een vlak af</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Bring na voorgrond</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Stuur na agtergrond</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Dui vlak waarop objek is, 0 beteken onder</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Sluit of ontsluit objek</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Sluit of ontsluit objekgrootte</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Skakel druk van objek aan/af</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Maak teks in laegeleëde rame om objek vloei</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Gebruik &apos;n omliggende regthoek ipv die vorm van die raam vir die teksvloei</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Gebruik &apos;n tweede lyn gebaseerd op die raamvorm vir teksvloei</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Lettertipe van geselekteerde teks of objek</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Lettergrootte</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skaalbreedte van letters</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Kleur van teks</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Kleur van teksagtergrond</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Versadiging van tekskleur</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Versadiging van teksagtergrondkleur</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Handgestelde letterspasiëring</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Lynspasiëring</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Styl van huidige paragraaf</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Afkaptaalinstelling van raam</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Verander instellings vir linker of eindpunte</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Lynpatroon</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Lyndikte</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tipe lynverbindings</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tipe lyneinde</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Lynstyl van huidige objek</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Kies raamvorm...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Verander raamvorm...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Stel straal van ronde hoeke</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Aantal kolomme in teksraam</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Skakel tussen Tussenruimte of Kolombreedte</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Afstand tussen kolomme</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Afstand van teks tot raambokant</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Afstand van teks tot raamonderkant</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Afstand van teks tot raamlinkerkant</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Afstand van teks tot raamregterkant</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Verander Tabinstelling van teksraam...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Laat prent en raam grootte verskille toe</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Horisontale afstand van prente in raam</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Vertikale afstand van prente in raam</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Skaleer prent horisontaal</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Skaleer prent vertikaal</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Hou X- en Y-skaal gelyk</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Hoe prent in verhouding</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Maak prent presies in raam pas</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Gebruik prent proporsies ipv raam proporsies</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Bronprofiel van prent</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Rendering intent van prent</translation>
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
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Kolombreedte</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Die naam &quot;%1&quot; is nie uniek nie.
Kies &apos;n ander naam.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Regs na Links Skryf</translation>
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
        <source>Actual X-DPI:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
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
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Verander styl</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plat einde</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Vierkantige einde</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Ronde einde</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Miter koppel</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Bevel koppel</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Ronde koppel</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Lyndikte:</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Soliede lyn</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Gestreepte lyn</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Stippellyn</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Streep-stip lyn</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Streep-stip-stip lyn</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Die naam &quot;%1&quot; is nie uniek nie.
Kies &apos;n ander naam.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
</context>
<context>
    <name>MusterPages</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Verander templaat</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Wil u regtig hierdie templaat uitvee?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Nee</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Naam:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Nuwe templaat</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopie van %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Naam:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopie #%1 van</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normaal</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Verander templaat</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">Toe&amp;voeg</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nuwe</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;upliseer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">Verwy&amp;der</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Maak toe</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Waarskuwing</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Nee</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Naam:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Nuwe templaat</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopie van %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Naam:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopie #%1 van</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normaal</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Wil u regtig hierdie templaat uitvee?</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nuwe dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Bladsygrootte</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Grootte:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter (US)</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Ponie</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Tuisgemaak</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orië&amp;ntasie:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Regop</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Gekantel</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breedte:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hoogte:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Teenoorstaande bladsye</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Linker&amp;bladsy eerste</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Kant hulplyne</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Regs:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Bo:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">On&amp;der:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opsies</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>&amp;Eerste bladsy nommer:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Verstek eenheid:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Punte (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Duim (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Picas (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Outomatise teksrame</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Kolom hulplyne</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Ruimte:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Kolo&amp;mme:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Bladsygrootte, standaard of aangepas</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Oriëntatie van dokumentbladsye</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Breedte van dokumentbladsye, kies &apos;Tuisgemaak&apos; om self &apos;n waarde te kan insit</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Hoogte van dokumentbladsye, kies &apos;Tuisgemaak&apos; om self &apos;n waarde te kan insit</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Bladsye is almal eenders of daar is linker en regter bladsye</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Laat dokument met n&apos; linker bladsy begin</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Afstand tussen boënste kantlyn en papierrand</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Afstand tussen onderste kantlyn en papierrand</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Afstand tussen linkerkantlyn en papierrand.
As &apos;Teenoorstaande bladsye&apos; gekies is, word die breedte gebruik vir korrekte binding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Afstand tussen regterkantlyn en papierrand.
As &apos;Teenoorstaande bladsye&apos; gekies is, word die breedte gebruik vir korrekte binding</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Eerste bladsynommer van dokument</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Verstek meeteenheid vir dokumentbewerking</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Maak outomaties teksrame saammet bladsy byvoeging</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Aantal kolomme in outomaties geskepte teksrame</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Afstand tussen outomaties geskepte kolomme</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">B&amp;innenkant:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">B&amp;uitenkant:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Bestuurs</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Grootboek</translation>
    </message>
    <message>
        <source>Open Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Alle lêers (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Maak oop</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Linker bladsy</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Regter bladsy</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nodes</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolute koördinate</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-pos:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-pos:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Verander &amp;Kontoerlyn</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Herstel Kontoe&amp;rlyn</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Beëindigen bewerking</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Skuif Nodes</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Skuif Kontrolepunte</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Voeg Nodes by</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Verwyder Nodes</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Skuif kontrolepunte onafhanklik</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Skuif kontrolepunte simmetries</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Herstel kontrolepunte</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Herstel hierdie kontrolepunt</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Begin &apos;n Poligoon of sny &apos;n Bezierkurwe</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Sluit hierdie Bezierkurwe</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Spieël pad horisontaal</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Spieël pad vertikaal</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Trek pad horisontaal na regs skeef</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Trek pad horisontaal na links skeef</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Trek pad vertikaal opwaarts skeef</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Trek pad vertikaal afwaarts skeef</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Roteer pad antikloksgewys</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Roteer pad kloksgewys</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Verklein padgrootte met getoonde %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Vergroot padgrootte met getoonde %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Rotasiehoek</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% om mee te vergroot of verklein</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktiveer Kontoerlyn bewerking modus</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Herstel kontoerlyn na oorspronklik raamgrootte</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Indien gekies word koördinate relatief tot die bladsy
gebruik, anders is koördinate relatief tot die objek.</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Documentnaam als prefix vir alineastylen gebruiken</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation type="unfinished">Deze vraag niet meer stellen</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation type="obsolete">Scriptkonsole</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Maak PDF-lêer</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Voer &amp;uit na lêer:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Wysi&amp;g...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Uitvoerbereik</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Alle bladsye</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">Kies &amp;Bladsye</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Lêersopsies</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">&amp;Aanpasbaarheid:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">&amp;Binding:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Linkerkantlyn</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Regterkantlyn</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Minia&amp;tuur prentjies</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Stoor geskake&amp;lde teksrame as PDF-artikels</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">&amp;Voeg Boekmerke in</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete">dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Resolusie:</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Prentinstellings</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Outomaties</translation>
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
        <translation type="obsolete">Geen</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kwaliteit:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Maksimum</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Hoog</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Laag</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Minimum</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">&amp;Hermonster Prente af na:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">Al&amp;gemeen</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">Inbed alle l&amp;ettertipes</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Inbedding</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Beskikbare lettertipes:</translation>
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
        <source>Fonts to embed:</source>
        <translation type="obsolete">Lettertipes om in te bed:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Lettertipes</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Bladsy</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">&amp;Vertoon Bladsyvoorbeeld</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Effekte</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">Verton ty&amp;dsduur:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Effek &amp;tydsduur:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">Effekt&amp;ipe:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">&amp;Bewegende lyne:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">&amp;Van die:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">R&amp;igting:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete">sec</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Geen effek</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Blinders</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Boks</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Oplos</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Glinster</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Deel</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Uitvee</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horisontaal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikaal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Binne</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Buite</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Links na regs</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Bo na onder</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Onder na bo</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Regs na links</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Linksbo na regsonder</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">Pas Effek op &amp;alle bladsye toe</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">Gebruik enkripsie</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Wagwoord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">Gebr&amp;uiker:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">&amp;Eienaar:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Instellings</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Laat &amp;druk van dokument toe</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Laat Wy&amp;siging van dokument toe</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Laat &amp;Kopiëring van teks en prente toe</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Laat Toevoegen van &amp;annotasies en velde toe</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">V&amp;eiligheid</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Algemeen</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">&amp;Uitvoer bestem vir:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Skerm / web (RGB)</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Drukker (CMYK)</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">&amp;Gebruik tuisgemaakte rendering instellings</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Rendering instellings</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">Fre&amp;kwensie:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Hoek:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">&amp;Punt-funksie:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Enkele stip</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Lyn</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Rond</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Ovaal</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Soliede kleure:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Gebruik ICC-profiel</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profiel:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">Rendering intent:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Perseptueel</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Relatief kleurmetries</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Versadiging</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Absoluut kleurmetries</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Prente:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Moenie ingebedde ICC-profiele gebruik nie</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">&amp;Kleur</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">PDF/X-3 uitvoer doel</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">&amp;Inligting teks:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Uitvoer&amp;profiel:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Reghoekknipboks</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Stoor</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Voer alle bladsye na PDF uit</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Voer bereik van bladsye na PDF uit</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Skei bladsye met komme bv. 1,3,4,5 
of gebruik 1-4 wat als vanaf 1 tot en met 4 insluit 
of gebruik * vir alle bladsye</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Bepaal PDF-anpasbaarheid. Verstek is &quot;Acrobat 4.0&quot;, dit bied die meeste aanpasbaarheid.
Gebruik Acrobat 5.0 as u funksies uit PDF 1.4 gebruik, soos deursigtigheid of 128-bis enkripsie.
PDF/X-3 is vir kommersiële drukkers en is kiesbaar as jy kleurbestuur aktiveer.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Stel die bind van bladsye in die PDF. Los op verstek (links), tensy u weet wat u doen.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Maak miniatuurbladsye van elke bladsy in die PDF.
So mmige blaaiers kan dit vir navigasie gebruik.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Maak PDF-artikels, sodat gemakliker in geskakelde pdfs genavigeer kan word.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Inbed bladwysers in dokument, sodat lange
dokumente maklik deurblaai kan word.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Uitvoer resolusie van teks en vektorgrafika.
Dit het geen invloed op prent resolusies (soos JPG&apos;s) nie.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Kompressie van text en grafika
Moenie sonder rede verander nie. Dit verklein PDF groottes.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Kompressie metode vir prente.
Outomaties laat Scribus die beste metode kies.
ZIP is goed vir beelde met soliede kleure.
JPEG is veral effektief met baie prente (waarvoor &apos;n geringe verlies moontlik is).
Los op outomaties, tensy u &apos;n rede vir &apos;n bepaalde kompressie het.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Kompressievlakke: Minimum (25%), laag (50%), medium (75%), hoog (85%), maksimum (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Monster bitmap prente af na gekose DPI.
As u hierdie los, word hulle in hulle huidige resolusies gestoor.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">DPI (kolle per duim) vir prentjie uitvoer.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">In bed lettertipes in dIe PDF. Dit verseker dat dokument se voorkoms nie sal verander nie.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Skakel aanbieding effekte aan as Acrobat in volskerm modus is.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Wys bladsyvoorbeelde van bogenoemde bladsye.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Bladsy vertooning duurs voordat aanbieding effekte begin.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Tydsduur van effek.
Korter tyd sal effek versnel, &apos;n langer sal dit vertraag.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Tipe oorgangseffect.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Rigting van effek van bewegende lyne vir die split en verblind effekte.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Beginposisie vir die regthoek en split-effekte.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Rigting vir die glinster of vee-effekte.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Pas gekose effek op alle bladsye toe.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Schakelt beveiligingsfuncties in uw PDF in.
Als u Acrobat 4.0 selecteerde, zal de PDF beschermd zyn met 40-bit versleuteling.
Als u Acrobat 5.0 selecteerde, zal de PDF beschermd zyn met 128-bit versleuteling.
Let op: PDF-versleuteling is niet zo veilig als GPG of PGP-versleuteling en heeft enkele andere beperkingen.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Kies een master-wachtwoord om alle beveiligingsinstellingen
in of uit te schakelen in uw PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Kies een wachtwoord vir gebruikers om de PDF te kunnen lezen.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Het afdrukken van de PDF toestaan. Indien niet geselecteerd is afdrukken onmogelyk.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Wyzigen van de PDF toestaan. Indien niet geselecteerd is wyzigen onmogelyk.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Het kopiëren van teks of prente uit de PDF toestaan.
Indien niet geselecteerd is dit onmogelyk.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Het toevoegen van annotaties en velden aan de PDF toestaan.
Indien niet geselecteerd, is dit onmogelyk.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Kleurmodel vir uitvoer van u PDF.
Kies Skerm/web (RGB) vir PDF&apos;s wat op skerms vertoon of tipiese Inkjet drukkers gedruk word.
Kies Printer (CMYK) as u na &apos;n egte vierkleur (CMYK) drukker druk.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Hierdie is &apos;n gevorderde stelling wat net gebruik moet word as jou drukker vir jou sê om dit te stel. Raadpleeg die poslys vir u hiermee peuter.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Inbed &apos;n kleurprofiel vir soliede kleure</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Kleurprofiel vir soliede kleure</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">Rendering doel vir soliede kleure</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Inbed van &apos;n kleurprofiel vir prente</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Kleurprofiele in prentjies moenie gebruik word nie</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Kleurprofiel vir prente</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Rendering doel vir prente</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Uitvoerprofiel vir drukwerk. Indien moontlik, probeer leiding by u drukker te kry.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Verpligte string vir PDF/X-3, anders voldoen PDF nie aan PDF/X-3 standaard nie. Ons beveel die dokument titel aan.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Afloop-afstand vanaf die bokant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Afloop-afstand vanaf die onderkant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Afloop-afstand vanaf die linkerkant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Afloop-afstand vanaf die regterkant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Stoor as</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-lêers (*.pdf);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotasie:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Krimp teks en vektorgrafika</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">&amp;Substel alle lettertipes</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Lettertipes om te substel:</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">Sk&amp;akel aanbieding effekte aan</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Aanbieding</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Spieëlbeeld bladsy(e) horisontaal</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Spieëlbeeld bladsy(e) vertikaal</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Drukvoorbeeld</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Anti-aliaseer &amp;teks</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Anti-aliaseer &amp;grafika</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Vertoon deursi&amp;gtigheid</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>Onderleênde kle&amp;ur verwydering</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Vertoon CMYK</translation>
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
        <translation>Verskaf mooier teks in ruil vir spoed. Dit het net &apos;n effek op Tipe 1 lettertipes</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Vertoon deursigtigheid en deursigtige items in dokument. Vereis Ghostscript 7.07 of nuwer</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Gee &apos;n drukvoorbeeld weer met simulaties van CMYK-inke, ipv RGB-kleure</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Skakel die C (Cyaan) inkplaat aan of af</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Skakel die M (Magenta) inkplaat aan of af</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Skakel die Y (Yellow = geel) inkplaat aan of af</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Skakel die K (blacK = swart) inkplaat aan of af</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alles</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>I metode om CMY grys te vervang met swart grys.
UCR (Under Color Removal) het veral &apos;n invloed op donker en neutrale skadus naby grys. Sommige prente sal mooier druk met die, maar mens moet maar maarmee eksperimenteer.
UCR verklein die kans op overversadiging met CMY inke.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Verskaf mooier TrueType letters, Opentype letters, EPS, PDF en vektorgrafika vertoning, maar maak die proses effens stadiger.</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopiër hierheen</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Skuif hierheen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Kanselleer</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Plak</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Vertoon &amp;Kantlyne</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Vertoon &amp;Rame</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Wys pren&amp;te</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Wys R&amp;ooster</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Wys H&amp;ulplyne</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Wys &amp;Basislyn-rooster</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">Ma&amp;gnetise rooster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Magnetise hul&amp;plyne</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Prent</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Lêer: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">Oorspronklike PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">Werklike PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Geskakelde teks</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Teksraam</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Teks op &apos;n pad</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Paragrawe: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Woorde:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Letters:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Druk: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Aan</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Af</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">In&amp;lig</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">Kry &amp;Prent...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">P&amp;rent sigbaar</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">Opdatee&amp;r prent</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">Bewerk pr&amp;ent</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">Pas raamgrootte by &amp;prent aan</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">Kr&amp;y Teks...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">V&amp;oeg Teks agteraan...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">Bewerk T&amp;eks...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Dit is &apos;n PDF-&amp;boekmerk</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">Dit is &apos;n PDF-a&amp;nnotasie</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Eienskappe van annotasie</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">&amp;Veldeienskappe</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF-opsies</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Bewerk Teks...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">S&amp;luit</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">Onts&amp;luit</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Sluit Objek&amp;grootte</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Ontsluit Objek&amp;grootte</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Stuur na &amp;kladblok</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Stuur na &amp;laag</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">Voeg voorbeeldteks &amp;in</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Groepeer</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Ont&amp;groepeer</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">&amp;Vlak</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Stuur na &amp;agtergrond</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Bring na &amp;voorgrond</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Laatsak</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Lig</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Prent&amp;raam</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Pol&amp;ygoon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Buitelyne</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Teksraam</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezierkurwe</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">Skakel om n&amp;a</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Kni&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiër</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">Vee&amp;uit</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Maak Inhoud &amp;leeg</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Toon &amp;Eienskappe...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Versteek &amp;Eienskappe...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Geen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Waarskuwing</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Wil u werklik al die teks uitvee?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Die program</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">ontbreek op u masjien!</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopie van</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Prent</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Teks</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Lyn</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygoon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylyn</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Tekspad</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Kopie van</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
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
        <source>Page Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Naam</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Waarde</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Toevoeg</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Bladsy</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation>van %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Letter (US)</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished">Grootboek</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Post</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Crown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Large Post</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Royal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Elephant</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>STMT</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Resultaat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Soekresultate vir: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Voorbeeld</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Kies</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Prente</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Naam</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Pad</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Bladsy</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Druk</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ga na</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Ontbrekend</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Soek</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Prente beheren</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Persistent</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Standard</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">Onbekend</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygoon-eienskappe</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Ho&amp;eke:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotasie:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Aantal hoeke vir polygoon</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Rotasie in grade vir polygoon</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Voorbeeld polygoon</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Pas &amp;Faktor toe</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Pas konveks/konkaaf-faktor toe om vorm van polygone te verander</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">&apos;n Negatiewe waarde maak die polygoon konkaaf (stervormig), 
&apos;n positiewe waarde maak hom konveks</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished">Ho&amp;eke:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotasie:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished">Pas &amp;Faktor toe</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Faktor:</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished">&apos;n Negatiewe waarde maak die polygoon konkaaf (stervormig), 
&apos;n positiewe waarde maak hom konveks</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Voorkeure</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Algemeen</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hulplyne</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Gereedskap</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Kladblok</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vertoon</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation type="unfinished">Eksterne hulpprogramme</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Verskei</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>S&amp;tyl:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Lettergrootte:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Eenhede</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Punte (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Duime (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pica&apos;s (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Muisinstelling</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Wiel sprong:</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete">px</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">&amp;Gryp afstand:</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Menu&apos;s</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Onlangse dokumente:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Paaie</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Wysig...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-profiele:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Wys&amp;ig...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Wy&amp;sig...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Bladsygrootte</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Tuisgemaak</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Grootte:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Regop</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Gekantel</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orië&amp;ntasie:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breedte:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hoogte:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Ooreenstaande bladsye</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Linker&amp;bladsy eerste</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Kantlyn hulplyne</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>On&amp;der:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Bo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Regs:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Outomaties stoor</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>&amp;Aan</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Interval:</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Roosteruitleg</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Kl&amp;einrooster spasiëring:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">&amp;Grootrooster spasiëring:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Hulplyn&amp;magnetisme afstand:</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Roosterkleuren</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">&amp;Kleinrooster kleur:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">&amp;Grootrooster kleur:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">H&amp;ulplyne se kleur:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">Basis&amp;lyn-rooster kleur:</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Plasing</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">In die &amp;Agtergrond</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">In die &amp;Voorgrond</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Basislyn-rooster</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">&amp;Aan</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">&amp;Af</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Onderskrif</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">Ver&amp;plasing:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Skaalering:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Boskrif</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Verpla&amp;sing:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">S&amp;kaalering:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Kleinhoofletters</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Sk&amp;aalering:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Ander</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Basislyn &amp;rooster:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Basislyn &amp;beginafstand:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Outomatise &amp;lynspasiëring:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">Ver&amp;stek lettertipe:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">Verstek&amp;grootte:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">&amp;Tekskleur:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">Kolo&amp;mme:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">Tussen&amp;ruimte:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Filmquiz bracht knappe ex-yogi van de wys</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Geen</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">&amp;Lynkleur:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Skadu:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">&amp;Vulkleur:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">S&amp;kadu:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">Lynt&amp;ipe:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">Lyndi&amp;kte:</translation>
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
        <translation type="obsolete">&amp;Stappe van:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">&amp;Vry skalerend</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">&amp;Horisontale skaal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">&amp;Vertikale skaal:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">&amp;Skaleer prent na raamgrootte</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Behou Prentve&amp;rhouding</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">V&amp;ulkleur:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Ho&amp;eke:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotasie:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Ander opsies</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>&amp;Stoor inhoud met wysigen</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Voorbeeld</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Klein</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Middel</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Groot</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Vertoon Bladsye lang&amp;s mekaar</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Bladsykleure</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Agtergrond:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Kantlyne:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vertoon &amp;Nie-drukbare gebiede in kantlynkleur</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Gebruik PDF-1.4 se &amp;Deursigtigheidsfunksies</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Pas Vertoongrootte &amp;aan</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">Om aan te pas sleep die skuifknop op die lineaal hieronder.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>PostScript-interpreter</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Naam van uitvoerbare program:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Anti-aliaseer &amp;teks</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Anti-aliaseer &amp;grafika</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Program vir prentverwerking</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Naam van uitvoerbare &amp;program:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Druk</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Knip als buite bladsy&amp;kantlyne af</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Verwyder Onde&amp;rlêende kleur</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Kies die verstek vensterdekorasie en styl.
Scribus kan elke beskikbare KDE- of Qt-styl oorneem</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Verstek lettergrootte vir menu&apos;s en vensters</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Verstek meeteenheid vir dokumentbewerking</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Aantal lyntjies wat Scribus deurskuif vir elke muiswiel rol</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Gee die straal aan van die gebied waar Scribus u toelaat om &apos;n object se muisvatsels te gryp</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Aantal onlangs geopende dokumenten wat Scribus in Lêer menu moet vertoon</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Verstekgids vir dokumente</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Verstekgids vir ICC-profiele</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Verstekgids vir Scripts</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Verstek bladsygrootte</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Verstekoriëntasie van dokumentbladsye</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Breedte van bladsye, kies Tuisgemaak om u eie waardes in te vul</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Hoogte van bladsye, kies Tuisgemaak om u eie waardes in te vul</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Sit enkel of spreibasis uileg aan</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Laat dokument met &apos;n linker bladsy begin</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Afstand tussen die bokant van die kantlyn en die papierrand</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Afstand tussen die onderkant van die kantlyn en die papierrand</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Afstand tussen die linkerkant van die kantlyn en die papierrand
As teenoorstaande bladsy gekies is, word die breedte gebruikt aan die rugkant van die dokument</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Afstand tussen die regterkant van die kantlyn en die papierrand
As teenoorstaande bladsy gekies is, word die breedte gebruikt aan die buitekant van die dokument</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Indien aan, sal Scibus op elke tydsinterval &apos;n rugsteunkopie
van u dokument stoor met &apos;n .bak-uitbreiding</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Tydsinterval vir outomaties stoor</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Afstand tussen kleinroosterlyne</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Afstand tussen grootroosterlyne</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Afstand waarvandaan &apos;n objek na jou geplaasde hulplyne sal spring</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Kleur van kleinroosterlyne</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Kleur van grootroosterlyne</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Kleur van u eiehulplyne</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Plaas rooster onder u andere objekte</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Plaas rooster vir u andere objekte</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Skakel Basisrooster aan</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Skakel Basisraster af</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Verplasing bo basislyn van &apos;n lettertipe op &apos;n lyn</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Relatiewe grootte van boskrif tov normale teks</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Verplasing onder basislyn van &apos;n lettertipe op &apos;n lyn</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Relatiewe grootte van onderskrif tov normale teks</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Relatiewe grootte van kleinhoofletters tov normale teks</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Persentasie toename van lyn hoogte tov lettergrootte</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Eienskappe van teksraam</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Eienskappe van prentraam</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Eienskappe van vormsteken</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Verstek vergroottingsvlakke</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Lyneienskappe</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Polygooneienskappe</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Lettertipe vir nuwe teksrame</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Lettergrootte vir nuwe teksrame</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Letterkleur</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Aantal kolomme in &apos;n teksraam</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Tussenruimte tussen kolomme</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Voorbeeld van u lettertipe</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Prentrame laat prente na enige grootte skaleer</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Horisontale skaal vir prente</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Vertikale skaal vir prente</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Hou horisontale en vertikaleskaal gelyk</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Prente in prentrame word altyd na grootte van hul raam geskaleer</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Outomaties geskaalde prente houd hul oorspronklike prentverhouding</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Vulkleur vir prentrame</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Versadiging van die vulkleur</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Lynkleur van vorms</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Versadiging van die lynkleur</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Vulkleur van vorms</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Lynstyl van vorms</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Lyndikte van vorms</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Minimum toegelate vergroting</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Maksimum toegelate vergroting</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Verandering in grootte vir elke zoom-stap</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Lynkleur</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Kleurversadiging</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Lynstyl</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Dikte van lyne</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Aantal hoeke van polygone (veelhoeke)</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Grade in rotasie van polygone</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Voorbeeldpolygoon</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Kies die grootte van die voorbeeld in die kladblok-palet</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Stoor kladblok telkens na &apos;n wysiging</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Wanneer u teenoorstaande bladsye gebruik, toon die twee bladsye lanks mekaar</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Papierkleur</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Kleur vir die kantlyne</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Maskeer gebied buite die kantlyne in die kantlynkleur</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Skakel deursigtigheidsfunksie van PDF-1.4 aan</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Stel die verstekvergroottingsvlak</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Ghostscript-interpreter pad</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Antialiseer Teks vir EPS en PDF op skerm</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Antialiseer Grafieka vir EPS en PDF op skerm</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Pad na prentbewerker</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Moenie objekte buite die kantlyne op gedrukte of uitgevoerde lêers wys nie</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Kies gids</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">B&amp;innenkant:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">B&amp;uitenkant:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">T&amp;emplate:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Wysi&amp;g...</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Pas &amp;Faktor toe</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Ander gids vir dokument-template</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Pas konveks/konkaaf-faktor toe op die vorm van polygone</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">&apos;n Negatiewe waarde maak die polygoon konkaaf (stervormig), 
&apos;n positiewe waarde maak hom konveks</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>I metode om CMY grys te vervang met swart grys.
UCR (Under Color Removal) het veral &apos;n invloed op donker en neutrale skadus naby grys. Sommige prente sal mooier druk met die, maar mens moet maar maarmee eksperimenteer.
UCR verklein die kans op overversadiging met CMY inke.</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Uitvoerend</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Grootboek</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter (US)</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Ponie</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Kleur:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugins</source>
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
        <source>Select your default language for Scribus to run with.
Leave this blank to choose based on environment variables.
You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
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
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>dpi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How to run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Lêer</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Nee</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of linked frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
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
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
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
        <source>Locate Ghostscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="unfinished">Linker bladsy</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="unfinished">Regter bladsy</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Maak oop...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Stoor &amp;as...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Lêer</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
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
        <source>Script Console</source>
        <translation type="unfinished">Scriptkonsole</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open File With Python Commands</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Tin&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Vers.:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Waarde:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rooi:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Groen:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Blou:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>&amp;Alfakanaal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Basiskleure</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Tuisgemaakte kleure</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definieer tuisgemaakte kleure &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Voeg by tuisgemaakte kleure</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Kies Kleur</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Skuif of kopiër &apos;n Lêer</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Lees: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Skryf: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Lêer&amp;naam:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Lêer&amp;tipe:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Een Gids op</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle lêers (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Naam</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Grootte</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipe</translation>
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
        <translation>Reg</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Kyk &amp;in:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Terug</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Nuwe gids</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Lys aansig</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Detail aansig</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Vertoon Lêerinligting</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Vertoon Lêerinhoud</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lees-skryf</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Leesalleen</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Skryfalleen</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>ontoeganklik</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symlink na lêer</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symlink na gids</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symlink na spesiaal</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Lêer</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Gids</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Spesiaal</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Stoor as</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Maak oop</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Stoor</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>He&amp;rbenoem</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Uitv&amp;ee</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>W&amp;eerlaai</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Sorteer volgens &amp;naam</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Sorteer volgens &amp;grootte</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Sorteer volgens &amp;datum</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;ongesorteer</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sorteer</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Ver&amp;toon versteekte lêers</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>die lêer</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>die gids</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>die symlink</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Vee %1 uit</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Is u seker U wil %1 &quot;%2&quot; uitvee?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nee</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nuwe gids 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nuwe gids</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nuwe gids %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Soek gids</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Gidse</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Stoor</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fout</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Lêer nie gevind nie.
Kontroleer pad en lêernaam.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alle lêers (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Kies &apos;n gids</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Gids:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Lettertipe</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>St&amp;yl</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Grootte</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Dood&amp;getrek</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Onderstreep</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Kleur</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Voorbeeld</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Scr&amp;ipt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Pas toe</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Kanselleer</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Maak toe</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Kies Lettertipe</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Maak skoon</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Kies Alles</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Ongedaanmaak</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gedaanmaak</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kni&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiër</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>P&amp;lak</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Kry in lyn</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Pas self aan...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Inligting oor Qt&lt;/h3&gt;&lt;p&gt;Die program gebruik Qt weergawe %1.&lt;/p&gt;&lt;p&gt;Qt is &apos;n C++-toolkit vir die ontwikkeling van programme op verskillende platforms.&lt;/p&gt;&lt;p&gt;Qt bied bronkode-aanpasbaarheit tussen MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, en alle belangrike UNIX-variante.&lt;br&gt;Qt is ook beskikbaar vir ingebedde apparate.&lt;/p&gt;&lt;p&gt;Qt is &apos;n product van Trolltech. Sien &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; vir meer informasie.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation>Teks word ingevoer</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle ondersteunde formate</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle lêers (*)</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Besig om te initialiseer...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Agtergrond</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarskuwing</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Wil u werklik %1 oorskryf?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Lettertipe voorbeeld</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">Voeg spesiale ding &amp;In</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nuut van &amp;templaat...</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="obsolete">Templaat:</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Nuusbriewe</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brosjures</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalogusse</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Pamflette</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Tekens</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kaartjies</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Briefhoofde</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Briewe</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visitekaartjies</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalenders</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Advertenties</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Labels</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Spyskaarte</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programme</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF-vorms</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-aanbiedings</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Tydskrifte</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plakkate</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Aankondigings</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Teksdokumente</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Gevoude dokumente</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Eien Template</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Stoor as &amp;prent...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Stoor as prent</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Fout tydens het skryf na uitvoerlêer(s).</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Fout tydens het skryf na uitvoerlêer(s).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Uitvoer was suksesvol.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Lêer bestaan. Oorskryf?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>bestaat alreeds. Oorskryf?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nee</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Ja, alles</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Drukvoorbeeld</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="unfinished">Druk&amp;voorbeeld</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Voer &amp;EPS/PS in...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alle ondersteunde formate (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Stoor as &amp;templaat...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Scripter &amp;handleiding...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">&amp;Scribus scripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">Voer Script &amp;uit...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">&amp;Onlangse scripts</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Wys &amp;Konsole</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;cript</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Aanlyn naslaan</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python scripts (*.py);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Stoor bladsy as &amp;SVG...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Stoor as</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-prente (*.svg *.svgz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-prente (*.svg);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Voer &amp;SVG in...</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Lêers met kommageskeide-waardes (CSV)</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_gegewens</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_kopreel</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML-lêers</translation>
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
Eksterne skakelings
</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Tekslêers</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Lettertype %1 is beskadig en word weggelaat</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Teksfilters</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albaanies</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskies</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgaars</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Braziliaans</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalaans</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Sjinees</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tsjeggies</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Deens</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Nederlands</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Engels</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Engels (Brits)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Duits</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Fins</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Frans</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galies</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grieks</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hongaars</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonisies</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiaans</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreaans</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litouws</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Noors (Bokmal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Noors (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Noors</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Pools</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisch</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Zweeds</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spaans</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Spaans (Latyns)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovaaks</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Sloveens</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Servies</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Media gevalle</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Het vordering probeer stel &gt; maksimum vordering</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished">&amp;Oor Script...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished">Oor Script</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie lettergrootte vir nie-teksraam kry nie</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie lettertipe vir nie-teksraam kry nie</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie teksgrootte vir nie-kry nie</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie kolomtelling vir nie-teksraam kry nie</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie lynspasiëring vir nie-teksraam kry nie</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie kolomgaping vir nie-teksraam kry nie</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie teks vir nie-teksraam kry nie</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie teks vir nie-teksraam stel nie</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan nie teks in nie-teksraam invoeg nie</translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="obsolete">Ingevoeg indeks buite grense</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Skoueropsie bestaan nie, gebruik scribus konstantes</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie teks skouer op nie-teksraam nie</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="obsolete">Lettergrootte buite grense - moet tussen 1 en 512 wees</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie lettergrootte op nie-teksraam stel nie</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie lettertipe op &apos;n nie-teksraam stel nie</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Lettertipe nie gevind nie</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="obsolete">Lynspasiëring buite grense, moet &gt;=0,1 wees</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan e lynspasiëring op nie-teksraam doen nie</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="obsolete">Kolomgaping is buite grense, moet positief wees</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie kolomgaping vir &apos;n nie-teksraam toeken nie</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Aantal kolomme moet &gt;=1 wees</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie aantal kolomme stel in &apos;n nie-teksraam nie</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Keuse indeks buite grense</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie teks kies in &apos;n nie-teksraam nie</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie teks verwyder uit &apos;n nie-teksraam nie</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie teksfull stel in &apos;n nie-teksraam nie</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie teksstrepe stel in &apos;n nie-teksraam nie</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie teksskadu stel in &apos;n nie-teksraam nie</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan net teksrame aanmekaarskakel</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Teikenraam moet leeg wees</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Teikenraam skakel na &apos;n ander raam</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Daar word na teikenraam geskakel deur ander raam</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Bron en teiken is dieselfde objek</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie &apos;n nie-teksraam ontskakel nie</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished">Objek is nie &apos;n geskakelde teksraam nie, kan nie ontskakel nie</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished">Laaste skakel in reeks, ontskakel vorige raam.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie &apos;n nie-teksraam omskakel in buitelyne nie</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="obsolete">Kon nie dokument oopmaak nie</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="obsolete">Kon nie dokument stoor nie</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Eenheid buit bereik. Gebruik scribus.UNIT_* konstantes.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Teiken is nie &apos;n prentraam nie. </translation>
    </message>
    <message>
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie skaleer na 0% nie</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie leë voorbeelde teken nie</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie na leë lêer stoor nie</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie &apos;n leë laag &apos;n naam gee nie</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Laag nie gevind nie</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie laaste laag verwyder nie</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan nie &apos;n laag sonder &apos;n naam skep nie</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="obsolete">&apos;n Objek met die naam bestaan reeds</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Puntlys moet ten minste twee punte bevat (vier waardes)n</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Objek nie gevind nie</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Styl nie gevind nie</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="obsolete">Kon nie EPS stoor nie</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="obsolete">Bladsynommer buite bereik</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Lynstyl nie gekry nie</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan nie &apos;n kleur met &apos;n leë naam kry nie.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Kleur nie gekry nie</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan nie &apos;n kleur met &apos;n leë naam verander nie.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="obsolete">Kleur nie in dokument gevind nie</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="obsolete">Kleur nie in verstekkleure gevind nie</translation>
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
        <translation>Voer &amp;OpenOffice.org Draw in...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer dokumente</translation>
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
        <translation type="unfinished">Afrikaans</translation>
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
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Bladsy</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished">p</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="unfinished">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">p</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="unfinished">Punte (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File exists</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">&amp;Vervangen</translation>
    </message>
    <message>
        <source>All</source>
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
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
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
        <translation type="unfinished"></translation>
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
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="unfinished">Portugees</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="unfinished">Scribus is gecrashed</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished">Scribus is gecrashed met signaal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Reg</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4A0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2A0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comm10E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DLE</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Invalid argument: </source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output version information and exit</source>
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
        <source>Scribus Version </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> cm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> c</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>cm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>c</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="unfinished">&amp;Lettertypeweergave...</translation>
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
        <source>Short Words Manual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Leegmaak</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Kies alles</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Ongedaanmaak</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gedaanmaak</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kni&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiër</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>P&amp;lak</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Stelselmenu</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Skadu</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Ontskadu</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normaliseer</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimaliseer</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maksimaliseer</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Maak toe</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Herstel</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Skuif</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Grootte</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimaliseer</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ksimaliseer</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Maak toe</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Altyd in &amp;voorgrond</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimaliseer</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Herstel onder</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Maak toe</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Skadu</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Ontskadu</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Dokument instellings</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Kantlyn hulplyne</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Bo:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>On&amp;der:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Regs:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Teenoorstaande bladsye</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Linker&amp;bladsy eerste</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Bladsye is eenders of linker/regter</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Laat die dokument begin met &apos;n linker bladsy</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Afstand tussen de bokant van die kantlyn en die papierrand</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Afstand tussen de onderkant van die kantlyn en die papierrand</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Afstand tussen die linkerkant van de kantlyn en die papierrand.
As Teenoorstaande bladsye gekies is, word die breedte gebruik aan die rugkant van die dokument</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Afstand tussen die regterkant van de kantlyn en die papierrand.
As Teenoorstaande bladsye gekies is, word die breedte gebruik aan die buitekant van die dokument</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">B&amp;innenkant:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">B&amp;uitekant:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Bladsygrootte</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Grootte:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Tuisgemaak</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Orientasie:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Regop</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Gekantel</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Breedte:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Hoogte:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Eerste bladsy n&amp;ommer:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Grootte:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter (US)</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orië&amp;ntasie:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Breedte:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Hoogte:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Outomaties stoor</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Interval:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Hulplyne</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Kleur:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Vertoon &amp;Nie-drukbare gebiede in kantlynkleur</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display</source>
        <translation type="unfinished">Vertoon</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Typografie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the of linked frames on or off</source>
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
        <translation type="unfinished">Papierkleur</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Maskeer gebied buite die kantlyne in die kantlynkleur</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply margin settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Dokumentinformasie</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
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
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Bezig kleuren aan te passen</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished">Python scripts (*.py);;Alle lêers (*)</translation>
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
        <source>Style Settings</source>
        <translation>Stylinstelling</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Styl van huidige paragraaf</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Vulkleur-instelling</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Kleur van die teks agtergrond</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Versadiging van kleur van teks agtergrond</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Lynkleur-instelling</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Kleur van die tekslyne</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Versadiging van kleur van tekslyne</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Lettertipe-instelling</translation>
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
        <translation>Lettertipe van gekose teks</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Lettergrootte</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skaalbreedte van letters</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Letter-instelling</translation>
    </message>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Letterspasiëring:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Handmatige letterspasiëring</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScImportExportPlugin</name>
    <message>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie #%1 van</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Agtergrond</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>Normal</source>
        <translation>Normaal</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Lêer</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Skep nuwe dokument</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Maak dokument oop</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Stoor huidige dokument</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Maak huidige dokument toe</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Druk huidige dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Stoor huidige dokument as PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Besig om na lettertipes te soek</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Daar is geen Postscript-lettertipes op u stelsel nie</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Besig om toe te maak</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatale fout</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Intelligent afkap</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Links skouer</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Regs skouer</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Sentreer</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Voeg bladsynommer in</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Koppel teks aan &apos;n pad</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Wys Lae</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Javascripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Ongedaanmaak</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Wys bladsypallet</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Sluit/Onsluit</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Harde spasie</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Voorkeure word gelees</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">ICC-profiele word gekry</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Afkapper word aangesit</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Kortpaaie word gelaai</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">&amp;Kleurbeheer...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Kladblok word gelees</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Plugins word geinisialiseer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nuwe</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Maak oop...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>&amp;Onlangs oop gemaak</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nuwe</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Maak oop...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Maak toe</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Maak toe</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Stoor</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Stoor</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Stoor &amp;as...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Stoor as...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Gaa&amp;n terug na gestoorde</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Versamel vir &amp;uitvoer...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Kry Teks/prent...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Voeg teks &amp;toe...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Voer &amp;Bladsy(e) in...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Voer in</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Stoor &amp;teks...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Bladsy stoor als &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Stoor as P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exporteren</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">Document&amp;informatie...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Documentinformatie...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">Documentin&amp;stellingen...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Documentinstellingen...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">Afdru&amp;kken...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Afdrukken...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">A&amp;fsluiten</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Afsluiten</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Ongedaan maken</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Kni&amp;ppen</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiëren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">P&amp;lakken</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">W&amp;issen</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">&amp;Alles selecteren</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Knippen</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopiëren</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Plakken</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Wissen</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Alles selecteren</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Zoeken/vervangen...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">Kle&amp;uren...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Kleuren...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">&amp;Alineastylen...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">&amp;Lynstylen...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Stylen...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Sjablonen...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Sjablonen...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Nuwe lettertype selecteren</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;upliceren</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Dupliceren</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">&amp;Meervoudig dupliceren</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Meervoudig dupliceren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">Verwy&amp;deren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Verwyderen</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Groeperen</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Groeperen</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Groep losmaken</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Groep losmaken</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">Vergrende&amp;len</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Naar &amp;achtergrond</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Naar achtergrond</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Naar &amp;voorgrond</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Naar voorgrond</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Lager</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Lager</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Hoger</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Hoger</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Verdelen/&amp;uitlynen...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Verdelen/uitlynen...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">Vorm b&amp;ewerken</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Vorm</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">Teks &amp;aan pad toevoegen</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">Teks van &amp;pad losmaken</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">Polygonen &amp;samenvoegen</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">&amp;Polygonen opsplitsen</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">Omzetten na &amp;outlines</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Invoegen...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Invoegen...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">Verwy&amp;deren...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Verwyderen...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Verplaatsen...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Verplaatsen...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">Sj&amp;abloon toepassen...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Sjabloon toepassen...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">&amp;Hulplynen beheren...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Hulplynen beheren...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">Aanpassen aan &amp;venstergrootte</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Aanpassen aan venstergrootte</translation>
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
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="obsolete">100%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation type="obsolete">200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="obsolete">Minia&amp;turen</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Miniaturen</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">&amp;Kantlyne tonen</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Kantlyne verbergen</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">&amp;Frames tonen</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Frames verbergen</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Afbeeld&amp;ingen tonen</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Prente verbergen</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">R&amp;aster tonen</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Raster tonen</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">H&amp;ulplynen tonen</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">&amp;Basislyn-raster tonen</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">Ma&amp;gnetisch raster</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Magnetisch raster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Magnetische hul&amp;plynen</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Eienskappe</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Eienskappe</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Outline</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Outline</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;Kladblok</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Kladblok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Lagen</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">P&amp;aginapalet</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Bladwyzers</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Prente &amp;beheren</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Prente beheren</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">Teks a&amp;fbreken</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Teks afbreken</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Hul&amp;pballonnen</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Gereedschappen</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">P&amp;DF-gereedschappen</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Gereedschappen</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Hulpballonnen</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">&amp;Voorkeuren...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Lettertypen...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Lettertypen...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">&amp;Afbreekinstellingen...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">&amp;Sneltoetsen...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Info over Scribus</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Info over Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Info over &amp;Qt</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Info over Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">Scribus &amp;handboek...</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Online hulp...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Lêer</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Be&amp;werken</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>Sti&amp;jl</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Item</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Bladsy</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>Beel&amp;d</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">In&amp;stellingen</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Venster</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Help</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Links</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Midden</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">&amp;Regs</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Blok</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">Ge&amp;forceerd</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Overig...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Onderstreept</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Doorhalen</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Kleinkapitalen</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Superscript</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Subscript</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Outline</translation>
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
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Gereed</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Trapsgewys</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Tegels</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Teks/prent verkrygen...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Afbeelding verkrygen...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Kleur</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Inverteren</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">Teks verkry&amp;gen...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Lettertype</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Grootte</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effecten</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Uitlyning</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Tint</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Tabulators...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Teks verkrygen...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Lettertype</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Grootte</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Styl</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Kleur</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tint</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Losmaken</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Bladsye worden geïmporteerd...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Bladsy(e) importeren</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;U probeert meer bladsye te importeren dan beschikbaar in het huidige dokument (geteld vanaf de actieve bladsy).&lt;/p&gt;Kies een van de volgende mogelykheden:&lt;br&gt;&lt;ul&gt;&lt;li&gt;Ontbrekende bladsye &lt;b&gt;aanmaken&lt;/b&gt;&lt;/li&gt;&lt;li&gt;Bladsye &lt;b&gt;importeren&lt;/b&gt; tot aan laatste bladsy&lt;/li&gt;&lt;li&gt;&lt;b&gt;Kanselleer&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Aanmaken</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importeren tot laatste</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importeren voltooid</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Niets gevonden om te importeren</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarschuwing</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Het lêer %1 is niet in Scribus-formaat</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Laden...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle ondersteunde formaten</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle lêers (*)</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">Kan niet na lêer schryven: 
%1</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Stoor as</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Stoor...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Bezig met afdrukken...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Document</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Afdrukken mislukt!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus handboek</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekslêers (*.txt);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Grootte:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Tint:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Basislyn-raster verbergen</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Basislyn-raster tonen</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">De volgende programma&apos;s ontbreken:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript: U kunt niet met EPS-prente werken</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS-lêers (*.eps *.epsi);;Alle lêers (*)</translation>
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
        <translation>Enkele objecten zyn vergrendeld.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Kanselleer</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Alle vergrendelen</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Alle losmaken</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Losmaken</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Vergrendelen</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Laden:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Bezig kleuren aan te passen</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Object verwyderen ongedaan maken</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Object verplaatsen ongedaan maken</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Object wyzigen ongedaan maken</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Duits</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Pools</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Engels</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spaans</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiaans</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Frans</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisch</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Deens</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovaaks</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hongaars</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tsjechisch</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Nederlands</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugees</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukraïns</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grieks</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalaans</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Fins</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Iers</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litouws</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Zweeds</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Sloveens</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Kies een map</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Inverteren</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Scribus is gecrashed</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus is gecrashed met signaal #%1</translation>
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
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="obsolete">&amp;Onderstreep</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="obsolete">&amp;Bewerk Vorm...</translation>
    </message>
    <message>
        <source>&amp;50%</source>
        <translation type="obsolete">&amp;50%</translation>
    </message>
    <message>
        <source>&amp;75%</source>
        <translation type="obsolete">&amp;75%</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation type="obsolete">&amp;200%</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">Voeg spesiale ding &amp;In</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>File %1 
is not in an acceptable format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="unfinished">Bulgaars</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Die program</translation>
    </message>
    <message>
        <source>is already running!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Info</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="unfinished">ontbreek op u masjien!</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">Vlak</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Stuur na &amp;laag</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;PDF-opsies</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="unfinished">Karakter</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Stoor as</translation>
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
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The program</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation type="unfinished">Nuwe laag</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normaal</translation>
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
        <translation>Laag</translation>
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
        <translation type="obsolete">Alles</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="unfinished">Kopiër hierheen</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="unfinished">Skuif hierheen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Kanselleer</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="unfinished">Prent</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="unfinished">Lêer: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">Oorspronklike PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">Werklike PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="unfinished">Geskakelde teks</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="unfinished">Teksraam</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Teks op &apos;n pad</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="unfinished">Druk: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="unfinished">Aan</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Af</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished">In&amp;lig</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">P&amp;rent sigbaar</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">Opdatee&amp;r prent</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">Bewerk pr&amp;ent</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">Pas raamgrootte by &amp;prent aan</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">Bewerk T&amp;eks...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Dit is &apos;n PDF-&amp;boekmerk</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">Dit is &apos;n PDF-a&amp;nnotasie</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Eienskappe van annotasie</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">&amp;Veldeienskappe</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;PDF-opsies</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Bewerk Teks...</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Sluit Objek&amp;grootte</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Ontsluit Objek&amp;grootte</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Stuur na &amp;kladblok</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Stuur na &amp;laag</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">Voeg voorbeeldteks &amp;in</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Ont&amp;groepeer</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished">&amp;Vlak</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Prent&amp;raam</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Pol&amp;ygoon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Buitelyne</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Teksraam</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezierkurwe</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished">Skakel om n&amp;a</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Maak Inhoud &amp;leeg</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Geen</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopie van</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>There is a problem with text frames linking. You are trying to link filled frames or a frame to the same one itself</source>
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
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>Toch a&amp;fsluiten</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Toch s&amp;luiten</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarschuwing</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Het dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>is gewyzigd na de laatste keer stoor.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>Nu op&amp;slaan</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="obsolete">&amp;Scribus scripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">Voer Script &amp;uit...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">&amp;Onlangse scripts</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Wys &amp;Konsole</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">&amp;Oor Script...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;cript</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Maak oop</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python scripts (*.py);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation type="unfinished">Scriptfout</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="unfinished">As dit &apos;n amptelike script is, gee hierdie fout asb. in by &lt;a href=&quot;http://bugs.scribus.net&quot;&gt; bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="unfinished">Die is in die knip-en-plakbord. Met Ctrl+V kan u die fout in bugtracker gaan plak.</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Versteek &amp;Konsole</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">Oor Script</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Atl+O</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Gevorderde opsies</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Bladsyvoorbeelden tonen</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Sjabloonnamen tonen</translation>
    </message>
    <message>
        <source>Show Master Page Names</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Zoeken/vervangen</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Zoeken na:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Teks</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Alineastyl</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Lettertype</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Lettergrootte</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Lettertype-effecten</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Vulkleur</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Vultint</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Lynkleur</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Lyntint</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Midden</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Regs</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blok</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Geforceerd</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Geen</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Vervangen door:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Heel woord</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Hoofdletteron&amp;gevoelig</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Zoeken</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Vervangen</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>&amp;Alles vervangen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Sluiten</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Zoeken voltooid</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>W&amp;issen</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Bladsye herschikken</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Beschikbare sjablonen:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Documentbladsye:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Tegenover elkaar</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Linkerbladsy eerst</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Sleep bladsye of Sjabloonbladsye na de prullenbak om ze te verwyderen.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Toont voorbeelden van alle bladsye in uw dokument.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Hier zyn al uw sjablonen. Sleep, om een nuwe bladsy
aan te maken, een sjabloon na de bladsyweergave hieronder.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaal</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previews all the pages of your document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Velden selecteren</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Beschikbare velden</translation>
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
        <translation>Geselecteerde velden</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Overig...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Tint:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tint</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
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
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Geen styl</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Geen styl</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Stylen bewerken</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">Toe&amp;voegen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuwe</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Be&amp;werken</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;upliceren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Verwy&amp;deren</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">Op&amp;slaan</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie van %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuwee styl</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarschuwing</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nee</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Wilt u deze styl werkelyk verwyderen?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Voer in</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Story-editor</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuwe</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>Teks uit raam &amp;herladen</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>Op&amp;slaan na lêer...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Laden uit lêer...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>&amp;Document stoor</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>Teksraam bywerken en sl&amp;uiten</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Sluit&amp;en zonder teksraam by te werken</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;Alles selecteren</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kni&amp;ppen</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiëren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>P&amp;lakken</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>W&amp;issen</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Zoeken/vervangen...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Invoegen speciaal...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Stylen b&amp;ewerken...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Lettertypeweergave...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>Teksraam by&amp;werken</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Achtergrond...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>Weergave&amp;lettertype...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Lêer</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Be&amp;werken</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>In&amp;stellingen</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Lêer</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Alle teks wissen</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Teks laden uit lêer</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Teks stoor na lêer</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Teksraam bywerken en sluiten</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Sluiten zonder teksraam by te werken</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Teks uit raam herladen</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Teksraam bywerken</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Zoeken/vervangen</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Huidige alinea:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Woorden:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tekens:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Totalen:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Alinea&apos;s:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Waarschuwing</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Wilt u uw wyzigingen stoor?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Wilt u werkelyk al uw wyzigingen verwerpen?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Wilt u werkelyk alle teks wissen?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Maak oop</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekslêers (*.txt);;Alle lêers (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Stoor as</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Onderstreept</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kleinkapitalen</translation>
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
        <source>Strike Out</source>
        <translation>Doorhalen</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Outline teks</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="unfinished">Outline</translation>
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
        <source>Shadow</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Alineastylen bywerken</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Documentnaam als prefix vir alineastylen gebruiken</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Deze vraag niet meer stellen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>At the end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Toevoeg</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished">dpi</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">px</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="obsolete">Hulplyne</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation type="unfinished"></translation>
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
        <source>Color:</source>
        <translation type="unfinished">Kleur:</translation>
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
        <source>Baseline Grid</source>
        <translation type="obsolete">Basislyn-rooster</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished">Basislyn-raster tonen</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Outomatise &amp;lynspasiëring:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">Basislyn &amp;rooster:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">Basislyn &amp;beginafstand:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="unfinished">Afstand tussen kleinroosterlyne</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished">Afstand tussen grootroosterlyne</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished">Afstand waarvandaan &apos;n objek na jou geplaasde hulplyne sal spring</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished">Gee die straal aan van die gebied waar Scribus u toelaat om &apos;n object se muisvatsels te gryp</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished">Kleur van kleinroosterlyne</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="unfinished">Kleur van grootroosterlyne</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished">Kleur van u eiehulplyne</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="unfinished">Kleur vir die kantlyne</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
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
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Persentasie toename van lyn hoogte tov lettergrootte</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
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
        <source>Color for the baseline grid</source>
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
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Tabulators beheren</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Uitvoerbereik</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">&amp;Alle bladsye</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">Kies &amp;Bladsye</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotasie:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">Lêersopsies</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">&amp;Aanpasbaarheid:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished">&amp;Binding:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Linkerkantlyn</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Regterkantlyn</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Minia&amp;tuur prentjies</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Stoor geskake&amp;lde teksrame as PDF-artikels</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">&amp;Voeg Boekmerke in</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished">dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Resolusie:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="unfinished">Prentinstellings</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Outomaties</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation type="unfinished">JPEG</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation type="unfinished">Zip</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="unfinished">&amp;Metode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished">&amp;Kwaliteit:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Maksimum</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">Hoog</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Laag</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Minimum</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">&amp;Hermonster Prente af na:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">Al&amp;gemeen</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">Inbed alle l&amp;ettertipes</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished">&amp;Substel alle lettertipes</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished">Inbedding</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Beskikbare lettertipes:</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="unfinished">&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="unfinished">&amp;&lt;&lt;</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="unfinished">Lettertipes om in te bed:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished">Lettertipes om te substel:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Lettertipes</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Bladsy</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">&amp;Vertoon Bladsyvoorbeeld</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Effekte</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">Verton ty&amp;dsduur:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Effek &amp;tydsduur:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">Effekt&amp;ipe:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">&amp;Bewegende lyne:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">&amp;Van die:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">R&amp;igting:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished">sec</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">Geen effek</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished">Blinders</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="unfinished">Boks</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Oplos</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished">Glinster</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="unfinished">Deel</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished">Uitvee</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Horisontaal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Vertikaal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Binne</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Buite</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Links na regs</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Bo na onder</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Onder na bo</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Regs na links</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Linksbo na regsonder</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">Pas Effek op &amp;alle bladsye toe</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">Gebruik enkripsie</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Wagwoord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">Gebr&amp;uiker:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">&amp;Eienaar:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Instellings</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">Laat &amp;druk van dokument toe</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Laat Wy&amp;siging van dokument toe</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Laat &amp;Kopiëring van teks en prente toe</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Laat Toevoegen van &amp;annotasies en velde toe</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">V&amp;eiligheid</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Algemeen</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished">&amp;Uitvoer bestem vir:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="unfinished">Skerm / web (RGB)</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Drukker (CMYK)</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished">&amp;Gebruik tuisgemaakte rendering instellings</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished">Rendering instellings</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished">Fre&amp;kwensie:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished">&amp;Hoek:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">&amp;Punt-funksie:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished">Enkele stip</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Lyn</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished">Rond</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished">Ovaal</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="unfinished">Soliede kleure:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished">Gebruik ICC-profiel</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="unfinished">Profiel:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished">Rendering intent:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished">Perseptueel</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Versadiging</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="unfinished">Prente:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished">Moenie ingebedde ICC-profiele gebruik nie</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">&amp;Kleur</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">PDF/X-3 uitvoer doel</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished">&amp;Inligting teks:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished">Uitvoer&amp;profiel:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="unfinished">Reghoekknipboks</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="unfinished">In bed lettertipes in dIe PDF. Dit verseker dat dokument se voorkoms nie sal verander nie.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Skakel aanbieding effekte aan as Acrobat in volskerm modus is.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Wys bladsyvoorbeelde van bogenoemde bladsye.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished">Bladsy vertooning duurs voordat aanbieding effekte begin.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished">Tydsduur van effek.
Korter tyd sal effek versnel, &apos;n langer sal dit vertraag.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Tipe oorgangseffect.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">Rigting van effek van bewegende lyne vir die split en verblind effekte.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">Beginposisie vir die regthoek en split-effekte.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Rigting vir die glinster of vee-effekte.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished">Pas gekose effek op alle bladsye toe.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Voer alle bladsye na PDF uit</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Voer bereik van bladsye na PDF uit</translation>
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
        <translation type="obsolete">Bepaal PDF-anpasbaarheid. Verstek is &quot;Acrobat 4.0&quot;, dit bied die meeste aanpasbaarheid.
Gebruik Acrobat 5.0 as u funksies uit PDF 1.4 gebruik, soos deursigtigheid of 128-bis enkripsie.
PDF/X-3 is vir kommersiële drukkers en is kiesbaar as jy kleurbestuur aktiveer.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished">Stel die bind van bladsye in die PDF. Los op verstek (links), tensy u weet wat u doen.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished">Maak miniatuurbladsye van elke bladsy in die PDF.
So mmige blaaiers kan dit vir navigasie gebruik.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Maak PDF-artikels, sodat gemakliker in geskakelde pdfs genavigeer kan word.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="unfinished">Inbed bladwysers in dokument, sodat lange
dokumente maklik deurblaai kan word.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished">Uitvoer resolusie van teks en vektorgrafika.
Dit het geen invloed op prent resolusies (soos JPG&apos;s) nie.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Kompressie van text en grafika
Moenie sonder rede verander nie. Dit verklein PDF groottes.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Kompressie metode vir prente.
Outomaties laat Scribus die beste metode kies.
ZIP is goed vir beelde met soliede kleure.
JPEG is veral effektief met baie prente (waarvoor &apos;n geringe verlies moontlik is).
Los op outomaties, tensy u &apos;n rede vir &apos;n bepaalde kompressie het.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="unfinished">Kompressievlakke: Minimum (25%), laag (50%), medium (75%), hoog (85%), maksimum (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Monster bitmap prente af na gekose DPI.
As u hierdie los, word hulle in hulle huidige resolusies gestoor.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished">DPI (kolle per duim) vir prentjie uitvoer.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Schakelt beveiligingsfuncties in uw PDF in.
Als u Acrobat 4.0 selecteerde, zal de PDF beschermd zyn met 40-bit versleuteling.
Als u Acrobat 5.0 selecteerde, zal de PDF beschermd zyn met 128-bit versleuteling.
Let op: PDF-versleuteling is niet zo veilig als GPG of PGP-versleuteling en heeft enkele andere beperkingen.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="unfinished">Kies een master-wachtwoord om alle beveiligingsinstellingen
in of uit te schakelen in uw PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Kies een wachtwoord vir gebruikers om de PDF te kunnen lezen.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Het afdrukken van de PDF toestaan. Indien niet geselecteerd is afdrukken onmogelyk.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Wyzigen van de PDF toestaan. Indien niet geselecteerd is wyzigen onmogelyk.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="unfinished">Het kopiëren van teks of prente uit de PDF toestaan.
Indien niet geselecteerd is dit onmogelyk.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="unfinished">Het toevoegen van annotaties en velden aan de PDF toestaan.
Indien niet geselecteerd, is dit onmogelyk.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished">Kleurmodel vir uitvoer van u PDF.
Kies Skerm/web (RGB) vir PDF&apos;s wat op skerms vertoon of tipiese Inkjet drukkers gedruk word.
Kies Printer (CMYK) as u na &apos;n egte vierkleur (CMYK) drukker druk.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">Hierdie is &apos;n gevorderde stelling wat net gebruik moet word as jou drukker vir jou sê om dit te stel. Raadpleeg die poslys vir u hiermee peuter.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished">Inbed &apos;n kleurprofiel vir soliede kleure</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished">Kleurprofiel vir soliede kleure</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished">Rendering doel vir soliede kleure</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="unfinished">Inbed van &apos;n kleurprofiel vir prente</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished">Kleurprofiele in prentjies moenie gebruik word nie</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished">Kleurprofiel vir prente</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">Rendering doel vir prente</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">Uitvoerprofiel vir drukwerk. Indien moontlik, probeer leiding by u drukker te kry.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Verpligte string vir PDF/X-3, anders voldoen PDF nie aan PDF/X-3 standaard nie. Ons beveel die dokument titel aan.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Afloop-afstand vanaf die bokant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Afloop-afstand vanaf die onderkant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Afloop-afstand vanaf die linkerkant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Afloop-afstand vanaf die regterkant van die fisiese bladsy</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished">Spieëlbeeld bladsy(e) horisontaal</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished">Spieëlbeeld bladsy(e) vertikaal</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Lettertype:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Grootte:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
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
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">Kolo&amp;mme:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Filmquiz bracht knappe ex-yogi van de wys</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished">&amp;Lynkleur:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Skadu:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished">&amp;Vulkleur:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">S&amp;kadu:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">Lyndi&amp;kte:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished">&amp;Horisontale skaal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished">&amp;Vertikale skaal:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished">&amp;Skaleer prent na raamgrootte</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished">Behou Prentve&amp;rhouding</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="unfinished">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished">Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished">&amp;Stappe van:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="unfinished">Eienskappe van teksraam</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="unfinished">Eienskappe van prentraam</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished">Eienskappe van vormsteken</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished">Verstek vergroottingsvlakke</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="unfinished">Lyneienskappe</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished">Polygooneienskappe</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="unfinished">Lettertipe vir nuwe teksrame</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="unfinished">Lettergrootte vir nuwe teksrame</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="unfinished">Letterkleur</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">Aantal kolomme in &apos;n teksraam</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">Tussenruimte tussen kolomme</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="unfinished">Voorbeeld van u lettertipe</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished">Prentrame laat prente na enige grootte skaleer</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished">Horisontale skaal vir prente</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="unfinished">Vertikale skaal vir prente</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished">Hou horisontale en vertikaleskaal gelyk</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished">Prente in prentrame word altyd na grootte van hul raam geskaleer</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished">Outomaties geskaalde prente houd hul oorspronklike prentverhouding</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="unfinished">Vulkleur vir prentrame</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="unfinished">Versadiging van die vulkleur</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="unfinished">Lynkleur van vorms</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="unfinished">Versadiging van die lynkleur</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="unfinished">Vulkleur van vorms</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="unfinished">Lynstyl van vorms</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="unfinished">Lyndikte van vorms</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished">Minimum toegelate vergroting</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished">Maksimum toegelate vergroting</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished">Verandering in grootte vir elke zoom-stap</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="unfinished">Lynkleur</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished">Kleurversadiging</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="unfinished">Lynstyl</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="unfinished">Dikte van lyne</translation>
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
        <source>Custom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished">Ver&amp;plasing:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished">&amp;Skaalering:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished">Verpla&amp;sing:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished">S&amp;kaalering:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished">Sk&amp;aalering:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished">Verplasing bo basislyn van &apos;n lettertipe op &apos;n lyn</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">Relatiewe grootte van boskrif tov normale teks</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">Verplasing onder basislyn van &apos;n lettertipe op &apos;n lyn</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">Relatiewe grootte van onderskrif tov normale teks</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">Relatiewe grootte van kleinhoofletters tov normale teks</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation type="unfinished">Lyndikte:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="unfinished">Doorhalen</translation>
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
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Persentasie toename van lyn hoogte tov lettergrootte</translation>
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
        <translation>Links</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Regs</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Volledige stop</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centreren</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Positie:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">Eerste &amp;regel:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Alles verwyderen</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Inspringen vir de eerste regel van de alinea</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Inspringen vanaf links vir de hele alinea</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Alle tabstops verwyderen</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Geen</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Outline</translation>
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
        <translation type="obsolete">Info</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Waarschuwing</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Die naam &quot;%1&quot; is nie uniek nie.
Kies &apos;n ander naam.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
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
        <translation type="obsolete">Lettertype:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Groep</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Afbeelding</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Teks</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Lyn</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Polygoon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Polylyn</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Tekspad</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Bladsy</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Bladsy</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="unfinished">Groeperen</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished">Aanmaken</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Kanselleer</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Vergrendelen</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="unfinished">Losmaken</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished">Herbenoem</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="unfinished">Plakken</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="unfinished">Knippen</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Polygoon</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Polylyn</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation type="unfinished"></translation>
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
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Get image</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Waarde invoegen</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Voer een waarde in en klik op Reg.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Voer een waarde in en klik op Reg</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Atl+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Geef uw waarde door aan het script</translation>
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
        <translation type="obsolete">&amp;Reg</translation>
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
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished">&amp;Help</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter name of the plain text editor executable:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Gereedschappen</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Items selecteren</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Teksraam invoegen</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Afbeelding invoegen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Tabel invoegen</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Eienskappe...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Polygoon invoegen</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Lynen invoegen</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Beziercurven invoegen</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Vryehand-curve invoegen</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Item roteren</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">In- of uitzoomen</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Inhoud van raam bewerken</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Teks met de story-editor bewerken</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Teksrame koppelen</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Teksrame loskoppelen</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Metingen uitvoeren</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Verschillende vormen tekenen</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>PDF-gereedschappen</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knop</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Teksveld</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Checkbox</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Combobox</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listbox</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>PDF-velden invoegen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Teks</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Koppeling</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>PDF-annotaties invoegen</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Karakter selecteren:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Invoegen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">W&amp;issen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Sluiten</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Voegt de tekens in op de huidige cursorpositie</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Verwydert de huidige selectie(s).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Sluit deze dialoog om verder te werken aan de teks.</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Selecteer de te gebruiken importer</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Outomaties</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Alleen de teks ophalen</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Teks importeren zonder enige opmaak</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importer:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codering:</translation>
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
        <translation>Selecteer de te gebruiken importer</translation>
    </message>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Deze associatie onhouden</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Onthoudt de lêersnaamextensie-importer associatie.
De volgende keer wordt by het importeren van een lêer
van dit type dezelfde importer gebruikt.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Reg</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nuwe van sjabloon</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Ve&amp;rwyderen</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Maak oop</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alles</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Naam</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Bladsygrootte</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Kleuren</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Omschryving</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Gebruik</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Aangemaakt met</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Auteur</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Sjablonen downloaden</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>U kunt dokumentsjablonen vinden op http://www.scribus.net/ in de Downloads-sectie.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Sjablonen installeren</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Pak het pakketje uit in de sjablonenmap van Scibus: ~/.scribus/templates vir de huidige gebruiker of PREFIX/share/scribus/templates vir alle gebruikers van het systeem.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Sjabloon prepareren</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Vergewis u ervan dat prente en lettertypen vry gebruikt kunnen worden. Als lettertypen niet kunnen worden gedeeld, verzamel deze dan niet als u een dokument opslaat als een sjabloon.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Wie een sjabloon maakt dient ook zeker te zyn dat het installeren van de sjabllon goed zal werken, zodat een gebruiker deze eenvoudig kan uitpakken en beginnen te gebruiken.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Sjablonen verwyderen</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Als u een sjabloon verwydert wordt enkel de ingang in het lêer template.xml verwyderd, niet de dokumentlêers zelf. De optie verwyderen wordt enkel weergegeven als u schryftoegang hebt tot het template-xml-lêer.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Het template.xml-lêer vertalen</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Dit kan door een bestaand template.xml te kopiëren na een lêer met de naam template.lang_LANDCODE.xml (gebruik dezelfde landcode als het .ts-lêer vir uw taal), byvoorbeeld template.nl.xml vir de Nederlandse vertaling van sjabloonnamen. De kopie dient zich in dezelfde map te bevingen als het oorspronkelyke lêer, zodat Scribus het kan vinden.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Stoor as sjabloon</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Naam</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categorie</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Bladsygrootte</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Kleuren</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Omschryving</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Gebruik</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Auteur</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-mail</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Meer informatie</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Reg</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Minder informatie</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Letter (US)</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>liggend</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>staand</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>aangepast</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Filter aanmaken</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>W&amp;issen</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>Verwy&amp;deren</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Kies een eerder bewaard filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Geef dit filter een naam om op te slaan</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Voer een naam in</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Reg</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Kanselleer</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Deze filterregel in- of uitschakelen</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Deze filterregel verwyderen</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Een nuwe regel toevoegen</translation>
    </message>
    <message>
        <source>to</source>
        <translation>aan</translation>
    </message>
    <message>
        <source>and</source>
        <translation>en</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>verwyder overeenkomst</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>verwyder overeenkomst niet</translation>
    </message>
    <message>
        <source>words</source>
        <translation>woorden</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Verwyderen</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Vervangen</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Toepassen</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>De waarde links is een reguliere expressie</translation>
    </message>
    <message>
        <source>with</source>
        <translation>met</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>alinastyl</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>alle instanties van</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>alle alinea&apos;s</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>alinea&apos;s die beginnen met</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>alinea&apos;s met minder dan</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>alinea&apos;s met meer dan</translation>
    </message>
</context>
</TS>
