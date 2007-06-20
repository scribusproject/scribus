<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS><TS version="1.1" language="en_US">
<defaultcodec></defaultcodec>
<context>
    <name></name>
    <message>
        <location filename="" line="7471221"/>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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

numPage = Number of pages to be created.

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3, 1)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>@default</name>
    <message>
        <location filename="" line="7471221"/>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getVGuides()

See getHGuides.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setVGuides()

See setHGuides.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Build-ID:</source>
        <translation type="obsolete">Build-ID:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;About</source>
        <translation>&amp;Névjegy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Contributions from:</source>
        <translation>Közreműködők:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A&amp;uthors</source>
        <translation>Szerzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Translations</source>
        <translation>&amp;Fordítók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Online</source>
        <translation>&amp;Online</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Development Team:</source>
        <translation>Feljesztő csapat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Official Documentation:</source>
        <translation>Hivatalos dokumentáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Other Documentation:</source>
        <translation>További dokumentáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Homepage</source>
        <translation>Honlap</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Online Reference</source>
        <translation>Online referencia</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bugs and Feature Requests</source>
        <translation>Hibabejelentés és új funkció kérés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mailing List</source>
        <translation>Levelező lista</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Official Translations and Translators:</source>
        <translation>Hivatalos fordítások és fordítók:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Previous Translation Contributors:</source>
        <translation>Előző fordítás kiadások:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>About Scribus %1</source>
        <translation>Scribus %1 névjegy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 %2 %3 </source>
        <translation type="obsolete">%1 %2 %3 </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Using GhostScript version %1</source>
        <translation type="obsolete">GhostScript %1 verzió használata</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No GS version available</source>
        <translation type="obsolete">Nincs elérhető GS verzió</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="obsolete">Scribus verzió %1 
%2 %3 (%4)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mac OSX Aqua Port:</source>
        <translation type="obsolete">Mac OSX Aqua Port:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Windows Port:</source>
        <translation type="obsolete">Windows Port:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%3-%2-%1 %4 %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Using Ghostscript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Ghostscript version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Build ID:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Windows&amp;#174; Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tango Project Icons:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation>Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No</source>
        <translation>Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Filename:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Version:</source>
        <translation type="unfinished">Verzió:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enabled:</source>
        <translation type="unfinished">Engedélyezve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Release Date:</source>
        <translation type="unfinished">Kiadás dátuma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Description:</source>
        <translation type="unfinished">Leírás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Author(s):</source>
        <translation type="unfinished">Szerző(k):</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copyright:</source>
        <translation type="unfinished">Copyright:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>License:</source>
        <translation type="unfinished">Licensz:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: Bedolgozó modulok névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File Name:</source>
        <translation type="obsolete">Fájlnév:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Version:</source>
        <translation type="obsolete">Verzió:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enabled:</source>
        <translation type="obsolete">Engedélyezve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Release Date:</source>
        <translation type="obsolete">Kiadás dátuma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copyright:</source>
        <translation type="obsolete">Copyright:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Author(s):</source>
        <translation type="obsolete">Szerző(k):</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Description:</source>
        <translation type="obsolete">Leírás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>License:</source>
        <translation type="obsolete">Licensz:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Open...</source>
        <translation>&amp;Megnyitás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation>&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;As...</source>
        <translation>Ment másként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Get Text...</source>
        <translation>Szöveg betöltése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Append &amp;Text...</source>
        <translation>Szöveg hozzáfűzése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Get Image...</source>
        <translation>Kép betöltése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;Text...</source>
        <translation>Szöveg mentése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save Page as &amp;EPS...</source>
        <translation>Oldal mentése EPS-ként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as P&amp;DF...</source>
        <translation>Mentés PDF-ként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document &amp;Setup...</source>
        <translation>Dokumentum beállítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Print...</source>
        <translation>Nyomtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Quit</source>
        <translation>&amp;Kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Undo</source>
        <translation>&amp;Visszavonás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Redo</source>
        <translation>&amp;Újra végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Elem akció mód</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cu&amp;t</source>
        <translation>&amp;Kivágás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">&amp;Tartalmak törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select &amp;All</source>
        <translation>&amp;Minden kijelölése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Deselect All</source>
        <translation>Minden kiválasztásának törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Keresés/csere...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Image...</source>
        <translation>Kép szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;olors...</source>
        <translation>&amp;Színek...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Bekezdés stílusa...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Vonal stílusa...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Mester lapok...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>P&amp;references...</source>
        <translation>&amp;Beállítások...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Other...</source>
        <translation>&amp;Egyéb...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Left</source>
        <translation>&amp;Bal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Center</source>
        <translation>&amp;Középső</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Right</source>
        <translation>&amp;Jobb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Block</source>
        <translation>&amp;Klisé</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Forced</source>
        <translation>&amp;Kikényszerített</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Normal</source>
        <translation>&amp;Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Underline</source>
        <translation>&amp;Aláhúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline &amp;Words</source>
        <translation>&amp;Szavak aláhúzása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Strike Through</source>
        <translation>&amp;Áthúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;All Caps</source>
        <translation>&amp;Minden nagybetűs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kiskapitális</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Su&amp;perscript</source>
        <translation>&amp;Felső index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Su&amp;bscript</source>
        <translation>&amp;Alsó index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Körvonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;hadow</source>
        <translation>&amp;Árnyék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Image Effects</source>
        <translation>&amp;Kép effektek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulátorok...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Kettőzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Többszörös duplázás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Group</source>
        <translation>&amp;Csoport</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Ungroup</source>
        <translation>&amp;Csoportosítás megszüntetése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is &amp;Locked</source>
        <translation>rögzített</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Si&amp;ze is Locked</source>
        <translation>&amp;Méret rögzített</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lower to &amp;Bottom</source>
        <translation>&amp;Legalulra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Raise to &amp;Top</source>
        <translation>&amp;Legfelülre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Lower</source>
        <translation>&amp;Hátrébb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Raise</source>
        <translation>&amp;Előrébb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to S&amp;crapbook</source>
        <translation>Küldés gyűjtőalbumba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Attributes...</source>
        <translation>&amp;Jellemzők...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>I&amp;mage Visible</source>
        <translation>&amp;Kép látható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Update Image</source>
        <translation>Kép &amp;frissítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Adjust Frame to Image</source>
        <translation>Keret képhez igazítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Extended Image Properties</source>
        <translation>Bővített kép tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Alacsony felbontás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Normál felbontás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Teljes felbontás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is PDF &amp;Bookmark</source>
        <translation>ez PDF könyvjelző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is PDF A&amp;nnotation</source>
        <translation>ez PDF kommentár</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Kommentár tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Mező tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Alak szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Szöveg hozzáadása az útvonalhoz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Detach Text from Path</source>
        <translation>Szö&amp;veg eltávolítása az útvonalból</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Sokszögek összekapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Split &amp;Polygons</source>
        <translation>So&amp;kszögek felosztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier görbe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Image Frame</source>
        <translation>&amp;Kép keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Körvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Polygon</source>
        <translation>&amp;Sokszög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Text Frame</source>
        <translation>&amp;Szöveg keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample Text</source>
        <translation>Minta szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Insert...</source>
        <translation>&amp;Beillesztés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Im&amp;port...</source>
        <translation>&amp;Importálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete...</source>
        <translation>&amp;Törlés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy...</source>
        <translation>&amp;Másolás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Move...</source>
        <translation>&amp;Mozgatás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Mester oldal alkalmazása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Segédvonalak kezelése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Manage Page Properties...</source>
        <translation>Oldal tulajdonságok kezelése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Fit in window</source>
        <translation>&amp;Ablakhoz igazítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;50%</source>
        <translation>&amp;50%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;75%</source>
        <translation>&amp;75%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;200%</source>
        <translation>&amp;200%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Margins</source>
        <translation>&amp;Margók mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Frames</source>
        <translation>&amp;Keretek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Images</source>
        <translation>&amp;Képek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Grid</source>
        <translation>&amp;Rács mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show G&amp;uides</source>
        <translation>&amp;Segédvonalak mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Alaprács mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Text Chain</source>
        <translation>&amp;Szöveg lánc mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Control Characters</source>
        <translation>Vezérlő karakterek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sn&amp;ap to Grid</source>
        <translation>Rácshoz igazít</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Properties</source>
        <translation>&amp;Tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Gyűjtőalbum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Layers</source>
        <translation>&amp;Rétegek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Oldalak rendezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Könyvjelzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Measurements</source>
        <translation>&amp;Mértékegységek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Action &amp;History</source>
        <translation>&amp;Művelet előzmény</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preflight &amp;Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Igazítás és elosztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Tools</source>
        <translation>&amp;Eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Item</source>
        <translation>Elem kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>T&amp;able</source>
        <translation>&amp;Táblázat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shape</source>
        <translation>&amp;Alak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Line</source>
        <translation>&amp;Vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Szabadkézi vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rotate Item</source>
        <translation>Elem forgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Zoom in or out</source>
        <translation>Zoomolás be és ki</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Zoom in</source>
        <translation>Bezoomolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Zoom out</source>
        <translation>Kizoomolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Contents of Frame</source>
        <translation>Keret tartalmának szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Text...</source>
        <translation>Szöveg szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Link Text Frames</source>
        <translation>Szöveg keretek összekapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unlink Text Frames</source>
        <translation>Szöveg keretek közti kapcsolat megszüntetése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Eye Dropper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy Item Properties</source>
        <translation>Elem tulajdonságának másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit the text with the Story Editor</source>
        <translation>Szöveg szerkesztése a Szövegszerkesztővel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Text Frame</source>
        <translation>Szöveg keret beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Image Frame</source>
        <translation>Kép keret beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Table</source>
        <translation>Táblázat beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Shape</source>
        <translation>Alak beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Polygon</source>
        <translation>Sokszög beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Line</source>
        <translation>Vonal beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Bezier Curve</source>
        <translation>Bezier görbe beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Freehand Line</source>
        <translation>Szabadkézi vonal beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Képek kezelése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Elválasztás a szövegben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dehyphenate Text</source>
        <translation>Nincs elválasztás a szövegben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Tartalomjegyzék előállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;About Scribus</source>
        <translation>&amp;Scribus névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>About &amp;Qt</source>
        <translation>&amp;Qt névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Toolti&amp;ps</source>
        <translation>Buboréksúgók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;kézikönyv...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Okos elválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Non Breaking &amp;Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page &amp;Number</source>
        <translation>Oldalszám</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Line</source>
        <translation>Új sor</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Frame Break</source>
        <translation>Keret törés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Column Break</source>
        <translation>Oszlop törés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Registered Trademark</source>
        <translation>Regisztrált védjegy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Trademark</source>
        <translation>Védjegy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bullet</source>
        <translation>Golyó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Em Dash</source>
        <translation>Kötőjel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>En Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Figure Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quotation Dash</source>
        <translation>Idéző jel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apostrophe</source>
        <translation>Aposztróf</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Straight Double</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Reversed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Reversed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Left Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Right Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Left Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Right Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Low Single Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Low Double Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CJK Single Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CJK Single Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CJK Double Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CJK Double Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Toggle Palettes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Toggle Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print Previe&amp;w</source>
        <translation>Nyomtatás előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaSzkriptek...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Convert to Master Page...</source>
        <translation>Mester oldallá konvertálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaszkád</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Tile</source>
        <translation>&amp;Cím</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;Modulok névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>More Info...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Printing Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Flip Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Flip Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Rulers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation type="unfinished">&amp;Körvonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Solidus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>En Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Em Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Thin Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Thick Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mid Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hair Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Smart Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Non Breaking Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Page Number</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>ff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>fi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>fl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>ffi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>ffl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>ft</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>st</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;tyles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation type="unfinished">&amp;Körvonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation type="unfinished">&amp;Körvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paste (&amp;Absolute)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Text Frame Columns</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Align and Distribute</source>
        <translation>Igazítás és elosztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align</source>
        <translation>Igazítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Relative to:</source>
        <translation>&amp;Viszonylagos:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>First Selected</source>
        <translation>Először kiválasztott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Last Selected</source>
        <translation>Utoljára kiválasztott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page</source>
        <translation>Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Margins</source>
        <translation>Margók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Guide</source>
        <translation>Segédlet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Selection</source>
        <translation>Kiválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Az objektum jobb oldalának igazítása a horgony bal oldalához</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Az objektum bal oldalának igazítása a horgony jobb oldalához</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align bottoms</source>
        <translation>Igazítás alulra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align right sides</source>
        <translation>Igazítás jobb oldalra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Az objektum tetejének igazítása a horgony aljához</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Center on vertical axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align left sides</source>
        <translation>Igazítés a bal oldalhoz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Center on horizontal axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Az objektum aljának igazítása a horgony tetejéhez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align tops</source>
        <translation>Igazítás felülre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute</source>
        <translation>Elosztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Objektumok közötti vízszintes közök egyenlőek legyenek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Objektumok közötti vízszintes közök egy adott értéknek megfelelőek legyenek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute right sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute bottoms equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute centers equidistantly horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make vertical gaps between objects equal</source>
        <translation>Objektumok közötti függőleges közök egyenlőek legyenek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Objektumok közötti függőleges közök egy adott értéknek megfelelőek legyenek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute left sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute centers equidistantly vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute tops equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Distance:</source>
        <translation>&amp;Távolság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distribute the items with the distance specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None Selected</source>
        <translation>Nem kiválasztott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Some objects are locked.</source>
        <translation>Néhány objektum zárolt.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Unlock All</source>
        <translation>&amp;Minden zárolás feloldása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y: %1%2</source>
        <translation>Y: %1%2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X: %1%2</source>
        <translation>X: %1%2</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Align Text Left</source>
        <translation>Szöveg balra igazítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align Text Right</source>
        <translation>Szöveg jobbra igazítása </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align Text Center</source>
        <translation>Szöveg középre igazítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align Text Justified</source>
        <translation>Sorkizárt igazítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align Text Forced Justified</source>
        <translation>Kényszerített sorkizárt igazítás</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Field Properties</source>
        <translation>Mező jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type:</source>
        <translation>Típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Button</source>
        <translation>Gomb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Field</source>
        <translation>Szövegmező</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check Box</source>
        <translation>Jelölő nézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Combo Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Properties</source>
        <translation>Tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name:</source>
        <translation>Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tool-Tip:</source>
        <translation>Buboréksúgó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text</source>
        <translation>Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font for use with PDF 1.3:</source>
        <translation>PDF 1.3-al használható betűtípusok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Border</source>
        <translation>Keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color:</source>
        <translation>Szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Width:</source>
        <translation>Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Thin</source>
        <translation>Vékony</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Wide</source>
        <translation>Széles</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Style:</source>
        <translation>Stílus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Solid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dashed</source>
        <translation>Gondolatjel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline</source>
        <translation>Aláhúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Beveled</source>
        <translation>Ferde</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Inset</source>
        <translation>Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Other</source>
        <translation>Egyéb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Read Only</source>
        <translation>Csak olvasható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Required</source>
        <translation>Szükséges</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Don&apos;t Export Value</source>
        <translation>Nem exportálható érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Visibility:</source>
        <translation>Láthatóság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Visible</source>
        <translation>Látható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hidden</source>
        <translation>Láthatatlan</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Print</source>
        <translation>Nem nyomtatható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No View</source>
        <translation>Nem látható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Appearance</source>
        <translation>Megjelenés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text for Button Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text for Roll Over</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Icons</source>
        <translation>Ikonok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use Icons</source>
        <translation>Ikonok használata</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove</source>
        <translation>Eltávolítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Pressed</source>
        <translation>Lenyomva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Roll Over</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Icon Placement...</source>
        <translation>Ikon elhelyezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Highlight</source>
        <translation>Kijelölés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Invert</source>
        <translation>Invertálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Outlined</source>
        <translation>Szerkezet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Push</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Multi-Line</source>
        <translation>Többsoros</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Password</source>
        <translation>Jelszó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Limit of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Characters</source>
        <translation>Karakterek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do Not Scroll</source>
        <translation>Ne görgesd</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do Not Spell Check</source>
        <translation>Ne ellenőrzid a helyesírást</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check Style:</source>
        <translation>Kijelölés stílusa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check</source>
        <translation>Kijelölés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cross</source>
        <translation>Kereszt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Diamond</source>
        <translation>Gyémánt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Circle</source>
        <translation>Kör</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Star</source>
        <translation>Csillag</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Square</source>
        <translation>Négyzet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default is Checked</source>
        <translation>Alapértelmezett: kijelölve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Editable</source>
        <translation>Szerkeszthető</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Go To</source>
        <translation>Ugrás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Submit Form</source>
        <translation>Űrlap elküldése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset Form</source>
        <translation>Űrlap törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Data</source>
        <translation>Adat importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Event:</source>
        <translation>Esemény:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mouse Up</source>
        <translation>Egér fel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mouse Down</source>
        <translation>Egér le</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mouse Enter</source>
        <translation>Egér belépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mouse Exit</source>
        <translation>Egér kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>On Focus</source>
        <translation>Fókusz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>On Blur</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Script:</source>
        <translation>Szkript:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit...</source>
        <translation>Szerkesztés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Submit to URL:</source>
        <translation>Küldés URL-re:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Submit Data as HTML</source>
        <translation>Adat küldése HTML-ként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Data from:</source>
        <translation>Adat importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Destination</source>
        <translation>Cél</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To File:</source>
        <translation>Fájlba:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change...</source>
        <translation>Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page:</source>
        <translation>Oldal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X-Pos:</source>
        <translation>X pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y-Pos:</source>
        <translation>Y pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Action</source>
        <translation>Művelet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Field is formatted as:</source>
        <translation>Mező formázva:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plain</source>
        <translation>Egyszerű</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number</source>
        <translation>Szám</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Percentage</source>
        <translation>Százalék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Date</source>
        <translation>Dátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Time</source>
        <translation>Idő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number Format</source>
        <translation>Számformátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Decimals:</source>
        <translation>Tizedesjegyek:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use Currency Symbol</source>
        <translation>Pénzügyi szimbólum használata</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Prepend Currency Symbol</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Formatting</source>
        <translation>Formázás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Percent Format</source>
        <translation>Százalékformátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Date Format</source>
        <translation>Dátumformátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Time Format</source>
        <translation>Időformátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom Scripts</source>
        <translation>Saját szkriptek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Format:</source>
        <translation>Formátum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keystroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Format</source>
        <translation>Formátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value is not validated</source>
        <translation>Az érték nincs jóváhagyva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value must be greater than or equal to:</source>
        <translation>Az értéknek nagyobbnak vagy egyenlőnek kell lenni, mint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>and less or equal to:</source>
        <translation>és kisebbnek vagy egyenlőnek, mint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom validate script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Validate</source>
        <translation>Érvényesít</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value is not calculated</source>
        <translation>Érték nincs kiszámolva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value is the</source>
        <translation>Az érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>sum</source>
        <translation>összeg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>product</source>
        <translation>termék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>average</source>
        <translation>átlagos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>minimum</source>
        <translation>minimum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>maximum</source>
        <translation>maximum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>of the following fields:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Pick...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom calculation script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Calculate</source>
        <translation>Kiszámítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flag is ignored for PDF 1.3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enter a comma separated list of fields here</source>
        <translation>Adja meg a vesszővel elválasztott mező listát</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Example:</source>
        <translation>Példa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Selection Change</source>
        <translation>Kiválasztás megváltoztatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF fájlok (*.pdf);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>JavaScript</source>
        <translation>JavaSzkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Képek (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>highlight</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>action</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Annotation Properties</source>
        <translation>Megjegyzés tulajdonságai</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text</source>
        <translation>Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Link</source>
        <translation>Kapcsolat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>External Link</source>
        <translation>Külső kapcsolat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>External Web-Link</source>
        <translation>Külső web kapcsolat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Destination</source>
        <translation>Cél</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF dokumentum (*.pdf);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Type:</source>
        <translation>&amp;Típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Page:</source>
        <translation>&amp;Oldal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;X-Pos</source>
        <translation>&amp;X pozíció</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y pozíció:</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply Master Page</source>
        <translation>Mester oldal alkalmazása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Master Page:</source>
        <translation>&amp;Mester oldal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply To</source>
        <translation>Alkalmazás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Current &amp;page</source>
        <translation>Jelenlegi oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Even pages</source>
        <translation>&amp;Páros oldalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>O&amp;dd pages</source>
        <translation>Pá&amp;ratlan oldalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;All pages</source>
        <translation>&amp;Minden oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Within range</source>
        <translation>&amp;Tartományon belül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;A kiválasztott sablon alkalmazása a páros, páratlan, vagy az összes oldalra a következő tartományon belül&lt;/qt&gt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Barcode Generator...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Error opening file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>12 or 13 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>8 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>11 or 12 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>7 or 8 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>5 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>2 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of digits and capital letters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Variable number of hexadecimal characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Barcode incomplete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Barcode Creator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Type:</source>
        <translation type="unfinished">&amp;Típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select one of the available barcode type here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset the barcode samples</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Include text in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Guard whitespace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+G</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Draw arrows to be sure of space next the code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colors</source>
        <translation type="unfinished">Színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Background color - under the code lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of the lines in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Text</source>
        <translation type="unfinished">&amp;Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of the text and numbers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hints and help is shown here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview of the result. 72dpi sample.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Co&amp;de:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>I&amp;nclude checksum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Generate and include a checksum in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Incl&amp;ude checksum digit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+U</source>
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Include the checksum digit in the barcode text</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Scrapbook</source>
        <translation>Gyűjtőalbum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Load...</source>
        <translation>&amp;Betöltés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;As...</source>
        <translation>M&amp;ent másként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Small</source>
        <translation type="obsolete">&amp;Kicsi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Medium</source>
        <translation type="obsolete">Kö&amp;zepes</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Large</source>
        <translation type="obsolete">&amp;Nagy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation>&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Preview</source>
        <translation>&amp;Előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Gyűjtőalbumok (*.scs);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rename</source>
        <translation>Átnevezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete</source>
        <translation>Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">A &quot;%1&quot; fájl már létezik.
Válasszon másikat.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object</source>
        <translation>Objektum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name:</source>
        <translation>&amp;Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Entry</source>
        <translation>Új bejegyzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Scrapbook Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scrapbook (*.scs)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a scrapbook file to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import Scrapbook File...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Main</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Bookmarks</source>
        <translation>Könyvjelzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Bookmark</source>
        <translation>Könyvjelző mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Bookmark</source>
        <translation>Könyvjelző beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Bookmarks</source>
        <translation>Könyvjelzők</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Icon Placement</source>
        <translation>Ikon elhelyezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Layout:</source>
        <translation>Elrendezés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Caption only</source>
        <translation>Csak szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Icon only</source>
        <translation>Csak ikon</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Caption below Icon</source>
        <translation>Szöveg az ikon alatt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Caption above Icon</source>
        <translation>Szöveg az ikon felett</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Caption right to Icon</source>
        <translation>Szöveg az ikontól jobbra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Caption left to Icon</source>
        <translation>Szöveg az ikontól balra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Caption overlays Icon</source>
        <translation>Szöveg az ikonon</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scale:</source>
        <translation>Méretezés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Always</source>
        <translation>Mindig</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>When Icon is too small</source>
        <translation>Ha az ikon túl kicsi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>When Icon is too big</source>
        <translation>Ha az ikon túl nagy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Never</source>
        <translation>Soha</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scale How:</source>
        <translation>Méretezés módja:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Proportional</source>
        <translation>Arányos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Non Proportional</source>
        <translation>Nem arányos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Icon</source>
        <translation>Ikon</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset</source>
        <translation>Visszaállítás</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Szín kezelés aktiválása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>System Profiles</source>
        <translation>Rendszer profilok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>P&amp;rinter:</source>
        <translation>&amp;Nyomtató:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering Intents</source>
        <translation>Megjelenítési prioritások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation</source>
        <translation>Telítettség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Absolute Colorimetric</source>
        <translation>Abszolút kolorimetriás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>M&amp;onitor:</source>
        <translation>Monitor:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Pr&amp;inter:</source>
        <translation>Nyomtató:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Nyomtató szimulálása a képernyőn</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default color profile for solid colors on the page</source>
        <translation>Alapértelmezett szín profil az oldalon lévő egyedi színekhez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Szín profil, melyet Ön előállított, vagy a gyártótól kapott. 
Ez a profil a monitorához kialakított s nem egy általános profil (pl. sRGB).</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Szín profil a nyomtatójához a gyártótól. Ez a profil a nyomtatójához kialakított s nem egy általános profil (pl. sRGB).</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>A képernyőn megjelenő színek módszere miatt a nyomtatás nem megfelelő lehet. Emiatt nagyon precíz profilokra van szükség.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>A Fekete pont kompenzáció egy módszer fényképek kontrasztjának növelésére. Javasolt az engedélyezése, ha a dokumentuma fényképet is tartalmaz.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB képek:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK képek:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default color profile for imported CMYK images</source>
        <translation>Alapértelmezett szín profil az importált CMYK képekhez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default color profile for imported RGB images</source>
        <translation>Alapértelmezett szín profil az importált RGB képekhez</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Color</source>
        <translation>Szín szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name:</source>
        <translation>&amp;Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color &amp;Model</source>
        <translation>&amp;Szín modell</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CMYK</source>
        <translation>CMYK</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Web Safe RGB</source>
        <translation>Webre alkalmas RGB</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New</source>
        <translation>Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Old</source>
        <translation>Régi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>HSV-Colormap</source>
        <translation>HSV-Színtérkép</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>M:</source>
        <translation>M:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>K:</source>
        <translation>K:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dynamic Color Bars</source>
        <translation>Dinamikus színcsúszkák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Static Color Bars</source>
        <translation>Statikus színcsúszkák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>R:</source>
        <translation>R:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>G:</source>
        <translation>G:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>B:</source>
        <translation>B:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">A szín elnevezése már létezik</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Nem hozhatja létre a \&quot;%1\&quot; nevű színt. Ez egy transzparens szín foglalt neve.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is Registration Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of the color is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <location filename="" line="7471221"/>
        <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
        <translation type="obsolete">Miniatűr nézetre válthat, ha lenyomja és nyomva tartja a jobb egér gombot 

Az Insert gomb beszúrt egy Glyph-et az alábbi Kiválasztásba, a Törlés gomb eltávolítja az utolsónak beillesztettet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Character:</source>
        <translation>Karakter kiválasztása:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font:</source>
        <translation>Betűtípus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Character Class:</source>
        <translation>Karakter osztály:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Insert</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Karakterek beillesztése a szövegbe a kurzornál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete the current selection(s).</source>
        <translation>Az érvényes kiválasztás(ok) törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Dialódus bezárása és visszatérés a szöveg szerkesztéséhez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Full Character Set</source>
        <translation>Teljes karakter készlet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Basic Latin</source>
        <translation>Alap latin</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Latin-1 Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Latin Extended-A</source>
        <translation>Bővített latin A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Latin Extended-B</source>
        <translation>Bővített latin B</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>General Punctuation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Super- and Subscripts</source>
        <translation>Felső és alsó index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Currency Symbols</source>
        <translation>Pénz szimbólumok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Letterlike Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arrows</source>
        <translation>Nyílak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mathematical Operators</source>
        <translation>Matematikai operátorok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Box Drawing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Block Elements</source>
        <translation>Klisé elemek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Geometric Shapes</source>
        <translation>Geometrikus alakzatok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Miscellaneous Symbols</source>
        <translation>Különböző szimbólumok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dingbats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Small Form Variants</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ligatures</source>
        <translation>Ikerbetűk</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Specials</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Greek</source>
        <translation>Görörg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Greek Extended</source>
        <translation>Görög kibővített</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cyrillic</source>
        <translation>Ciril</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cyrillic Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arabic</source>
        <translation>Arab</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arabic Extended A</source>
        <translation>Bővített arab A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arabic Extended B</source>
        <translation>Bővített arab B</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hebrew</source>
        <translation>Héber</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Insert Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close this dialog and return to text editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Current Profile:</source>
        <translation>Jelenlegi profil:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Items</source>
        <translation>Elemek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Problems</source>
        <translation>Problémák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Glyphs missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text overflow</source>
        <translation>Szöveg túlcsordulás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object is not on a Page</source>
        <translation>Az objektum nincs az oldalon</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Missing Image</source>
        <translation>Hiányzó kép</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object has transparency</source>
        <translation>Az objektum transzparens</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object is a placed PDF</source>
        <translation>Az objektum PDF</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document</source>
        <translation>Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Problems found</source>
        <translation>Nem keletkezett probléma</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page </source>
        <translation>Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Free Objects</source>
        <translation>Szabad objektum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Problems found</source>
        <translation>Probléma keletkezett</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Az objektum egy PDF kommentár vagy mező</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Ignore Errors</source>
        <translation>Hibák figyelmen kívül hagyása</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose Styles</source>
        <translation>Stílus kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Styles</source>
        <translation>Elérhető stílusok</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Directory</source>
        <translation>Könyvtár kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Collecting...</source>
        <translation>Összegyűjtés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation>Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Nem gyűjthetők össze a fájlok a %1 kimeneti fáljhoz.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot collect the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Colors</source>
        <translation type="unfinished">Színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">&amp;Kettőzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished">&amp;Nem használtak eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Sets</source>
        <translation type="unfinished">Szín paletta</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Current Color Set:</source>
        <translation type="unfinished">Jelenlegi szín készlet:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished">&amp;Szín készket mentése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a color set to load</source>
        <translation type="unfinished">Válassza ki a betöltendő szín készletet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save the current color set</source>
        <translation type="unfinished">Jelenlegi szín készlet mentése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished">Nem használt színek eltávolítása a jelenlegi dokumentum szín készletéből</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished">Egy létező dokumentumból a színek importálása a jelenlegi készletbe.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create a new color within the current set</source>
        <translation type="unfinished">Új szín létrehozása a jelenlegi készletben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit the currently selected color</source>
        <translation type="unfinished">Kiválasztott szín szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make a copy of the currently selected color</source>
        <translation type="unfinished">Jelenleg kijelölt szín lemásolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete the currently selected color</source>
        <translation type="unfinished">A kiválasztott szín törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make the current colorset the default color set</source>
        <translation type="unfinished">A jelenlegi szín készlet legyen az alapértelmezett</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Name</source>
        <translation type="unfinished">Név kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation type="unfinished">Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy of %1</source>
        <translation type="unfinished">%1 másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Color</source>
        <translation type="unfinished">Új szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Monochromatic</source>
        <translation>Monokromatikus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Analogous</source>
        <translation>Analóg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Complementary</source>
        <translation>Komplemens</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Split Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tetradic (Double Complementary)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Base Color</source>
        <translation>Alapszín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Monochromatic Light</source>
        <translation>Monokromatikus világos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Monochromatic Dark</source>
        <translation>Monokromatikus sötét</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>1st. Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>2nd. Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>1st. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>2nd. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>3rd. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>4th. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>1st. Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>2nd. Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>1st. Tetradic (base opposite)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>2nd. Tetradic (angle)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal Vision</source>
        <translation>Normál nézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Full Color Blindness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vision Defect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Wheel</source>
        <translation>Színkerék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color</source>
        <translation>Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y</source>
        <translation>Y</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Method:</source>
        <translation>Módszer kiválasztása:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Szög (0 - 90 fok):</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Színek összefésülése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Replace Colors</source>
        <translation>Színek &amp;kicserélése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Merge created colors into the document colors</source>
        <translation>Létrehozott színek összefésülése a dokumentum színeivel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace created colors in the document colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Leave colors untouched</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Merging colors</source>
        <translation>Színek összefésülése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error: </source>
        <translation>Hiba: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Now opening the color manager.</source>
        <translation>Szín kezelő megnyitása folyik.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Merging</source>
        <translation>Szín összefésülés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cr&amp;eate color...</source>
        <translation>Szín létrehozása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import existing color...</source>
        <translation>Létező szín importálása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Merge colors</source>
        <translation>Színek összefésülése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Replace colors</source>
        <translation>Színek kicserélése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>E&amp;xit</source>
        <translation>Kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;olor</source>
        <translation>&amp;Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation:</source>
        <translation type="obsolete">Telítettség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value:</source>
        <translation type="obsolete">Érték:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Különbség a kiválasztott érték és a számított között. További információért forduljon a dokumentációhoz.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample color scheme</source>
        <translation>Szín séma minta</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>A szín séma létrehozásához válasszon a módszerek közül. További információt a dokumentációban talál.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colors of your chosen color scheme</source>
        <translation>A kiválasztott szín séma színei</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Color</source>
        <translation>Új szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;olor Components...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Protanopia (Red)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Deuteranopia (Green)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tritanopia (Blue)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color %1 exists already!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color %1 appended.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Színkerék...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color setting helper</source>
        <translation>Szín beállítás segítség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color selector with color theory included.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Apply</source>
        <translation>&amp;Alkalmazás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;OK</source>
        <translation>&amp;Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation>&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="unfinished">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>color name</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation type="unfinished">Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Page</source>
        <translation type="unfinished">Egyoldalas</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>3-Fold</source>
        <translation type="unfinished">3 lapos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>4-Fold</source>
        <translation type="unfinished">4 lapos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left Page</source>
        <translation type="unfinished">Bal oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle</source>
        <translation type="unfinished">Középső</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle Left</source>
        <translation type="unfinished">Középső bal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle Right</source>
        <translation type="unfinished">Középső jobb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right Page</source>
        <translation type="unfinished">Jobb oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation type="unfinished">Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Monday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tuesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Wednesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Thursday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Friday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sunday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>January</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>February</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>March</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>April</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>May</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>June</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>July</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>August</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>September</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>October</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>November</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>December</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Shade:</source>
        <translation>Árnyalat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Opacity:</source>
        <translation>Átlátszatlanság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal Gradient</source>
        <translation>Vízszintes átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical Gradient</source>
        <translation>Függőleges átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Diagonal Gradient</source>
        <translation>Átlós átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cross Diagonal Gradient</source>
        <translation>Kereszt átlós átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Radial Gradient</source>
        <translation>Sugaras átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Free linear Gradient</source>
        <translation>Szabad vonalas átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Free radial Gradient</source>
        <translation>Szabad sugaras átmenet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X1:</source>
        <translation>X1:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y1:</source>
        <translation>Y1:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X2:</source>
        <translation>X2:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y2:</source>
        <translation>Y2:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Line Color Properties</source>
        <translation>Vonal szín tulajdonságainak szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Fill Color Properties</source>
        <translation>Kitöltő szín tulajdonságainak szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color</source>
        <translation>Színtelítettség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal or gradient fill method</source>
        <translation>Normál vagy átmenet kitöltési módszer</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set the transparency for the color selected</source>
        <translation>Kiválasztott szín atlátszhatóságának beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Vector</source>
        <translation>Vektor mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>CSV Importer Options</source>
        <translation>CSV importálás beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Field delimiter:</source>
        <translation>Mező határoló:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value delimiter:</source>
        <translation>Érték határoló:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>First row is a header</source>
        <translation>Az első sor a fejléc</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>delimiter</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Printer Options</source>
        <translation>Nyomtató beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Set</source>
        <translation>Oldal beállítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Pages</source>
        <translation>Minden oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Even Pages only</source>
        <translation>Csak páros oldalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Odd Pages only</source>
        <translation>Csak páratlan oldalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror</source>
        <translation>Tükrözés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No</source>
        <translation>Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation>Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orientation</source>
        <translation>Orientáció</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portrait</source>
        <translation>Álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Landscape</source>
        <translation>Fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>N-Up Printing</source>
        <translation>n-Up nyomtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page per Sheet</source>
        <translation>Oldal ívenként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Pages per Sheet</source>
        <translation>Oldalak ívenként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Option</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value</source>
        <translation>Érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Compress File</source>
        <translation>&amp;Fájl tömörítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Fontok belefoglalása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Encoding:</source>
        <translation>Kódolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Eljutás a dokumentum könyvtárába. Ez megadható a Beállításoknál.</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Set Color Components</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CMYK</source>
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>HSV</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>H:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>V:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>R:</source>
        <translation type="unfinished">R:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>G:</source>
        <translation type="unfinished">G:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>B:</source>
        <translation type="unfinished">B:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C:</source>
        <translation type="unfinished">C:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>M:</source>
        <translation type="unfinished">M:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y:</source>
        <translation type="unfinished">Y:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>K:</source>
        <translation type="unfinished">K:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set &amp;RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set C&amp;MYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set &amp;HSV</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancelled by user</source>
        <translation>Felhasználó megszakította</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Color</source>
        <translation>Szín törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Color:</source>
        <translation>Szín törlése:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace With:</source>
        <translation>Helyettesítve:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Pages</source>
        <translation>Oldalak törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>to:</source>
        <translation>oldalig:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete From:</source>
        <translation>Törlés eddig:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace With:</source>
        <translation type="unfinished">Helyettesítve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Style</source>
        <translation type="unfinished">Nincs stílus megadva</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing Word document failed 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Information</source>
        <translation>Dokumentum információ</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Title:</source>
        <translation>&amp;Cím:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Author:</source>
        <translation>&amp;Szerző:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Keywords:</source>
        <translation>&amp;Kulcsszavak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Leírás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Kiadó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Contributors:</source>
        <translation>&amp;Közreműködők:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dat&amp;e:</source>
        <translation>&amp;Dátum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>T&amp;ype:</source>
        <translation>&amp;Típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formátum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Azonosító:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Source:</source>
        <translation>&amp;Forrás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Language:</source>
        <translation>&amp;Nyelv:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Relation:</source>
        <translation>&amp;Összefüggés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Co&amp;verage:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Jogok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Further &amp;Information</source>
        <translation>&amp;További információ</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A person or organisation responsible for making the document available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Információ a dokumentummal kapcsolatos jogokról, pl. másolásvédelem, szabadalom, védett márka</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documen&amp;t</source>
        <translation>&amp;Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>1, 2, 3, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>i, ii, iii, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>I, II, III, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>a, b, c, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A, B, C, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Number Out Of Bounds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation type="unfinished">Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>From</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relates To</source>
        <translation>Összefüggésben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Frames</source>
        <translation>Szöveg keretek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image Frames</source>
        <translation>Kép keretek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Boolean</source>
        <translation>Logikai</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Integer</source>
        <translation>Egész</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>String</source>
        <translation>Sztring</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Item Attributes</source>
        <translation type="obsolete">Dokumentum elem jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation type="obsolete">Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation type="obsolete">Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value</source>
        <translation type="obsolete">Érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Parameter</source>
        <translation type="obsolete">Paraméter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relationship</source>
        <translation type="obsolete">Összefüggés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>auto add</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>types</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Real Number</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Item Attributes</source>
        <translation type="unfinished">Dokumentum elem jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation type="unfinished">Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation type="unfinished">Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value</source>
        <translation type="unfinished">Érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Parameter</source>
        <translation type="unfinished">Paraméter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relationship</source>
        <translation type="unfinished">Összefüggés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Auto Add To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Setup Printer</source>
        <translation>Nyomtató beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print Destination</source>
        <translation>Nyomtatási cél</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation>Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Options...</source>
        <translation>&amp;Beállítások...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File:</source>
        <translation>&amp;Fájl:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Alternatív nyomtató parancs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Használjon alternatív nyomtató kezelőt, mint például kprinter vagy gtklp, a további nyomtatási beállítások használatához</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Parancs:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Range</source>
        <translation>Tartomány</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print &amp;All</source>
        <translation>&amp;Minden nyomtatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print Current Pa&amp;ge</source>
        <translation>&amp;Aktuális oldal nyomtatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print &amp;Range</source>
        <translation>&amp;Tartomány nyomtatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Másolatok száma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All</source>
        <translation>Minden</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cyan</source>
        <translation>Kékeszöld</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Magenta</source>
        <translation>Bíborvörös</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yellow</source>
        <translation>Sárga</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Black</source>
        <translation>Fekete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Print</source>
        <translation>&amp;Nyomtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation>Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print Normal</source>
        <translation>Normál nyomtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print Separations</source>
        <translation type="unfinished">Nyomtatás színrebontással</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print in Color if Available</source>
        <translation>Színes nyomtatás, ha lehetséges</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print in Grayscale</source>
        <translation>Nyomtatás szürkeskálában</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript Level 1</source>
        <translation>PostScript 1 szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript Level 2</source>
        <translation>PostScript 2 szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript Level 3</source>
        <translation>PostScript 3 szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page</source>
        <translation>Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Vízszintesen tükrözött oldal(ak)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror Page(s) Vertical</source>
        <translation>Függőlegesen tükrözött oldal(ak)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set Media Size</source>
        <translation>Média méret beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color</source>
        <translation>Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply ICC Profiles</source>
        <translation>ICC profilok alkalmazása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Advanced Options</source>
        <translation>További beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview...</source>
        <translation>Előnézet...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript fájlok (*.ps);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to retrieve printer settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing File:
%1
failed!</source>
        <translation>%1 fájl importálásakor hiba történt!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fatal Error</source>
        <translation>Végzetes hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error</source>
        <translation type="unfinished">hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Analyzing PostScript:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Generating Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Converting of %1 images failed!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Style</source>
        <translation>Stílus szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name:</source>
        <translation>&amp;Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Character</source>
        <translation>Karakter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Lines:</source>
        <translation>&amp;Vonalak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tabulators and Indentation</source>
        <translation>Tabulátorok és rovátkolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of your paragraph style</source>
        <translation>Bekezdés stílusának neve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font of selected text or object</source>
        <translation>A kijelölt szöveg vagy objektum betűtípusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Size</source>
        <translation>Betűméret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of text fill</source>
        <translation>Szöveg kitöltő színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spacing above the paragraph</source>
        <translation>Távolság a bekezdés előtt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spacing below the paragraph</source>
        <translation>Távolság a bekezdés után</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Spacing</source>
        <translation>Vonal távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">A megadott stílusnév már létezik</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distances</source>
        <translation>Távolságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fixed Linespacing</source>
        <translation>Rögzített sortávolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatic Linespacing</source>
        <translation>Automatikus vonaltávolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align to Baseline Grid</source>
        <translation>Igazítás alaprácshoz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance from Text:</source>
        <translation>Távolság a szövegtől:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview of the Paragraph Style</source>
        <translation>Bekezdés stílus előnézete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Toggles sample text of this paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Background</source>
        <translation type="unfinished">Háttér</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of the style is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Manual Tracking</source>
        <translation type="unfinished">Kézi követés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished">Karakterek alapvonalának eltolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Editor</source>
        <translation>Szerkesztő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Open...</source>
        <translation>&amp;Megnyitás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;As...</source>
        <translation>&amp;Ment másként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Mentés és kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Kilépés mentés nélkül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Undo</source>
        <translation>&amp;Visszavonás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Redo</source>
        <translation>&amp;Újra végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cu&amp;t</source>
        <translation>&amp;Kivágás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Mező nevek megadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation>&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation>&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>Java Szkriptek (*.js);;Minden fájl (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Image Effects</source>
        <translation>Kép effektusok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options:</source>
        <translation>Beállítások:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color:</source>
        <translation>Szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shade:</source>
        <translation>Árnyalat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Brightness:</source>
        <translation>Fényesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Contrast:</source>
        <translation>Kontraszt:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Radius:</source>
        <translation>Sugár:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value:</source>
        <translation>Érték:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Posterize:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Effects</source>
        <translation>Elérhető effektek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Blur</source>
        <translation>Életlenség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Brightness</source>
        <translation>Fényesség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colorize</source>
        <translation>Színezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Contrast</source>
        <translation>Kontraszt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Grayscale</source>
        <translation>Szürkeskála</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Invert</source>
        <translation>Invertálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Posterize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sharpen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&gt;&gt;</source>
        <translation>&gt;&gt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;&lt;</source>
        <translation>&lt;&lt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Effects in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Export Directory</source>
        <translation>Exportálás könyvtár kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export as Image(s)</source>
        <translation>Exportálás kép(ek)ként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportálás könyvtárba:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image &amp;Type:</source>
        <translation>Kép &amp;típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Quality:</source>
        <translation>&amp;Minőség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Resolution:</source>
        <translation>&amp;Felbontás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Range</source>
        <translation>Tartomány</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Current page</source>
        <translation>&amp;Aktuális oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;All pages</source>
        <translation>&amp;Minden oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Range</source>
        <translation>&amp;Tartomány</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export a range of pages</source>
        <translation>Oldal tartomány exportálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export all pages</source>
        <translation>Összes oldal exportálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export only the current page</source>
        <translation>Csak a jelenlegi oldal exportálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Kép felbontása 
A képekhez 72 dpi-t használjon.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Képek minősége - 100% a legjobb, 1% a legrosszabb minőség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available export formats</source>
        <translation>Elérhető exportálási formátumok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change the output directory</source>
        <translation>A Kimeneti könyvtár megváltoztatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation>&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Képek mérete. 100%: eredeti méret, 200 %: kétszeres méret stb.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Extended Image Properties</source>
        <translation>Bővített kép tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Darken</source>
        <translation>Sötétít</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lighten</source>
        <translation>Világosít</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hue</source>
        <translation>Színárnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation</source>
        <translation>Telítettség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color</source>
        <translation>Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Luminosity</source>
        <translation>Fényesség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Multiply</source>
        <translation>Többszörös</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Screen</source>
        <translation>Képernyő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dissolve</source>
        <translation>Eltűnés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Overlay</source>
        <translation>Átfedés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hard Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Soft Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Difference</source>
        <translation>Különbség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exclusion</source>
        <translation>Kivétel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Dodge</source>
        <translation>Szín trükk</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Burn</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exlusion</source>
        <translation>Kivétel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Blend Mode:</source>
        <translation>Szemcsézés módja:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Opacity:</source>
        <translation>Átlátszatlanság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Background</source>
        <translation>Háttér</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Layers</source>
        <translation>Rétegek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paths</source>
        <translation>Útvonalak</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Size:</source>
        <translation>Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Title:</source>
        <translation>Cím:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Title</source>
        <translation>Nincs cím</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Author:</source>
        <translation>Szerző:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unknown</source>
        <translation>Ismeretlen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Document</source>
        <translation>Scribus dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resolution:</source>
        <translation>Felbontás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CMYK</source>
        <translation>CMYK</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colorspace:</source>
        <translation>Színskála:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Grayscale</source>
        <translation>Szükreskála</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Colors</source>
        <translation type="obsolete">Színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Kettőzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">&amp;Nem használtak eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Sets</source>
        <translation type="obsolete">Szín paletta</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Current Color Set:</source>
        <translation type="obsolete">Jelenlegi szín készlet:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">&amp;Szín készket mentése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a color set to load</source>
        <translation type="obsolete">Válassza ki a betöltendő szín készletet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save the current color set</source>
        <translation type="obsolete">Jelenlegi szín készlet mentése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Nem használt színek eltávolítása a jelenlegi dokumentum szín készletéből</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Új szín létrehozása a jelenlegi készletben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Kiválasztott szín szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Jelenleg kijelölt szín lemásolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">A kiválasztott szín törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">A jelenlegi szín készlet legyen az alapértelmezett</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Name</source>
        <translation type="obsolete">Név kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation type="obsolete">Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy of %1</source>
        <translation type="obsolete">%1 másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Color</source>
        <translation type="obsolete">Új szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import colors to the current set from an existing document</source>
        <translation type="obsolete">Egy létező dokumentumból a színek importálása a jelenlegi készletbe.</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished">A dokumentum néhány betűtípusa helyettesítve van:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> was replaced by: </source>
        <translation type="unfinished">helyettesítve: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Fonts</source>
        <translation>Rendelkezésre álló betűtípusok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Substitutions</source>
        <translation>Betűtípus helyettesítés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Additional Paths</source>
        <translation>További útvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Name</source>
        <translation>Betűtípus neve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Available Fonts</source>
        <translation>Elérhető betűtípusok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replacement</source>
        <translation>Helyettesítés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Betűtípus helyettesítések</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A&amp;dd...</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Additional &amp;Paths</source>
        <translation>További &amp;útvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Directory</source>
        <translation>Könyvtár kiválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Betűtípus neve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Betűtípus használata</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Beágyazás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Részhalmaz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="obsolete">Betűtípus neve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="obsolete">Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="obsolete">Részhalmaz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="obsolete">Hozzáférés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Felhasználó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Rendszer</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="obsolete">Betűtípus előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Előnézet bezárása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Betűtípusnéző próbamondatszörny.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Size:</source>
        <translation type="obsolete">Betűtípus méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quick Search: </source>
        <translation type="obsolete">Gyors keresés: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Search</source>
        <translation type="obsolete">&amp;Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="obsolete">Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Start searching</source>
        <translation>Keresés indítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size of the selected font</source>
        <translation>A kiválasztott betűtípus mérete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Hozzáfűzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample will be shown after key release</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts Preview</source>
        <translation type="unfinished">Betűtípus előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Quick Search:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Name</source>
        <translation type="unfinished">Betűtípus neve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Doc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation type="unfinished">Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Subset</source>
        <translation type="unfinished">Részhalmaz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Access</source>
        <translation type="unfinished">Hozzáférés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font Size:</source>
        <translation type="unfinished">Betűméret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text</source>
        <translation type="unfinished">Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample text to display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Se&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Hozzáfűzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation type="unfinished">Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Betűtípus előnézet...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Preview dialog</source>
        <translation>Betűtípus előnézet párbeszéd ablak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Az elérhető betűtípusok rendezése, keresése, listázása.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Substitution</source>
        <translation>Betűtípus helyettesítés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Original Font</source>
        <translation>Eredeti betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Substitution Font</source>
        <translation>Helyettesítő betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make these substitutions permanent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Ez a dokumentum a rendszerében nem telepített betűtípusokat tartalmaz,kérem válasszon helyettesítő típust. A Mégsem megállítja a dokumentum betöltését.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>A betűtípus helyettesítés megszakítása és a dokumentum betötlésének megállítása.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Ezt engedélyezve a Scribus a hiányzó betűtípusok helyettesítésére a jövőben ezt a típust használja. Ezt módosíthatja, megváltoztathatja a Szerkesztés &gt; Beállítások &gt; Betűtípusok -nál.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Ha OK-ra kattint, majd elmenti, akkor a a helyettesítés állandóvá válik a dokumentumban.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Position:</source>
        <translation>Elhelyezkedés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add, change or remove color stops here</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Manage Guides</source>
        <translation>Segédvonalak kezelése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal Guides</source>
        <translation>Vízszintes segédvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;elete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical Guides</source>
        <translation>Függőleges segédvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A&amp;dd</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>De&amp;lete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Segédvonalak rögzítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Sorok és oszlopok - Automatikus vonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Rows:</source>
        <translation>&amp;Sorok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;olumns:</source>
        <translation>&amp;Oszlopok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Row &amp;Gap</source>
        <translation>&amp;Sortávolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colum&amp;n Gap</source>
        <translation>&amp;Oszlop távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Refer to:</source>
        <translation>Hivatkozva:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Page</source>
        <translation>&amp;Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Margins</source>
        <translation>&amp;Margók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Selection</source>
        <translation>&amp;Kiválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Update</source>
        <translation>&amp;Frissítés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Apply to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Guide</source>
        <translation type="unfinished">Segédlet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview</source>
        <translation type="unfinished">Előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>There is empty (0.0) guide already</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Kézikönyv nem elérhető! Kérem nézzen utánna a http://docs.scribus.net honlapon a frissített dokumentumnak, valamint a www.scribus.net honlapon a letöltéseknek.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Contents</source>
        <translation>Tartalom</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Link</source>
        <translation>Kapcsolat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Online Help</source>
        <translation>Scribus online segítség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Contents</source>
        <translation>&amp;Tartalom</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Search</source>
        <translation>&amp;Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Se&amp;arch</source>
        <translation>&amp;Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>De&amp;lete All</source>
        <translation>&amp;Minden törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Book&amp;marks</source>
        <translation>&amp;Könyvjelző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Print...</source>
        <translation>&amp;Nyomtatás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>E&amp;xit</source>
        <translation>&amp;Kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Searching is case unsensitive</source>
        <translation>Keresés nem nagy betű érzékeny</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>unknown</source>
        <translation type="obsolete">ismeretlen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Find</source>
        <translation>Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search Term:</source>
        <translation>Keresési feltétel:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Bookmark</source>
        <translation>Új könyvjelző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Új könyvjelző címe:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation>&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Find...</source>
        <translation>&amp;Keresés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Find &amp;Next</source>
        <translation>&amp;Következő keresése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Find &amp;Previous</source>
        <translation>&amp;Előző keresése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation>&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Könyvjelző hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;elete All</source>
        <translation>&amp;Minden törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Könyvjelző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relevance</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Possible Hyphenation</source>
        <translation>Lehetséges elválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Accept</source>
        <translation>Elfogadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Skip</source>
        <translation>Kihagyás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Language:</source>
        <translation>&amp;Nyelv</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Legrövidebb szöveg:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>A legrövidebb szó, melyet elválaszthatunk.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Az egymást követő elválasztás maximális száma. 0 érték esetén nincs elválasztási korlát.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;Elválasztási javaslat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>&amp;Szöveg elválasztás automatikusan gépelés alatt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Automatikus elválasztás engedélyezése gépelés közben.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Image Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>General Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Date / Time:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Has Embedded Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation type="unfinished">Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No</source>
        <translation type="unfinished">Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Profile Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Has Embedded Paths:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Has Layers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>EXIF Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Artist:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>User Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Camera Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Camera Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Description:</source>
        <translation type="unfinished">Leírás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copyright:</source>
        <translation type="unfinished">Copyright:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scanner Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scanner Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Import &amp;EPS/PS...</source>
        <translation>&amp;EPS/PS importálása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports EPS Files</source>
        <translation>EPS fájl importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>A legtöbb EPS fájl importálása a jelenlegi dokumentumba, vektor adatok konvertálása a Scribus objektumokba.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation type="unfinished">PostScript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Page</source>
        <translation>Oldal beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page(s)</source>
        <translation>Oldal(ak)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>before Page</source>
        <translation>Oldal elé</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>after Page</source>
        <translation>Oldal után</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>at End</source>
        <translation>Végén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Insert</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Master Pages</source>
        <translation>Mester oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Master Page:</source>
        <translation>&amp;Mester oldal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation>&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portrait</source>
        <translation>Álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Landscape</source>
        <translation>Fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Width:</source>
        <translation>&amp;Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Height:</source>
        <translation>&amp;Magasság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Objects with their Page</source>
        <translation>Objektumok mozgatása az oldalukkal együtt</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert Table</source>
        <translation>Táblázat beillesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of rows:</source>
        <translation>Sorok száma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of columns:</source>
        <translation>Oszlopok száma:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit JavaScripts</source>
        <translation>JavaSzkript szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit...</source>
        <translation>&amp;Szerkesztés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add...</source>
        <translation>&amp;Hozzáadás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New Script:</source>
        <translation>&amp;Új szkript:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Script</source>
        <translation>Új szkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;No</source>
        <translation>&amp;Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Yes</source>
        <translation>&amp;Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Tényleg törölni akarja ezt a szkriptet?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to delete this script?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Action</source>
        <translation>Művelet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Current Key</source>
        <translation>Jelenlegi billentyűparancs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select a Key for this Action</source>
        <translation>Válasszon ki egy billentyűt a művelethez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;No Key</source>
        <translation>&amp;Nincs billentyűzet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Felhasználó által definiált billentyű</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set &amp;Key</source>
        <translation>&amp;Billentyű beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt</source>
        <translation>Alt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ctrl</source>
        <translation>Ctrl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shift</source>
        <translation>Shift</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shift+</source>
        <translation>Shift+</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+</source>
        <translation>Alt+</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ctrl+</source>
        <translation>Ctrl+</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">A billentyűzet kombináció már foglalt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Loadable Shortcut Sets</source>
        <translation>Betölthető gyors billentyű készletek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Load</source>
        <translation>&amp;Betöltés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import...</source>
        <translation>&amp;Importálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Export...</source>
        <translation>&amp;Exportálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Reset</source>
        <translation>&amp;Alaphelyzetbe állítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Gyors billentyű készlet betölthető</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Load the selected shortcut set</source>
        <translation>A kijelölt gyors billentyű készlet betöltése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Gyors billentyű készlet importálása a jelenlegi konfigurációba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export the current shortcuts into an importable file</source>
        <translation>A jelenlegi gyors billentyűk exportálása egy importálható fájlba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Az alapértelmezett Scribus gyors billentyűk visszatöltése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Key Set XML fájlok (*.ksxml)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Meta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Meta+</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This key sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Layers</source>
        <translation>Rétegek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Layer</source>
        <translation>Réteg törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Tényleg törölni akarja ezen a rétegen lévő összes objektumot is?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add a new layer</source>
        <translation>Új réteg hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete layer</source>
        <translation>Réteg törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Raise layer</source>
        <translation>Réteg előrébb mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lower layer</source>
        <translation>Réteg hátrébb mozgatása</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Line Styles</source>
        <translation>Vonal stílus szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation>&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Kettőzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation>&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy of %1</source>
        <translation>%1 másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Style</source>
        <translation>Új stílus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;No</source>
        <translation>&amp;Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Yes</source>
        <translation>&amp;Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Tényleg törölni akarja ezt a stílust?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation>&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished">Tényleg törölni akarja ezt a stílust?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Width:</source>
        <translation type="unfinished">Vonalvastagság:</translation>
    </message>
</context>
<context>
    <name>LineStyleWidget</name>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flat Cap</source>
        <translation type="unfinished">14*17\&quot; papíralak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Square Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bevel Join</source>
        <translation type="unfinished">Ferde átlapolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Solid Line</source>
        <translation type="unfinished">Folytonos vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dashed Line</source>
        <translation type="unfinished">Szaggatott vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dotted Line</source>
        <translation type="unfinished">Pontozott vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dash Dot Line</source>
        <translation type="unfinished">Tá-ti vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">Tá-ti-ti vonal</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*)</source>
        <translation type="unfinished">Minden fájl (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Author:</source>
        <translation>Szerző:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Get More:</source>
        <translation>További információ:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>XML File:</source>
        <translation>XML fájl:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paragraphs:</source>
        <translation>Bekezdések:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Manage Page Properties</source>
        <translation>Oldal tulajdonságok kezelése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation>&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portrait</source>
        <translation>Álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Landscape</source>
        <translation>Fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Width:</source>
        <translation>&amp;Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Height:</source>
        <translation>&amp;Magasság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Objects with their Page</source>
        <translation>Objektumok mozgatása az oldalukkal együtt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type:</source>
        <translation>Típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Margin Guides</source>
        <translation>Margók</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Alsó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Top:</source>
        <translation>&amp;Felső:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Right:</source>
        <translation>&amp;Jobb:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Left:</source>
        <translation>&amp;Bal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Felső margó és az oldal széle közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Alsó margó és az oldal széle közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Bal margó és az oldal széle közötti távolság.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Inside:</source>
        <translation>&amp;Belül:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>O&amp;utside:</source>
        <translation>&amp;Kívül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preset Layouts:</source>
        <translation>Előre beállított rétegek:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply margin settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printer Margins...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Master Pages</source>
        <translation>Mester oldal szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Duplicates the selected master page</source>
        <translation type="obsolete">A kiválasztott mester oldal megkettőzése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Deletes the selected master page</source>
        <translation type="obsolete">Kiválasztott mester oldal törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Adds a new master page</source>
        <translation type="obsolete">Új mester oldal hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports master pages from another document</source>
        <translation type="obsolete">Mester oldal importálása más dokumentumból</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to delete this master page?</source>
        <translation>Tényleg törölni akarja ezt a mester oldalt?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;No</source>
        <translation>&amp;Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Yes</source>
        <translation>&amp;Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name:</source>
        <translation>&amp;Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Master Page</source>
        <translation>Új mester oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy of %1</source>
        <translation>%1 másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name:</source>
        <translation>Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New MasterPage</source>
        <translation>Új mester oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy #%1 of </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Duplicate the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unable to Rename Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rename Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Name:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Multiple Duplicate</source>
        <translation>Többszörös duplikálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Másolatok száma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vízszintes eltolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Függőleges eltolás:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Distances</source>
        <translation>Távolságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X1:</source>
        <translation>X1:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y1:</source>
        <translation>Y1:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X2:</source>
        <translation>X2:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y2:</source>
        <translation>Y2:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>DX:</source>
        <translation>DX:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>DY:</source>
        <translation>DY:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Angle:</source>
        <translation>Szög:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Length:</source>
        <translation>Hosszúság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Page(s)</source>
        <translation>Oldal(ak) importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> from 0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create Page(s)</source>
        <translation>Oldal(ak) létrehozása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> from %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;From Document:</source>
        <translation>&amp;Dokumentumból:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Oldal(ak) importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Before Page</source>
        <translation>Oldal elé</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>After Page</source>
        <translation>Oldal után</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>At End</source>
        <translation>Végén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation>&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Master Page</source>
        <translation>Mester oldal importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Mester oldal importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Missing Font</source>
        <translation>Hiányzó betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The Font %1 is not installed.</source>
        <translation>A(z) %1 betűtípus nincs telepítve.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use</source>
        <translation>Használd</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>instead</source>
        <translation>-t helyette</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Pages</source>
        <translation>Oldalak mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy Page</source>
        <translation>Oldal másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Page(s):</source>
        <translation>Oldal(ak) mozgatása:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>to:</source>
        <translation type="obsolete">oldalig:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Page(s)</source>
        <translation>Oldal(ak) mozagtása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Before Page</source>
        <translation>Oldal elé</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>After Page</source>
        <translation>Oldal után</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>At End</source>
        <translation>Végén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of copies:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Properties</source>
        <translation>Tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shape</source>
        <translation>&amp;Alak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Text</source>
        <translation>&amp;Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Image</source>
        <translation>&amp;Kép</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Line</source>
        <translation>&amp;Vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Colors</source>
        <translation>&amp;Színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Geometry</source>
        <translation>Elhelyezkedés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Width:</source>
        <translation>&amp;Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Height:</source>
        <translation>&amp;Magasság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Rotation:</source>
        <translation>&amp;Forgatás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Basepoint:</source>
        <translation>Bázispont:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Level</source>
        <translation>Szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shape:</source>
        <translation>Alak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Alak szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>R&amp;ound
Corners:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance of Text</source>
        <translation>Szöveg távolsága</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Oszlopok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Gap:</source>
        <translation>&amp;Hézag:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To&amp;p:</source>
        <translation>&amp;Felső:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Alsó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Left:</source>
        <translation>&amp;Bal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Right:</source>
        <translation>&amp;Jobb:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulátorok...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Path Text Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Curve</source>
        <translation>Görbe mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Start Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance from Curve:</source>
        <translation>Távolság a görbétől:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text &amp;Flows Around Frame</source>
        <translation>&amp;Szöveg a keretet körbeveszi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use &amp;Bounding Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Use Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>St&amp;yle:</source>
        <translation>&amp;Stílus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Nyelv</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Szabad skálázású</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X-Sc&amp;ale:</source>
        <translation>X-&amp;Skála:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-&amp;Skála:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scale &amp;To Frame Size</source>
        <translation>&amp;Méretezés a keret méretéhez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>P&amp;roportional</source>
        <translation>&amp;Arányos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Input Profile:</source>
        <translation>Bemeneti profil:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering Intent:</source>
        <translation>Megjelenítési prioritások:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation</source>
        <translation>Telítettség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Absolute Colorimetric</source>
        <translation>Abszolút kolorimetriás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left Point</source>
        <translation>Bal pont</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>End Points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Bázispont:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Vonal típusa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line &amp;Width:</source>
        <translation>&amp;Vonalvastagság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bevel Join</source>
        <translation>Ferde átlapolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Metszés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flat Cap</source>
        <translation>14*17\&quot; papíralak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Square Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Endings:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Style</source>
        <translation>Nincs stílus megadva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cell Lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line at Top</source>
        <translation>Felső vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line at the Left</source>
        <translation>Bal oldali vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line at the Right </source>
        <translation>Jobb oldali vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line at Bottom</source>
        <translation>Alsó vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of selected object</source>
        <translation>Kijelölt objektum neve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal position of current basepoint</source>
        <translation>Jelenlegi alappont vízszintes pozíciója</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical position of current basepoint</source>
        <translation>Jelenlegi alappont függőleges pozíciója</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Width</source>
        <translation>Szélesség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Height</source>
        <translation>Magasság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rotation of object at current basepoint</source>
        <translation>Objektum jelenlegi alappont körüli forgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select top left for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select top right for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select bottom left for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select bottom right for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select center for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flip Horizontal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flip Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move one level up</source>
        <translation>Egy szinttel előrébb mozgat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move one level down</source>
        <translation>Egy szinttel hátrébb mozgat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move to front</source>
        <translation>Előre mozgat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move to back</source>
        <translation>Hátra mozgat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lock or unlock the object</source>
        <translation>Objektum zárolása vagy zárolás feloldása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lock or unlock the size of the object</source>
        <translation>Objektum méretének zárolása vagy zárolás feloldása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable or disable printing of the object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font of selected text or object</source>
        <translation>A kijelölt szöveg vagy objektum betűtípusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Size</source>
        <translation>Betűméret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scaling width of characters</source>
        <translation type="unfinished">Karakter szélesség skálázása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of text fill</source>
        <translation type="obsolete">Szöveg kitöltő színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color of text fill</source>
        <translation>Szöveg kitöltő szín telítettsége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Spacing</source>
        <translation>Vonal távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Style of current paragraph</source>
        <translation>Jelenlegi bekezdés stílusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hyphenation language of frame</source>
        <translation>Keret elválasztási nyelve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change settings for left or end points</source>
        <translation>Bal vagy végpontok beállításainak megváltoztatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Pattern of line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Thickness of line</source>
        <translation>Vonal vékonyság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type of line joins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type of line end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line style of current object</source>
        <translation>Jelenlegi objektum vonal stílusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose the shape of frame...</source>
        <translation>Válassza ki a keret alakját...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit shape of the frame...</source>
        <translation>Keret alakjának szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set radius of corner rounding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of columns in text frame</source>
        <translation>Oszlopok száma egy szöveg keretben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Switches between Gap or Column width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between columns</source>
        <translation>Oszlopok közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance of text from top of frame</source>
        <translation>Szöveg távolsága a keret tetejétől</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance of text from bottom of frame</source>
        <translation>Szöveg távolsága a keret aljától</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance of text from left of frame</source>
        <translation>Szöveg távolsága a keret bal szélétől</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance of text from right of frame</source>
        <translation>Szöveg távolsága a keret jobb szélétől</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit tab settings of text frame...</source>
        <translation>Szöveg keret tabulátor beálíítások szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Kép különböző méretű lehet mint a keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal offset of image within frame</source>
        <translation>Kép vízszintes eltolása a kereten belül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical offset of image within frame</source>
        <translation>A kép kereten belüli függőleges eltolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resize the image horizontally</source>
        <translation>Kép átnéretezése vízszintesen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resize the image vertically</source>
        <translation>Kép átméretezése függőlegesen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keep the X and Y scaling the same</source>
        <translation>X és Y skálázás azonos maradjon</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keep the aspect ratio</source>
        <translation>Oldalarány megtartása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make the image fit within the size of the frame</source>
        <translation>A képet a keret méretéhez illessze</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use image proportions rather than those of the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Source profile of the image</source>
        <translation>Kép forrás profilja</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering intent for the image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;X1:</source>
        <translation>&amp;X1:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X&amp;2:</source>
        <translation>X&amp;2:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y&amp;1:</source>
        <translation>Y&amp;1:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Y2:</source>
        <translation>&amp;Y2:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Column width</source>
        <translation>Oszlop szélesség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">A &quot;%1&quot; fájl már létezik.
Válasszon másikat.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right to Left Writing</source>
        <translation>Jobbról balra írás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Start Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>End Arrow:</source>
        <translation>Vége nyíl:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fixed Linespacing</source>
        <translation>Rögzített sortávolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatic Linespacing</source>
        <translation>Automatikus vonaltávolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align to Baseline Grid</source>
        <translation>Igazítás alaprácshoz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Actual X-DPI:</source>
        <translation>Érvényes X-DPI:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Actual Y-DPI:</source>
        <translation>Érvényes Y-DPI:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Offset to baseline of characters</source>
        <translation>Karakterek alapvonalának eltolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scaling height of characters</source>
        <translation>Karakterek magasságának skálázása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Manual Tracking</source>
        <translation>Kézi követés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill Rule</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Even-Odd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Non Zero</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Style</source>
        <translation>Stílus szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flat Cap</source>
        <translation type="unfinished">14*17\&quot; papíralak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Square Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bevel Join</source>
        <translation>Ferde átlapolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Width:</source>
        <translation>Vonalvastagság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt </source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Solid Line</source>
        <translation>Folytonos vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dashed Line</source>
        <translation>Szaggatott vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dotted Line</source>
        <translation>Pontozott vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dash Dot Line</source>
        <translation>Tá-ti vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dash Dot Dot Line</source>
        <translation>Tá-ti-ti vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">A &quot;%1&quot; fájl már létezik.
Válasszon másikat.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Progress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Overall Progress:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Mégsem</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>My &amp;Plugin</source>
        <translation>&amp;Beépülő moduljaim</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Beépülő moduljaim</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The plugin worked!</source>
        <translation>Beépülő modul működik!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <location filename="" line="7471221"/>
        <source>New Document</source>
        <translation>Új dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation>&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portrait</source>
        <translation>Álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Landscape</source>
        <translation>Fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Width:</source>
        <translation>&amp;Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Height:</source>
        <translation>&amp;Magasság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Margin Guides</source>
        <translation>Margók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">&amp;Első oldal száma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Default Unit:</source>
        <translation>Alapértelmezett egység:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Automatikus szöveg keretek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Gap:</source>
        <translation>&amp;Hézag:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Oszlopok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Dokument oldal méret, vagy standard vagy egyedi méretű</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Dokumentum oldalak orientációja</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentum oldal szélessége, ha egyedi oldal méretet választ, szerkeszthető</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentum oldal magassága, ha egyedi oldal méretet választ, szerkeszthető</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>First page number of the document</source>
        <translation type="obsolete">Dolumentum első oldal száma</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default unit of measurement for document editing</source>
        <translation>Dokumentum szerkesztésének alapértelmezett mértékegysége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Szöveg keretek automatikus létrehozása új oldal hozzáadásakor</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Oszlopok száma egy automatikusan létrehozott szöveg keretben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between automatically created columns</source>
        <translation>Automatikusan létrehozott oszlopok közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open Document</source>
        <translation type="obsolete">Dokumentum megnyitása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Recent Documents</source>
        <translation type="obsolete">Jelenlegi dokumentumok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not show this dialog again</source>
        <translation>Ne mutasd ezt az ablakot ismét</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initial number of pages of the document</source>
        <translation>Dokumentum kezdeti oldalszáma</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;Oldalak száma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumentumok (*.sla *.scd);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*)</source>
        <translation type="obsolete">Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open &amp;Existing Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open Recent &amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation type="unfinished">Egyedi</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>New &amp;from Template...</source>
        <translation>&amp;Új sablonból...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Load documents with predefined layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Nodes</source>
        <translation>Csomópontok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Abszolút koordináták</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit &amp;Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Reset Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;End Editing</source>
        <translation>&amp;Szerkesztés vége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Nodes</source>
        <translation>Csomópontok mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Control Points</source>
        <translation>Kontrollpontok mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add Nodes</source>
        <translation>Csomópontok hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete Nodes</source>
        <translation>Csomópontok törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Control Points Independently</source>
        <translation>Vezérlő pontok mozgatása egymástól függetlenül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Control Points Symmetrical</source>
        <translation>Vezérlő pontok mozgatása szimmetrikusan</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset Control Points</source>
        <translation>Kontrollpontok mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset this Control Point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close this Bezier Curve</source>
        <translation>Bezier görbe bezárása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror the Path Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror the Path Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Órajárás irányának ellenkezőleg forgatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rotate the Path Clockwise</source>
        <translation>Órajárás irányába forgatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Angle of Rotation</source>
        <translation>Forgatási szög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Activate Contour Line Editing Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Ha kijelöli, a koordináták az oldalhoz viszonyítva értendők, egyébként az objektumhoz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shrink the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>% to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <location filename="" line="7471221"/>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>&amp;OpenOffice.org Rajz importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports OpenOffice Draw Files</source>
        <translation type="obsolete">OpenOffice Rajz fájl importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports most OpenOffice Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="obsolete">A legtöbb OpenOffice Rajz fájl importálása a jelenlegi dokumentumba, vektor adatok konvertálása a Scribus objektumokba.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenDocument Importer Options</source>
        <translation>OpenDocument importáló beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Overwrite Paragraph Styles</source>
        <translation>Bekezdés stílusainak felülírása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Engedélyezve felülírja a jelenlegi Scribus dokumentumban lévő stílusokat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Merge Paragraph Styles</source>
        <translation>Bekezdés stílusok összefésülése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Bekezdés stílusok összefűzése jellemzők szerint. Ez kevesebb hasonló bekezdés stílust eredményez, de megtartja a stílus jellemzőket, még akkor is, ha az eredeti dokumentum stílusai különböző nevűek.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Használja a dokumentum nevét a bekezdés stílus előtagjaként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not ask again</source>
        <translation>Ne kérdezd ismét</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>A beállítások legyenek alapértlemezettek, többé ne kérdezzen, mikor OASIS OpenDocument -et importálok.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation type="unfinished">Mégsem</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Document</source>
        <translation type="unfinished">Scribus dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Linewidth</source>
        <translation>Vonalvastagság</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as PDF</source>
        <translation type="unfinished">Mentés PDF-ként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>O&amp;utput to File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation type="unfinished">Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished">PDF fájlok (*.pdf);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation type="obsolete">Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF fájlok (*.pdf);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as PDF</source>
        <translation type="obsolete">Mentés PDF-ként</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Saving PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exporting Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exporting Items on Current Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Print Preview</source>
        <translation>Nyomtatási nézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display Trans&amp;parency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Display CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;C</source>
        <translation>&amp;C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;M</source>
        <translation>&amp;M</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Y</source>
        <translation>&amp;Y</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;K</source>
        <translation>&amp;K</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All</source>
        <translation>Minden</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cyan</source>
        <translation>Kékeszöld</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Magenta</source>
        <translation>Bíborvörös</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yellow</source>
        <translation>Sárga</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Black</source>
        <translation>Fekete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scaling:</source>
        <translation>Méretezés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print...</source>
        <translation>Nyomtatás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resize the scale of the page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close</source>
        <translation type="unfinished">Bezárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation type="unfinished">Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable &amp;Antialiasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Processing Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Image</source>
        <translation>Kép</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text</source>
        <translation>Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line</source>
        <translation>Vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polygon</source>
        <translation>Sokszög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polyline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PathText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy of</source>
        <translation>Másolása</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relates To</source>
        <translation>Összefüggésben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Item Attributes</source>
        <translation type="obsolete">oldal elem jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation type="obsolete">Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation type="obsolete">Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value</source>
        <translation type="obsolete">Érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Parameter</source>
        <translation type="obsolete">Paraméter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relationship</source>
        <translation type="obsolete">Összefüggés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Item Attributes</source>
        <translation type="unfinished">oldal elem jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation type="unfinished">Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation type="unfinished">Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value</source>
        <translation type="unfinished">Érték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Parameter</source>
        <translation type="unfinished">Paraméter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relationship</source>
        <translation type="unfinished">Összefüggés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Mégsem</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Layout</source>
        <translation type="obsolete">Oldal elrendezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>First Page is:</source>
        <translation>Első oldal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Double sided</source>
        <translation type="unfinished">Kétoldalas</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle Right</source>
        <translation type="unfinished">Középső jobb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished">Dobja az oldalakat, vagy mester oldalakat a szemetesbe a törlésükhöz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished">Itt található az összes mester oldal. Új oldal létrehozásához dobja a mester oldalt az oldal nézetre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation type="unfinished">Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arrange Pages</source>
        <translation type="unfinished">Oldalak elrendezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Master Pages:</source>
        <translation type="unfinished">Elérhető mester oldalak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Pages:</source>
        <translation type="unfinished">Oldalak:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 of %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 of %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Quarto</source>
        <translation>Kvartó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Foolscap</source>
        <translation>Írópapír (43*34 cm)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Government Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Legal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Executive</source>
        <translation>Végrehajtó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Post</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Crown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Large Post</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Medium</source>
        <translation>Közepes</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Royal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Elephant</source>
        <translation>Elefánt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Demy</source>
        <translation>Kettős papíralak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quad Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>STMT</source>
        <translation>STMT</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A</source>
        <translation>A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>B</source>
        <translation>B</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D</source>
        <translation>D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>E</source>
        <translation>E</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Result</source>
        <translation>Eredmény</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search Results for: </source>
        <translation>A keresés eredménye: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview</source>
        <translation>Előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select</source>
        <translation>Kijelölés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Path</source>
        <translation>Útvonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page</source>
        <translation>Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print</source>
        <translation>Nyomtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Status</source>
        <translation>Státusz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Goto</source>
        <translation>Ugrás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation>Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Missing</source>
        <translation>Hiányzik</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search</source>
        <translation>Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel Search</source>
        <translation>Keresés leállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Manage Pictures</source>
        <translation>Képek kezelése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus - Image Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The search failed: %1</source>
        <translation>A keresés hibát jelzett: %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>\&quot;%1\&quot; nevű kép nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select a base directory for search</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as &amp;Image...</source>
        <translation>&amp;Mentés képként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export As Image</source>
        <translation>Exportálás képként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exports selected pages as bitmap images.</source>
        <translation>A kijelölt oldal exportálása bitmap képként.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Bedolgozó modul nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>ismeretlen hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Nem található szimbólum (%1)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Beépülő modul: %1 betöltése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>Hiba történt az inicializáláskor</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>ismeretlen beépülő modul típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Beépülő modul: %1 betöltve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Beépülő modul: %1 betöltési hiba: %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVGH képek (*.svg *.svgz);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG képek (*.svg);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Rajz (*.sxd);;</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugin Manager</source>
        <translation>Beépülő modul kezelő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugin</source>
        <translation>Beépülő modul</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>How to run</source>
        <translation>Hogyan futtassa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation>Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Load it?</source>
        <translation>Betöltsem?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugin ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation>Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation>Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No</source>
        <translation>Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Polygon Properties</source>
        <translation>Sokszög tulajdonságai</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Rotation:</source>
        <translation>&amp;Forgatás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of corners for polygons</source>
        <translation>Sokszögek szögeinek száma</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample Polygon</source>
        <translation>Minta sokszög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Preferences</source>
        <translation>Alapbeállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>General</source>
        <translation>Általános</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document</source>
        <translation>Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Guides</source>
        <translation>Segédvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Typography</source>
        <translation>Tipográfia</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tools</source>
        <translation>Eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scrapbook</source>
        <translation type="obsolete">Gyűjtőalbum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display</source>
        <translation>Kijelző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>GUI</source>
        <translation>Grafikus felhasználói felület</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Theme:</source>
        <translation>&amp;Téma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">Betűméret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Görgetés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Jelenlegi dokumentumok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paths</source>
        <translation>Útvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumentumok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Change...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profilok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Scripts:</source>
        <translation>&amp;Szkriptek:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ch&amp;ange...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation>&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portrait</source>
        <translation>Álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Landscape</source>
        <translation>Fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Width:</source>
        <translation>&amp;Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Height:</source>
        <translation>&amp;Magasság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Margin Guides</source>
        <translation>Margók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Alsó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Top:</source>
        <translation>&amp;Felső:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Right:</source>
        <translation>&amp;Jobb:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Left:</source>
        <translation>&amp;Bal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Autosave</source>
        <translation>Automatikus mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Enabled</source>
        <translation type="obsolete">&amp;Engedélyezve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>min</source>
        <translation>perc</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Interval:</source>
        <translation>&amp;Időzítés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Other Options</source>
        <translation type="obsolete">Egyéb opciók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview</source>
        <translation type="obsolete">Előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Small</source>
        <translation type="obsolete">Kicsi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Medium</source>
        <translation type="obsolete">Közepes</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Large</source>
        <translation type="obsolete">Nagy</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Megjelenítés méretének módosítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Futtató neve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image Processing Tool</source>
        <translation>Kép feldolgozó eszköz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name of &amp;Executable:</source>
        <translation>&amp;Futtató neve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing</source>
        <translation type="obsolete">Nyomtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished">Dokumentum szerkesztésének alapértelmezett mértékegysége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Az egér görgő mozgatásakor a görgetett sorok száma</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>A fájl menüben mutatott legutóbbi dokumentumok száma</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default documents directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default orientation of document pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Time period between saving automatically</source>
        <translation>Automatikus mentések közötti időtartam</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color for paper</source>
        <translation>Lap színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set the default zoom level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="obsolete">Ghostscript értelemző meghatározása a fájl rendszerben.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a Directory</source>
        <translation>Könyvtár kiválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Language:</source>
        <translation>&amp;Nyelv</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Units:</source>
        <translation>Mértékegység:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Undo/Redo</source>
        <translation>Visszavonás/Újra végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Action history length</source>
        <translation>Művelet előzmény hossza</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hyphenator</source>
        <translation>Elválasztó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts</source>
        <translation>Betűtípusok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Management</source>
        <translation>Szín kezelés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Export</source>
        <translation>PDF exportálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keyboard Shortcuts</source>
        <translation>Gyors billentyűk</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Display</source>
        <translation>Oldal megjelenítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color:</source>
        <translation>Szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Pictures</source>
        <translation>Képek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Text Chains</source>
        <translation>Szöveg láncok mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Frames</source>
        <translation>Keretek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Plugins</source>
        <translation>Beépülő modulok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the display of frames on or off</source>
        <translation>Keretek megjelenítésének be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the display of pictures on or off</source>
        <translation>Képek megjelenítésének be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document T&amp;emplates:</source>
        <translation>&amp;Dokumentum sablon:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Item Attributes</source>
        <translation>Dokumentum elem jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Table of Contents and Indexes</source>
        <translation>Tartalomjegyzék és indexek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Text Control Characters</source>
        <translation>Szöveg vezérlő karakterek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Gaps between Pages</source>
        <translation>Oldalak közötti hézag</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal:</source>
        <translation>Vízszintes:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical:</source>
        <translation>Függőleges:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resolution:</source>
        <translation>Felbontás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>External Tools</source>
        <translation>Külső eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Startup Dialog</source>
        <translation>Indítási dialógus mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Miscellaneous</source>
        <translation>Különböző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Nem nyomtatandó karakterek, mint bekezdés jelölés, mutatása a szöveg keretben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Additional directory for document templates</source>
        <translation>További könyvtárak a dokumentum sablonhoz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="obsolete">Grafikus szerkesztő meghatározása a fájl rendszerben. Ha gimp-et használ és a disztribúció tartalmazza, javasoljuk a &apos;gimp-remote&apos; használatát, mely megengedi, hogy egy már futó gimp példányban szerkessze a képet.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Locate Ghostscript</source>
        <translation>Ghostscript helye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Locate your image editor</source>
        <translation>Kép szerkesztő kijelölése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript Interpreter</source>
        <translation>PostScript értelmező</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Kapcsolot ekretek megjelenítésének engedélyezése vagy tiltása.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Scribus alapértelmezett nyelvének kiválasztása. Hagyja üresen, ha a rendszer változót akarja használni. Ezt felülírhatja a Scribus indításakor a parancssori kapcsoló megadásával.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Defaults</source>
        <translation>&amp;Alapértékek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Page</source>
        <translation type="obsolete">Egyoldalas</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double sided</source>
        <translation type="obsolete">Kétoldalas</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left Page</source>
        <translation type="obsolete">Bal oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right Page</source>
        <translation type="obsolete">Jobb oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>3-Fold</source>
        <translation type="obsolete">3 lapos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle</source>
        <translation type="obsolete">Középső</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>4-Fold</source>
        <translation type="obsolete">4 lapos</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle Left</source>
        <translation type="obsolete">Középső bal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Middle Right</source>
        <translation type="obsolete">Középső jobb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Migrálja a régi Scribus beállításokat?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus 1.2 verziójú beállítási fájlt talált. Akarja migrálni az új Scribus verzióba?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Nem lehet a \&quot;%1\&quot; beállítási fájlt megnyitni írásra: %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>\&quot;%1\&quot; beállítási fájl írásakor hiba történt: QIODevice státusz kód %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Hiba történt a \&quot;%1\&quot; beállítás fájl megnyitásakor: %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Hiba történt a \&quot;%1\&quot; -ből az XML beállítások olvasásakor: a %3 sornál, %4 oszlopnál %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF 1.3</source>
        <translation>PDF 1.3</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF 1.4</source>
        <translation>PDF 1.4</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF/X-3</source>
        <translation>PDF/X-3</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error Writing Preferences</source>
        <translation>Hiba a beállítások mentésénél</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus nem tudta mentesi a beállításait: &lt;br&gt;%1&lt;br&gt; Kérem ellenőrizze a fájl és konyvtár engedélyeket, valamint a szabad lemez területet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error Loading Preferences</source>
        <translation>Hiba a beállítások betöltésénél</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus nem tudja betölteni a beállításokat:&lt;br&gt;%1&lt;br&gt; Az alapértelmezett beálításokat tölti be.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Book</source>
        <translation type="obsolete">Könyv</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Magazine</source>
        <translation>Magazin</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Gutenberg</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fibonacci</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Golden Mean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Nine Parts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <comment>layout type</comment>
        <translation type="unfinished">Nincs</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Open...</source>
        <translation>&amp;Megnyitás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation>&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;As...</source>
        <translation>&amp;Ment másként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Exit</source>
        <translation>&amp;Kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation>&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Run</source>
        <translation>&amp;Futtatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Run As &amp;Console</source>
        <translation>&amp;Futtatás konzolként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save Output...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Script</source>
        <translation>&amp;Szkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Python Console</source>
        <translation>Scribus Python konzol</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Script Console</source>
        <translation>Szkript konzol</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Írja be a parancsokat. A kijelölés szkriptként hajtódik végre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output of your script</source>
        <translation>Szkript kimenete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Python Scripts (*.py)</source>
        <translation>Python szkript (*.py)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open File With Python Commands</source>
        <translation type="obsolete">Fájl megnyitása Python parancsokkal együtt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save the Python Commands in File</source>
        <translation>Python parancs fájlba mentése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation>Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Files (*.txt)</source>
        <translation>Szöveg fájlok (*.txt)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open Python Script File</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Hu&amp;e:</source>
        <translation>&amp;Színárnyalat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Sat:</source>
        <translation>&amp;Telítettség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Val:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Red:</source>
        <translation>&amp;Vörös:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Green:</source>
        <translation>&amp;Zöld:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Kék:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A&amp;lpha channel:</source>
        <translation>&amp;Alfa csatorna:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Basic colors</source>
        <translation>Alapszínek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Custom colors</source>
        <translation>Saját színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>Saját színek definiálása &gt;&gt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add to Custom Colors</source>
        <translation>Egyedi színek hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select color</source>
        <translation>Szín kiválasztása</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy or Move a File</source>
        <translation>Fájl másolása vagy mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Read: %1</source>
        <translation>%1 olvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Write: %1</source>
        <translation>Írás: %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File &amp;name:</source>
        <translation>Fájl&amp;név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File &amp;type:</source>
        <translation>Fájl&amp;típus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>One directory up</source>
        <translation>Egy könyvtárral feljebb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*)</source>
        <translation>Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size</source>
        <translation>Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type</source>
        <translation>Típus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Date</source>
        <translation>Dátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Attributes</source>
        <translation>Jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Look &amp;in:</source>
        <translation>Megtek&amp;intés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Back</source>
        <translation>Vissza</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create New Folder</source>
        <translation>Könyvtár létrehozása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List View</source>
        <translation>Listanézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Detail View</source>
        <translation>Részletes nézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview File Info</source>
        <translation>Fálj információ mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview File Contents</source>
        <translation>Fájl tartalmának mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Read-write</source>
        <translation>Olvasható - írható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Read-only</source>
        <translation>Csak olvasható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Write-only</source>
        <translation>Csak írható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Inaccessible</source>
        <translation>Elérhetetlen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Symlink to File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Symlink to Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Symlink to Special</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation>Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dir</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Special</source>
        <translation>Speciális</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save As</source>
        <translation>Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Open</source>
        <translation>&amp;Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation>&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Rename</source>
        <translation>&amp;Átnevezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>R&amp;eload</source>
        <translation>&amp;Újratöltés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sort by &amp;Name</source>
        <translation>Rendezés &amp;név szerint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sort by &amp;Size</source>
        <translation>Rendezés &amp;méret szerint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sort by &amp;Date</source>
        <translation>Rendezés &amp;dátum szerint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Unsorted</source>
        <translation>&amp;Nem rendezett</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sort</source>
        <translation>Rendezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;hidden files</source>
        <translation>Rejtett fájlok mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>the file</source>
        <translation>a fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>the directory</source>
        <translation>a könyvtár</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>the symlink</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete %1</source>
        <translation>%1 törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Biztos törölni akarja %1 \&quot;%2\&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Yes</source>
        <translation>&amp;Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;No</source>
        <translation>&amp;Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Folder 1</source>
        <translation>Új könyvtár %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Folder</source>
        <translation>Új könyvtár</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Folder %1</source>
        <translation>Új könyvtár %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Find Directory</source>
        <translation>Könyvtár keresése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Directories</source>
        <translation>Könyvtárak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save</source>
        <translation>Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error</source>
        <translation>hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1 fájl nem található. Ellenőrizze az elérési útvonalat és a fájl nevet.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*.*)</source>
        <translation>Minden fájl (*.*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select a Directory</source>
        <translation>Válasszon ki egy könyvtárt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Directory:</source>
        <translation>Könyvtár:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font</source>
        <translation>Betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font st&amp;yle</source>
        <translation>Betűstílus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size</source>
        <translation>&amp;Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Effects</source>
        <translation>Hatások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Stri&amp;keout</source>
        <translation>Áthúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Underline</source>
        <translation>Aláhúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Color</source>
        <translation>&amp;Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample</source>
        <translation>Minta</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scr&amp;ipt</source>
        <translation>Szkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply</source>
        <translation>Alkalmazás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close</source>
        <translation>Bezárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Font</source>
        <translation>Betűtípus választása</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Clear</source>
        <translation>Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select All</source>
        <translation>Minden kijelölése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Undo</source>
        <translation>&amp;Visszavonás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Redo</source>
        <translation>&amp;Újra végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cu&amp;t</source>
        <translation>&amp;Kivágás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Line up</source>
        <translation>Felgörgetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Customize...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing text</source>
        <translation>Szöveg importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Supported Formats</source>
        <translation>Minden támogatott formátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*)</source>
        <translation>Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing...</source>
        <translation>Inicializálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Background</source>
        <translation>Háttér</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation>Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Tényleg felül akarja írni a(z)
%1 fájlt?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Newsletters</source>
        <translation>Hírlevél</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Brochures</source>
        <translation>Brosúra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Catalogs</source>
        <translation>Katalógus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flyers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Signs</source>
        <translation>Jelek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Letterheads</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Envelopes</source>
        <translation>Boriték</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Business Cards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Calendars</source>
        <translation>Naptár</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Advertisements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Labels</source>
        <translation>Címkék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Menus</source>
        <translation>Menük</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Programs</source>
        <translation>Programok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Forms</source>
        <translation>PDF űrlap</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Presentations</source>
        <translation>PDF bemutatók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Magazines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Posters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Announcements</source>
        <translation>Kommentárok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Documents</source>
        <translation>Szöveg dokumetumok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Folds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Own Templates</source>
        <translation>Sablonjaim</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as Image</source>
        <translation>Mentés képként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error writing the output file(s).</source>
        <translation>Hiba a fájl(ok) írásakor.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export successful.</source>
        <translation>Exportálás sikeresen megtörtént.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File exists. Overwrite?</source>
        <translation>Fájl már létezik. Felülírja?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>exists already. Overwrite?</source>
        <translation>már létezik. Felülírja?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No</source>
        <translation>Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation>Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes all</source>
        <translation>Igen mindenre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Minden támogatott formátum (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation>Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVGH képek (*.svg *.svgz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG képek (*.svg);; Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Comma Separated Value Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CSV_data</source>
        <translation>CSV_adat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CSV_header</source>
        <translation>CSV_fej</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>HTML Files</source>
        <translation>HTML fájlok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>
External Links
</source>
        <translation>Külső kapcsolatok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Files</source>
        <translation>Szöveg fájlok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken, discarding it</source>
        <translation>%1 betűtípus sérült, figyelmen kívül hagyva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Filters</source>
        <translation>Szöveg szűrők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Albanian</source>
        <translation>Albán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Basque</source>
        <translation>Baszk</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bulgarian</source>
        <translation>Bulgár</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Brazilian</source>
        <translation type="obsolete">Brazil</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Catalan</source>
        <translation>Katalán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Chinese</source>
        <translation>Kinai</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Czech</source>
        <translation>Cseh</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Danish</source>
        <translation>Dán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dutch</source>
        <translation>Holland</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>English</source>
        <translation>Angol</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>English (British)</source>
        <translation>Angol (Brit)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Esperanto</source>
        <translation>Eszperanto</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>German</source>
        <translation>Német</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Finnish</source>
        <translation>Befejezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>French</source>
        <translation>Francia</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Greek</source>
        <translation>Görög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hungarian</source>
        <translation>Magyar</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Indonesian</source>
        <translation>Indonézia</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Italian</source>
        <translation>Olasz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Korean</source>
        <translation>Koreai</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lithuanian</source>
        <translation>Litván</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norvég (Bokmaal)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norvég (Nnyorsk)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Norwegian</source>
        <translation>Norvég</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polish</source>
        <translation>Lengyel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Russian</source>
        <translation>Orosz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Swedish</source>
        <translation>Svéd</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spanish</source>
        <translation>Spanyol</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spanish (Latin)</source>
        <translation>Spanyol (latin)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Slovak</source>
        <translation>Szlovák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Slovenian</source>
        <translation>Szlovén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Serbian</source>
        <translation>Szerb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret betűtípus mérete nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret betűtípusa nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret szöveg mérete nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret oszlop száma nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret vonal távolsága nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret oszlop köze nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret szövege nem határozható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret szövege nem adható meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keretbe szöveg nem tölthető be.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Igazítás nem esik a tartományba. Használj az egyik scribus.ALIGN* állandót.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Kijelölt index tartományon kívüli</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>A cél nem kép keret.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Sarok lekerekítés pozitív szám legyen.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Név nélkül nem határozható meg a szín.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Név nélkül nem változtatható meg a szín.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nem hozható létre név nélkül szín.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Név nélkül nem törölhető a szín.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nem cserélhető le a szín egy üres névvel.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org Rajz (*.sxd);; Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Szöveg dokumentumok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Szín nem található - python hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Egyedi (opcionális) konfiguráció: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Sztandard konfiguráció: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Rövid szó feldolgozás. Kérem várjon...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Rövid szó feldolgozás. Kész.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Afrikaans</source>
        <translation>Afrikai</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turkish</source>
        <translation>Török</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ukranian</source>
        <translation>Ukrán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Welsh</source>
        <translation>Welszi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>A fájlnév karakter sorozat legyen.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Nem törölhetők a kép típus beállítások.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>A kép típusa karakter sorozat legyen.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Hiba történt a kép exportálásakor</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Nem skálázható át 0%-kal.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>A kijelölt elem nem kép keret.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Betűtípus nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Üres réteg név nem lehetséges.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Réteg nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Az utolsó réteg nem távolítható el.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Név nélkül nem hozható létre réteg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Betűméret nem megfelelő - 1 és 512 közé kell esnie</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Vonal távolságnak 0,1-nél nagyobbnak kell lennie.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem törölhető szöveg a nem szöveg keretből.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Csak szöveg keretek kapcsolhatók össze.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Cél keretnek üresnek kell lennie.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>A cél keret egy másik kerethez van kapcsolva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>A cél keretet egy másik keret összekapcsolja.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>A forrás és cél ugyanaz az objektum.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nem szöveg keret kapcsolat nem szüntethető meg.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Nem kovertálható a nem szöveg keret körvolnallá.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page</source>
        <translation>Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Development Version</source>
        <translation>Scribus fejlesztői változat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Proceed</source>
        <translation type="obsolete">&amp;Végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> in</source>
        <translation>ban</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>in</source>
        <translation>ban</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>p</source>
        <translation>p</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Points (pt)</source>
        <translation>Pont (pt)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Inches (in)</source>
        <translation>Hüvelyk (in)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Picas (p)</source>
        <translation>Pika (p)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File exists</source>
        <translation>Fájl már létezik</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="obsolete">%1 nevű fájl már létezik. Le akarja cserélni a mentendő fájlra?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Replace</source>
        <translation>&amp;Csere</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All</source>
        <translation>Minden</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Hiba történt a dokumentum megnyitásakor.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Hiba történt a dolumentum mentésekor.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Property not found</source>
        <translation>Tulajdonság nem található</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Child not found</source>
        <translation>Gyermek nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>%1 típusú tulajdonság nem támogatott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Types matched, but setting property failed.</source>
        <translation>Típusok egyeznek, de a tulajdonság beállításakor hiba történt.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objektum nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Stílus nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Hiba történt az EPS mentésekor.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Oldalszám kívül esik a tartományon.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Vonal szélességnek 0 és 12 közé kell esnie.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Vonal árnyékolásnak 0 és 100 közé kell esnie.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Vonal stílus nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Az objektum nem egy kapcsolt szöveg keret, így a kapcsolat nem bontható.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Az objektum az utolsó keret a sorozatban, kapcsolat nem bontható. Ehelyett az előző keret kapcsolatát bontsa.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Csak a szöveg keretek ellenőrizhetők túlcsordulásra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Szkript végrehajtása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show &amp;Console</source>
        <translation>&amp;Konzol mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;About Script...</source>
        <translation>Szkript névjegye...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Script</source>
        <translation>&amp;Szkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus szkriptek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Legutóbbi szkriptek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>About Script</source>
        <translation>Szkript névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Croatian</source>
        <translation>Horvát</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portuguese</source>
        <translation>Portugál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portuguese (BR)</source>
        <translation>Portugál (BR)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Crash</source>
        <translation>Scribus összeomlott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>A Scribus összeomlott a #%1 Szignál miatt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;OK</source>
        <translation>&amp;Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Master Page </source>
        <translation>Mester oldal </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>4A0</source>
        <translation>4A0</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>2A0</source>
        <translation>2A0</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Comm10E</source>
        <translation>Comm10E</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>DLE</source>
        <translation>DLE</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not open output file %1</source>
        <translation>%1 kimeneti fálj nem nyitható meg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output stream not writeable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Verification of settings failed: %1</source>
        <translation>Beállítások ellenőrzése hibát jelzett: %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not open input file %1</source>
        <translation>%1 bemeneti fájl nem nyitható meg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unable to read settings XML:</source>
        <translation>Nem lehet az XML beállításokat olvasni:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unable to read settings XML: %1</source>
        <translation>Nem lehet az XML beállításokat olvasni: %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; érvénytelen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;%1&gt; elem értéke &apos;igaz&apos; vagy &apos;hamis&apos; lehet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Freetype2 library not available</source>
        <translation>Freetype2 könyvtár nem elérhető</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 cannot be read, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Hiba történt a %1 betűtípus betöltésekor - betűtípus ismeretlen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 loaded from %2(%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>%1 betűtípus betöltése (fontconfig szerint)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Hiba történt a betűtípus betöltésekor - freetype2 nem található a betűtípus fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>table &apos;%1&apos;</source>
        <translation>&apos;%1&apos; táblázat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Invalid argument: </source>
        <translation type="obsolete">Érvénytelen jellemző: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File %1 does not exist, aborting.</source>
        <translation type="obsolete">%1 fájl nem létezik, megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options:</source>
        <translation type="obsolete">Beállítások:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print help (this message) and exit</source>
        <translation type="obsolete">Segítség (ezen üzenet) nyomtatása és kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List the currently installed interface languages</source>
        <translation type="obsolete">A jelenleg telepített nyelvek listázása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="obsolete">Mutassa az információkat a konzolon a betűtípusok betöltése alaltt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not show the splashscreen on startup</source>
        <translation type="obsolete">Ne mutasd az indító képet az induláskor</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="obsolete">A Scribus telepített nyelvei a következők:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To override the default language choice:</source>
        <translation type="obsolete">Az alapértlemezett nyelv felülírásához válassza:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Version </source>
        <translation type="obsolete">Scribus verzió</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="obsolete">Scribus, nyílt forráskódú kiadvány szerkesztő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>---------------------------------------</source>
        <translation type="obsolete">--------------------------------------</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="obsolete">Honlap:       http://www.scribus.net</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="obsolete">Dokumentáció:  http://docs.scribus.net</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="obsolete">Wiki:            http://wiki.scribus.net</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="obsolete">Kiadás:           http://bugs.scribus.net</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> cm</source>
        <translation>cm</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> c</source>
        <translation>c</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>cm</source>
        <translation>cm</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>c</source>
        <translation>c</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Millimeters (mm)</source>
        <translation>Milliméter (mm)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Centimeters (cm)</source>
        <translation>Centiméter (cm)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cicero (c)</source>
        <translation>Ciceró (c)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>page</source>
        <comment>page export</comment>
        <translation>oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Template: </source>
        <translation>Dokumentum sablon: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>A szín nem található.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>A szín nem található a dokumentumban.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>A szín nem található az alapértelmezett színek között.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Kettőnél kevesebb elemet nem csoportosíthat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Kettőnél kevesebb elemet nem csoportosíthat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>A fájlnév nem lehet üres karakter sorozat.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
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
        <location filename="" line="7471221"/>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument szöveg dokumentumok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy #%1 of </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Black</source>
        <translation>Fekete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cyan</source>
        <translation>Kékeszöld</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Magenta</source>
        <translation>Bíborvörös</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yellow</source>
        <translation>Sárga</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Wheel</source>
        <translation>Színkerék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Preview</source>
        <translation>Betűtípus előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>My Plugin</source>
        <translation>Beépülő moduljaim</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New From Template</source>
        <translation>Új sablonból</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export As Image</source>
        <translation>Exportálás képként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS importáló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save As Template</source>
        <translation>Mentés sablonként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scripter</source>
        <translation type="unfinished">Szkripter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words</source>
        <translation>Rövid szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG Export</source>
        <translation>SVG exportálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG Import</source>
        <translation>SVG importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Rajz importáló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>German (Trad.)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exporting PostScript File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Creating Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Modified Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Writing updated Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Betűtípusok keresése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;reate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Old .sla format support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Thai</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Barcode Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Word Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Luxembourgish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arabic</source>
        <translation type="unfinished">Arab</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Estonian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Japanese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Icelandic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Breton</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>English (American)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>English (Australian)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>German (Swiss)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Clear</source>
        <translation>Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select All</source>
        <translation>Minden kijelölése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Undo</source>
        <translation>&amp;Visszavonás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Redo</source>
        <translation>&amp;Újra végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cu&amp;t</source>
        <translation>&amp;Kivágás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <location filename="" line="7471221"/>
        <source>System Menu</source>
        <translation>Rendszermenü</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shade</source>
        <translation>Árnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unshade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normalize</source>
        <translation>Normalizálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Minimize</source>
        <translation>Minimalizálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Maximize</source>
        <translation>Maximalizálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close</source>
        <translation>Bezárás</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Restore</source>
        <translation>Hely&amp;reállítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Move</source>
        <translation>&amp;Mozgatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size</source>
        <translation>&amp;Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimalizálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximalizálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Mindig felül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Minimize</source>
        <translation>Minimalizálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Restore Down</source>
        <translation>Legörgetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Close</source>
        <translation>Bezárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sh&amp;ade</source>
        <translation>&amp;Árnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Unshade</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Setup</source>
        <translation>Dokumentumbeállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Margin Guides</source>
        <translation>Margók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Top:</source>
        <translation>&amp;Felső:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Left:</source>
        <translation>&amp;Bal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Alsó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Right:</source>
        <translation>&amp;Jobb:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portrait</source>
        <translation>Álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Landscape</source>
        <translation>Fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">&amp;Első oldal száma:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation>&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientáció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Width:</source>
        <translation>&amp;Szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Height:</source>
        <translation>&amp;Magasság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Unit:</source>
        <translation>&amp;Egység:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Autosave</source>
        <translation>Automatikus mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>min</source>
        <translation>perc</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervallum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document</source>
        <translation>Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Guides</source>
        <translation>Segédvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Display</source>
        <translation>Oldal megjelenítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color:</source>
        <translation>Szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Pictures</source>
        <translation>Képek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Text Chains</source>
        <translation>Szöveg láncok mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Frames</source>
        <translation>Keretek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display</source>
        <translation>Kijelző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Typography</source>
        <translation>Tipográfia</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tools</source>
        <translation>Eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hyphenator</source>
        <translation>Elválasztó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts</source>
        <translation>Betűtípusok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Export</source>
        <translation>PDF exportálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Management</source>
        <translation>Szín kezelés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the display of frames on or off</source>
        <translation>Keretek megjelenítésének be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the display of pictures on or off</source>
        <translation>Képek megjelenítésének be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color for paper</source>
        <translation>Lap színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options</source>
        <translation type="obsolete">Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply size settings to all Pages</source>
        <translation type="obsolete">Méret beállítás alkalmazása az összes oldalra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply margin settings to all Pages</source>
        <translation type="obsolete">Margó beállítás alkalmazása az összes oldalra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Information</source>
        <translation>Dokumentum információ</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Text Control Characters</source>
        <translation>Szöveg vezérlő karakterek mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Gaps between Pages</source>
        <translation>Oldalak közötti hézag</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal:</source>
        <translation>Vízszintes:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical:</source>
        <translation>Függőleges:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Item Attributes</source>
        <translation>Dokumentum elem jellemzők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Table of Contents and Indexes</source>
        <translation>Tartalomjegyzék és indexek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Nem nyomtatandó karakterek, mint bekezdés jelölés, mutatása a szöveg keretben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Adjusting Colors</source>
        <translation>A színek módosítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Összekapcsolt szöveg keret mutatásának engedélyezése, tiltása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply size settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python szkript (*.py);; Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Bővítő szkript futtatása</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Style Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Column 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name:</source>
        <translation type="unfinished">Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>O&amp;K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>A&amp;pply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+P</source>
        <translation type="unfinished">Alt+P</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ca&amp;ncel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Properties</source>
        <translation type="unfinished">Tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lines</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Style Settings</source>
        <translation>Stílus beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Style of current paragraph</source>
        <translation>Jelenlegi bekezdés stílusa</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill Color Settings</source>
        <translation>Kitöltő szín beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of text fill</source>
        <translation>Szöveg kitöltő színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color of text fill</source>
        <translation>Szöveg kitöltő szín telítettsége</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Stroke Color Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Settings</source>
        <translation>Betűtípus beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font of selected text</source>
        <translation>A kijelölt szöveg betűtípusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Size</source>
        <translation>Betűméret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scaling width of characters</source>
        <translation>Karakter szélesség skálázása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scaling height of characters</source>
        <translation>Karakter magasság skálázása</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Character Settings</source>
        <translation>Karakter beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Manual Tracking</source>
        <translation>Kézi követés</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Save Page as &amp;SVG...</source>
        <translation>&amp;Oldal mentése SVG-ként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exports SVG Files</source>
        <translation>SVG fájlok exportálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exports the current page into an SVG file.</source>
        <translation>A jelenlegi oldal exportálása SVG fájlba.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Import &amp;SVG...</source>
        <translation>&amp;SVG importálása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports SVG Files</source>
        <translation>SVG fájl importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>A legtöbb SVG fájl importálása a jelenlegi dokumentumba, vektor adatok konvertálása a Scribus objektumokba.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scalable Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>SVG file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Rövid szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Kiválaszott keretek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Aktív &amp;oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Minden elem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Csak a kijelölt kereteket dolgozta fel.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Csak az aktuális oldalt dolgozta fel.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">A dokumentum minden eleme feldolgozva.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <location filename="" line="7471221"/>
        <source>User settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>System wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Mentés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Alaphelyzetbe állítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save user configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words</source>
        <translation type="unfinished">Rövid szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot write file %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>User settings saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>System wide configuration reloaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot open file %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as &amp;Template...</source>
        <translation>Mentés sablonként...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save a document as a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScActionPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">%1 cél fájl nem található: %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">%1 cél fájl nem nyitható meg: %2</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Állandó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Exportálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Művelet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Load/Save/Import/Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unknown</source>
        <translation type="unfinished">Ismeretlen</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right</source>
        <translation type="unfinished">Jobbra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left</source>
        <translation type="unfinished">Bal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow Docking To...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal</source>
        <translation type="unfinished">Vízszintes</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical</source>
        <translation type="unfinished">Függőleges</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Floating Orientation...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing...</source>
        <translation type="unfinished">Nyomtatás...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy #%1 of </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Background</source>
        <translation>Háttér</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation type="obsolete">Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation type="obsolete">Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Betűtípusok keresése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fatal Error</source>
        <translation type="obsolete">Végzetes hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading Preferences</source>
        <translation type="obsolete">Alapbeállítások beolvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">Gyorsgombok beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Vágólap olvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Beépülő modulok inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">&amp;Legutóbbi megnyitása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Export</source>
        <translation type="obsolete">&amp;Exportálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Alak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Tools</source>
        <translation type="obsolete">&amp;Eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>About Qt</source>
        <translation type="obsolete">A Qt névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>St&amp;yle</source>
        <translation type="obsolete">&amp;Stílus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Item</source>
        <translation type="obsolete">&amp;Elem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Nézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>E&amp;xtras</source>
        <translation type="obsolete">&amp;Extrák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Windows</source>
        <translation type="obsolete">&amp;Ablak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Segítség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X-Pos:</source>
        <translation type="obsolete">X pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ready</source>
        <translation type="obsolete">Kész</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font</source>
        <translation type="obsolete">Betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Hatások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Alignment</source>
        <translation type="obsolete">&amp;Igazítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shade</source>
        <translation type="obsolete">&amp;Árnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size</source>
        <translation type="obsolete">Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shade</source>
        <translation type="obsolete">Árnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation type="obsolete">Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing Pages...</source>
        <translation type="obsolete">Oldalak importálása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Page(s)</source>
        <translation type="obsolete">Oldal(ak) importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create</source>
        <translation type="obsolete">Létrehozás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import</source>
        <translation type="obsolete">Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import done</source>
        <translation type="obsolete">Importálás megtörtént</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Found nothing to import</source>
        <translation type="obsolete">Nincs mit importálni</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Loading...</source>
        <translation type="obsolete">Betöltés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Supported Formats</source>
        <translation type="obsolete">Minden támogatott formátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*)</source>
        <translation type="obsolete">Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation type="obsolete">Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumentumok (*.sla *.sla.gz *.scd *scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saving...</source>
        <translation type="obsolete">Mentés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing...</source>
        <translation type="obsolete">Nyomtatás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document</source>
        <translation type="obsolete">Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing failed!</source>
        <translation type="obsolete">Sikertelen nyomtatás!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Manual</source>
        <translation type="obsolete">Scribus kézikönyv</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Szöveg fájlok (*.txt);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shade:</source>
        <translation type="obsolete">&amp;Árnyalat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All</source>
        <translation type="obsolete">Minden</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>German</source>
        <translation type="obsolete">Német</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polish</source>
        <translation type="obsolete">Lengyel</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>English</source>
        <translation type="obsolete">Angol</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spanish</source>
        <translation type="obsolete">Spanyol</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Italian</source>
        <translation type="obsolete">Olasz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>French</source>
        <translation type="obsolete">Francia</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Russian</source>
        <translation type="obsolete">Orosz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Danish</source>
        <translation type="obsolete">Dán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Slovak</source>
        <translation type="obsolete">Szlovák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hungarian</source>
        <translation type="obsolete">Magyar</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Czech</source>
        <translation type="obsolete">Cseh</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dutch</source>
        <translation type="obsolete">Holland</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portuguese</source>
        <translation type="obsolete">Portugál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Greek</source>
        <translation type="obsolete">Görög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Catalan</source>
        <translation type="obsolete">Katalán</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Finnish</source>
        <translation type="obsolete">Befejezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Irish</source>
        <translation type="obsolete">Ír</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lithuanian</source>
        <translation type="obsolete">Litván</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Swedish</source>
        <translation type="obsolete">Svéd</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Slovenian</source>
        <translation type="obsolete">Szlovén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikai</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font System Initialized</source>
        <translation type="obsolete">Betűtípus rendszer inicializálva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Portuguese (BR)</source>
        <translation type="obsolete">Porgutál (BR)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit</source>
        <translation type="obsolete">Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumentumok (*.sla *.scd);;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> was replaced by: </source>
        <translation type="obsolete">helyettesítve: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>(converted)</source>
        <translation type="obsolete">(konvertált)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The following programs are missing:</source>
        <translation type="obsolete">A következő programok hiányoznak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS fájlok (*.eps);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot write the file: 
%1</source>
        <translation type="obsolete">Fájl nem írható: %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Lock All</source>
        <translation type="obsolete">&amp;Minden rögzítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Unlock All</source>
        <translation type="obsolete">&amp;Minden zárolás feloldása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bulgarian</source>
        <translation type="obsolete">Bulgár</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Information</source>
        <translation type="obsolete">Információ</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="obsolete">Gyors billentyűk inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Story Editor</source>
        <translation type="obsolete">Szövegszerkesztő inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">ICC profilok olvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Hyphenator</source>
        <translation type="obsolete">Elválasztás inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>There are no fonts found on your system.</source>
        <translation type="obsolete">A rendszerében nem található semmilyen betűtípus.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exiting now.</source>
        <translation type="obsolete">Kilépés most.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview Settings</source>
        <translation type="obsolete">Előnézet beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Level</source>
        <translation type="obsolete">Szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">&amp;Küldés a rétegre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;onvert To</source>
        <translation type="obsolete">&amp;Átalakítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>I&amp;nsert</source>
        <translation type="obsolete">&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Character</source>
        <translation type="obsolete">Karakter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quote</source>
        <translation type="obsolete">Idézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Space</source>
        <translation type="obsolete">Távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="obsolete">A dokumentum néhány betűtípusa helyettesítve van:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="obsolete">A dokumentumban használt néhány ICC profil nincs telepítve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save As</source>
        <translation type="obsolete">Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot Cut In-Use Item</source>
        <translation type="obsolete">A használatban lévő elem nem vágható ki.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="obsolete">%1 elem jelenleg a szövegszerkesztőben változtatható. A kivágás megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="obsolete">Ghostscript: EPS képek, illetve nyomtatási kép nem használható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Croatian</source>
        <translation type="obsolete">Horvát</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="obsolete">A kiválasztott szín nem található a dokumentum szín készletében. Kérem adja meg az új szín nevét.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Not Found</source>
        <translation type="obsolete">A szín nem található</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="obsolete">A kiválasztott név már létezik. Kérem adjon meg egy másik nevet az új színnek.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Level</source>
        <translation type="obsolete">&amp;Szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to Layer</source>
        <translation type="obsolete">Küldés a rétegre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Previe&amp;w Settings</source>
        <translation type="obsolete">&amp;Előnézet beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File %1 is not in an acceptable format</source>
        <translation type="obsolete">%1 fájl nem elfogadható formátumú</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus has detected some errors.
Consider using the Pre-flight Checker to correct them</source>
        <translation type="obsolete">Scribus néhány hibát észlelt. A javításukhoz használja az előzetes ellenőrzőt.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Abort</source>
        <translation type="obsolete">&amp;Megszakítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Ignore</source>
        <translation type="obsolete">&amp;Figyelmen kívül hagyás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name:</source>
        <translation type="obsolete">Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Master Page</source>
        <translation type="obsolete">Új mester oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>-Page%1</source>
        <translation type="obsolete">-oldal %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Some objects are locked.</source>
        <translation type="obsolete">Néhány objektum zárolt.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The program %1 is already running!</source>
        <translation type="obsolete">%1 program már fut!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The program %1 is missing!</source>
        <translation type="obsolete">%1 program hiányzik!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Convert Page to Master Page</source>
        <translation type="obsolete">Oldal mester oldallá alakítása</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample</source>
        <translation type="obsolete">Minta</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color</source>
        <translation type="obsolete">Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Colors</source>
        <translation>Dokumentum színei</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <location filename="" line="7471221"/>
        <source>New Layer</source>
        <translation>Új réteg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation>Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document</source>
        <translation>Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Background</source>
        <translation>Háttér</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished">Tényleg törölni akarja a teljes szöveget?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot Delete In-Use Item</source>
        <translation type="unfinished">Nem törölhető a használatban lévő elem.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="unfinished">%1 elem jelenleg a szövegszerkesztőben változtatható. A törlés megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Plugins</source>
        <translation type="unfinished">Beépülő modulok inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished">Gyors billentyűk inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading Preferences</source>
        <translation type="unfinished">Alapbeállítások beolvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Story Editor</source>
        <translation type="unfinished">Szövegszerkesztő inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished">ICC profilok olvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initializing Hyphenator</source>
        <translation type="unfinished">Elválasztás inicializálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reading Scrapbook</source>
        <translation type="unfinished">Vágólap olvasása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Setting up Shortcuts</source>
        <translation type="unfinished">Gyorsgombok beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation type="unfinished">Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit</source>
        <translation type="unfinished">Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Betűtípusok keresése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished">A rendszerében nem található semmilyen betűtípus.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exiting now.</source>
        <translation type="unfinished">Kilépés most.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fatal Error</source>
        <translation type="unfinished">Végzetes hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font System Initialized</source>
        <translation type="unfinished">Betűtípus rendszer inicializálva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open &amp;Recent</source>
        <translation type="unfinished">&amp;Legutóbbi megnyitása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Export</source>
        <translation type="unfinished">&amp;Exportálás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>St&amp;yle</source>
        <translation type="unfinished">&amp;Stílus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Color</source>
        <translation type="unfinished">&amp;Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shade</source>
        <translation type="unfinished">&amp;Árnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Font</source>
        <translation type="unfinished">Betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Effects</source>
        <translation type="unfinished">&amp;Hatások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Item</source>
        <translation type="unfinished">&amp;Elem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview Settings</source>
        <translation type="unfinished">Előnézet beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Level</source>
        <translation type="unfinished">Szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">&amp;Küldés a rétegre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;PDF beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Alak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;onvert To</source>
        <translation type="unfinished">&amp;Átalakítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>I&amp;nsert</source>
        <translation type="unfinished">&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Character</source>
        <translation type="unfinished">Karakter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quote</source>
        <translation type="unfinished">Idézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Space</source>
        <translation type="unfinished">Távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Page</source>
        <translation type="unfinished">&amp;Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;View</source>
        <translation type="unfinished">&amp;Nézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>E&amp;xtras</source>
        <translation type="unfinished">&amp;Extrák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Windows</source>
        <translation type="unfinished">&amp;Ablak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Help</source>
        <translation type="unfinished">&amp;Segítség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Alignment</source>
        <translation type="unfinished">&amp;Igazítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ready</source>
        <translation type="unfinished">Kész</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation type="unfinished">Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Importing Pages...</source>
        <translation type="unfinished">Oldalak importálása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Page(s)</source>
        <translation type="unfinished">Oldal(ak) importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import done</source>
        <translation type="unfinished">Importálás megtörtént</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Found nothing to import</source>
        <translation type="unfinished">Nincs mit importálni</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished">%1 fájl nem elfogadható formátumú</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Loading...</source>
        <translation type="unfinished">Betöltés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation type="unfinished">PostScript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished">A dokumentumban használt néhány ICC profil nincs telepítve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> was replaced by: </source>
        <translation type="unfinished">helyettesítve: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>(converted)</source>
        <translation type="unfinished">(konvertált)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Supported Formats</source>
        <translation type="unfinished">Minden támogatott formátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Files (*)</source>
        <translation type="unfinished">Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished">Fájl nem írható: %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumentumok (*.sla *.sla.gz *.scd *scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save As</source>
        <translation type="unfinished">Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saving...</source>
        <translation type="unfinished">Mentés...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Ignore</source>
        <translation type="unfinished">&amp;Figyelmen kívül hagyás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Abort</source>
        <translation type="unfinished">&amp;Megszakítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing...</source>
        <translation type="unfinished">Nyomtatás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document</source>
        <translation type="unfinished">Dokumentum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printing failed!</source>
        <translation type="unfinished">Sikertelen nyomtatás!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot Cut In-Use Item</source>
        <translation type="unfinished">A használatban lévő elem nem vágható ki.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="unfinished">%1 elem jelenleg a szövegszerkesztőben változtatható. A kivágás megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>About Qt</source>
        <translation type="unfinished">A Qt névjegye</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Manual</source>
        <translation type="unfinished">Scribus kézikönyv</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation type="unfinished">Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished">Szöveg fájlok (*.txt);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation type="unfinished">Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name:</source>
        <translation type="unfinished">Név:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished">Oldal mester oldallá alakítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Master Page</source>
        <translation type="obsolete">Új mester oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size</source>
        <translation type="unfinished">Méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shade:</source>
        <translation type="unfinished">&amp;Árnyalat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shade</source>
        <translation type="unfinished">Árnyalat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Style</source>
        <translation type="unfinished">Nincs stílus megadva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The following programs are missing:</source>
        <translation type="unfinished">A következő programok hiányoznak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished">Ghostscript: EPS képek, illetve nyomtatási kép nem használható</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All</source>
        <translation type="unfinished">Minden</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished">EPS fájlok (*.eps);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>-Page%1</source>
        <translation type="unfinished">-oldal %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Some objects are locked.</source>
        <translation type="unfinished">Néhány objektum zárolt.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Lock All</source>
        <translation type="unfinished">&amp;Minden rögzítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Unlock All</source>
        <translation type="unfinished">&amp;Minden zárolás feloldása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Information</source>
        <translation type="unfinished">Információ</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The program %1 is already running!</source>
        <translation type="unfinished">%1 program már fut!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The program %1 is missing!</source>
        <translation type="unfinished">%1 program hiányzik!</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="unfinished">A kiválasztott szín nem található a dokumentum szín készletében. Kérem adja meg az új szín nevét.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color Not Found</source>
        <translation type="unfinished">A szín nem található</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="unfinished">A kiválasztott név már létezik. Kérem adjon meg egy másik nevet az új színnek.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Level</source>
        <translation type="unfinished">&amp;Szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to Layer</source>
        <translation type="unfinished">Küldés a rétegre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Previe&amp;w Settings</source>
        <translation type="unfinished">&amp;Előnézet beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Tools</source>
        <translation type="unfinished">&amp;Eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X-Pos:</source>
        <translation type="unfinished">X pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y-Pos:</source>
        <translation type="unfinished">Y pozíció:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to replace your existing image?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Contents</source>
        <translation type="unfinished">Tartalom</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Liga&amp;ture</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Invalid argument: </source>
        <translation type="unfinished">Érvénytelen jellemző: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File %1 does not exist, aborting.</source>
        <translation type="unfinished">%1 fájl nem létezik, megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Options:</source>
        <translation type="unfinished">Beállítások:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print help (this message) and exit</source>
        <translation type="unfinished">Segítség (ezen üzenet) nyomtatása és kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List the currently installed interface languages</source>
        <translation type="unfinished">A jelenleg telepített nyelvek listázása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="unfinished">Mutassa az információkat a konzolon a betűtípusok betöltése alaltt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not show the splashscreen on startup</source>
        <translation type="unfinished">Ne mutasd az indító képet az induláskor</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output version information and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>filename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use filename as path for user given preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="unfinished">A Scribus telepített nyelvei a következők:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>To override the default language choice:</source>
        <translation type="unfinished">Az alapértlemezett nyelv felülírásához válassza:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished">Scribus, nyílt forráskódú kiadvány szerkesztő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Homepage</source>
        <translation type="unfinished">Honlap</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Wiki</source>
        <translation type="unfinished">Wiki</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Issues</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display a console window</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Layer</source>
        <translation>Réteg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy Here</source>
        <translation>Másolás ide</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move Here</source>
        <translation>Mozgatás ide</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Picture</source>
        <translation>Kép</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File: </source>
        <translation>Fájl:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Original PPI: </source>
        <translation>Eredeti PPI: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Actual PPI: </source>
        <translation>Érvényes PPI: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Linked Text</source>
        <translation>Kapcsolt szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Frame</source>
        <translation>Szöveg keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text on a Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paragraphs: </source>
        <translation>Bekezdések:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Words: </source>
        <translation>Szavak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Chars: </source>
        <translation>Karakterek: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print: </source>
        <translation>Nyomtatás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enabled</source>
        <translation>Engedélyezve</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Disabled</source>
        <translation>Letiltva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>In&amp;fo</source>
        <translation>&amp;Információ</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to La&amp;yer</source>
        <translation>&amp;Küldés a rétegre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Le&amp;vel</source>
        <translation>&amp;Szint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Átalakízás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preview Settings</source>
        <translation>Előnézet beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Linking Text Frames</source>
        <translation>Kapcsolt szöveg keretek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot Delete In-Use Item</source>
        <translation type="obsolete">Nem törölhető a használatban lévő elem.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="obsolete">%1 elem jelenleg a szövegszerkesztőben változtatható. A törlés megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to clear all your text?</source>
        <translation type="obsolete">Tényleg törölni akarja a teljes szöveget?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cannot Convert In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>%1 elem jelenleg a szövegszerkesztőben változtatható. A konvertálás megszakítva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page %1 to %2</source>
        <translation>%1 -től %2 -ig oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colorspace: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unknown</source>
        <translation type="unfinished">Ismeretlen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>CMYK</source>
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Contents</source>
        <translation type="unfinished">Tartalom</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">&amp;Bezárás mindenképpen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">Be&amp;zárás mindenképpen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document:</source>
        <translation>Dokumentum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>has been changed since the last save.</source>
        <translation>megváltozott a legutóbbi mentés óta.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Mentés most</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Discard</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Embedded Python scripting support.</source>
        <translation>Beépített Python szkript támogatás.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scripter</source>
        <translation>Szkripter</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Script error</source>
        <translation>Szkript hiba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Ez az üzenet a vágólapra is kikerült. Használja a Crtl+V billentyűzet parancsot a hiba nyomkövetőbe való beillesztésre.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>A megadott parancs végrehajtásakor belső hiba lépett fel. A részeletek a stderr-re kerültek.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Examine Script</source>
        <translation>Minta szkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python szkript (*.py)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documentation for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> doesn&apos;t contain any docstring!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Extension Scripts</source>
        <translation type="obsolete">Szkript kiterjesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Startup Script</source>
        <translation type="obsolete">Indítási szkript</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Startup Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change...</source>
        <translation type="unfinished">Módosítás...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Locate Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Page Previews</source>
        <translation>Gyorsnézet mutatása</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Master Page Names</source>
        <translation type="obsolete">Mester oldal nevének mutatása</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Search/Replace</source>
        <translation>Keresés/csere</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search for:</source>
        <translation>Keresés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text</source>
        <translation>Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paragraph Style</source>
        <translation>Bekezdés stílusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font</source>
        <translation>Betűtípus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Size</source>
        <translation>Betűméret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font Effects</source>
        <translation>Betűtípus hatások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill Color</source>
        <translation>Kitöltő szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Stroke Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Stroke Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left</source>
        <translation>Bal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Center</source>
        <translation>Középre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right</source>
        <translation>Jobbra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Block</source>
        <translation>Klisé</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Forced</source>
        <translation>Kikényszerített</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation type="obsolete">Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace with:</source>
        <translation>Helyettesítve:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Whole Word</source>
        <translation>&amp;Teljes szó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Ignore Case</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Search</source>
        <translation>&amp;Keresés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Replace</source>
        <translation>&amp;Csere</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace &amp;All</source>
        <translation>&amp;Minden cseréje</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Close</source>
        <translation>Be&amp;zárás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search finished</source>
        <translation>Keresés befejeződött</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search finished, found %1 matches</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Arrange Pages</source>
        <translation type="obsolete">Oldalak elrendezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Pages:</source>
        <translation type="obsolete">Oldalak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Facing Pages</source>
        <translation type="obsolete">Szembenéző oldalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal</source>
        <translation type="obsolete">Normál</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="obsolete">Dobja az oldalakat, vagy mester oldalakat a szemetesbe a törlésükhöz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Previews all the pages of your document</source>
        <translation type="obsolete">Dokumentum összes oldalának előnézete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="obsolete">Itt található az összes mester oldal. Új oldal létrehozásához dobja a mester oldalt az oldal nézetre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Master Pages:</source>
        <translation type="obsolete">Elérhető mester oldalak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left Page First</source>
        <translation type="obsolete">Bal oldali lap először</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Select Fields</source>
        <translation>Mezők kijelölése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Fields</source>
        <translation>Felhasználóható mezők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Selected Fields</source>
        <translation>Mezők kijelölése</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Other...</source>
        <translation>Egyéb...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shade:</source>
        <translation>&amp;Árnyalat:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shade</source>
        <translation>Árnyalat</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X-Offset</source>
        <translation>X eltolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Y-Offset</source>
        <translation>Y eltolás</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Rövid szavak...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words</source>
        <translation>Rövid szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <location filename="" line="7471221"/>
        <source>No Style</source>
        <translation>Nincs stílus megadva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Styles...</source>
        <translation>Stílusok szerkesztése...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>No Style</source>
        <translation>Nincs stílus megadva</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit Styles</source>
        <translation>Stílusok szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation>&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Kettőzés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy of %1</source>
        <translation>%1 másolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>New Style</source>
        <translation>Új stílus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No</source>
        <translation type="obsolete">Nem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Yes</source>
        <translation type="obsolete">Igen</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumentumok (*.sla *.sla.gz *.scd *.scd.gz);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentumok (*.sla *.scd);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Import</source>
        <translation>&amp;Importálás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to delete this style?</source>
        <translation type="obsolete">Tényleg törölni akarja ezt a stílust?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Story Editor</source>
        <translation>Szövegszerkesztő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;New</source>
        <translation>&amp;Új</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Reload Text from Frame</source>
        <translation>Szöveg újratöltése a keretből</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Save to File...</source>
        <translation>&amp;Mentés fájlba...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Load from File...</source>
        <translation>&amp;Betöltés fájlból</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;Document</source>
        <translation>&amp;Domkumentum mentése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Szöveg keret frissítése és kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Kilépés a szöveg keret frissírése nélkül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select &amp;All</source>
        <translation>&amp;Minden kijelölése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cu&amp;t</source>
        <translation>&amp;Kivágás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Copy</source>
        <translation>&amp;Másolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Paste</source>
        <translation>&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Keresés/csere...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Stílusok szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Betűtípus előnézet...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Szöveg keret frissítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Background...</source>
        <translation>&amp;Háttér...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Display Font...</source>
        <translation>&amp;Betűtípus megjelenítése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;File</source>
        <translation>&amp;Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation>&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Settings</source>
        <translation>&amp;Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File</source>
        <translation>Fájl</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Load Text from File</source>
        <translation type="obsolete">Szöveg betöltése fájlból</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save Text to File</source>
        <translation type="obsolete">Szöveg mentése fájlba</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Szöveg keret frissítése és kilépés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Kilépés a szöveg keret frissírése nélkül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Szöveg újratöltése a keretből</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Update Text Frame</source>
        <translation type="obsolete">Szöveg keret frissítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Search/Replace</source>
        <translation type="obsolete">Keresés/csere</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Current Paragraph:</source>
        <translation>Jelenlegi bekezdés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Words: </source>
        <translation>Szavak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Chars: </source>
        <translation>Karakterek: </translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Totals:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paragraphs: </source>
        <translation>Bekezdések:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Warning</source>
        <translation type="obsolete">Figyelmeztetés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you want to save your changes?</source>
        <translation>Akarja menteni a változtatásait?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation>Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Szöveg fájlok (*.txt);;Minden fájl (*)</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as</source>
        <translation>Ment másként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Okos szöveg kijelölés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Insert Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Clear All Text</source>
        <translation>Teljes szöveg törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Story Editor - %1</source>
        <translation>Szövegszerkesztő - %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to lose all your changes?</source>
        <translation>Tényleg el akarja veszíteni az összes változtatását?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do you really want to clear all your text?</source>
        <translation>Tényleg törölni akarja a teljes szöveget?</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Character</source>
        <translation type="unfinished">Karakter</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quote</source>
        <translation type="unfinished">Idézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Space</source>
        <translation type="unfinished">Távolság</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement</source>
        <translation>Eltolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Linewidth</source>
        <translation>Vonalvastagság</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>More than one item selected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline</source>
        <translation type="obsolete">Aláhúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Small Caps</source>
        <translation>Kiskapitális</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Subscript</source>
        <translation>Alsó index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Superscript</source>
        <translation>Felső index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Outline</source>
        <translation type="obsolete">Szerkezet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline Words Only</source>
        <translation type="obsolete">Csak a szavak aláhúzása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All Caps</source>
        <translation>minden nagybetűs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shadow</source>
        <translation type="obsolete">Árnyék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation type="obsolete">Szerkezet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Használja a dokumentum nevét a bekezdés stílus előtagjaként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not ask again</source>
        <translation>Ne kérdezd ismét</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Szöveg importáló beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Overwrite Paragraph Styles</source>
        <translation>Bekezdés stílusainak felülírása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished">Engedélyezve felülírja a jelenlegi Scribus dokumentumban lévő stílusokat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Merge Paragraph Styles</source>
        <translation>Bekezdés stílusok összefésülése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished">Bekezdés stílusok összefűzése jellemzők szerint. Ez kevesebb hasonló bekezdés stílust eredményez, de megtartja a stílus jellemzőket, még akkor is, ha az eredeti dokumentum stílusai különböző nevűek.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>A beállítások legyenek alapértlemezettek, többé ne kérdezzen, mikor OpenOffice.org 1.x dokumentumot importálok.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation type="unfinished">Mégsem</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>At the beginning</source>
        <translation>A kezdetén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>At the end</source>
        <translation>A végén</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Not Shown</source>
        <translation>Nincs mutatva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Table of Contents and Indexes</source>
        <translation>Tartalomjegyzék és indexek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Table Of Contents</source>
        <translation>Tartalomjegyzék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation>&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Include frames that are set to not print as well</source>
        <translation>A nem nyomtatandónak beállított keretek is beletartoznak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paragraph Style:</source>
        <translation>Bekezdés stílusa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Destination Frame:</source>
        <translation>Cél keret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Inde&amp;x</source>
        <translation type="obsolete">&amp;Index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Table of Contents %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">Tartalomjegyzék és indexek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Table Of Contents</source>
        <translation type="unfinished">Tartalomjegyzék</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Hozzáadás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished">A nem nyomtatandónak beállított keretek is beletartoznak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paragraph Style:</source>
        <translation type="unfinished">Bekezdés stílusa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Destination Frame:</source>
        <translation type="unfinished">Cél keret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Inde&amp;x</source>
        <translation type="obsolete">&amp;Index</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Ignore all errors</source>
        <translation>Minden hiba figyelmen kívül hagyása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatic check before printing or exporting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for missing glyphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for objects not on a page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for overflow in text frames</source>
        <translation>Ellenőrizze a túlcsordulást a szöveg keretben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for transparencies used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for missing images</source>
        <translation>Ellenőrizze a hiányzó képet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check image resolution</source>
        <translation>Ellenőrizze a kép felbontást</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lowest allowed resolution</source>
        <translation>Legalacsonyabb megengedett felbontás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add Profile</source>
        <translation>Profil hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove Profile</source>
        <translation>Profil eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check for placed PDF Files</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Common Settings</source>
        <translation>Közös beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Placing in Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>In the Background</source>
        <translation>Háttérben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>In the Foreground</source>
        <translation>Előtérben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Snapping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Snap Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Grab Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Guides</source>
        <translation>Segédvonalak mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Margins</source>
        <translation>Margók mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Page Grid</source>
        <translation>Oldal rács mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Major Grid</source>
        <translation>Fő rács</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color:</source>
        <translation>Szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Spacing:</source>
        <translation>Távolság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Minor Grid</source>
        <translation>Segédrács</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Baseline Grid</source>
        <translation>Alaprács mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Baseline &amp;Grid:</source>
        <translation>&amp;Alaprács:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Baseline &amp;Offset:</source>
        <translation>&amp;Alapvonal eltolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the minor grid lines</source>
        <translation>Alrács vonalak közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the major grid lines</source>
        <translation>Főrács vonalak közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of the minor grid lines</source>
        <translation>Alrács vonalak színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of the major grid lines</source>
        <translation>Főrács vonalak színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color for the margin lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the basegrid on or off</source>
        <translation>Alaprács be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the gridlines on or off</source>
        <translation>Rácsvonalak be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the guides on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Turns the margins on or off</source>
        <translation>Margók be- és kikapcsolása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Baseline Settings</source>
        <translation>Alapvonal beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Guides are not visible through objects on the page</source>
        <translation>Az oldalon lévő objektumokon keresztül a segédvonalak nem láthatók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Guides are visible above all objects on the page</source>
        <translation>Az oldalon lévő összes objektumok előtt láthatók a segédvonalak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color for the baseline grid</source>
        <translation>Alaprács színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Alaprács vonalak közötti távolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance from the top of the page for the first baseline</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Manage Tabulators</source>
        <translation>Tabulátorok kezelése</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Export Range</source>
        <translation>Exportálás hatóköre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;All Pages</source>
        <translation>&amp;Minden oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Oldalak kiválasztása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Rotation:</source>
        <translation>&amp;Forgatás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File Options</source>
        <translation>Fájl beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Compatibilit&amp;y:</source>
        <translation>Kompatibilitás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Binding:</source>
        <translation>&amp;Fedél:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left Margin</source>
        <translation>Bal margó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right Margin</source>
        <translation>Jobb margó</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Generate &amp;Thumbnails</source>
        <translation>&amp;Miniatűrök létrehozása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Include Bookmarks</source>
        <translation>Könyvjelzők belefoglalása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image Settings</source>
        <translation type="obsolete">Kép beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatic</source>
        <translation>Automatikus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>JPEG</source>
        <translation type="obsolete">JPEG</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Zip</source>
        <translation type="obsolete">Zip</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Módszer:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Minőség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Maximum</source>
        <translation>Maximum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>High</source>
        <translation>Magas</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Medium</source>
        <translation>Közepes</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Low</source>
        <translation>Alacsony</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Minimum</source>
        <translation>Minimum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;General</source>
        <translation>&amp;Általános</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">Az összes betűtípus beágyazása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">Betűtípusok részhalmaza</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Embedding</source>
        <translation>Beágyazás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available Fonts:</source>
        <translation>Rendelkezésre álló betűtípusok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts to embed:</source>
        <translation>Beágyazandó betűtípusok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Fonts</source>
        <translation>&amp;Betűtípusok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page</source>
        <translation>Oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show Page Pre&amp;views</source>
        <translation>Gyorsnézet mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Effects</source>
        <translation>Hatások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Hatás típusok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Vonalak mozgatása:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>F&amp;rom the:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;irection:</source>
        <translation>Irány:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> sec</source>
        <translation>másodperc</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Effect</source>
        <translation>Nincs effektus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Blinds</source>
        <translation>Vaknyomás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Box</source>
        <translation>Doboz</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dissolve</source>
        <translation>Eltűnés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Glitter</source>
        <translation>Csillogás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Split</source>
        <translation>Felosztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Wipe</source>
        <translation>Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal</source>
        <translation>Vízszintes</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical</source>
        <translation>Függőleges</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Inside</source>
        <translation>Belül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Outside</source>
        <translation>Kívül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Left to Right</source>
        <translation>Balról jobbra:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Top to Bottom</source>
        <translation>Függőlegesen, lefelé:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bottom to Top</source>
        <translation>Függőlegesen, fölfelé:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right to Left</source>
        <translation>Jobbról balra:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Top-left to Bottom-Right</source>
        <translation>Bal felsőtől a jobb alsóig</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>E&amp;xtras</source>
        <translation>&amp;Extrák</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Titkosítás használata</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Passwords</source>
        <translation>Jelszavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;User:</source>
        <translation>&amp;Felhasználó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Owner:</source>
        <translation>&amp;Tulajdonos:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Settings</source>
        <translation>Beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;ecurity</source>
        <translation>&amp;Biztonság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>General</source>
        <translation>Általános</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Screen / Web</source>
        <translation>Képernyő / web</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Printer</source>
        <translation>Nyomtató</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Grayscale</source>
        <translation>Szürkeskála</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering Settings</source>
        <translation>Renderelési beállítások</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Angle:</source>
        <translation>&amp;Szög:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Simple Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line</source>
        <translation>Vonal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Round</source>
        <translation>Kör</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ellipse</source>
        <translation>Ellipszis</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use ICC Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering-Intent:</source>
        <translation>Megjelenítési prioritások:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation</source>
        <translation>Telítettség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Absolute Colorimetric</source>
        <translation>Abszolút kolorimetriás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Images:</source>
        <translation>Képek:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;olor</source>
        <translation>&amp;Szín</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Info String:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Trim Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show page previews of each page listed above.</source>
        <translation>Az alább felsorolt oldalak előnézetének mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Az oldal megjelenítésésnek idötartama mielőtt a bemutató elindul az oldalon.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Időtartam, amíg az effektus fut. A rövidebb idő gyorsítja, a hosszabb lassítja az effektust.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Type of the display effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export all pages to PDF</source>
        <translation>Összes oldal exportálása PDF-be</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export a range of pages to PDF</source>
        <translation>Oldal tartomány exportálása PDF-be</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color profile for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Embed a color profile for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color profile for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rendering intent for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror Page(s) horizontally</source>
        <translation>Oldal(ak) vízszintes tükrözése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mirror Page(s) vertically</source>
        <translation>Oldal(ak) függőleges tükrözése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Include Layers</source>
        <translation type="obsolete">Rétegek belefoglalása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;EPS grafika felbontása:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Include La&amp;yers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Compression Metho&amp;d:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Compression &amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Layout</source>
        <translation type="obsolete">Oldal elrendezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Single Page</source>
        <translation type="unfinished">Egyoldalas</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Continuous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Page Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Double Page Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Visual Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use Viewers Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use Full Screen Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display Bookmarks Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Display Layers Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hide Viewers Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hide Viewers Menubar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Special Actions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Viewer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lossy - JPEG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lossless - Zip</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image Compression Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Embed All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts to outline:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Outline &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Font:</source>
        <translation>Betűtípus:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size:</source>
        <translation>Méret:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill Color:</source>
        <translation>Kitöltő szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Stroke Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Oszlopok:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Gap:</source>
        <translation>&amp;Hézag:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Betűtípusnéző próbamondatszörny.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Line Color:</source>
        <translation>&amp;Vonal színe:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Shading:</source>
        <translation>&amp;Árnyákolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Kitöltő szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;hading:</source>
        <translation>&amp;Árnyékolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Style:</source>
        <translation>Vonal stílusa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line &amp;Width:</source>
        <translation>&amp;Vonalvastagság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line S&amp;tyle:</source>
        <translation>&amp;Vonal stílusa:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Arrows:</source>
        <translation>Nyílak:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Start:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>End:</source>
        <translation>Vége:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Szabad skálázású</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Függőleges skálázás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Kép méretezése a keret méretéhez</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>&amp;Oldalarány megtartása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Kitöltő szín:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;Minimum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ma&amp;ximum:</source>
        <translation>&amp;Maximum:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Stepping:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Frame Properties</source>
        <translation>Szöveg keret tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Picture Frame Properties</source>
        <translation>Kép keret tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Drawing Properties</source>
        <translation>Vonal rajzolás tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polygon Drawing Properties</source>
        <translation>Sokszög rajzolási tulajdonságok</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Font for new text frames</source>
        <translation>A következő szöveg keretek betűtípusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Size of font for new text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of font</source>
        <translation>Betűtípus színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Number of columns in a text frame</source>
        <translation>Oszlopok száma egy szöveg keretben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Gap between text frame columns</source>
        <translation>Szöveg keret oszlopok közötti hézag</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sample of your font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Kép keret a képek bármilyen átméretezését megengedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Vertical scaling of images</source>
        <translation>Képek függőleges skálázása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Kép keretben lévő képek a keret méretére átméretezhetők</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill color of picture frames</source>
        <translation>Kép keret kitöltő színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color of fill</source>
        <translation>kitöltő szín telítettsége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line color of shapes</source>
        <translation>Alakzat vonal színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color of lines</source>
        <translation>Vonal szín telítettsége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill color of shapes</source>
        <translation>Alakzat kitöltő színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line style of shapes</source>
        <translation>Alak vonal stílusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line width of shapes</source>
        <translation>Alakok vonal szélessége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color of lines</source>
        <translation>Vonal színe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Saturation of color</source>
        <translation>Színtelítettség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Style of lines</source>
        <translation>Vonal stílusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Width of lines</source>
        <translation>Vonal szélessége</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tab Fill Character:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tab Width:</source>
        <translation>Tabulátor szélesség:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>On Screen Preview</source>
        <translation>Előnézet képernyőn</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Full Resolution Preview</source>
        <translation>Teljes felbontású előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Normal Resolution Preview</source>
        <translation>Normál felbontású előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Low Resolution Preview</source>
        <translation>Alacsony felbontású előnézet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom:</source>
        <translation>Egyedi:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom: </source>
        <translation>Egyedi:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Color:</source>
        <translation>Szöveg színe:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Shading:</source>
        <translation>Árnyékolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Stroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dot</source>
        <translation>Pont</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hyphen</source>
        <translation>Elválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Subscript</source>
        <translation>Alsó index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Displacement:</source>
        <translation>&amp;Eltolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Scaling:</source>
        <translation>&amp;Méretezés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Superscript</source>
        <translation>Felső index</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>D&amp;isplacement:</source>
        <translation>&amp;Eltolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;caling:</source>
        <translation>&amp;Méretezés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Small Caps</source>
        <translation>Kiskapitális</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Sc&amp;aling:</source>
        <translation>&amp;Méretezés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underline</source>
        <translation>Aláhúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement:</source>
        <translation>Eltolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Width:</source>
        <translation>Vonalvastagság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Strikethru</source>
        <translation>Áthúzott</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automatikus &amp;vonaltávolság</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line Spacing:</source>
        <translation>Vonal távolság:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Line width expressed as a percentage of the font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Left</source>
        <translation>Bal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right</source>
        <translation>Jobbra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Full Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Comma</source>
        <translation>Vessző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Center</source>
        <translation>Középre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Position:</source>
        <translation>&amp;Elhelyezkedés:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete All</source>
        <translation>Minden törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Indentation for first line of the paragraph</source>
        <translation>Bekezdés első sorának azonosítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Indentation from the left for the whole paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete all Tabulators</source>
        <translation>Minden tabulátor törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>None</source>
        <translation>Nincs</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Dot</source>
        <translation>Pont</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Hyphen</source>
        <translation>Elválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom</source>
        <translation>Egyedi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Fill Char:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom:</source>
        <translation>Egyedi:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Custom: </source>
        <translation>Egyedi:</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Outline</source>
        <translation>Szerkezet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Element</source>
        <translation>Elem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Group </source>
        <translation>Csoport</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Free Objects</source>
        <translation>Szabad objektum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page </source>
        <translation>Oldal</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Displacement</source>
        <translation>Eltolás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Linewidth</source>
        <translation>Vonalvastagság</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Add vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove vertical guide</source>
        <translation>Függőleges segédvonalak eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove horizontal guide</source>
        <translation>Vízszintes segédvonalak eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lock guides</source>
        <translation>Segédvonalak rögzítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unlock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Move</source>
        <translation>Mozgatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Resize</source>
        <translation>Átméretezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rotate</source>
        <translation>Forgatás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation>W1: %1, H1: %2
W2: %3, H2: %4</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Selection</source>
        <translation>Kiválasztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Group</source>
        <translation>Csoport</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create</source>
        <translation>Létrehozás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align/Distribute</source>
        <translation>Igazítás/elosztás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Items involved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cancel</source>
        <translation>Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set fill color</source>
        <translation>Kitöltő szín beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Color1: %1, Color2: %2</source>
        <translation>Szín1: %1, szín2: %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set line color</source>
        <translation>Vonal szín beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set line color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flip horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Flip vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lock</source>
        <translation>Rögzítés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unlock</source>
        <translation>Feloldás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lock size</source>
        <translation>Zárolás mérete</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Unlock size</source>
        <translation>Méret zárolás feloldása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Ungroup</source>
        <translation>Csoportosítás megszüntetése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete</source>
        <translation>Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rename</source>
        <translation>Átnevezés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>From %1
to %2</source>
        <translation>%1-től %2-ig</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Paste</source>
        <translation>Beillesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Cut</source>
        <translation>Kivágás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set fill color transparency</source>
        <translation>Kitöltő szín átlátszhatóság beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set line color transparency</source>
        <translation>Vonal szín átlátszhatóság beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set line style</source>
        <translation>Vonal stílus beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set the style of line end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set the style of line join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set line width</source>
        <translation>Vonal szélesség beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No style</source>
        <translation>Nincs stílus megadva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set custom line style</source>
        <translation>Egyedi sor stílus beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Do not use custom line style</source>
        <translation>Ne használd az egyedi vonal stílust</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set start arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set end arrow</source>
        <translation>Vége nyíl beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Create table</source>
        <translation>Tábla létrehozása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rows: %1, Cols: %2</source>
        <translation>Sor: %1, oszlop: %2</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font</source>
        <translation>Betűtípus beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font size</source>
        <translation>Betűtípus méret beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font width</source>
        <translation>Betűtípus szélesség beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font fill color</source>
        <translation>Betűtípus szín beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font stroke color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font fill color shade</source>
        <translation>Betűtípus szín árnyék beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font stroke color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set kerning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set line spacing</source>
        <translation>Sortávolság beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set paragraph style</source>
        <translation>Bekezdés stílus beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set language</source>
        <translation>Nyelv beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Align text</source>
        <translation>Szöveg igazítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font effect</source>
        <translation>Betűtípus hatás beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Image frame</source>
        <translation>Kép keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text frame</source>
        <translation>Szöveg keret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polygon</source>
        <translation>Sokszög</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Bezier curve</source>
        <translation>Bezier görbe</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Polyline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Convert to</source>
        <translation>Átalakítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import SVG image</source>
        <translation>SVG kép importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import EPS image</source>
        <translation>EPS kép importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Scratch space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text flows around the frame</source>
        <translation>Szöveg a keretet körbeveszi</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text flows around bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text flows around contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No text flow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page %1</source>
        <translation>Oldal %1</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set image scaling</source>
        <translation>Kép skálázás beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Frame size</source>
        <translation>Keret méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Free scaling</source>
        <translation>Szabad skálázású</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Keep aspect ratio</source>
        <translation>Oldalarány megtartása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Break aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit shape</source>
        <translation>Alak szerkesztése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply Master Page</source>
        <translation>Mester oldal alkalmazása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Set font height</source>
        <translation>Betűtípus magasság beállítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import OpenOffice.org Draw image</source>
        <translation>&amp;OpenOffice.org Rajz kép importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add page</source>
        <translation>Oldal hozáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add pages</source>
        <translation>Oldalak hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete page</source>
        <translation>Oldal törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete pages</source>
        <translation>Oldal törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add layer</source>
        <translation>Réteg hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Delete layer</source>
        <translation>Réteg törlése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Rename layer</source>
        <translation>Réteg átnevezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Raise layer</source>
        <translation>Réteg előrébb mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Lower layer</source>
        <translation>Réteg hátrébb mozgatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send to layer</source>
        <translation>Küldés a rétegre</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable printing of layer</source>
        <translation>Réteg nyomtatásának engedélyezése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Disable printing of layer</source>
        <translation>Kéreg nyomtatásának tiltása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change name of the layer</source>
        <translation>Réteg nevének megváltoztatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Get image</source>
        <translation>Kép betöltése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text on a Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Disable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Multiple duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change Image Offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Change Image Scale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>X: %1, Y: %2
X: %4, Y: %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset control point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Reset control points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Modify image effects</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Action History</source>
        <translation>Művelet előzmény</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Show selected object only</source>
        <translation>Csak a kiválasztott objektumok mutatása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Undo</source>
        <translation>&amp;Visszavonás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Redo</source>
        <translation>&amp;Újra végrehajtás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Initial State</source>
        <translation>Kezdeti állapot</translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <location filename="" line="7471221"/>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation>%1: %2</translation>
    </message>
</context>
<context>
    <name>UpgradeChecker</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Attempting to get the Scribus version update file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Timed out when attempting to get update file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Error when attempting to get update file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>File not found on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>No updates are available for your version of Scribus %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>This list may contain development versions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Please visit www.scribus.net for details.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Minimum Margins for Page Size %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Use Printer Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Select &amp;Printer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Margins</source>
        <translation type="unfinished">Margók</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Felső:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Alsó:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Bal:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Mégsem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert value</source>
        <translation>Érték beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enter a value then press OK.</source>
        <translation>Adja meg az értéket, majd nyomja meg az OK-t</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enter a value then press OK</source>
        <translation>Adja meg az értéket, majd nyomja meg az OK-t</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Send your value to the script</source>
        <translation>Érték küldése a szkriptnek</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Rövid szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Kiválaszott keretek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aktív &amp;oldal</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Minden elem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Alapértelmezések lecserélése a felhasználói beállításokra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Csak a kijelölt kereteket dolgozta fel.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Csak az aktuális oldalt dolgozta fel.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">A dokumentum minden eleme feldolgozva.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Rövid szavak a Scribusban</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Elérhető az alábi nyelveken</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit &amp;system configuration...</source>
        <translation type="obsolete">&amp;Rendszer konfiguráció szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Edit &amp;user configuration...</source>
        <translation type="obsolete">&amp;Felhasználói beállítások szerkesztése...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>S&amp;etup editor...</source>
        <translation type="obsolete">&amp;Szerkesztő beállítása...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Szerkesztés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Info and Languages...</source>
        <translation type="obsolete">&amp;Információ és nyelvek...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Segítség</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Short Words</source>
        <translation type="obsolete">Rövid szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="obsolete">%1 alkalmazási hiba. Nem indítható.</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Tools</source>
        <translation>Eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Properties...</source>
        <translation>Tulajdonságok...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF Tools</source>
        <translation>PDF eszközök</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Button</source>
        <translation>Gomb</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text Field</source>
        <translation>Szövegmező</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Check Box</source>
        <translation>Jelölő négyzet</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Combo Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>List Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert PDF Fields</source>
        <translation>PDF mezők beszúrása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Text</source>
        <translation>Szöveg</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Link</source>
        <translation>Kapcsolat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Insert PDF Annotations</source>
        <translation>PDF kommentár beszúrása</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose the importer to use</source>
        <translation>Válassza ki az importert</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Automatic</source>
        <translation>Automatikus</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import text without any formatting</source>
        <translation>Szöveg importálása formázás nélkül</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Importer:</source>
        <translation>Importáló:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Encoding:</source>
        <translation>Kódolás:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Import Text Only</source>
        <translation>Csak szöveg importálása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Open</source>
        <translation type="unfinished">Megnyitás</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose the importer to use</source>
        <translation>Válassza ki az importert</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remember association</source>
        <translation>Emlékezzen a társításra</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>New From Template</source>
        <translation>Új sablonból</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Remove</source>
        <translation>&amp;Eltávolítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Open</source>
        <translation>&amp;Megnyitás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>All</source>
        <translation>Minden</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colors</source>
        <translation>Színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Description</source>
        <translation>Leírás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Usage</source>
        <translation>Használat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Created with</source>
        <translation>Létrehozva</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Date</source>
        <translation>Dátum</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Author</source>
        <translation>Szerző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Downloading Templates</source>
        <translation>Sablonok letöltése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Dokumentum sablonok a http://www.scribus.net/ honlapon, a Letötlések részben találhatók.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Installing Templates</source>
        <translation>Sablonok telepítése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Preparing a template</source>
        <translation>Sablon létrehozása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Removing a template</source>
        <translation>Sablon eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Translating template.xml</source>
        <translation>sablon.xml fordítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Save as Template</source>
        <translation>Mentés sablonként</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Name</source>
        <translation>Név</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Category</source>
        <translation>Kategória</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Page Size</source>
        <translation>Oldal méret</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Colors</source>
        <translation>Színek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Description</source>
        <translation>Leírás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Usage</source>
        <translation>Használat</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Author</source>
        <translation>Szerző</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Email</source>
        <translation>E-mail</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>More Details</source>
        <translation>További részletek</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>OK</source>
        <translation>Rendben</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Less Details</source>
        <translation>Részletek elrejtése</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Legal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>landscape</source>
        <translation>fekvő</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>portrait</source>
        <translation>álló</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>custom</source>
        <translation>egyedi</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Create filter</source>
        <translation>Szűrő létrehozása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>C&amp;lear</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>&amp;Delete</source>
        <translation>&amp;Törlés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Choose a previously saved filter</source>
        <translation>Válasszon egy előzőleg elmentett szűrőt</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Give a name to this filter for saving</source>
        <translation>A mentéshez adja meg a szűrő nevét</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Give a name for saving</source>
        <translation>Adja meg a mentés nevét</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Disable or enable this filter row</source>
        <translation>Szűrő sor engedélyezése, tiltása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove this filter row</source>
        <translation>Szűrő eltávolítása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Add a new filter row</source>
        <translation>Új szűrő sor hozzáadása</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>and</source>
        <translation>és</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>do not remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>words</source>
        <translation>szavak</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Remove</source>
        <translation>Eltávolítás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Replace</source>
        <translation>Csere</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Apply</source>
        <translation>Alkalmazás</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Value at the left is a regular expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>paragraph style</source>
        <translation>bekezdés stílusa</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>all instances of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>all paragraphs</source>
        <translation>minden bekezdés</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>paragraphs starting with</source>
        <translation>bekezdés kezdődik</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>paragraphs with less than</source>
        <translation>bekezdés kisebb mint</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>paragraphs with more than</source>
        <translation>bekezdés nagyobb mint</translation>
    </message>
</context>
</TS>
