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
        <translation>getColor(&quot;name&quot;) -&gt; tuple

Returns a tuple (C, M, Y, K) containing the four colour components of the
colour &quot;name&quot; from the current document. If no document is open, returns
the value of the named colour from the default document colours.

May raise NotFoundError if the named colour wasn&apos;t found.
May raise ValueError if an invalid colour name is specified.
</translation>
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
        <translation>getColorAsRGB(&quot;name&quot;) -&gt; tuple

Returns a tuple (R,G,B) containing the three colour components of the
colour &quot;name&quot; from the current document, converted to the RGB colour
space. If no document is open, returns the value of the named colour
from the default document colours.

May raise NotFoundError if the named colour wasn&apos;t found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
        <source>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>changeColour(&quot;name&quot;, c, m, y, k)

Changes the colour &quot;name&quot; to the specified CMYK value. The colour value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Colour components should be in the range from 0 to 255.

May raise NotFoundError if the named colour wasn&apos;t found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new colour &quot;name&quot;. The colour Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Colour components should be in
the range from 0 to 255.

May raise ValueError if an invalid colour name is specified.
</translation>
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
        <translation>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Deletes the colour &quot;name&quot;. Every occurence of that colour is replaced by the
colour &quot;replace&quot;. If not specified, &quot;replace&quot; defaults to the colour
&quot;None&quot; - transparent.

deleteColor works on the default document colours if there is no document open.
In that case, &quot;replace&quot;, if specified, has no effect.

May raise NotFoundError if a named colour wasn&apos;t found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the colour &quot;name&quot; is replaced by the colour &quot;replace&quot;.

May raise NotFoundError if a named colour wasn&apos;t found.
May raise ValueError if an invalid colour name is specified.
</translation>
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
        <translation type="unfinished">fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -&gt; string with filename

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
</translation>
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
        <translation type="unfinished">messageBox(&quot;caption&quot;, &quot;message&quot;,
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
</translation>
    </message>
    <message>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation type="unfinished">valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation type="unfinished">closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation type="unfinished">haveDoc() -&gt; bool

Returns true if there is a document open.
</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation type="unfinished">openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation type="unfinished">saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation type="unfinished">saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished">saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation type="unfinished">setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation type="unfinished">setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation type="unfinished">getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation type="unfinished">loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation type="unfinished">setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished">getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation type="unfinished">getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation type="unfinished">getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation type="unfinished">getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation type="unfinished">getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished">getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished">getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="unfinished">getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation type="unfinished">getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation type="unfinished">getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation type="unfinished">getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</translation>
    </message>
    <message>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation type="unfinished">getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</translation>
    </message>
    <message>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation type="unfinished">getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</translation>
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
        <translation type="unfinished">getProperty(object, property)

Return the value of the property `property&apos; of the passed `object&apos;.

The `object&apos; argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property&apos; argument must be a string, and is the name of the property
to look up on `object&apos;.

The return value varies depending on the type of the property.
</translation>
    </message>
    <message>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation type="unfinished">setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</translation>
    </message>
    <message>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation type="unfinished">getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</translation>
    </message>
    <message>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation type="unfinished">getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished">moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</translation>
    </message>
    <message>
        <source>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertext at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished">rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertext at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished">rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished">sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation type="unfinished">getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation type="unfinished">selectionCount() -&gt; integer

Returns the number of selected objects.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation type="unfinished">selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation type="unfinished">deselectAll()

Deselects all objects in the whole document.
</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation type="unfinished">groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation type="unfinished">unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</translation>
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
        <translation type="unfinished">scaleGroup(factor [,&quot;name&quot;])

Scales the group the object &quot;name&quot; belongs to. Values greater than 1 enlarge
the group, values smaller than 1 make the group smaller e.g a value of 0.5
scales the group to 50 % of its original size, a value of 1.5 scales the group
to 150 % of its original size.  The value for &quot;factor&quot; must be greater than
0. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if an invalid scale factor is passed.
</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished">loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished">scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation type="unfinished">lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished">isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation type="unfinished">setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation type="unfinished">getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation type="unfinished">getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</translation>
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
        <translation type="unfinished">renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
If &quot;filename&quot; is not &quot;&quot;, image is saved into &quot;filename&quot;. Otherwise
image data is returned as a string. The optional &quot;format&quot; argument
specifies the image format to generate, and supports any format allowed
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation type="unfinished">getLayers() -&gt; list

Returns a list with the names of all defined layers.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation type="unfinished">getActiveLayer() -&gt; string

Returns the name of the current active layer.
</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished">createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation type="unfinished">getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</translation>
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
        <translation type="unfinished">createEllipse(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new ellipse on the current page and returns its name.
The coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name for further referencing of that object. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</translation>
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
        <translation type="unfinished">createImage(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new picture frame on the current page and returns its name. The
coordinates are given in the current measurement units of the document.
&quot;name&quot; should be a unique identifier for the object because you need this
name for further access to that object. If &quot;name&quot; is not given Scribus will
create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</translation>
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
        <translation type="unfinished">createText(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new text frame on the actual page and returns its name.
The coordinates are given in the actual measurement unit of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further referencing of that object. If &quot;name&quot; is not
given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</translation>
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
        <translation type="unfinished">createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; string

Creates a new line from the point(x1, y1) to the point(x2, y2) and returns
its name. The coordinates are given in the current measurement unit of the
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the
object because you need this name for further access to that object. If
&quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</translation>
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
        <translation type="unfinished">createPolyLine(list, [&quot;name&quot;]) -&gt; string

Creates a new polyline and returns its name. The points for the polyline are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
The coordinates are given in the current measurement units of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further access to that object. If &quot;name&quot; is not given
Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don&apos;t group into points without leftovers.
</translation>
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
        <translation type="unfinished">createPolygon(list, [&quot;name&quot;]) -&gt; string

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
</translation>
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
        <translation type="unfinished">createBezierLine(list, [&quot;name&quot;]) -&gt; string

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
</translation>
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
        <translation type="unfinished">createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -&gt; string

Creates a new pathText by merging the two objects &quot;textbox&quot; and
&quot;beziercurve&quot; and returns its name. The coordinates are given in the current
measurement unit of the document (see UNIT constants). &quot;name&quot; should be a
unique identifier for the object because you need this name for further access
to that object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
May raise NotFoundError if one or both of the named base object don&apos;t exist.
</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation type="unfinished">deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation type="unfinished">textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation type="unfinished">objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation type="unfinished">setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation type="unfinished">getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation type="unfinished">currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation type="unfinished">redrawAll()

Redraws all pages.
</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation type="unfinished">savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</translation>
    </message>
    <message>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation type="unfinished">deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</translation>
    </message>
    <message>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation type="unfinished">gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation type="unfinished">pageCount() -&gt; integer

Returns the number of pages in the document.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation type="unfinished">getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation type="unfinished">setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation type="unfinished">getVGuides()

See getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation type="unfinished">setVGuides()

See setHGuides.
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation type="unfinished">getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation type="unfinished">getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="unfinished">getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill colour of the object &quot;name&quot; to the colour &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colours. If &quot;name&quot; is not given the
currently selected item is used.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line colour of the object &quot;name&quot; to the colour &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation type="unfinished">setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</translation>
    </message>
    <message>
        <source>setLineShade(shade, [&quot;name&quot;])

Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full color intensity). If &quot;name&quot; is not given the currently selected item
is used.

May raise ValueError if the line shade is out of bounds.
</source>
        <translation>setLineShade(shade, [&quot;name&quot;])

Sets the shading of the line colour of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full colour intensity). If &quot;name&quot; is not given the currently selected item
is used.

May raise ValueError if the line shade is out of bounds.
</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation type="unfinished">setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation type="unfinished">setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation type="unfinished">setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</translation>
    </message>
    <message>
        <source>setFillShade(shade, [&quot;name&quot;])

Sets the shading of the fill color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full Color intensity). If &quot;name&quot; is not given the currently selected
Item is used.

May raise ValueError if the fill shade is out of bounds.
</source>
        <translation>setFillShade(shade, [&quot;name&quot;])

Sets the shading of the fill colour of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full Colour intensity). If &quot;name&quot; is not given the currently selected
Item is used.

May raise ValueError if the fill shade is out of bounds.
</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation type="unfinished">setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation type="unfinished">setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished">getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished">getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished">getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished">getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished">getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished">getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished">getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished">setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="unfinished">insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation type="unfinished">setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</translation>
    </message>
    <message>
        <source>setFontSize(size, [&quot;name&quot;])

Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated
as a value in points. If there is some text selected only the selected text is
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError for a font size that&apos;s out of bounds.
</source>
        <translation type="unfinished">setFontSize(size, [&quot;name&quot;])

Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated
as a value in points. If there is some text selected only the selected text is
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError for a font size that&apos;s out of bounds.
</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation type="unfinished">setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation type="unfinished">setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation type="unfinished">setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation type="unfinished">setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</translation>
    </message>
    <message>
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared.  If &quot;name&quot; is not given the currently
selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation type="unfinished">selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared.  If &quot;name&quot; is not given the currently
selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished">deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text colour of the text frame &quot;name&quot; to the colour &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished">setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
        <source>setTextShade(shade, [&quot;name&quot;])

Sets the shading of the text color of the object &quot;name&quot; to &quot;shade&quot;. If
there is some text selected only the selected text is changed. &quot;shade&quot; must
be an integer value in the range from 0 (lightest) to 100 (full color
intensity). If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setTextShade(shade, [&quot;name&quot;])

Sets the shading of the text colour of the object &quot;name&quot; to &quot;shade&quot;. If
there is some text selected only the selected text is changed. &quot;shade&quot; must
be an integer value in the range from 0 (lightest) to 100 (full colour
intensity). If &quot;name&quot; is not given the currently selected item is
used.
</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished">linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished">unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation type="unfinished">traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</translation>
    </message>
    <message>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation type="unfinished">textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished">setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished">isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation type="unfinished">progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation type="unfinished">progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</translation>
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
        <translation type="unfinished">progressSet(nr)

Set the progress bar position to &quot;nr&quot;, a value relative to the previously set
progressTotal. The progress bar uses the concept of steps; you give it the
total number of steps and the number of steps completed so far and it will
display the percentage of steps that have been completed. You can specify the
total number of steps with progressTotal(). The current number of steps is set
with progressSet(). The progress bar can be rewound to the beginning with
progressReset(). [based on info taken from Trolltech&apos;s Qt docs]
</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation type="unfinished">setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation type="unfinished">docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation type="unfinished">newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</translation>
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
        <translation type="unfinished">newDoc(size, margins, orientation, firstPageNumber,
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
</translation>
    </message>
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation type="unfinished">closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished">masterPageNames()

Returns a list of the names of all master pages in the document.
</translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished">editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation>createMasterPage(pageName)

生成一个叫 pageName 的新母页，并且打开它来编辑
</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(pageName)

删除命名的母页
</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(double)

在主图形界面窗口缩放文件。动作包括整数值如20.0，100.0，等待。缩放以-100作为记号。
</translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Contributions from:</source>
        <translation>贡献者：</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;A 关于</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;U 作者</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;T 翻译</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;O 在线资料</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>开发团队</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>官方文档：</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>其他文档：</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>主页</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>在线参考</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>纠错和功能请求</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>邮件列表</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>官方翻译和翻译者：</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>先前翻译贡献者：</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>关于 %1</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation>Windows 移植：</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation>Mac OSX Aqua 移植：</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished">Wiki</translation>
    </message>
    <message>
        <source>%1 %2 %3</source>
        <translation type="unfinished">%1 %2 %3</translation>
    </message>
    <message>
        <source>%3-%2-%1 %4 %5</source>
        <translation type="unfinished">%3-%2-%1 %4 %5</translation>
    </message>
    <message>
        <source>Using Ghostscript version %1</source>
        <translation>当前Ghostscript版本 %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ghostscript不可知 </translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation type="unfinished">&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>编译 ID:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>这个面板显示Scribus版本，编译日期以及编译进去的库支持。这里C-C-T-F指的是C=littlecms C=CUPS T=TIFF 支持 F=Fontconfig 支持。最后一个字母是渲染器 C=cairo 或者 A=libart。不存在的库支持以*号表示。这个也注明了Scribus检测到的Ghostscript版本号</translation>
    </message>
    <message>
        <source>January</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <source>No</source>
        <translation>否</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: 关于插件</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation>文件名：</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>版本：</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>启用：</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>发布日期：</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>版权：</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>作者：</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>描述：</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>授权：</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;I 图像效果</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;T 制表符...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;N 新建</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;O 打开...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>&amp;A 另存为...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;V 恢复到已保存</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>&amp;U 准备输出...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>导入文本...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;T 追加文本...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>导入图像...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>&amp;T 保存文本...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>&amp;E 页面存为EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>&amp;D 保存为PDF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;S 文档设置...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;P 打印...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Q 退出</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;U 撤销</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;R 重做</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;I 项目动作模式</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;t 剪切</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;C 复制</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;P 粘贴</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;L 清除内容</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;A 全选</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;D 取消全选</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;S 查找/替换...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;O 颜色...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;P 段落样式...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;L 线条样式...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;M 母页...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;R 首选项...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="unfinished">%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;O 其他...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;L 左边</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;C 中间</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;R 右边</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;B 区块</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;F 强制</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation type="unfinished">&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;N 普通</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;U 下划线</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>&amp;W 添加下划线</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;S 中划线</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;A 全部大写</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;C 小写</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;P 上标</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>&amp;B 下标</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;O 文档结构树</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;U 复制</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;M 多重复制</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;G 群组</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;U 解散群组</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>&amp;L 锁定</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>&amp;Z 大小锁定</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>&amp;T 升高到顶部</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;L 降低</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;R 升高</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>&amp;C发送到寄存本</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;A 属性...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>&amp;M 显示图像</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>&amp;B 为PDF书签</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>&amp;N 为PDF注释</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;R 注释属性</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;R 文字域属性</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;E 编辑形状...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;A 附文字到路径</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;D 从路径分离文字</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;C 合并多边形</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>&amp;P 分离多边形</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;B 齐贝尔曲线</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;I 图像框</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;O 轮廓</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;P 多边形</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;T 文本框</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;G 符号...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>文本字样</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;I 插入...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>&amp;P 导入...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;D 删除...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;C 复制...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;M 移动...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;A 应用母页...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;G 管理参考线...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>&amp;F 符合窗口</translation>
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
        <translation>&amp;T 缩图</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;M 显示边距</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>&amp;F 显示框体</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>&amp;I 显示图像</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>&amp;G 显示网格</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;U 显示参考线</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;B 显示底线网格</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>&amp;T 显示文本链</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;A 吸附到网格</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;P 吸附到参考线</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;P 属性</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;S 寄存本</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;L 层</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;B 书签</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;M 尺寸</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;H 动作历史</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;V 印前检验器</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;工具</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF 工具</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>选择项目</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>&amp;A 表格</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;S 形状</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;L 线条</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;F 手绘线</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>旋转项目</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>放大或缩小</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>放大</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>缩小</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>编辑框内容</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>串联文本框</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>解链文本框</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;E 取色器</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>复制项目属性</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>用故事编辑器编辑文本</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>插入文本框体</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>插入图像框体</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>插入表格</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>插入形状</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>插入多边形</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>插入线</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>插入齐贝尔曲线</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>插入手绘线</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;M 管理图片</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;H 用连字符</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;G 生成目录索引</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;A 关于Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>关于 &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;P 提示</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;M Scribus 使用手册...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;H 智能连字符</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>&amp;S 不间断空格</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>&amp;N 页码</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>版权</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>注册商标</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>商标</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>项目符号</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Em 破折号</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>En 破折号</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>图形破折号</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>引用破折号</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>上标点</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>双倍直线</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>单面左边</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>单面右边</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>双面左边</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>双面右边</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>单面反转</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>双面反转</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>左单引号</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>右单引号</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>左双引号</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>右双引号</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>下单逗号</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>下双逗号</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>中日韩左单</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>中日韩右单</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>中日韩双左</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>中日韩双右</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>切换色板</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;H 阴影</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;L 底分辨率</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;N 正常分辨率</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;F 完全分辨率</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>编辑图片...</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;U 刷新图片</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>调整框体适应图片大小</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>图片扩展属性</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>显示控制符</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;A 对齐和分布</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>编辑文本...</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>新建线条</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>分框</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>分列</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>不间断破折号</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>切换参考线</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;A 安排页</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>取消连字符</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>管理页面属性...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>标尺相对应于页面</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>&amp;W 打印预览</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation type="unfinished">&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation> 转换为母页...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;C 层叠</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;T 平铺</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;A 关于插件</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>更多信息...</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>对齐和分布</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>对齐</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;R 对应于：</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>先选择</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>后选择</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>页</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>边距</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>选择</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>对象的右边和锚点的左边对齐</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>对象的左边和锚点的右边对齐</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>底部对齐</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>右边对齐</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>对象顶部和锚点底部对齐</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>以垂直轴居中</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>左边对齐</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>以水平轴居中</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>对象底部和锚点顶部对齐</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>顶部对齐</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>分布</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>左边等距分布</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>底部等距分布</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>中间水平等距分布</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>让对象垂直间距相等</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>中间垂直等距分布</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>顶部等距分布</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>右边等距分布</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>参考线</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;S 已选参考线：</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>让对象水平间距相等</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>让对象水平间距和指定值相等</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>让对象垂直间距和指定值相等</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;D 间距：</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>按指定的间距分布项目</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>没有选定</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>某些对象被锁定</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;U 全部解锁</translation>
    </message>
    <message>
        <source>Y: %1%2</source>
        <translation type="unfinished">Y: %1%2</translation>
    </message>
    <message>
        <source>X: %1%2</source>
        <translation type="unfinished">X: %1%2</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>文本左对齐</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>文本右对齐</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>文本居中对齐</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>文本两端对齐</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>文本强制两端对齐</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>域属性</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>类型：</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>按钮</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>文本域</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>勾选框</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>多选框</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>列举框</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>属性</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名称：</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>工具提示：</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>边</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>颜色：</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>宽度：</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>瘦</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>正常</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>宽</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>样式:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>实</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>点</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>下划线</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>斜面</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>凹入</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>其他</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>只读</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>必需</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Don&apos;t 输出值</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>可见度：</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>可见</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>隐蔽</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>不可打印</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>不可查看</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>外观</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>按钮下陷文本</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>鼠标经过文本</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>图标</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>使用图标</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>按下</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>鼠标经过</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>图标位置...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>高亮</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>反转</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>概要</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>推</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>多行</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>密码</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>限制于</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>字符</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>不要滚动</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>不拼写检查</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>勾选样式：</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>勾选</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>十字</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>宝石</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>圆形</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>星星</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>方形</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>默认已勾选</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>可编辑</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>转到</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>提交表单</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>清空表单</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>导入数据</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>事件：</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>鼠标上移</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>鼠标下移</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>鼠标进入</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>鼠标退出</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>聚焦</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>模糊</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>脚本：</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>编辑...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>提交到URL地址：</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>作为HTML来提交数据</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>从...导入数据：</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>目的地</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>到文件：</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>更改...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>页：</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-坐标：</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-坐标：</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>动作</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>域格式化为：</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>纯</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>数字</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>百分数</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>日期</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>时间</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定义</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>数字格式</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>十进制：</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>使用货币符号</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>预设货币符号</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>格式</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>百分数格式</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>日期格式</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>时间格式</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>自定义脚本</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>格式：</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>按键：</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>格式</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>为验校值</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>值必须大于或等于：</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>而且小于或等于：</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation type="unfinished">Custom validate script:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>检验</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>没有计算值</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>值是</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>数字</translation>
    </message>
    <message>
        <source>product</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>average</source>
        <translation>平均</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>最少</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>最多</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>下面域：</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>选择...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>自定义计算脚本：</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>计算</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>在此输入以逗号隔开的域列表</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>你最起码需要用于普通的图标来使用按钮的图标</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>范例：</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>选择更改</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>用于PDF 1.3的字体：</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>选项被PDF 1.3所忽略</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF 文件 (*.pdf);;全部文件 (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation type="unfinished">JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>图像 (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;全部文件 (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>注释属性</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>链接</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>外部链接</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>外部网页链接</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>目的地</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF文件 (*.pdf);;全部文件 (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;T 类型</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;H 更改...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;P 页：</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-坐标：</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-坐标：</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>应用母页</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;M 母页：</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>应用到</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>&amp;P 当前页</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished">Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;偶数页</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished">Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;D 奇数页</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;A 全部页</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;W 在范围内</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation type="unfinished">Alt+W</translation>
    </message>
    <message>
        <source>to</source>
        <translation type="unfinished">to</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Apply the selected template to even, odd or all pages within the following range</source>
        <translation>在以下的范围内应用所选的模板到偶数页、奇数页或者全部页面</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>寄存本</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>寄存本 (*.scs);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>删除</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>对象</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>新项目</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>重命名</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;N 新建</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;L 加载...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>&amp;A 保存为...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;S 小</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;M 中</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;L 大</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;F 文件</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;P 预览</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;N 名称：</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>名称 &quot;%1&quot;非唯一，请另选 
<byte value="x9"/></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>书签</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>移动书签</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>插入书签</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>书签</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>图标位置</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>版面：</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>仅标题</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>仅图标</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>图标在标题上面</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>标题在图标上面</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>标题在图标右边</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>标题在图标左边</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>标题覆盖图标</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>缩放：</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>总是</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>当图标太大</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>从不</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>如何缩放：</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>按比例</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>不按比例</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>图标</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>重置</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>当图标太大</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>System Profiles</source>
        <translation>系统配置文件</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>渲染意图</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>感官</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>相对比色</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>饱和度</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>绝对比色</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>默认页面单色的颜色配置文件</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Colour profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Colour profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation type="unfinished">Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colourimetric or Perceptual should be chosen.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Default rendering intent for your printer. Unless you know why to change it,
Relative Colourimetric or Perceptual should be chosen.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Enable &apos;soft proofing&apos; of how your document colours will print,
based on the chosen printer profile.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Method of showing colours on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Activate Colour Management</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;S 单色：</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;M 监视器：</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;R 打印机</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>&amp;O 监视器</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;I 打印机</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>&amp;U 在监视器上模拟打印机</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>&amp;G 标出超出范围的颜色</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>&amp;B 使用黑点补偿</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB 图像：</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK 图像：</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>导入的CMYK图像的默认颜色配置文件</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>导入的RGB图像的默认颜色配置文件</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>编辑颜色</translation>
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
        <source>Web Safe RGB</source>
        <translation>网页安全 RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation>新</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>旧</translation>
    </message>
    <message>
        <source>C:</source>
        <translation type="unfinished">C:</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="unfinished">M:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">Y:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="unfinished">K:</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>动态颜色条</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>静态颜色条</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="unfinished">R:</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="unfinished">G:</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="unfinished">B:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-颜色表</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;N 名字：</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;M 颜色型号：</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Is Spot Colour</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>是记录颜色</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>你不能生成一个叫 &quot;%1&quot;的颜色。
这个是给透明色预留的名字</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>颜色名称不唯一</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>如果你长按鼠标右键，你能看到一个微缩图。插入键插入一个符号到选择的下面，删除键删除最后插入的符号。</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>选择字符：</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>字体：</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>字符类：</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;I 插入</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>在文本的光标处插入字符</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>删除当前选择</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>关闭对话窗并返回到文本编辑。</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>完全字符集</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>基本拉丁</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>拉丁-1 附录</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>拉丁扩展-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>拉丁扩展-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>常规标点符号</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>上标和下标</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>货币符号</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>类似字母符号</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>数字形式</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>箭头</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>数学运算符</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>方盒绘图</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>区块元素</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>几何图形</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>各种符号</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>装饰符</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>小形状变体</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>连字</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>特别符号</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>希腊字符</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>扩展希腊字符</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>斯拉夫字符</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>斯拉夫字符附录</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>阿拉伯字符</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>阿拉伯字符扩展 A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>阿拉伯字符扩展 B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>希伯来字符</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>符号丢失</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>文本溢出</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>对象不在页面上</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>图像丢失</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>图像的DPI-值低于 %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>对象有透明</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>对象是一个PDF注释或域</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>对象是个嵌入的PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>文档</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>没发现问题</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>页 </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>自由对象</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>发现问题</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>印前检验器</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>项目</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>问题</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>当前配置文件：</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;I 忽略错误</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>选择样式</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>可用样式</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>选择一个目录</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>收集中...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>不能收集所有文件来输出文件：
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>不能收集文件： 
%1</translation>
    </message>
</context>
<context>
    <name>ColorListBox</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>单色的</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>相似的</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>补充的</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>拆分补充的</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>三合一的</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>四分的（双补充的）</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>基本颜色</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>单色亮</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>单色暗</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>第一个相似的</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>第二个相似的</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>第一个拆分</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>第二个拆分</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>第三个拆分</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>第四个拆分</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation type="unfinished">1st. Triadic</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation type="unfinished">2nd. Triadic</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation type="unfinished">1st. Tetradic (base opposite)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation type="unfinished">2nd. Tetradic (angle)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation type="unfinished">3rd. Tetradic (angle opposite)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Color Wheel</source>
        <translation>颜色转盘</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>颜色</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="unfinished">M</translation>
    </message>
    <message>
        <source>Y</source>
        <translation type="unfinished">Y</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="unfinished">K</translation>
    </message>
    <message>
        <source>Select Method:</source>
        <translation>选择方式：</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>角度 (0 - 90 度):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;M 混合颜色</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;R 替代颜色</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Merge created colours into the document colours</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Replace created colours in the document colours</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Leave colours untouched</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Merging colours</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="unfinished">Error: </translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Now opening the colour manager.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Colour Merging</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation type="unfinished">Normal Vision</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Full Colour Blindness</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation type="unfinished">Vision Defect:</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>Cr&amp;eate colour...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Import existing colour...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Merge colours</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>&amp;Replace colours</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="unfinished">E&amp;xit</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>C&amp;olour</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>New Colour</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished">Difference between the selected value and the counted ones. Refer to documentation for more information.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Click the wheel to get the base colour. It is hue in HSV mode.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Sample colour scheme</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Select one of the methods to create a colour scheme. Refer to documentation for more information.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>你所选颜色组合的颜色</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished">Simulate common vision defects here. Select type of the defect.</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Unable to find the requested colour. You have probably selected black, gray or white. There is no way to process this colour.</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Colour %1 exists already!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>颜色 %1 被追加。</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>C&amp;olour Components...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="unfinished">Protanopia (Red)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="unfinished">Deuteranopia (Green)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="unfinished">Tritanopia (Blue)</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;C 颜色转盘...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Colour setting helper</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Colour selector with colour theory included.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;A 应用</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;C 取消</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;O 确认</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation type="unfinished">Horizontal Gradient</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation type="unfinished">Vertical Gradient</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation type="unfinished">Diagonal Gradient</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation type="unfinished">Cross Diagonal Gradient</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation type="unfinished">Radial Gradient</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished">Opacity:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">None</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Shade:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Edit Line Colour Properties</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Edit Fill Colour Properties</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturation of colour</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation type="unfinished">Normal or gradient fill method</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Set the transparency for the colour selected</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation type="unfinished">Free linear Gradient</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation type="unfinished">Free radial Gradient</translation>
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
        <translation type="unfinished"> pt</translation>
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
        <source>Move Vector</source>
        <translation type="unfinished">Move Vector</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation type="unfinished">Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation type="unfinished">CSV Importer Options</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation type="unfinished">Field delimiter:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation type="unfinished">(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation type="unfinished">Value delimiter:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation type="unfinished">First row is a header</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancel</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">None</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation type="unfinished">Printer Options</translation>
    </message>
    <message>
        <source>Option</source>
        <translation type="unfinished">Option</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Value</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation type="unfinished">Page Set</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="unfinished">All Pages</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation type="unfinished">Even Pages only</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation type="unfinished">Odd Pages only</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation type="unfinished">Mirror</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Yes</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation type="unfinished">Orientation</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portrait</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Landscape</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation type="unfinished">N-Up Printing</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation type="unfinished">Page per Sheet</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation type="unfinished">Pages per Sheet</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation type="unfinished">This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation type="unfinished">Encoding:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation type="unfinished">Moves to your Document Directory.
This can be set in the Preferences.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;C 压缩文件</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;I 包括字体</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>设置颜色组成</translation>
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
        <source>HSV</source>
        <translation type="unfinished">HSV</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="unfinished">H:</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="unfinished">S:</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="unfinished">V:</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="unfinished">R:</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="unfinished">G:</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="unfinished">B:</translation>
    </message>
    <message>
        <source>C:</source>
        <translation type="unfinished">C:</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="unfinished">M:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">Y:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="unfinished">K:</translation>
    </message>
    <message>
        <source>Set &amp;RGB</source>
        <translation type="unfinished">Set &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation type="unfinished">Set C&amp;MYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation type="unfinished">Set &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>被用户取消</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>删除颜色</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>删除颜色：</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>替换上：</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>删除页</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>到：</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>删除来自：</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>删除样式</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>删除样式：</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>替换上：</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>没有样式</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>文档信息</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;T 标题：</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;A 作者：</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;K 关键词：</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;P 描述：</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;U 出版人：</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;C 贡献者：</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;E 日期：</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Y 类型：</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;O 格式：</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;F 身份标记</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;S 来源：</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;L 语言：</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;R 关系：</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;V 范围：</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;G 权利：</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>&amp;I 进一步信息</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>负责制作该文档的一个人或组织</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>负责为该文档内容作出贡献的一个人或组织</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>一个和文档存在时的某事件相关联的日期，以ISO 8601标准表示，格式是年－月－日</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>文档内容的性质或流派，例如类别，功能或流派等等</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>一个在特定的上下文里对文档的清晰参考，例如ISBN或URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>一个关于当前文档怎么起源的参考，例如ISBN或URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>一个关于相关文档的参考，兴许使用一个正式的身份标记，例如ISBN或者URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>文档内容的区域或者范围，可能的话包括地点，时间和管辖范围</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>关于在文档里或对文档拥有的权力，例如版权，专利或者商标</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>&amp;T 文档</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>制作此文档内容的主要人或组织。这个项目可以被嵌入到Scribus文档以及PDF文档中的元数据中备考。</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>赋予文档的名称。这个项目可以被嵌入到Scribus文档以及PDF文档中的元数据中备考。</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>文档内容的说明。这个项目是文档的简单描述或者摘要。这个在输出PDF文档时将被嵌入</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>文档内容的主题。这个项目是你想嵌入到PDF文档中的关键词，以便帮助查找和检索PDF文档</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>文档的物理或数码的证明。媒体类型或者尺寸将值得关注。RFC2045，RFC2046在此也有用</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>书写文档内容所用的语言，一般上是可能带横杠后缀和一个ISO-3166国家代码（如en-GB，fr-CH）的ISO-639语言编码</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>添加一个页码区域到文档。这个新区域将被添加到当前选中的区域后面</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>删除当前选中的区域</translation>
    </message>
    <message>
        <source>1, 2, 3, ...</source>
        <translation type="unfinished">1, 2, 3, ...</translation>
    </message>
    <message>
        <source>i, ii, iii, ...</source>
        <translation type="unfinished">i, ii, iii, ...</translation>
    </message>
    <message>
        <source>I, II, III, ...</source>
        <translation type="unfinished">I, II, III, ...</translation>
    </message>
    <message>
        <source>a, b, c, ...</source>
        <translation type="unfinished">a, b, c, ...</translation>
    </message>
    <message>
        <source>A, B, C, ...</source>
        <translation type="unfinished">A, B, C, ...</translation>
    </message>
    <message>
        <source>Name: Optional name for section eg. Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</source>
        <translation>名称：区域的可选名称，例如：Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>文档区域</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>From</source>
        <translation>来自</translation>
    </message>
    <message>
        <source>To</source>
        <translation type="unfinished">To</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>样式</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>开始</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;A 添加</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>显示</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>相关于</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>父级于</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>子级于</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>文本框</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>图片框</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>布尔型</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>整数</translation>
    </message>
    <message>
        <source>String</source>
        <translation>字串</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>文档项目属性</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>数值</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>参数</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>关系</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>有关系于</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>自动添加到</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;A 添加</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;C 复制</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>设置打印机</translation>
    </message>
    <message>
        <source>File</source>
        <translation>文件</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>保存为</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>青</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>洋红</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>黄</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>黑</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>插入一个以逗号隔开的记号列表，这里一个记号可以以*来代表所有页，1-5代表页的范围，或者单独一个页码
</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>打印到</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;O 选项...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;F 文件：</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;H 更改...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;L 另外的打印机命令</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;M 命令：</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>范围</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>&amp;A 全部打印</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>&amp;G 打印当前页</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>&amp;R 打印范围</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;U 打印份数：</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;P 打印</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>普通打印</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>分色打印</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>尽量用彩色打印</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>灰度打印</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript 级别 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript 级别 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript 级别 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>页</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>页水平镜像</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>页垂直镜像</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>设置介质尺寸</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>颜色</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>应用底色清除</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>把Spot Colours 转成 Process Colours</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>应用ICC配置文件</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>高级选项</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>预览...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>设置PostScript 级别.
设置成级别 1或2可能生成巨大文件</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript 文件 (*.ps);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>使用其他打印管理器来使用额外的打印选项，例如kprinter或者gtklp</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Enables Spot Colours to be converted to composite colours. Unless you are planning to print spot colours at a commercial printer, this is probably best left enabled.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Allows you to embed ICC profiles in the print stream when colour management is enabled</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished">This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>导入文件：
%1
失败！</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>致命错误</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>编辑样式</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>字符</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>行距</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation type="unfinished">Name of your paragraph style</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>所选文本或对象的字体</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字体大小</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>文本填充颜色</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>文本轮廓颜色</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished">Determines the overall height, in line numbers, of the Drop Caps</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>段落上方的距离</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>段落下方的距离</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>制表符和缩进</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;N 名称：</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;L 线条：</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>距离</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>固定行距</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>自动行距</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>和基线网格对齐</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>首字下沉</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>到文本距离：</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>预览段落样式</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>确定下沉字和文本的间隔</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>切换这个段落样式的文本范例</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>这个样式的名称非唯一</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;N 新建</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;O 打开...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>&amp;A 另存为...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;S 保存并退出</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;E 不保存退出</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;U 撤销动作</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;R 重复动作</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;T 剪切</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;C 复制</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;P 粘贴</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;G 获取域名称</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;F 文件</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;E 编辑</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;全部文件 (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>图像效果</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>选项：</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>颜色：</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>阴影：</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>亮度：</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>对比：</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>半径：</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>数值：</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>海报效果化：</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>可用效果</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>模糊</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>亮度</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>颜色化</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>对比</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>灰度</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>反转</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>海报效果化</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>锐化</translation>
    </message>
    <message>
        <source>&gt;&gt;</source>
        <translation type="unfinished">&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation type="unfinished">&lt;&lt;</translation>
    </message>
    <message>
        <source>Effects in use</source>
        <translation>使用中效果</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>确认</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;A 全部页</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>更改输出目录</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>输出目录－保存你的图像的地方。输出文件的名称将会是
&apos;文件名-页码.文件类型&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>仅导出当前页</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>可用导出格式</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>选择一个导出目录</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;H 更改...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;E 导出到目录：</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;T 图像类型：</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Q 质量：</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>作为图像导出</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;R 分辨率：</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>范围</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;C 当前页</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation type="unfinished">&amp;Range</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>导出页段</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>插入一个以逗号隔开的记号，这里记号可以是*以代表所有页，1-5代表页范围或者一个单独的页码。
</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>输出所有页</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>图像的分辨率。
如果图像用于屏幕显示，用72 dpi</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>图像的质量－100%为最好，1%为最低质量</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;S 尺寸</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>图像尺寸。100%为没有更改，200%是两倍大，以此类推</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>图像扩展属性</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>更暗</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>更亮</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>色调</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>饱和度</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>颜色</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>亮度</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>倍增</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>屏幕</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>溶解</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>重叠</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>强光</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>柔光</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>差分</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>排除</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>颜色模糊</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>颜色焦灼</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>混合模式：</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>透明度：</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>层</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>不用任何路径</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>路径</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>排除</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>尺寸</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>标题：</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>无标题</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>作者</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus文档</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>分辨率</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation type="unfinished">DPI</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>灰度</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>颜色模式：</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>这个文件使用的一些字体被替代为：</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>已被替代成：</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>可用字体</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>字体替代</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>其他路径</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>字体名称</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>替代</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>选择一个目录</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;A 可用字体</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;S 字体替代</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;P 其他路径</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;H 更改...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;D 添加...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;R 移除</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>字体名称</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>使用字体</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>嵌入在：</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>子集</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>字体文件的路径</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished">PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation>字体搜索路径只能在首选项中设定，而且要在没有打开任何文档的情况下。关闭所有文档，然后用编辑－设置来更改字体搜索路径。</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>字体名称</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation>文件</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation>类型</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>子集</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="unfinished">Access</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation>字体尺寸：</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation>字体预览</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation>快速查找：</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;S 查找</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>追加选中的字体到样式，字体菜单</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Leave 预览</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation>在此输入字以便快速查找字体名称。例如&apos;粗体&apos;显示所带有粗体的名称。查找大小写有别</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>开始查找</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>所选字体的尺寸</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>用户</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>系统</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;A 追加</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;F 字体预览...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>字体预览窗口</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>排列，查找和浏览可用的字体。</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>字体替代</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>原始字体</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>替代字体</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>让这些替代永久有效</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>这个文档包括一些你的系统没有安装的字体，请为它们选择合适的替代字体。按取消将停止载入文档。</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>取消这些字体替代并停止载入文档。</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>启用这个就告诉Scribus永久性地在以后的版面使用替代字体来代替不存在的字体。这个可以在编辑－首选项－字体中被改变或逆转。</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>如果你选择确认然后保存，这些替代将永久保留到文档中。</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>位置：</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>这里你可以增加，更改或者删除颜色节点</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>管理参考线</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>水平参考线</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>垂直参考线</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-坐标:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;A 添加</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;E 删除</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-坐标：</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;D 添加</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;L 删除</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;L 锁定参考线</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;R 行：</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;O 列：</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>参考：</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;P 页</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;M 页边距</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;S 选择</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>行和列－自动参考线</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>&amp;G 行距</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>&amp;N 列距</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>&amp;U 更新</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>在文档设置参考线。参考线管理器还是打开状态，但是更改是延续的</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>&amp;A 应用到所有页面</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>参考线</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Preview</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>对不起，没有使用手册可用！请访问： http://docs.scribus.net 来获得最新文档，如果要下载，请访问
www.scribus.net 。</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>内容</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>链接</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus在线帮助</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;C 内容</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;S 查找</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>查找</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>查找条件：</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>&amp;A 查找</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;N 新建</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>&amp;L 全部删除</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;M 书签</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;P 打印...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;X 退出</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>查找大小写有别</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>新书签</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>新书签标题：</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;F 文件</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;F 查找...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>&amp;N 查找下一个</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>&amp;P 查找上一个</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;E 编辑</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;A 添加书签</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>&amp;E 全部删除</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;B 书签</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>可能连字符</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>接受</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>跳过</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>需要用连字符的最小的词的长度。</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation type="unfinished">Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;L 语言：</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;S 最小字：</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;H 连字符提示</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>&amp;D 输入时自动添加连字符到文本</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished">A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation type="unfinished">Enables automatic hyphenation of your text while typing.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation type="unfinished">Consecutive Hyphenations &amp;Allowed:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation type="unfinished">Image Info</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation type="unfinished">General Info</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation type="unfinished">Date / Time:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation type="unfinished">Has Embedded Profile:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Yes</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation type="unfinished">Profile Name:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation type="unfinished">Has Embedded Paths:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation type="unfinished">Has Layers:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation type="unfinished">EXIF Info</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation type="unfinished">Artist:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation type="unfinished">Comment:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation type="unfinished">User Comment:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation type="unfinished">Camera Model:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation type="unfinished">Camera Manufacturer:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="unfinished">Description:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished">Copyright:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation type="unfinished">Scanner Model:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation type="unfinished">Scanner Manufacturer:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished">Import &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation type="unfinished">Imports EPS Files</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished">Imports most EPS files into the current document,
converting their vector data into Scribus objects.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished">PostScript</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation type="unfinished">Insert Page</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="unfinished">before Page</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="unfinished">after Page</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="unfinished">at End</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>页面</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;I 插入</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;M 母页：</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>页面尺寸</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;S 尺寸：</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定义</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;N 文档方向：</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>竖式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>横式</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;W 宽度：</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;H 高度：</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>对象和它们的页一起移动</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>母页</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>插入表格</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>行数：</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>列数：</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>新脚本</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>编辑JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;E 编辑...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;A 添加...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;N 新脚本：</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;N 否</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Y 是</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>你真的要删除这个脚本吗？</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>动作</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>当前键位</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>为这个动作选择一个键位</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation type="unfinished">Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="unfinished">Ctrl</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="unfinished">Shift</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="unfinished">Shift+</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation type="unfinished">Alt+</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation type="unfinished">Ctrl+</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;N 没键</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;U 用户自定义键</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;K 设置键</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>可载入快捷键组合</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;L 载入</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;I 导入...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;E 导出...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;R 重置</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>可载入快捷键组合</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>载入选中的快捷键组合</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>导入一个快捷键组合到当前配置中</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>导出当前快捷键到一个可被导入的文件中</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>重新载入默认的Scribus快捷键</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>键位组合 XML 文件 (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>这个键位组合已经被使用</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="unfinished">Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished">Meta+</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>层</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>删除层</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>你确信一并删除此层上的所有对象吗？</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>增加层</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>删除层</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>升高层</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>降低层</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>编辑线条样式</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>复件 %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>新样式</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>文件 (*.sla *.sla.gz *.scd *.scd.gz);;所有文件 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>文件 (*.sla *.scd);;所有文件 (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;N 新建</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;E 编辑</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;U 复制</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;N 否</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Y 是</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;I 导入</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>你真的要删除此样式吗？</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>所有文件 (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>选择 Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>作者：</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>获得更多：</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML 文件：</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>段落：</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Margin Guides</source>
        <translation>页边距参考线</translation>
    </message>
    <message>
        <source>Manage Page Properties</source>
        <translation>管理页属性</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>页面尺寸</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;S 尺寸：</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定义</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;N 页面方向</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>竖式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>横式</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;W 宽度：</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;H 高度：</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>把对象和页一起移动</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>类型：</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;B 底部：</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;T 顶部：</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;R 右边：</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;L 左边：</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>顶部边距线到页边的距离</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>底部边距线到页边的距离</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;I 里面：</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;U 外面：</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>预设版面：</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>应用页边距设置到所有页面</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>应用页边距设置到文档中现存的所有页面</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>左边页边距线到页边的距离。如果对面页被选择，这个距离可以用来获得正确的装订边距。</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>右边页边距线到页边的距离。如果对面页被选择，这个距离可以用来获得正确的装订边距。</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>编辑母页</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>你确认要删除这个母页吗？</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;N 否</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Y 是</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;N 名称：</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>新建母页</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>复件 %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名称：</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>新建母页</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>复件 #%1 </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>复制当前选中的母页</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>删除当前选中的母页</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>增加一个新的母页</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>从另外一个文件导入母页</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>多重复制</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;N 复件数：</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;H 水平提升：</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;V 垂直提升：</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>距离</translation>
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
        <translation type="unfinished">DX:</translation>
    </message>
    <message>
        <source>DY:</source>
        <translation type="unfinished">DY:</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation>角度：</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>长度：</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>文档 (*.sla *.sla.gz *.scd *.scd.gz);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>文档 (*.sla *.scd);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>输入页</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> 来自 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>创建页</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> 来自 %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>导入母页</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;F 来自文档：</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>&amp;G 更改...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;I 导入页：</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;I 导入母页</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>页之前</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>页之后</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>最后</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;I 导入</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>插入野鸽以逗号隔开的标记，这里一个记号可以是*来表示全部也，1-5表示页范围，或者输入单个页码。</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>丢失字体</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>字体 %1 没有安装。</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>使用</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>代以</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>移动页</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>复制页</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>移动页：</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>移动页</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>页之前</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>页之后</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>最后</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>到：</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>复件数量：</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>属性</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>排列</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>基点</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>高低</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>形状：</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>文本距离</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>显示曲线</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>开始位移：</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>到曲线的距离：</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>输入配置文件：</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>渲染意图：</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>知觉</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>相对比色</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>饱和度</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>绝对比色</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>左点</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>尾点</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>尖角连接</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>斜角连接</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>圆角连接</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>平头</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>方头</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>圆头</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>没有样式</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字体大小</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>行距</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>所选对象名称</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>当前基点的水平位置</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>当前基点的垂直位置</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>宽度</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>高度</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>在当前基点旋转对象</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>表示尺寸和旋转角度的点</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>选择左上为基点</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>选择右上为基点</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>选择左下为基点</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>选择右下为基点</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>选择中间为基点</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>水平翻转</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>垂直翻转</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>上升一级</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>下降一级</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>移到最上面</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>移到最下面</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>锁定或解锁对象</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation type="unfinished">Lock or unlock the size of the object</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation type="unfinished">Enable or disable printing of the object</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation type="unfinished">Font of selected text or object</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished">Scaling width of characters</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturation of colour of text stroke</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturation of colour of text fill</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation type="unfinished">Style of current paragraph</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation type="unfinished">Change settings for left or end points</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>线条样式</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>线条粗细</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>线条类型</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>线条端类型</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation type="unfinished">Line style of current object</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation type="unfinished">Choose the shape of frame...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation type="unfinished">Edit shape of the frame...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation type="unfinished">Set radius of corner rounding</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation type="unfinished">Number of columns in text frame</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation type="unfinished">Distance between columns</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation type="unfinished">Distance of text from top of frame</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation type="unfinished">Distance of text from bottom of frame</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation type="unfinished">Distance of text from left of frame</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation type="unfinished">Distance of text from right of frame</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation type="unfinished">Edit tab settings of text frame...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation type="unfinished">Allow the image to be a different size to the frame</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation type="unfinished">Horizontal offset of image within frame</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation type="unfinished">Vertical offset of image within frame</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation type="unfinished">Resize the image horizontally</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation type="unfinished">Resize the image vertically</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation type="unfinished">Keep the X and Y scaling the same</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation type="unfinished">Make the image fit within the size of the frame</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation type="unfinished">Use image proportions rather than those of the frame</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation type="unfinished">Cell Lines</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation type="unfinished">Line at Top</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation type="unfinished">Line at the Left</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation type="unfinished">Line at the Right </translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation type="unfinished">Line at Bottom</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation type="unfinished">Keep the aspect ratio</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation type="unfinished">Source profile of the image</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation type="unfinished">Rendering intent for the image</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation type="unfinished">Path Text Properties</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation type="unfinished">Indicates the level the object is on, 0 means the object is at the bottom</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="unfinished">Make text in lower frames flow around the object shape</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation type="unfinished">Switches between Gap or Column width</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>列宽</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation type="unfinished">X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;S 形状</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;T 文本</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;I 图像</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;L 线条</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;C 颜色</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-坐标：</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-坐标：</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;W 宽度：</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;H 高度：</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;R 旋转：</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;E 编辑形状...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>&amp;O 圆角
</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;M 列：</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;G 间距：</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;P 顶：</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;B 底：</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;L 左：</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;R 右：</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;A 制表符...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>&amp;F 字体环绕框</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>&amp;B 使用限制方框</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;U 使用轮廓线条</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>&amp;Y 样式：</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;G 语言：</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;F 自由缩放</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-&amp;A 缩放：</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-&amp;E 缩放：</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation type="unfinished">Scale &amp;To Frame Size</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation type="unfinished">P&amp;roportional</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation type="unfinished">&amp;Basepoint:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation type="unfinished">T&amp;ype of Line:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">Line &amp;Width:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation type="unfinished">Ed&amp;ges:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation type="unfinished">&amp;Endings:</translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation type="unfinished">&amp;X1:</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation type="unfinished">X&amp;2:</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation type="unfinished">Y&amp;1:</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation type="unfinished">&amp;Y2:</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="unfinished">Use a surrounding box instead of the frame&apos;s shape for text flow</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="unfinished">Use a second line originally based on the frame&apos;s shape for text flow</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="unfinished">Hyphenation language of frame</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation type="unfinished">Right to Left Writing</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished">Manual Tracking</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation type="unfinished">Start Arrow:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation type="unfinished">End Arrow:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished">Offset to baseline of characters</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished">Scaling height of characters</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished">Fixed Linespacing</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished">Automatic Linespacing</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished">Align to Baseline Grid</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation type="unfinished">Actual X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation type="unfinished">Actual Y-DPI:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished">Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation>Colour of text stroke. Only available with &quot;outline&quot; text decoration.</translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation>Colour of text fill. Only available with &quot;outline&quot; text decoration.</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation type="unfinished">Edit Style</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation type="unfinished">Flat Cap</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation type="unfinished">Square Cap</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation type="unfinished">Round Cap</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Mitre Join</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation type="unfinished">Bevel Join</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation type="unfinished">Round Join</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation type="unfinished">Line Width:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="unfinished"> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished">Solid Line</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">Dashed Line</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">Dotted Line</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">Dash Dot Line</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">Dash Dot Dot Line</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished">Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation type="unfinished">Progress</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation type="unfinished">Overall Progress:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Cancel</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation type="unfinished">My &amp;Plugin</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation type="unfinished">Scribus - My Plugin</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation type="unfinished">The plugin worked!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>新建文档</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>页面大小</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>竖式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>横式</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>页边距参考线</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>文档的页面大小，或是标准尺寸或者是自定义尺寸</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>文档页面的方向</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>文档的页面宽度，如果你选择了一个自定义的页面尺寸，它是可以编辑的</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>文档的页面高度，如果你选择了一个自定义的页面尺寸，它是可以编辑的</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>文档的起始页码</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>文档编辑默认所使用的尺寸单位</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>当新页被添加时自动创建文本框</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>自动创建的列的列距</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>自动创建的文本框里的列数</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;S 尺寸：</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;N 页面方向：</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;W 宽度：</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;H 高度：</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>&amp;I 起始页码：</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;D 默认单位：</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;A 自动文本框</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;G 间隔：</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;M 列：</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>不再显示此对话</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>文档起始页数</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;U 页数：</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;N 新建文档</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>&amp;E 打开现有文档</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>&amp;D 打开最近文档</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>&amp;F 从模板新建...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>以预设的版面载入文档</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>从一个别人或自己创建的模板中开创文档。（例如，如果你的文档有固定的样式）</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>节点</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>移动节点</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>移动控制点</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>添加节点</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>删除节点</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>重置控制点</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>重置这个节点</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;A 绝对坐标</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-坐标：</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-坐标：</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>&amp;C 编辑轮廓线</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;R 重置轮廓线</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;E 结束编辑</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>独立移动控制点</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>对称移动控制点</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>打开一个多边形或剪切齐贝尔曲线</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>闭合这个齐贝尔曲线</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation type="unfinished">Mirror the Path Horizontally</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation type="unfinished">Mirror the Path Vertically</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished">Shear the Path Horizontally to the Left</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished">Shear the Path Vertically Up</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished">Shear the Path Vertically Down</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation type="unfinished">Rotate the Path Counter-Clockwise</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation type="unfinished">Rotate the Path Clockwise</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="unfinished">Reduce the Size of the Path by shown %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation type="unfinished">Enlarge the Size of the Path by shown %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation type="unfinished">Angle of Rotation</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="unfinished">% to Enlarge or Reduce By</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation type="unfinished">Activate Contour Line Editing Mode</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation type="unfinished">Reset the Contour Line to the Original Shape of the Frame</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="unfinished">Shear the Path Horizontally to the Right</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished">When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished">Import &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation type="unfinished">Imports OpenOffice.org Draw Files</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished">Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</translation>
    </message>
    <message>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished">OpenDocument 1.0 Draw</translation>
    </message>
    <message>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished">OpenOffice.org 1.x Draw</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished">Use document name as a prefix for paragraph styles</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation type="unfinished">Do not ask again</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished">Enabling this will overwrite existing styles in the current Scribus document</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished">Merge Paragraph Styles</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished">Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished">Prepend the document name to the paragraph style name in Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation type="unfinished">Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished">Overwrite Paragraph Styles</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation type="unfinished">OpenDocument Importer Options</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancel</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation type="unfinished">Scribus Document</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished">Scribus 1.2.x Document</translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished">Linewidth</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
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
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation type="unfinished">Saving PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation type="unfinished">Exporting Master Pages:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished">Exporting Pages:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation type="unfinished">Exporting Items on Current Page:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>打印预览</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>显示文档中的透明度和透明项目。需要Ghostscript 7.07或更高</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>不用RGB颜色而以CMYK墨水来模拟打印预览</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>启用/停用 C （青）墨水板</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>启用/停用 M （洋红）墨水板</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>启用/停用 Y （黄）墨水板</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>启用/停用 K （黑）墨水板</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>&amp;T 反锯齿文本</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>&amp;T 反锯齿图像</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>&amp;P 显示透明度</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;D 显示CMYK</translation>
    </message>
    <message>
        <source>&amp;C</source>
        <translation type="unfinished">&amp;C</translation>
    </message>
    <message>
        <source>&amp;M</source>
        <translation type="unfinished">&amp;M</translation>
    </message>
    <message>
        <source>&amp;Y</source>
        <translation type="unfinished">&amp;Y</translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation type="unfinished">&amp;K</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;U 清除底色</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>分离名称</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>青</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>洋红</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>黄</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>黑</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>伸缩：</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>打印...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="unfinished">Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished">Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>A way of switching off some of the grey shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the grey. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>调整页面的伸缩比例</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <source>File</source>
        <translation>文件</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>正处理母页中：</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>导出页面：</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>图像</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>线条</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>多边形</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>线套</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>路径文本</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>复件</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>相对于</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>是...的父级</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>是...的子级</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>页面项目属性</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>数值</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>参数</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished">Relationship</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished">Relationship To</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;A 添加</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;C 复制</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;O 确认</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;C 取消</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>页面排版</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>第一页是：</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>双面</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>中右</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished">Drag pages or master pages onto the trashbin to delete them</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished">Here are all your master pages. To create a new page, drag a master page to the page view below</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="unfinished">Arrange Pages</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished">Available Master Pages:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="unfinished">Document Pages:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %2</source>
        <translation type="unfinished">%1 of %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation type="unfinished">%1 of %1</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation type="unfinished">Quarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="unfinished">Foolscap</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Letter</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation type="unfinished">Government Letter</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished">Ledger</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished">Executive</translation>
    </message>
    <message>
        <source>Post</source>
        <translation type="unfinished">Post</translation>
    </message>
    <message>
        <source>Crown</source>
        <translation type="unfinished">Crown</translation>
    </message>
    <message>
        <source>Large Post</source>
        <translation type="unfinished">Large Post</translation>
    </message>
    <message>
        <source>Demy</source>
        <translation type="unfinished">Demy</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Medium</translation>
    </message>
    <message>
        <source>Royal</source>
        <translation type="unfinished">Royal</translation>
    </message>
    <message>
        <source>Elephant</source>
        <translation type="unfinished">Elephant</translation>
    </message>
    <message>
        <source>Double Demy</source>
        <translation type="unfinished">Double Demy</translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation type="unfinished">Quad Demy</translation>
    </message>
    <message>
        <source>STMT</source>
        <translation type="unfinished">STMT</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="unfinished">A</translation>
    </message>
    <message>
        <source>B</source>
        <translation type="unfinished">B</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="unfinished">D</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="unfinished">E</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation type="unfinished">Result</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="unfinished">Search Results for: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Preview</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="unfinished">Select</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancel</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Goto</source>
        <translation type="unfinished">Goto</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Yes</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation type="unfinished">Missing</translation>
    </message>
    <message>
        <source>Search</source>
        <translation type="unfinished">Search</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Name</translation>
    </message>
    <message>
        <source>Path</source>
        <translation type="unfinished">Path</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Page</translation>
    </message>
    <message>
        <source>Print</source>
        <translation type="unfinished">Print</translation>
    </message>
    <message>
        <source>Status</source>
        <translation type="unfinished">Status</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation type="unfinished">Cancel Search</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Manage Pictures</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation type="unfinished">Scribus - Image Search</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation type="unfinished">The search failed: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation type="unfinished">No images named &quot;%1&quot; were found.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation type="unfinished">Select a base directory for search</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished">Save as &amp;Image...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation type="unfinished">Export As Image</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation type="unfinished">Exports selected pages as bitmap images.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">Cannot find plugin</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">unknown error</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">Cannot find symbol (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">Plugin: loading %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation type="unfinished">init failed</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation type="unfinished">unknown plugin type</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">Plugin: %1 loaded</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">Plugin: %1 failed to load: %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation type="unfinished">Plugin Manager</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation type="unfinished">Plugin</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation type="unfinished">How to run</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Type</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation type="unfinished">Load it?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation type="unfinished">Plugin ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">File</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Yes</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="unfinished">You need to restart the application to apply the changes.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation type="unfinished">Polygon Properties</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished">Corn&amp;ers:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotation:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished">Apply &amp;Factor</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Factor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Number of corners for polygons</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Degrees of rotation for polygons</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished">Apply Convex/Concave Factor to change shape of Polygons</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Sample Polygon</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished">A negative value will make the polygon concave (or star shaped), a positive value will make it convex</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation type="unfinished">Preferences</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">General</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Document</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Guides</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Typography</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Tools</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">Scrapbook</translation>
    </message>
    <message>
        <source>Display</source>
        <translation type="unfinished">Display</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="unfinished">GUI</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Paths</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Page Size</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Custom</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portrait</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Landscape</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Margin Guides</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Autosave</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="unfinished">Other Options</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Preview</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="unfinished">Small</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Medium</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Choose a Directory</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation type="unfinished">External Tools</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="unfinished">Image Processing Tool</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="unfinished">Printing</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished">Default font size for the menus and windows</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished">Default unit of measurement for document editing</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished">Number of lines Scribus will scroll for each move of the mouse wheel</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished">Number of recently edited documents to show in the File menu</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="unfinished">Default documents directory</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished">Default Scripter scripts directory</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished">Default page size, either a standard size or a custom size</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="unfinished">Default orientation of document pages</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Width of document pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Height of document pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="unfinished">Time period between saving automatically</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation type="unfinished">Choose the size of the preview in the scrapbook palette</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Colour for paper</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Mask the area outside the margins in the margin colour</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="unfinished">Set the default zoom level</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="unfinished">Antialias text for EPS and PDF onscreen rendering</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="unfinished">Antialias graphics for EPS and PDF onscreen rendering</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="unfinished">Do not show objects outside the margins on the printed page or exported file</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="unfinished">Save the scrapbook contents everytime after a change</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished">&amp;Theme:</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished">&amp;Wheel Jump:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished">&amp;Recent Documents:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished">&amp;Documents:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Change...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished">&amp;ICC Profiles:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">C&amp;hange...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;S 脚本：</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished">Ch&amp;ange...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Size:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntation:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Width:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Height:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Bottom:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Right:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Left:</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Interval:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="unfinished">Sa&amp;ve Contents on Changes</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="unfinished">Large</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Display &amp;Unprintable Area in Margin Colour</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="unfinished">&amp;Adjust Display Size</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished">&amp;Name of Executable:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished">Antialias &amp;Text</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished">Antialias &amp;Graphics</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished">Name of &amp;Executable:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="unfinished">Clip to Page &amp;Margins</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Apply &amp;Under Colour Removal</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">Cha&amp;nge...</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Language:</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="unfinished">Document T&amp;emplates:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished">Units:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="unfinished">Undo/Redo</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished">Action history length</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished">Hyphenator</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="unfinished">Fonts</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Colour Management</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation type="unfinished">PDF Export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished">Document Item Attributes</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">Table of Contents and Indexes</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation type="unfinished">Keyboard Shortcuts</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished">Page Display</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Colour:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Show Pictures</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Show Text Chains</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Show Frames</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished">Scratch Space</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation type="unfinished">Miscellaneous</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation type="unfinished">Plugins</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">Turns the display of frames on or off</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">Turns the display of pictures on or off</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished">Additional directory for document templates</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="unfinished">Filesystem location for the Ghostscript interpreter</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished">Always ask before fonts are replaced when loading a document</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished">Show Text Control Characters</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished">Preview of current Paragraph Style visible when editing Styles</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished">To adjust the display drag the ruler below with the slider.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Display non-printing characters such as paragraph markers in text frames</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished">Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished">Preflight Verifier</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation type="unfinished">dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished">Resolution:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished">Always use standard Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished">Count of the Paragraphs:</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">Rulers relative to Page</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished">Gaps between Pages</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished">Horizontal:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished">Vertical:</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished">Show Startup Dialog</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation type="unfinished">Locate Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished">Locate your image editor</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished">PostScript Interpreter</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished">Enable or disable  the display of linked frames.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished">Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished">&amp;Font Size (Menus):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished">Font Size (&amp;Palettes):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished">Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished">Default font size for the tool windows</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished">Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished">When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished">Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished">File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation type="unfinished">&amp;Defaults</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation type="unfinished">Save...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation type="unfinished">Save Preferences</translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
    <message>
        <source>PDF 1.3</source>
        <translation type="unfinished">PDF 1.3</translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="unfinished">PDF 1.4</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="unfinished">PDF/X-3</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation type="unfinished">Migrate Old Scribus Settings?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="unfinished">Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>单页</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation>双页</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>左页</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>右页</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3折</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>中间</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4折</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>中左</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>中右</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished">PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation type="unfinished">Could not open preferences file &quot;%1&quot; for writing: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation type="unfinished">Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation type="unfinished">Failed to open prefs file &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation type="unfinished">Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation>错误写入首选项</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation type="unfinished">Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation type="unfinished">Error Loading Preferences</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation type="unfinished">Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>杂志</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>斐波纳契</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation type="unfinished">Golden Mean</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation type="unfinished">Nine Parts</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation type="unfinished">Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished">You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;O 打开...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;E 退出</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;F 文档</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;R 运行</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;S 保存输出...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;S 脚本</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>脚本控制台</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation type="unfinished">Write your commands here. A selection is processed as script</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation type="unfinished">Output of your script</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="unfinished">Python Scripts (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished">Save the Python Commands in File</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Warning</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished">Text Files (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished">Save Current Output</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Save &amp;As...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation type="unfinished">Run As &amp;Console</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation type="unfinished">Scribus Python Console</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="unfinished">This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation type="unfinished">Open Python Script File</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation type="unfinished">Hu&amp;e:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation type="unfinished">&amp;Sat:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation type="unfinished">&amp;Val:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation type="unfinished">&amp;Red:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation type="unfinished">&amp;Green:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation type="unfinished">Bl&amp;ue:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation type="unfinished">A&amp;lpha channel:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Basic colours</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Custom colours</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Define Custom Colours &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancel</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Add to Custom Colours</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Select colour</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation type="unfinished">Copy or Move a File</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation type="unfinished">Read: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation type="unfinished">Write: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation type="unfinished">File &amp;name:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation type="unfinished">File &amp;type:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation type="unfinished">One directory up</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancel</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">All Files (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Name</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">Size</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Type</translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="unfinished">Date</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation type="unfinished">Attributes</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation type="unfinished">Look &amp;in:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation type="unfinished">Back</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation type="unfinished">Create New Folder</translation>
    </message>
    <message>
        <source>List View</source>
        <translation type="unfinished">List View</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation type="unfinished">Detail View</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation type="unfinished">Preview File Info</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation type="unfinished">Preview File Contents</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation type="unfinished">Read-write</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation type="unfinished">Read-only</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation type="unfinished">Write-only</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation type="unfinished">Inaccessible</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation type="unfinished">Symlink to File</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation type="unfinished">Symlink to Directory</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation type="unfinished">Symlink to Special</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">File</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation type="unfinished">Dir</translation>
    </message>
    <message>
        <source>Special</source>
        <translation type="unfinished">Special</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Open</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Save As</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation type="unfinished">&amp;Open</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="unfinished">&amp;Rename</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Delete</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation type="unfinished">R&amp;eload</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation type="unfinished">Sort by &amp;Name</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation type="unfinished">Sort by &amp;Size</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation type="unfinished">Sort by &amp;Date</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation type="unfinished">&amp;Unsorted</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation type="unfinished">Sort</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation type="unfinished">Show &amp;hidden files</translation>
    </message>
    <message>
        <source>the file</source>
        <translation type="unfinished">the file</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation type="unfinished">the directory</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation type="unfinished">the symlink</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation type="unfinished">Delete %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation type="unfinished">&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Yes</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;N 否</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation type="unfinished">New Folder 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation type="unfinished">New Folder</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation type="unfinished">New Folder %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation type="unfinished">Find Directory</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation type="unfinished">Directories</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="unfinished">Save</translation>
    </message>
    <message>
        <source>Error</source>
        <translation type="unfinished">Error</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation type="unfinished">%1
File not found.
Check path and filename.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation type="unfinished">All Files (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation type="unfinished">Select a Directory</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation type="unfinished">Directory:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished">&amp;Font</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation type="unfinished">Font st&amp;yle</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Size</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Effects</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation type="unfinished">Stri&amp;keout</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="unfinished">&amp;Underline</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Colour</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation type="unfinished">Sample</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation type="unfinished">Scr&amp;ipt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Apply</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancel</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Close</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation type="unfinished">Select Font</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation type="unfinished">Clear</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation type="unfinished">Select All</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Undo</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Redo</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Cu&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copy</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Paste</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation type="unfinished">Line up</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Customise...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Initialising...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Background</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Warning</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation type="unfinished">Do you really want to overwrite the File:
%1 ?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Open</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Save as</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation type="unfinished">SVG-Images (*.svg *.svgz);;All Files (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation type="unfinished">SVG-Images (*.svg);;All Files (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Yes</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="unfinished">Save as Image</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished">Error writing the output file(s).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="unfinished">Export successful.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="unfinished">File exists. Overwrite?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished">exists already. Overwrite?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="unfinished">Yes all</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished">All Supported Formats (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">All Files (*)</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation type="unfinished">Newsletters</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation type="unfinished">Brochures</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Catalogues</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation type="unfinished">Flyers</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation type="unfinished">Signs</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation type="unfinished">Cards</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation type="unfinished">Letterheads</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation type="unfinished">Envelopes</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation type="unfinished">Business Cards</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation type="unfinished">Calendars</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation type="unfinished">Advertisements</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation type="unfinished">Labels</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="unfinished">Menus</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation type="unfinished">Programs</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation type="unfinished">PDF Forms</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation type="unfinished">PDF Presentations</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation type="unfinished">Magazines</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation type="unfinished">Posters</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation type="unfinished">Announcements</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation type="unfinished">Text Documents</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation type="unfinished">Folds</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation type="unfinished">Own Templates</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation type="unfinished">Importing text</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="unfinished">All Supported Formats</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation type="unfinished">HTML Files</translation>
    </message>
    <message>
        <source>html</source>
        <translation type="unfinished">html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation type="unfinished">Text Files</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation type="unfinished">Comma Separated Value Files</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation type="unfinished">CSV_data</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation type="unfinished">CSV_header</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation type="unfinished">Font %1 is broken, discarding it</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
External Links</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation type="unfinished">Text Filters</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished">Media Cases</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation type="unfinished">Albanian</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation type="unfinished">Basque</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="unfinished">Bulgarian</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation type="unfinished">Brazilian</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="unfinished">Catalan</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished">Chinese</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Czech</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Danish</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Dutch</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">English</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation type="unfinished">English (British)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation type="unfinished">Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="unfinished">German</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Finnish</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">French</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished">Galician</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Greek</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Hungarian</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished">Indonesian</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Italian</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished">Korean</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Lithuanian</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation type="unfinished">Norwegian (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation type="unfinished">Norwegian (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation type="unfinished">Norwegian</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="unfinished">Polish</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Russian</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Swedish</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Spanish</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished">Spanish (Latin)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Slovak</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Slovenian</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation type="unfinished">Serbian</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get font size of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get font of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get text size of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get column count of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get line space of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get column gap of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot get text of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set text of non-text frame.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot insert text into non-text frame.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished">Alignment out of range. Use one of the scribus.ALIGN* constants.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished">Selection index out of bounds</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished">Unit out of range. Use one of the scribus.UNIT_* constants.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Target is not an image frame.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished">Corner radius must be a positive number.</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Cannot get a colour with an empty name.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Cannot change a colour with an empty name.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Cannot create a colour with an empty name.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Cannot delete a colour with an empty name.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Cannot replace a colour with an empty name.</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="unfinished">OpenOffice.org Draw (*.sxd);;All Files (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation type="unfinished">OpenOffice.org Writer Documents</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Colour not found - python error</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Custom (optional) configuration: </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Standard configuration: </translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Short Words processing. Wait please...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Short Words processing. Done.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="unfinished">Afrikaans</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation type="unfinished">Turkish</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation type="unfinished">Ukranian</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation type="unfinished">Welsh</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished">The filename must be a string.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot delete image type settings.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished">The image type must be a string.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation type="unfinished">&apos;allTypes&apos; attribute is READ-ONLY</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation type="unfinished">Failed to export image</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Colour not found.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Colour not found in document.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Colour not found in default colours.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot scale by 0%.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Specified item not an image frame.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation type="unfinished">Font not found.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot render an empty sample.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot have an empty layer name.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation type="unfinished">Layer not found.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot remove the last layer.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot create layer without a name.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished">Insert index out of bounds.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set text alignment on a non-text frame.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation type="unfinished">Font size out of bounds - must be 1 &lt;= size &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set font size on a non-text frame.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set font on a non-text frame.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation type="unfinished">Line space out of bounds, must be &gt;= 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set line spacing on a non-text frame.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation type="unfinished">Column gap out of bounds, must be positive.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set column gap on a non-text frame.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation type="unfinished">Column count out of bounds, must be &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set number of columns on a non-text frame.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot select text in a non-text frame</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot delete text from a non-text frame.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set text fill on a non-text frame.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set text stroke on a non-text frame.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set text shade on a non-text frame.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation type="unfinished">Can only link text frames.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation type="unfinished">Target frame must be empty.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Target frame links to another frame.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Target frame is linked to by another frame.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation type="unfinished">Source and target are the same object.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot unlink a non-text frame.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot convert a non-text frame to outlines.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished">Can&apos;t set bookmark on a non-text frame</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished">Can&apos;t get info from a non-text frame</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation type="unfinished">OpenDocument Text Documents</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="unfinished">Croatian</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="unfinished">Portuguese</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished">Portuguese (BR)</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Custom</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Page</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation type="unfinished">Master Page </translation>
    </message>
    <message>
        <source>4A0</source>
        <translation type="unfinished">4A0</translation>
    </message>
    <message>
        <source>2A0</source>
        <translation type="unfinished">2A0</translation>
    </message>
    <message>
        <source>Comm10E</source>
        <translation type="unfinished">Comm10E</translation>
    </message>
    <message>
        <source>DLE</source>
        <translation type="unfinished">DLE</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation type="unfinished">Output stream not writeable</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation type="unfinished">Verification of settings failed: %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation type="unfinished">Unable to read settings XML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">%1 (line %2 col %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation type="unfinished">Unable to read settings XML: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">null root node</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">&lt;pdfVersion&gt; invalid</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">found %1 &lt;%2&gt; nodes, need 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">unexpected null &lt;%2&gt; node</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">node &lt;%1&gt; not an element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">element &lt;%1&gt; lacks `value&apos; attribute</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">element &lt;%1&gt; value must be `true&apos; or `false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation type="unfinished">Font %1 (found using fontconfig) is broken, discarding it</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation type="unfinished">Scribus Development Version</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"> p</translation>
    </message>
    <message>
        <source> cm</source>
        <translation type="unfinished"> cm</translation>
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
        <source>cm</source>
        <translation type="unfinished">cm</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="unfinished">Points (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Millimetres (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Inches (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Picas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimetres (cm)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation type="unfinished">File exists</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">&amp;Replace</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">All</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation type="unfinished">Document Template: </translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation type="unfinished">Failed to open document.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation type="unfinished">Failed to save document.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished">Argument must be page item name, or PyCObject instance</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation type="unfinished">Property not found</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation type="unfinished">Child not found</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation type="unfinished">Couldn&apos;t convert result type &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation type="unfinished">Property type &apos;%1&apos; not supported</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation type="unfinished">Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation type="unfinished">Types matched, but setting property failed.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation type="unfinished">Unable to save pixmap</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation type="unfinished">An object with the requested name already exists.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation type="unfinished">Point list must contain at least two points (four values).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation type="unfinished">Point list must contain an even number of values.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation type="unfinished">Point list must contain at least three points (six values).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation type="unfinished">Point list must contain at least four points (eight values).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation type="unfinished">Point list must have a multiple of six values.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation type="unfinished">Object not found.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished">Style not found.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set style on a non-text frame.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation type="unfinished">Failed to save EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation type="unfinished">Page number out of range.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished">argument is not list: must be list of float values.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished">argument contains non-numeric values: must be list of float values.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished">argument contains no-numeric values: must be list of float values.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation type="unfinished">Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished">Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished">Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished">Line style not found.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished">Object is not a linked text frame, can&apos;t unlink.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished">Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished">Only text frames can be checked for overflowing</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;E 执行脚本...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;C 显示控制台</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;A 关于脚本...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;S 脚本</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus脚本</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;R 最近脚本</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>关于脚本</translation>
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
        <translation>Scribus Python interface module

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
function&apos;s documentation, though as with most Python code this list
is not exhaustive due to exceptions from called functions.
</translation>
    </message>
    <message>
        <source>Could not open output file %1</source>
        <translation type="unfinished">Could not open output file %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation type="unfinished">Could not open input file %1</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Freetype2库不可用 </translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>字体 %1 破损，将不嵌入</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>字体 %1 破损 （查看流），将不嵌入</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>字体 %1 破损 （FreeType2）,将放弃它</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>字体 %1 破损 (没字型），将放弃它</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>字体 %1 有破损字符 %2 （字符码 %3）</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>字体 %1 破损而且将被放弃</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation type="unfinished">Font %1 cannot be read, no embedding</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation type="unfinished">Failed to load font %1 - font type unknown</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation type="unfinished">Font %1 loaded from %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation type="unfinished">Font %1(%2) is duplicate of %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation type="unfinished">Loading font %1 (found using fontconfig)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation type="unfinished">Failed to load a font - freetype2 couldn&apos;t find the font file</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="unfinished">Font %1 is broken (FreeType), discarding it</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="unfinished">Font %1  has invalid glyph %2 (charcode %3), discarding it</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished">extracting face %1 from font %2 (offset=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished">memcpy header: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation type="unfinished">table &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished">memcpy table: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished">memcpy offset: %1 %2 %3</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus崩溃</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus崩溃了，因由信号  #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;O 确认</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>不能群组少于2个项目</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>群组需要项目选择范围或者参数列表</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>不能群组少于2个项目</translation>
    </message>
    <message>
        <source> c</source>
        <translation type="unfinished"> c</translation>
    </message>
    <message>
        <source>c</source>
        <translation type="unfinished">c</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation type="unfinished">Cicero (c)</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>文件名不应该是一个空白字串。</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>页</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>复件 #%1 </translation>
    </message>
    <message>
        <source>Black</source>
        <translation>黑</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>青</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>洋红</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>黄</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>颜色转盘</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>字体预览</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>我的插件</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>从模板新建</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>以图像导出</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS 导入器</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>另存为模板</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>脚本器(Scripter)</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>导出SVG</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>导入SVG</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Draw导入器</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus崩溃了，因由以下异常： %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>创建字体缓冲</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>发现新字体，验证中...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>发现更改过的字体，验证中...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>读取字体缓冲</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>写入已更新字体缓冲</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>搜索字体</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished">You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished">The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished">A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished">firstPageOrder is bigger than allowed.</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation type="unfinished">Old .sla format support</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation type="unfinished">German (Trad.)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation type="unfinished">Exporting PostScript File</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation type="unfinished">Printing File</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished">&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation type="unfinished">C&amp;reate</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Import</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation type="unfinished">Clear</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation type="unfinished">Select All</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Undo</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Redo</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Cu&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copy</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Paste</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation type="unfinished">System Menu</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Shade</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation type="unfinished">Unshade</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalise</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimise</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximise</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Close</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation type="unfinished">&amp;Restore</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation type="unfinished">&amp;Move</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Size</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimise</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximise</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Close</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation type="unfinished">Stay on &amp;Top</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimise</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation type="unfinished">Restore Down</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Close</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation type="unfinished">Sh&amp;ade</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation type="unfinished">%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation type="unfinished">&amp;Unshade</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation type="unfinished">Document Setup</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Margin Guides</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Left:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Bottom:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Right:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Page Size</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Custom</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portrait</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Landscape</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Size:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntation:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Width:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Height:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="unfinished">&amp;Unit:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Autosave</translation>
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
        <translation type="unfinished">Document</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Guides</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished">Page Display</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Colour:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Display &amp;Unprintable Area in Margin Colour</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Show Pictures</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Show Text Chains</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Show Frames</translation>
    </message>
    <message>
        <source>Display</source>
        <translation type="unfinished">Display</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Typography</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Tools</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished">Hyphenator</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="unfinished">Fonts</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation type="unfinished">PDF Export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished">Document Item Attributes</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">Table of Contents and Indexes</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Colour Management</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">Turns the display of frames on or off</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">Turns the display of pictures on or off</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Colour for paper</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Mask the area outside the margins in the margin colour</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Document Information</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished">Show Text Control Characters</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Display non-printing characters such as paragraph markers in text frames</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished">Preflight Verifier</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">Rulers relative to Page</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished">Minimum Scratch Space</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished">Gaps between Pages</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished">Horizontal:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished">Vertical:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Adjusting Colours</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished">Enable or disable the display of linked text frames.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="unfinished">Apply size settings to all pages</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="unfinished">Sections</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished">Apply the page size changes to all existing pages in the document</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished">Python Scripts (*.py);; All Files (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation type="unfinished">Run as Extension Script</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation type="unfinished">Style of current paragraph</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation type="unfinished">Style Settings</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation type="unfinished">None</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Colour of text fill</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturation of colour of text fill</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Fill Colour Settings</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation type="unfinished">None</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Colour of text stroke</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturation of colour of text stroke</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Stroke Colour Settings</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation type="unfinished">Font of selected text</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Font Size</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished">Scaling width of characters</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation type="unfinished">Font Settings</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished">Scaling height of characters</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation type="unfinished">Character Settings</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished">Manual Tracking</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished">Save Page as &amp;SVG...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation type="unfinished">Exports SVG Files</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation type="unfinished">Exports the current page into an SVG file.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="unfinished">Import &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation type="unfinished">Imports SVG Files</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished">Imports most SVG files into the current document,
converting their vector data into Scribus objects.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation type="unfinished">Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation type="unfinished">SVG file contains some unsupported features</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Apply unbreakable space on:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Selected frames</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Active &amp;page</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;All items</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Only selected frames processed.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Only actual page processed.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">All items in document processed.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation type="unfinished">User settings</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation type="unfinished">System wide configuration</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;S 保存</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Reset</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation type="unfinished">Save user configuration</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation type="unfinished">Reload system wide configuration and remove user defined one</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation type="unfinished">Edit custom configuration. If you save it, it will be used over system wide configuration</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation type="unfinished">User configuration exists elready. Do you really want to overwrite it?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation type="unfinished">Cannot write file %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation type="unfinished">User settings saved</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation type="unfinished">System wide configuration reloaded</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>不能打开文件 %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>&amp;T 保存为模板...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>把文档保存为模板</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>把文档保存为模板。轻松创建具有类似外观的文件的一个好方法。</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>持续</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>动作</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>载入/保存/导入/导出</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>复件 #%1</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation>文档颜色</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>新层</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>文档</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>启动插件</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>启动键盘快捷键</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>读取首选项</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>启动故事编辑器</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>读取ICC配置文件</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>启动连字符器</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>读取寄存本</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>设置快捷键</translation>
    </message>
    <message>
        <source>File</source>
        <translation>文件</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>编辑</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>查找字体</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>系统中没有找到字体。</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>现在退出中。</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>致命错误</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>字体系统被启动</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;F 文件</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>&amp;R 打开最近</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;I 导入</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;E 导出</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;E 编辑</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Y 格式</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;C 颜色</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;S 尺寸</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;S 阴暗</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;F 字体</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;E 效果</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;I 效果</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>预览设置</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>高低</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>&amp;Y 发送到层</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF选项</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;S 形状</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;O 转换为</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;N 插入</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>字符</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>引号</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>空间</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;P 页面</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;V 查看</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;X 附加命令</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;W 窗口</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;H 帮助</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;A 对齐</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>就绪</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>O 打开</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>导入页...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>导入页</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>导入结束</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>导入没有找到任何东西</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>文件 %1 格式不可接受</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>载入...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished">PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>这个文档使用的一些ICC配置文件没有安装：</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>被取代为：</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>（被转换）</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>所有支持的格式</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>所有文件 (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>不能写入文件： 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>文档 (*.sla *.sla.gz *.scd *scd.gz);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>文档 (*.sla *.scd);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>另存为</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>保存中...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus已经检测到一些错误。考虑用印前检验器来纠正它们。</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;I 忽略</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;A 退出</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>打印中...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>文档</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="unfinished">Printing failed!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>不能剪切正在使用的项目</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>项目 %1 目前正被故事编辑器编辑中，剪切操作将被取消</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>关于Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus使用手册</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>另存为</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>文本文件 (*.txt);;全部文件(*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名称：</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>把页转换为母页</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>新建母页</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;S 尺寸：</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>尺寸</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;S 阴暗：</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>阴暗</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>没有样式</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>以下段落丢失：</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript：你不能使用ESP图像或者打印预览</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus检测到一些错误。
考虑使用印前检验器来纠正它们</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS文件 (*.eps);;所有文件 (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>检测到一些错误。
考虑使用印前检验器来纠正它们</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-页%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>某些对象被锁定</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;L 全部锁定</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;U 全部解锁</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>信息</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>程序 %1 已经在运行中！</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>程序 %1 不存在！</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>所选的颜色在文档的颜色组合中不存在。请为这个新颜色输入一个名字。</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>颜色不存在</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>你为新颜色选择的名承已经存在。请输入一个新名称。</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;L 高低</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>发送到层</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>&amp;W 预览设置</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;T 工具</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-坐标：</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-坐标：</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
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
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
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
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>filename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
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
        <source>Scribus Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Issues</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation>％</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>层</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>复制到这里</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>移动到这里</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;P 粘贴</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>图片</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>文件：</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>原始PPI： </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>实际PPI： </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>串联的文本</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>文本框</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>路径上文本</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>段落：</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>词：</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>字符：</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>打印：</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>已启用</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>已禁用</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;F 信息</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>预览设置</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF选项 </translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>&amp;Y 发送到层</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>&amp;V 高低</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;R 转换为</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>不能删除正在使用的项目</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>项目 %1 正被故事编辑器使用中。删除操作将被取消。</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>串联文本框</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>你正在尝试链接到一个已填充框，或者本框</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>你真的要清除所有的文本吗？</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>不能转换正在使用的项目</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>项目 %1 当前正在被故事编辑器编辑。把当前项目转换为曲线的操作将被跳过</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>页 %1 到 %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>颜色模式：</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>灰度</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>文档：</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>自上次保存已被更改过。</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;D 放弃</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>嵌入的Python脚本支持。</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>脚本器（Scripter）</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>脚本错误</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>如果你正在运行一个官方的脚本，请在 &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; 报告。</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>这个信息同时存在于剪切班。使用Ctrl+V来粘贴到臭虫追踪器里面</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>检验脚本</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>在尝试你所输入的命令是出现一个内部错误。详细信息被打印到标准输出了。</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>设置Python插件失败。具体错误信息已被打印到标准输出。</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>脚本器首选项</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>启用扩展脚本</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>扩展</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>控制台</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>启动运行脚本：</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>错误：</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>说明：</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>关键词：</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>签名：</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>数字：</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>字串：</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>基本颜色：</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>选择颜色</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>显示页面预览</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>查找/替换</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>查找：</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>段落样式</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>字体</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字体尺寸</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>字体效果</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>填充颜色</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>填充阴影</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>边颜色</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>边阴暗</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>左</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>中</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>右</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>区块</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>强制</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>替代为：</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>查找结束</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;W 整个字</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;I 忽略大小写</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;S 查找</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;R 替代</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>&amp;A 全部替代</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;C 关闭</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>查找结束，找到 %1 符合</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>选择域</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>可用域</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>选中域</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="unfinished">&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="unfinished">&amp;&lt;&lt;</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>其他...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>阴暗</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;S 阴暗：</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>X-Offset</source>
        <translation>X-移位 (ENGB)</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-移位</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Short &amp;Words...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>没有样式</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>编辑样式...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>没有样式</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>编辑样式</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>复件 %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>新样式</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>文档 (*.sla *.sla.gz *.scd *.scd.gz);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>文档 (*.sla *.scd);;全部文件 (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;N 新建</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;E 编辑</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;U 复件</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;I 导入</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation type="unfinished">Story Editor</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation type="unfinished">Current Paragraph:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished">Words: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished">Chars: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation type="unfinished">Totals:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished">Paragraphs: </translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Open</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished">Text Files (*.txt);;All Files(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Save as</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation type="unfinished">Do you want to save your changes?</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;New</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation type="unfinished">&amp;Reload Text from Frame</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation type="unfinished">&amp;Save to File...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation type="unfinished">&amp;Load from File...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation type="unfinished">Save &amp;Document</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation type="unfinished">&amp;Update Text Frame and Exit</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation type="unfinished">&amp;Exit Without Updating Text Frame</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Cu&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copy</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Paste</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">C&amp;lear</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation type="unfinished">&amp;Update Text Frame</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;F 文件</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;E 编辑</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;A 全选</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;E 编辑样式...</translation>
    </message>
    <message>
        <source>File</source>
        <translation>文件</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="unfinished">Load Text from File</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="unfinished">Save Text to File</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="unfinished">Update Text Frame and Exit</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="unfinished">Exit Without Updating Text Frame</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="unfinished">Reload Text from Frame</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="unfinished">Update Text Frame</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Search/Replace...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="unfinished">&amp;Fonts Preview...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation type="unfinished">&amp;Background...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation type="unfinished">&amp;Display Font...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="unfinished">&amp;Settings</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="unfinished">Search/Replace</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation type="unfinished">&amp;Smart text selection</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;I 插入符号...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation type="unfinished">Clear All Text</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>故事编辑器 - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>你真的想丢失所有更改吗？</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>你真的想清除你所有的文本吗？</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>自动</translation>
    </message>
    <message>
        <source> %</source>
        <translation>％</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>移位</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>线宽</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>下划线</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>小写</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>下标</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>上标</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>中划线</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>全部大写</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>轮廓</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation>阴影</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation>只文字下划线</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>把文档名称作为段落样式的一个前缀</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>下次不再问</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>确认</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Writer导入器选项</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>启用这个将覆写当前Scribus文档现有的样式</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>合并段落样式</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished">Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished">Prepend the document name to the paragraph style name in Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation type="unfinished">Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>覆写段落样式</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>内容索引列表</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>内容列表</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;A 添加</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>项目属性名称：</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>目标框：</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>在开始处</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>在结尾处</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>不显示</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>内容列表将被放入的框体</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>放置的页码：</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>将被创建条目作为基础的框体里的项目属性</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished">Place page numbers of the entries at the beginning or the end of the line, or not at all</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>列出不可打印的条目</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>也包括被设为不可打印的框体</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>被条目行使用的段落样式</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>段落样式：</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>&amp;X 索引</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">Table of Contents and Indexes</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation type="unfinished">Table Of Contents</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;A 添加</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Delete</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished">The frame the table of contents will be placed into</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished">Page Numbers Placed:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation type="unfinished">Item Attribute Name:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished">The Item Attribute that will be set on frames used as a basis for creation of the entries</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished">Place page numbers of the entries at the beginning or the end of the line, or not at all</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished">List Non-Printing Entries</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished">Include frames that are set to not print as well</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished">The paragraph style used for the entry lines</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation type="unfinished">Paragraph Style:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation type="unfinished">Destination Frame:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="unfinished">Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation type="unfinished">Ignore all errors</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation type="unfinished">Automatic check before printing or exporting</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation type="unfinished">Check for missing glyphs</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation type="unfinished">Check for objects not on a page</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation type="unfinished">Check for overflow in text frames</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation type="unfinished">Check for transparencies used</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation type="unfinished">Check for missing images</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation type="unfinished">Check image resolution</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation type="unfinished">Lowest allowed resolution</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation type="unfinished">Check for placed PDF Files</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished">Check for PDF Annotations and Fields</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation type="unfinished">Add Profile</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation type="unfinished">Remove Profile</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation type="unfinished">Common Settings</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation type="unfinished">Placing in Documents</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation type="unfinished">In the Background</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation type="unfinished">In the Foreground</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation type="unfinished">Snapping</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation type="unfinished">Snap Distance:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation type="unfinished">Grab Radius:</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="unfinished"> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="unfinished">Show Guides</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Colour:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation type="unfinished">Show Margins</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation type="unfinished">Show Page Grid</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation type="unfinished">Major Grid</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation type="unfinished">Spacing:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation type="unfinished">Minor Grid</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished">Show Baseline Grid</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished">Baseline Settings</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">Baseline &amp;Grid:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">Baseline &amp;Offset:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="unfinished">Distance between the minor grid lines</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished">Distance between the major grid lines</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished">Distance within which an object will snap to your placed guides</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished">Radius of the area where Scribus will allow you to grab an objects handles</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Colour of the minor grid lines</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Colour of the major grid lines</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Colour of the guide lines you insert</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Colour for the margin lines</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Colour for the baseline grid</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation type="unfinished">Turns the basegrid on or off</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation type="unfinished">Turns the gridlines on or off</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation type="unfinished">Turns the guides on or off</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation type="unfinished">Turns the margins on or off</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation type="unfinished">Guides are not visible through objects on the page</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation type="unfinished">Guides are visible above all objects on the page</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation type="unfinished">Distance between the lines of the baseline grid</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation type="unfinished">Distance from the top of the page for the first baseline</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation type="unfinished">Manage Tabulators</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Export Range</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">&amp;All Pages</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">C&amp;hoose Pages</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotation:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">File Options</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">Compatibilit&amp;y:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished">&amp;Binding:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Left Margin</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Right Margin</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Generate &amp;Thumbnails</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Save &amp;Linked Text Frames as PDF Articles</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">&amp;Include Bookmarks</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation type="unfinished">&amp;Resolution for EPS Graphics:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="unfinished">Com&amp;press Text and Vector Graphics</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="unfinished">Image Settings</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Automatic</translation>
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
        <translation type="unfinished">None</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Maximum</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">High</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Low</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Minimum</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">&amp;General</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">&amp;Embed all Fonts</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished">&amp;Subset all Fonts</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished">Embedding</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Available Fonts:</translation>
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
        <translation type="unfinished">Fonts to embed:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished">Fonts to subset:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Fonts</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished">Enable &amp;Presentation Effects</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Page</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">Show Page Pre&amp;views</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Effects</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">&amp;Display Duration:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Effec&amp;t Duration:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">Effect T&amp;ype:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">&amp;Moving Lines:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">F&amp;rom the:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">D&amp;irection:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished"> sec</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">No Effect</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished">Blinds</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="unfinished">Box</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Dissolve</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished">Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="unfinished">Split</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished">Wipe</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Vertical</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Inside</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Outside</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Left to Right</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Top to Bottom</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Bottom to Top</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Right to Left</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Top-left to Bottom-Right</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">&amp;Apply Effect on all Pages</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">&amp;Use Encryption</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Passwords</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">&amp;User:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">&amp;Owner:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Settings</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">Allow &amp;Printing the Document</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Allow &amp;Changing the Document</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Allow Cop&amp;ying Text and Graphics</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Allow Adding &amp;Annotations and Fields</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">S&amp;ecurity</translation>
    </message>
    <message>
        <source>General</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>&amp;I 输出目的：</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>屏幕/网页</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>打印机</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>灰度</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;U 使用自定义渲染设置</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>渲染设置</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Q 频率：</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;A 角度：</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">S&amp;pot Function:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>单点</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>线条</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>圆</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>椭圆</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>单色</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>使用ICC配置文件</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>配置文件：</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>渲染意图：</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>感官</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>相对比色</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>饱和度</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>绝对比色</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>图像：</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>不使用嵌入ICC配置文件</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;O 颜色</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 输出意图</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;信息字串：</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>&amp;P 输出配置文件：</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>裁剪方框</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Show page previews of each page listed above.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished">Length of time the page is shown before the presentation starts on the selected page.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Type of the display effect.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">Direction of the effect of moving lines for the split and blind effects.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">Starting position for the box and split effects.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Direction of the glitter or wipe effects.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished">Apply the selected effect to all pages.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Export all pages to PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Export a range of pages to PDF</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Generate PDF Articles, which is useful for navigating linked articles in a PDF.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished">DPI (Dots Per Inch) for image export.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Choose a password for users to be able to read your PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Allow printing of the PDF. If un-checked, printing is prevented. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Embed a colour profile for solid colours</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Colour profile for solid colours</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Rendering intent for solid colours</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Embed a colour profile for images</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Do not use colour profiles that are embedded in source images</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Colour profile for images</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">Rendering intent for images</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">Output profile for printing. If possible, get some guidance from your printer on profile selection.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Distance for bleed from the top of the physical page</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Distance for bleed from the bottom of the physical page</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Distance for bleed from the left of the physical page</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Distance for bleed from the right of the physical page</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished">Mirror Page(s) horizontally</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished">Mirror Page(s) vertically</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished">Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Convert Spot Colours to Process Colours</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation type="unfinished">Compression &amp;Quality:</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want colour managed RGB for commercial printing and is selectable when you have activated colour management. Use only when advised by your printer or in some cases printing to a 4 colour digital colour laser printer.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colours. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="unfinished">Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation type="unfinished">Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation type="unfinished">Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Enables Spot Colours to be converted to composite colours. Unless you are planning to print spot colours at a commercial printer, this is probably best left enabled.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation type="unfinished">Include La&amp;yers</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation type="unfinished">Compression Metho&amp;d:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished">Resa&amp;mple Images to:</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation type="unfinished">Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished">Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished">Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation type="unfinished">Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished">Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished">Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation type="unfinished">Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished">Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation type="unfinished">Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation type="unfinished">Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished">Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation type="unfinished">Choose a master password which enables or disables all the security features in your exported PDF</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Colour model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 colour CMYK printer.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation>页面排版</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>单页</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Javascript to be executed
when Document is opened:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Font:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Size:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">None</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Fill Colour:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Stroke Colour:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">Colu&amp;mns:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">&amp;Gap:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Line Colour:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Shading:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Fill Colour:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">S&amp;hading:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="unfinished">Line Style:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">Line &amp;Width:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation type="unfinished">Line S&amp;tyle:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>箭形：</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>开始：</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>结束：</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;F 自由缩放</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;H 水平缩放：</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;V 垂直缩放：</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;S 按框体大小缩放图像</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>&amp;R 保持比例</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;L 填充颜色</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>使用嵌入剪切路径</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>屏幕预览</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>完全分辨率预览</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>普通分辨率预览</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>底分辨率预览</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;N 最小：</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>&amp;X 最大：</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;S 阶级：</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>文本框属性</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>图像框属性</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>形状绘制属性</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>默认放大级别</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>线条绘制属性</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>多边形绘制属性</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>新建文本框的字体</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>新建文本框的字体大小</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>字体颜色</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>文本框中的列数</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>文本框中列间隔</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>你的字体范例</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>图像框允许图像伸缩到任意尺寸</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>水平伸缩图像</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>垂直伸缩图像</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>保存水平和垂直伸缩相同</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>图像框中的图像被伸缩到图像框的尺寸</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>自动伸缩图像到原始的比例</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>给图像框填充颜色</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>填充颜色的饱和度</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>形状的线条颜色</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>线条颜色的饱和度</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>给形状填充颜色</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>形状的线条样式</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>形状的线条宽度</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>允许最小缩放率</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>允许最大缩放率</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>每次缩放操作的缩放比例变化</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>线条的颜色</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>颜色饱和度</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>线条样式</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>线条宽度</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>制表符填充字符：</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>制表符宽度：</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>自定义：</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>自定义：</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>文本颜色：</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>阴暗：</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>文本笔划：</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>点</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>连字符</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>底线</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定义</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>下标</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;D 移位：</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;S 缩放：</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>上标</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>&amp;I 移位</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>&amp;C 缩放：</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>小写</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>&amp;A 缩放：</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>下划线</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>移位：</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>自动</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>线条宽度：</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Strikeout</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>&amp;L 自动线条间隔</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>线条间隔：</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>在一条线上字体基线的位移</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">Relative size of the superscript compared to the normal font</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">Displacement below the baseline of the normal font on a line</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">Relative size of the subscript compared to the normal font</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">Relative size of the small caps font compared to the normal font</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Percentage increase over the font size for the line spacing</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished">Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation type="unfinished">Line width expressed as a percentage of the font size</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished">Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>左</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>右</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>句号</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>逗号</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centre</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>全部删除</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>段落首行缩进</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>整个段落从左缩进</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>删除所有制表符</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;P 位置：</translation>
    </message>
    <message>
        <source>None</source>
        <translation>无</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation type="unfinished">Fill Char:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>自定义：</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>自定义：</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>点</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>连字符</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>底线</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定义</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>结构树</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>元素</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>群组</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>自由对象</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>页</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation>自动</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>移位</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>线宽</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>添加垂直参考线</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>添加水平参考线</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>移除垂直参考线</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>移除水平参考线</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>移动垂直参考线</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>移动水平参考线</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>锁定参考线</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>解锁参考线</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>移动</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>缩放</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>旋转</translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation type="unfinished">X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation type="unfinished">W1: %1, H1: %2
W2: %3, H2: %4</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>选择</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>群组</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>选择/群组</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>生成</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation type="unfinished">X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>对齐/排列</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>参加项目</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>设置填充颜色</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>颜色1: %1, 颜色2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>设置填充颜色阴暗</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>设施线条颜色</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>设置线条颜色阴暗</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>水平翻转</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>垂直翻转</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>锁定</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>解锁</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>锁定尺寸</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>解锁尺寸</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>解群组</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>删除</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>重命名</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation type="unfinished">From %1
to %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>应用母页</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>粘贴</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>剪切</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>设置填充颜色透明度</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>设置线条颜色透明度</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>设置线条样式</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation type="unfinished">Set the style of line end</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation type="unfinished">Set the style of line join</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation type="unfinished">Set line width</translation>
    </message>
    <message>
        <source>No style</source>
        <translation type="unfinished">No style</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation type="unfinished">Set custom line style</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation type="unfinished">Do not use custom line style</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation type="unfinished">Set start arrow</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation type="unfinished">Set end arrow</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation type="unfinished">Create table</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation type="unfinished">Rows: %1, Cols: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation type="unfinished">Set font</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation type="unfinished">Set font size</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation type="unfinished">Set font width</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Set font fill colour</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Set font stroke colour</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Set font fill colour shade</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Set font stroke colour shade</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation type="unfinished">Set kerning</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation type="unfinished">Set line spacing</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation type="unfinished">Set paragraph style</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>设置语言</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>对齐文本</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>设置字体效果</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>图像框</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>文本框</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>多边形</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>齐贝尔曲线</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>多线</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>转换成</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>导入SVG图像</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>导入EPS图像</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation type="unfinished">Scratch space</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation type="unfinished">Text flows around the frame</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation type="unfinished">Text flows around bounding box</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation type="unfinished">Text flows around contour line</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>没有文本流</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>无限制方框</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation type="unfinished">No contour line</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation type="unfinished">Page %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation type="unfinished">Set image scaling</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation type="unfinished">Frame size</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation type="unfinished">Free scaling</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation type="unfinished">Keep aspect ratio</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation type="unfinished">Break aspect ratio</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation type="unfinished">Edit contour line</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation type="unfinished">Edit shape</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation type="unfinished">Reset contour line</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation type="unfinished">Add page</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation type="unfinished">Add pages</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>删除页</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>删除页</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>添加层</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>删除层</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>重命名层</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>升高层</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>降低层</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>发送到层</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>允许层打印</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>取消层打印</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>更改层名称</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>获取图像</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>导入OpenOffice.org Draw图像</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>设置字体高</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>路径里的文本</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>起始状态</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>动作历史</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>只显示选中对象</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;U 撤销动作</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;R 重复动作</translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation type="unfinished">%1: %2</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>插入数值</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>输入一个数值然后按确定</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>输入一个数值然后按确定</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>发一个数值给脚本</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>属性...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Button</source>
        <translation>按钮</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>文本域</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>勾选框</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>多选框</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>列举框</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>链接</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF 工具</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>插入PDF域</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>插入PDF注释</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>选择要使用的导入器</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>自动 </translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>不带格式地导入文本</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>导入器：</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>编码：</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>只导入文本</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>选择要用的导入器</translation>
    </message>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>记住关系</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation type="unfinished">Remember the file extension - importer association and do not ask again to select an importer for files of this type.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>从模板新建</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>页面尺寸</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>颜色</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>描述</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>使用</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>制作工具</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>作者</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;R 移除</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;O 打开</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>下载模板</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>安装模板中</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>把包解压缩到当前用户的模板目录 ~/.scribus/templates中，或者解压缩到PREFIX/share/scribus/templates目录中，以便系统中所有的用户都可以使用。</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>正在准备一个模板</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>正在删除一个模板</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>正在翻译 template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation type="unfinished">Document templates can be found at http://www.scribus.net/ in the Downloads section.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="unfinished">Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation type="unfinished">The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation type="unfinished">Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation type="unfinished">Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>日期</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>保存为模板</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>类别</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>页面尺寸</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>颜色</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>描述</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>用途</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>作者</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>电子邮件</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>更多细节</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>确认</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>更少细节</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>信纸</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>小报</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>横式</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>竖式</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>自定义</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>生成过滤器</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 删除</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>选择一个以前保存好的过滤器</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>给这个过滤器起个名称以便保存</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation type="unfinished">Give a name for saving</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>禁用或启用这个过滤行</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>清除这个过滤行</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>现在添加一个新过滤行</translation>
    </message>
    <message>
        <source>to</source>
        <translation>到</translation>
    </message>
    <message>
        <source>and</source>
        <translation>和</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>清楚符合的</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>不清除符合的</translation>
    </message>
    <message>
        <source>words</source>
        <translation>字</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>清除</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>替换</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>应用</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>左边的数值是一个正则表达式</translation>
    </message>
    <message>
        <source>with</source>
        <translation>用</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>段落样式</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>所有情况</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>所有段落</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>段落开始于</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>段落不多于</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>段落不少于</translation>
    </message>
</context>
</TS>
