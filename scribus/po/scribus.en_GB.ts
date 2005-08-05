<!DOCTYPE TS>
<TS>
  <context>
    <name></name>
    <message>
      <source>getColorNames() -> list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
      <translation>getColorNames() -> list

Returns a list containing the names of all defined colours in the document.
If no document is open, returns a list of the default document colours.
</translation>
    </message>
    <message>
      <source>newDocDialog() -> bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
      <translation>newDocDialog() -> bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</translation>
    </message>
    <message>
      <source>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -> bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type> e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumer = is the number of the first page in the document used for
    pagenumbering. While you'll usually want 1, it's useful to have higher
    numbers if you're creating a document in several parts.

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
      <translation>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -> bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type> e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumer = is the number of the first page in the document used for
    pagenumbering. While you'll usually want 1, it's useful to have higher
    numbers if you're creating a document in several parts.

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
      <source>getFillColor([&quot;name&quot;]) -> string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getFillColor([&quot;name&quot;]) -> string

Returns the name of the fill colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
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

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</translation>
    </message>
    <message>
      <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
      <translation>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</translation>
    </message>
    <message>
      <source>createRect(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createRect(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</translation>
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
      <translation>newPage(where [,&quot;masterpage&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;masterpage&quot; specifies the name of the
master page for the new page.

May raise IndexError if the page number is out of range
</translation>
    </message>
    <message>
      <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type>).
</source>
      <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Colour descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type>).
</translation>
    </message>
    <message>
      <source>getFontSize([&quot;name&quot;]) -> float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getFontSize([&quot;name&quot;]) -> float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or 'unicode' string(recommended).
</source>
      <translation>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or 'unicode' string(recommended).
</translation>
    </message>
    <message>
      <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or 'unicode' string(recommended).
</source>
      <translation>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or 'unicode' string(recommended).
</translation>
    </message>
  </context>
  <context>
    <name>@default</name>
    <message>
      <source>getColor(&quot;name&quot;) -> tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>getColor(&quot;name&quot;) -> tuple

Returns a tuple (C, M, Y, K) containing the four colour components of the
colour &quot;name&quot; from the current document. If no document is open, returns
the value of the named colour from the default document colours.

May raise NotFoundError if the named colour wasn't found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
      <source>getColorAsRGB(&quot;name&quot;) -> tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB color
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>getColorAsRGB(&quot;name&quot;) -> tuple

Returns a tuple (R,G,B) containing the three colour components of the
colour &quot;name&quot; from the current document, converted to the RGB colour
space. If no document is open, returns the value of the named colour
from the default document colours.

May raise NotFoundError if the named colour wasn't found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
      <source>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>changeColour(&quot;name&quot;, c, m, y, k)

Changes the colour &quot;name&quot; to the specified CMYK value. The colour value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Colour components should be in the range from 0 to 255.

May raise NotFoundError if the named colour wasn't found.
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

May raise NotFoundError if a named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Deletes the colour &quot;name&quot;. Every occurence of that colour is replaced by the
colour &quot;replace&quot;. If not specified, &quot;replace&quot; defaults to the colour
&quot;None&quot; - transparent.

deleteColor works on the default document colours if there is no document open.
In that case, &quot;replace&quot;, if specified, has no effect.

May raise NotFoundError if a named colour wasn't found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
      <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the colour &quot;name&quot; is replaced by the colour &quot;replace&quot;.

May raise NotFoundError if a named colour wasn't found.
May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
      <source>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -> string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don't want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. When the isdir parameter is True
the dialog shows and returns only directories. The default for all of the
opional parameters is False.

The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.
For example 'Images (*.png *.xpm *.jpg)'.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog('Open input', 'CSV files (*.csv)')
Example: fileDialog('Save report', defaultname='report.txt', issave=True)
</source>
      <translation>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -> string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don't want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. When the isdir parameter is True
the dialog shows and returns only directories. The default for all of the
opional parameters is False.

The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.
For example 'Images (*.png *.xpm *.jpg)'.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog('Open input', 'CSV files (*.csv)')
Example: fileDialog('Save report', defaultname='report.txt', issave=True)
</translation>
    </message>
    <message>
      <source>messageBox(&quot;caption&quot;, &quot;message&quot;,
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
    button2=BUTTON_NONE, button3=BUTTON_NONE) -> integer

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
result = messageBox('Script failed',
                    'This script only works when you have a text frame selected.',
                    ICON_ERROR)
result = messageBox('Monkeys!', 'Something went ook! &lt;i>Was it a monkey?&lt;/i>',
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Defined button and icon constants:
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.
</source>
      <translation>messageBox(&quot;caption&quot;, &quot;message&quot;,
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
    button2=BUTTON_NONE, button3=BUTTON_NONE) -> integer

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
result = messageBox('Script failed',
                    'This script only works when you have a text frame selected.',
                    ICON_ERROR)
result = messageBox('Monkeys!', 'Something went ook! &lt;i>Was it a monkey?&lt;/i>',
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Defined button and icon constants:
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.
</translation>
    </message>
    <message>
      <source>valueDialog(caption, message [,defaultvalue]) -> string

Shows the common 'Ask for string' dialog and returns its value as a string
Parameters: window title, text in the window and optional 'default' value.

Example: valueDialog('title', 'text in the window', 'optional')
</source>
      <translation>valueDialog(caption, message [,defaultvalue]) -> string

Shows the common 'Ask for string' dialog and returns its value as a string
Parameters: window title, text in the window and optional 'default' value.

Example: valueDialog('title', 'text in the window', 'optional')
</translation>
    </message>
    <message>
      <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
      <translation>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</translation>
    </message>
    <message>
      <source>haveDoc() -> bool

Returns true if there is a document open.
</source>
      <translation>haveDoc() -> bool

Returns true if there is a document open.
</translation>
    </message>
    <message>
      <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
      <translation>openDoc(&quot;name&quot;)

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
      <translation>saveDoc()

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
      <translation>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</translation>
    </message>
    <message>
      <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
      <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</translation>
    </message>
    <message>
      <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</source>
      <translation>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</translation>
    </message>
    <message>
      <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type>.

May raise ValueError if an invalid unit is passed.
</source>
      <translation>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type>.

May raise ValueError if an invalid unit is passed.
</translation>
    </message>
    <message>
      <source>getUnit() -> integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
      <translation>getUnit() -> integer (Scribus unit constant)

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
      <translation>loadStylesFromFile(&quot;filename&quot;)

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
      <translation>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
      <source>getLineColor([&quot;name&quot;]) -> string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getLineColor([&quot;name&quot;]) -> string

Returns the name of the line colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getLineWidth([&quot;name&quot;]) -> integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getLineWidth([&quot;name&quot;]) -> integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getLineShade([&quot;name&quot;]) -> integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getLineShade([&quot;name&quot;]) -> integer

Returns the shading value of the line colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getLineJoin([&quot;name&quot;]) -> integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
      <translation>getLineJoin([&quot;name&quot;]) -> integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</translation>
    </message>
    <message>
      <source>getLineEnd([&quot;name&quot;]) -> integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
      <translation>getLineEnd([&quot;name&quot;]) -> integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</translation>
    </message>
    <message>
      <source>getLineStyle([&quot;name&quot;]) -> integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
      <translation>getLineStyle([&quot;name&quot;]) -> integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</translation>
    </message>
    <message>
      <source>getFillShade([&quot;name&quot;]) -> integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getFillShade([&quot;name&quot;]) -> integer

Returns the shading value of the fill colour of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
      <translation>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</translation>
    </message>
    <message>
      <source>getImageScale([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getImageScale([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getImageName([&quot;name&quot;]) -> string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>getImageName([&quot;name&quot;]) -> string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</source>
      <translation>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</translation>
    </message>
    <message>
      <source>getSize([&quot;name&quot;]) -> (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type>
for reference.
</source>
      <translation>getSize([&quot;name&quot;]) -> (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type>
for reference.
</translation>
    </message>
    <message>
      <source>getRotation([&quot;name&quot;]) -> integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
      <translation>getRotation([&quot;name&quot;]) -> integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</translation>
    </message>
    <message>
      <source>getAllObjects() -> list

Returns a list containing the names of all objects on the current page.
</source>
      <translation>getAllObjects() -> list

Returns a list containing the names of all objects on the current page.
</translation>
    </message>
    <message>
      <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property' of `object'. See getProperty()
for details of arguments.

If `includesuper' is true, search inherited properties too.
</source>
      <translation>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property' of `object'. See getProperty()
for details of arguments.

If `includesuper' is true, search inherited properties too.
</translation>
    </message>
    <message>
      <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object'.
If `includesuper' is true, return properties supported
by parent classes as well.
</source>
      <translation>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object'.
If `includesuper' is true, return properties supported
by parent classes as well.
</translation>
    </message>
    <message>
      <source>getProperty(object, property)

Return the value of the property `property' of the passed `object'.

The `object' argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property' argument must be a string, and is the name of the property
to look up on `object'.

The return value varies depending on the type of the property.
</source>
      <translation>getProperty(object, property)

Return the value of the property `property' of the passed `object'.

The `object' argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property' argument must be a string, and is the name of the property
to look up on `object'.

The return value varies depending on the type of the property.
</translation>
    </message>
    <message>
      <source>setProperty(object, property, value)

Set `property' of `object' to `value'. If `value' cannot be converted to a type
compatible with the type of `property', an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
      <translation>setProperty(object, property, value)

Set `property' of `object' to `value'. If `value' cannot be converted to a type
compatible with the type of `property', an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</translation>
    </message>
    <message>
      <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object', possibly restricted to children
of class named `ofclass' or children named `ofname'. If `recursive' is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
      <translation>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object', possibly restricted to children
of class named `ofclass' or children named `ofname'. If `recursive' is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</translation>
    </message>
    <message>
      <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object' named `childname', possibly restricting
the search to children of type name `ofclass'. If `recursive' is true,
search recursively through children, grandchildren, etc.
</source>
      <translation>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object' named `childname', possibly restricting
the search to children of type name `ofclass'. If `recursive' is true,
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
      <translation>moveObjectAbs(x, y [, &quot;name&quot;])

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
      <translation>rotateObject(rot [, &quot;name&quot;])

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
      <translation>rotateObjectAbs(rot [, &quot;name&quot;])

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
      <translation>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getSelectedObject([nr]) -> string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
      <translation>getSelectedObject([nr]) -> string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</translation>
    </message>
    <message>
      <source>selectionCount() -> integer

Returns the number of selected objects.
</source>
      <translation>selectionCount() -> integer

Returns the number of selected objects.
</translation>
    </message>
    <message>
      <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
      <translation>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</translation>
    </message>
    <message>
      <source>deselectAll()

Deselects all objects in the whole document.
</source>
      <translation>deselectAll()

Deselects all objects in the whole document.
</translation>
    </message>
    <message>
      <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
      <translation>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</translation>
    </message>
    <message>
      <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
      <translation>unGroupObjects(&quot;name&quot;)

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
      <translation>scaleGroup(factor [,&quot;name&quot;])

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
      <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;])

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
      <translation>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</translation>
    </message>
    <message>
      <source>lockObject([&quot;name&quot;]) -> bool

Locks the object &quot;name&quot; if it's unlocked or unlock it if it's locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
      <translation>lockObject([&quot;name&quot;]) -> bool

Locks the object &quot;name&quot; if it's unlocked or unlock it if it's locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</translation>
    </message>
    <message>
      <source>isLocked([&quot;name&quot;]) -> bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
      <translation>isLocked([&quot;name&quot;]) -> bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</translation>
    </message>
    <message>
      <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection>)

Sets the scale to frame on the selected or specified image frame to `scaletoframe'.
If `proportional' is specified, set fixed aspect ratio scaling to `proportional'.
Both `scaletoframe' and `proportional' are boolean.

May raise WrongFrameTypeError.
</source>
      <translation>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection>)

Sets the scale to frame on the selected or specified image frame to `scaletoframe'.
If `proportional' is specified, set fixed aspect ratio scaling to `proportional'.
Both `scaletoframe' and `proportional' are boolean.

May raise WrongFrameTypeError.
</translation>
    </message>
    <message>
      <source>getFontNames() -> list

Returns a list with the names of all available fonts.
</source>
      <translation>getFontNames() -> list

Returns a list with the names of all available fonts.
</translation>
    </message>
    <message>
      <source>getXFontNames() -> list of tuples

Returns a larger font info. It's a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
      <translation>getXFontNames() -> list of tuples

Returns a larger font info. It's a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</translation>
    </message>
    <message>
      <source>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -> bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
If &quot;filename&quot; is not &quot;&quot;, image is saved into &quot;filename&quot;. Otherwise
image data is returned as a string. The optional &quot;format&quot; argument
specifies the image format to generate, and supports any format allowed
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM.

May raise NotFoundError if the specified font can't be found.
May raise ValueError if an empty sample or filename is passed.
</source>
      <translation>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -> bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
If &quot;filename&quot; is not &quot;&quot;, image is saved into &quot;filename&quot;. Otherwise
image data is returned as a string. The optional &quot;format&quot; argument
specifies the image format to generate, and supports any format allowed
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM.

May raise NotFoundError if the specified font can't be found.
May raise ValueError if an empty sample or filename is passed.
</translation>
    </message>
    <message>
      <source>getLayers() -> list

Returns a list with the names of all defined layers.
</source>
      <translation>getLayers() -> list

Returns a list with the names of all defined layers.
</translation>
    </message>
    <message>
      <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>getActiveLayer() -> string

Returns the name of the current active layer.
</source>
      <translation>getActiveLayer() -> string

Returns the name of the current active layer.
</translation>
    </message>
    <message>
      <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn't
exists or if it's the only layer in the document.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn't
exists or if it's the only layer in the document.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>getGuiLanguage() -> string

Returns a string with the -lang value.
</source>
      <translation>getGuiLanguage() -> string

Returns a string with the -lang value.
</translation>
    </message>
    <message>
      <source>createEllipse(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new ellipse on the current page and returns its name.
The coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name for further referencing of that object. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createEllipse(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new ellipse on the current page and returns its name.
The coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name for further referencing of that object. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</translation>
    </message>
    <message>
      <source>createImage(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new picture frame on the current page and returns its name. The
coordinates are given in the current measurement units of the document.
&quot;name&quot; should be a unique identifier for the object because you need this
name for further access to that object. If &quot;name&quot; is not given Scribus will
create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createImage(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new picture frame on the current page and returns its name. The
coordinates are given in the current measurement units of the document.
&quot;name&quot; should be a unique identifier for the object because you need this
name for further access to that object. If &quot;name&quot; is not given Scribus will
create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</translation>
    </message>
    <message>
      <source>createText(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new text frame on the actual page and returns its name.
The coordinates are given in the actual measurement unit of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further referencing of that object. If &quot;name&quot; is not
given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createText(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new text frame on the actual page and returns its name.
The coordinates are given in the actual measurement unit of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further referencing of that object. If &quot;name&quot; is not
given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</translation>
    </message>
    <message>
      <source>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -> string

Creates a new line from the point(x1, y1) to the point(x2, y2) and returns
its name. The coordinates are given in the current measurement unit of the
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the
object because you need this name for further access to that object. If
&quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -> string

Creates a new line from the point(x1, y1) to the point(x2, y2) and returns
its name. The coordinates are given in the current measurement unit of the
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the
object because you need this name for further access to that object. If
&quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</translation>
    </message>
    <message>
      <source>createPolyLine(list, [&quot;name&quot;]) -> string

Creates a new polyline and returns its name. The points for the polyline are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
The coordinates are given in the current measurement units of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further access to that object. If &quot;name&quot; is not given
Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</source>
      <translation>createPolyLine(list, [&quot;name&quot;]) -> string

Creates a new polyline and returns its name. The points for the polyline are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
The coordinates are given in the current measurement units of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further access to that object. If &quot;name&quot; is not given
Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</translation>
    </message>
    <message>
      <source>createPolygon(list, [&quot;name&quot;]) -> string

Creates a new polygon and returns its name. The points for the polygon are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
At least three points are required. There is no need to repeat the first point
to close the polygon. The polygon is automatically closed by connecting the
first and the last point.  The coordinates are given in the current measurement
units of the document (see UNIT constants).  &quot;name&quot; should be a unique
identifier for the object because you need this name for further access to that
object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</source>
      <translation>createPolygon(list, [&quot;name&quot;]) -> string

Creates a new polygon and returns its name. The points for the polygon are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
At least three points are required. There is no need to repeat the first point
to close the polygon. The polygon is automatically closed by connecting the
first and the last point.  The coordinates are given in the current measurement
units of the document (see UNIT constants).  &quot;name&quot; should be a unique
identifier for the object because you need this name for further access to that
object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</translation>
    </message>
    <message>
      <source>createBezierLine(list, [&quot;name&quot;]) -> string

Creates a new bezier curve and returns its name. The points for the bezier
curve are stored in the list &quot;list&quot; in the following order:
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]
In the points list, x and y mean the x and y coordinates of the point and kx
and ky meaning the control point for the curve.  The coordinates are given in
the current measurement units of the document (see UNIT constants). &quot;name&quot;
should be a unique identifier for the object because you need this name for
further access to that object. If &quot;name&quot; is not given Scribus will create one
for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</source>
      <translation>createBezierLine(list, [&quot;name&quot;]) -> string

Creates a new bezier curve and returns its name. The points for the bezier
curve are stored in the list &quot;list&quot; in the following order:
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]
In the points list, x and y mean the x and y coordinates of the point and kx
and ky meaning the control point for the curve.  The coordinates are given in
the current measurement units of the document (see UNIT constants). &quot;name&quot;
should be a unique identifier for the object because you need this name for
further access to that object. If &quot;name&quot; is not given Scribus will create one
for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</translation>
    </message>
    <message>
      <source>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -> string

Creates a new pathText by merging the two objects &quot;textbox&quot; and
&quot;beziercurve&quot; and returns its name. The coordinates are given in the current
measurement unit of the document (see UNIT constants). &quot;name&quot; should be a
unique identifier for the object because you need this name for further access
to that object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise NotFoundError if one or both of the named base object don't exist.
</source>
      <translation>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -> string

Creates a new pathText by merging the two objects &quot;textbox&quot; and
&quot;beziercurve&quot; and returns its name. The coordinates are given in the current
measurement unit of the document (see UNIT constants). &quot;name&quot; should be a
unique identifier for the object because you need this name for further access
to that object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise NotFoundError if one or both of the named base object don't exist.
</translation>
    </message>
    <message>
      <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
      <translation>deleteObject([&quot;name&quot;])

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
      <translation>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</translation>
    </message>
    <message>
      <source>objectExists([&quot;name&quot;]) -> bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
      <translation>objectExists([&quot;name&quot;]) -> bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</translation>
    </message>
    <message>
      <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it's applied on the selected object.
</source>
      <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it's applied on the selected object.
</translation>
    </message>
    <message>
      <source>getAllStyles() -> list

Return a list of the names of all paragraph styles in the current document.
</source>
      <translation>getAllStyles() -> list

Return a list of the names of all paragraph styles in the current document.
</translation>
    </message>
    <message>
      <source>currentPage() -> integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
      <translation>currentPage() -> integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</translation>
    </message>
    <message>
      <source>redrawAll()

Redraws all pages.
</source>
      <translation>redrawAll()

Redraws all pages.
</translation>
    </message>
    <message>
      <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
      <translation>savePageAsEPS(&quot;name&quot;)

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
      <translation>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</translation>
    </message>
    <message>
      <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (curently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
      <translation>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (curently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</translation>
    </message>
    <message>
      <source>pageCount() -> integer

Returns the number of pages in the document.
</source>
      <translation>pageCount() -> integer

Returns the number of pages in the document.
</translation>
    </message>
    <message>
      <source>getHGuides() -> list

Returns a list containing positions of the horizontal guides. Values are in the
document's current units - see UNIT_&lt;type> constants.
</source>
      <translation>getHGuides() -> list

Returns a list containing positions of the horizontal guides. Values are in the
document's current units - see UNIT_&lt;type> constants.
</translation>
    </message>
    <message>
      <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type> constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
      <translation>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type> constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</translation>
    </message>
    <message>
      <source>getVGuides()

See getHGuides.
</source>
      <translation>getVGuides()

See getHGuides.
</translation>
    </message>
    <message>
      <source>setVGuides()

See setHGuides.
</source>
      <translation>setVGuides()

See setHGuides.
</translation>
    </message>
    <message>
      <source>getPageSize() -> tuple

Returns a tuple with page dimensions measured in the document's current units.
See UNIT_&lt;type> constants and getPageMargins()
</source>
      <translation>getPageSize() -> tuple

Returns a tuple with page dimensions measured in the document's current units.
See UNIT_&lt;type> constants and getPageMargins()
</translation>
    </message>
    <message>
      <source>getPageItems() -> list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]
means that object named 'Text1' is a text frame (type 4) and is the first at
the page...
</source>
      <translation>getPageItems() -> list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]
means that object named 'Text1' is a text frame (type 4) and is the first at
the page...
</translation>
    </message>
    <message>
      <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type> constants and getPageSize().
</source>
      <translation>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type> constants and getPageSize().
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
      <translation>setLineWidth(width, [&quot;name&quot;])

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
predefined constants for join - JOIN_&lt;type>.
</source>
      <translation>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type>.
</translation>
    </message>
    <message>
      <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type>.
</source>
      <translation>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type>.
</translation>
    </message>
    <message>
      <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style>.
</source>
      <translation>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style>.
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
      <translation>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</translation>
    </message>
    <message>
      <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn't exist.
</source>
      <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn't exist.
</translation>
    </message>
    <message>
      <source>getFont([&quot;name&quot;]) -> string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>getFont([&quot;name&quot;]) -> string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
      <source>getTextLength([&quot;name&quot;]) -> integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getTextLength([&quot;name&quot;]) -> integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>getText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
      <source>getAllText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation>getAllText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</translation>
    </message>
    <message>
      <source>getLineSpacing([&quot;name&quot;]) -> float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getLineSpacing([&quot;name&quot;]) -> float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getColumnGap([&quot;name&quot;]) -> float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getColumnGap([&quot;name&quot;]) -> float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getColumns([&quot;name&quot;]) -> integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>getColumns([&quot;name&quot;]) -> integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2'). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2'). See the FAQ
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
      <translation>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

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
      <translation>setFont(&quot;font&quot;, [&quot;name&quot;])

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

May throw ValueError for a font size that's out of bounds.
</source>
      <translation>setFontSize(size, [&quot;name&quot;])

Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated
as a value in points. If there is some text selected only the selected text is
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError for a font size that's out of bounds.
</translation>
    </message>
    <message>
      <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
      <translation>setLineSpacing(size, [&quot;name&quot;])

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
      <translation>setColumnGap(size, [&quot;name&quot;])

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
      <translation>setColumns(nr, [&quot;name&quot;])

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
      <translation>setTextAlignment(align, [&quot;name&quot;])

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
      <translation>selectText(start, count, [&quot;name&quot;])

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
      <translation>deleteText([&quot;name&quot;])

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
      <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

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
      <translation>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

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
connected, eg 'a->b->c' becomes 'a->c' when you unlinkTextFrames(b)'

May throw ScribusException if linking rules are violated.
</source>
      <translation>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg 'a->b->c' becomes 'a->c' when you unlinkTextFrames(b)'

May throw ScribusException if linking rules are violated.
</translation>
    </message>
    <message>
      <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
      <translation>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</translation>
    </message>
    <message>
      <source>textOverflows([&quot;name&quot;, nolinks]) -> integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn't
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
      <translation>textOverflows([&quot;name&quot;, nolinks]) -> integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn't
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
      <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</translation>
    </message>
    <message>
      <source>isPDFBookmark([&quot;name&quot;]) -> bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation>isPDFBookmark([&quot;name&quot;]) -> bool

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
      <translation>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</translation>
    </message>
    <message>
      <source>progressTotal(max)

Sets the progress bar's maximum steps value to the specified number.
See progressSet.
</source>
      <translation>progressTotal(max)

Sets the progress bar's maximum steps value to the specified number.
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
progressReset(). [based on info taken from Trolltech's Qt docs]
</source>
      <translation>progressSet(nr)

Set the progress bar position to &quot;nr&quot;, a value relative to the previously set
progressTotal. The progress bar uses the concept of steps; you give it the
total number of steps and the number of steps completed so far and it will
display the percentage of steps that have been completed. You can specify the
total number of steps with progressTotal(). The current number of steps is set
with progressSet(). The progress bar can be rewound to the beginning with
progressReset(). [based on info taken from Trolltech's Qt docs]
</translation>
    </message>
    <message>
      <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
      <translation>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</translation>
    </message>
    <message>
      <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It's
useful to call this procedure when you're changing the document, because Scribus
won't automatically notice when you change the document using a script.
</source>
      <translation>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It's
useful to call this procedure when you're changing the document, because Scribus
won't automatically notice when you change the document using a script.
</translation>
    </message>
  </context>
  <context>
    <name>About</name>
    <message>
      <source>Build-ID:</source>
      <translation>Build-ID:</translation>
    </message>
    <message>
      <source>Contributions from:</source>
      <translation>Contributions from:</translation>
    </message>
    <message>
      <source>&amp;About</source>
      <translation>&amp;About</translation>
    </message>
    <message>
      <source>A&amp;uthors</source>
      <translation>A&amp;uthors</translation>
    </message>
    <message>
      <source>&amp;Translations</source>
      <translation>&amp;Translations</translation>
    </message>
    <message>
      <source>&amp;Online</source>
      <translation>&amp;Online</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>Development Team:</source>
      <translation>Development Team:</translation>
    </message>
    <message>
      <source>Official Documentation:</source>
      <translation>Official Documentation:</translation>
    </message>
    <message>
      <source>Other Documentation:</source>
      <translation>Other Documentation:</translation>
    </message>
    <message>
      <source>Homepage</source>
      <translation>Homepage</translation>
    </message>
    <message>
      <source>Online Reference</source>
      <translation>Online Reference</translation>
    </message>
    <message>
      <source>Bugs and Feature Requests</source>
      <translation>Bugs and Feature Requests</translation>
    </message>
    <message>
      <source>Mailing List</source>
      <translation>Mailing List</translation>
    </message>
    <message>
      <source>Official Translations and Translators:</source>
      <translation>Official Translations and Translators:</translation>
    </message>
    <message>
      <source>Previous Translation Contributors:</source>
      <translation>Previous Translation Contributors:</translation>
    </message>
    <message>
      <source>About Scribus %1</source>
      <translation>About Scribus %1</translation>
    </message>
    <message>
      <source>%1 %2 %3 </source>
      <translation>%1 %2 %3 </translation>
    </message>
    <message>
      <source>Windows Port:</source>
      <translation>Windows Port:</translation>
    </message>
    <message>
      <source>Mac OSX Aqua Port:</source>
      <translation>Mac OSX Aqua Port:</translation>
    </message>
    <message>
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>Using GhostScript version %1</source>
      <translation>Using GhostScript version %1</translation>
    </message>
    <message>
      <source>No GS version available</source>
      <translation>No GS version available</translation>
    </message>
    <message>
      <source>Scribus Version %1
%2 %3 (%4)</source>
      <translation>Scribus Version %1
%2 %3 (%4)</translation>
    </message>
    <message>
      <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
      <translation>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</translation>
    </message>
  </context>
  <context>
    <name>ActionManager</name>
    <message>
      <source>&amp;Image Effects</source>
      <translation>&amp;Image Effects</translation>
    </message>
    <message>
      <source>&amp;Tabulators...</source>
      <translation>&amp;Tabulators...</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Open...</source>
      <translation>&amp;Open...</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Save &amp;As...</translation>
    </message>
    <message>
      <source>Re&amp;vert to Saved</source>
      <translation>Re&amp;vert to Saved</translation>
    </message>
    <message>
      <source>Collect for O&amp;utput...</source>
      <translation>Collect for O&amp;utput...</translation>
    </message>
    <message>
      <source>Get Text...</source>
      <translation>Get Text...</translation>
    </message>
    <message>
      <source>Append &amp;Text...</source>
      <translation>Append &amp;Text...</translation>
    </message>
    <message>
      <source>Get Image...</source>
      <translation>Get Image...</translation>
    </message>
    <message>
      <source>Save &amp;Text...</source>
      <translation>Save &amp;Text...</translation>
    </message>
    <message>
      <source>Save Page as &amp;EPS...</source>
      <translation>Save Page as &amp;EPS...</translation>
    </message>
    <message>
      <source>Save as P&amp;DF...</source>
      <translation>Save as P&amp;DF...</translation>
    </message>
    <message>
      <source>Document &amp;Setup...</source>
      <translation>Document &amp;Setup...</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>&amp;Print...</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation>&amp;Quit</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Undo</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Redo</translation>
    </message>
    <message>
      <source>&amp;Item Action Mode</source>
      <translation>&amp;Item Action Mode</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>Cu&amp;t</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>&amp;Paste</translation>
    </message>
    <message>
      <source>C&amp;lear Contents</source>
      <translation>C&amp;lear Contents</translation>
    </message>
    <message>
      <source>Select &amp;All</source>
      <translation>Select &amp;All</translation>
    </message>
    <message>
      <source>&amp;Deselect All</source>
      <translation>&amp;Deselect All</translation>
    </message>
    <message>
      <source>&amp;Search/Replace...</source>
      <translation>&amp;Search/Replace...</translation>
    </message>
    <message>
      <source>C&amp;olors...</source>
      <translation>C&amp;olours...</translation>
    </message>
    <message>
      <source>&amp;Paragraph Styles...</source>
      <translation>&amp;Paragraph Styles...</translation>
    </message>
    <message>
      <source>&amp;Line Styles...</source>
      <translation>&amp;Line Styles...</translation>
    </message>
    <message>
      <source>&amp;Master Pages...</source>
      <translation>&amp;Master Pages...</translation>
    </message>
    <message>
      <source>&amp;Javascripts...</source>
      <translation>&amp;Javascripts...</translation>
    </message>
    <message>
      <source>P&amp;references...</source>
      <translation>P&amp;references...</translation>
    </message>
    <message>
      <source>%1 pt</source>
      <translation>%1 pt</translation>
    </message>
    <message>
      <source>&amp;Other...</source>
      <translation>&amp;Other...</translation>
    </message>
    <message>
      <source>&amp;Left</source>
      <translation>&amp;Left</translation>
    </message>
    <message>
      <source>&amp;Center</source>
      <translation>&amp;Centre</translation>
    </message>
    <message>
      <source>&amp;Right</source>
      <translation>&amp;Right</translation>
    </message>
    <message>
      <source>&amp;Block</source>
      <translation>&amp;Block</translation>
    </message>
    <message>
      <source>&amp;Forced</source>
      <translation>&amp;Forced</translation>
    </message>
    <message>
      <source>&amp;%1 %</source>
      <translation>&amp;%1 %</translation>
    </message>
    <message>
      <source>&amp;Normal</source>
      <translation>&amp;Normal</translation>
    </message>
    <message>
      <source>&amp;Underline</source>
      <translation>&amp;Underline</translation>
    </message>
    <message>
      <source>Underline &amp;Words</source>
      <translation>Underline &amp;Words</translation>
    </message>
    <message>
      <source>&amp;Strike Through</source>
      <translation>&amp;Strike Through</translation>
    </message>
    <message>
      <source>&amp;All Caps</source>
      <translation>&amp;All Caps</translation>
    </message>
    <message>
      <source>Small &amp;Caps</source>
      <translation>Small &amp;Caps</translation>
    </message>
    <message>
      <source>Su&amp;perscript</source>
      <translation>Su&amp;perscript</translation>
    </message>
    <message>
      <source>Su&amp;bscript</source>
      <translation>Su&amp;bscript</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <translation>&amp;Outline</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>D&amp;uplicate</translation>
    </message>
    <message>
      <source>&amp;Multiple Duplicate</source>
      <translation>&amp;Multiple Duplicate</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>&amp;Group</source>
      <translation>&amp;Group</translation>
    </message>
    <message>
      <source>&amp;Ungroup</source>
      <translation>&amp;Ungroup</translation>
    </message>
    <message>
      <source>Is &amp;Locked</source>
      <translation>Is &amp;Locked</translation>
    </message>
    <message>
      <source>Si&amp;ze is Locked</source>
      <translation>Si&amp;ze is Locked</translation>
    </message>
    <message>
      <source>Lower to &amp;Bottom</source>
      <translation>Lower to &amp;Bottom</translation>
    </message>
    <message>
      <source>Raise to &amp;Top</source>
      <translation>Raise to &amp;Top</translation>
    </message>
    <message>
      <source>&amp;Lower</source>
      <translation>&amp;Lower</translation>
    </message>
    <message>
      <source>&amp;Raise</source>
      <translation>&amp;Raise</translation>
    </message>
    <message>
      <source>Send to S&amp;crapbook</source>
      <translation>Send to S&amp;crapbook</translation>
    </message>
    <message>
      <source>&amp;Attributes...</source>
      <translation>&amp;Attributes...</translation>
    </message>
    <message>
      <source>I&amp;mage Visible</source>
      <translation>I&amp;mage Visible</translation>
    </message>
    <message>
      <source>Is PDF &amp;Bookmark</source>
      <translation>Is PDF &amp;Bookmark</translation>
    </message>
    <message>
      <source>Is PDF A&amp;nnotation</source>
      <translation>Is PDF A&amp;nnotation</translation>
    </message>
    <message>
      <source>Annotation P&amp;roperties</source>
      <translation>Annotation P&amp;roperties</translation>
    </message>
    <message>
      <source>Field P&amp;roperties</source>
      <translation>Field P&amp;roperties</translation>
    </message>
    <message>
      <source>&amp;Edit Shape...</source>
      <translation>&amp;Edit Shape...</translation>
    </message>
    <message>
      <source>&amp;Attach Text to Path</source>
      <translation>&amp;Attach Text to Path</translation>
    </message>
    <message>
      <source>&amp;Detach Text from Path</source>
      <translation>&amp;Detach Text from Path</translation>
    </message>
    <message>
      <source>&amp;Combine Polygons</source>
      <translation>&amp;Combine Polygons</translation>
    </message>
    <message>
      <source>Split &amp;Polygons</source>
      <translation>Split &amp;Polygons</translation>
    </message>
    <message>
      <source>&amp;Bezier Curve</source>
      <translation>&amp;Bezier Curve</translation>
    </message>
    <message>
      <source>&amp;Image Frame</source>
      <translation>&amp;Image Frame</translation>
    </message>
    <message>
      <source>&amp;Outlines</source>
      <translation>&amp;Outlines</translation>
    </message>
    <message>
      <source>&amp;Polygon</source>
      <translation>&amp;Polygon</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation>&amp;Text Frame</translation>
    </message>
    <message>
      <source>&amp;Glyph...</source>
      <translation>&amp;Glyph...</translation>
    </message>
    <message>
      <source>Sample Text</source>
      <translation>Sample Text</translation>
    </message>
    <message>
      <source>&amp;Insert...</source>
      <translation>&amp;Insert...</translation>
    </message>
    <message>
      <source>Im&amp;port...</source>
      <translation>Im&amp;port...</translation>
    </message>
    <message>
      <source>&amp;Delete...</source>
      <translation>&amp;Delete...</translation>
    </message>
    <message>
      <source>&amp;Copy...</source>
      <translation>&amp;Copy...</translation>
    </message>
    <message>
      <source>&amp;Move...</source>
      <translation>&amp;Move...</translation>
    </message>
    <message>
      <source>&amp;Apply Master Page...</source>
      <translation>&amp;Apply Master Page...</translation>
    </message>
    <message>
      <source>Manage &amp;Guides...</source>
      <translation>Manage &amp;Guides...</translation>
    </message>
    <message>
      <source>&amp;Fit in window</source>
      <translation>&amp;Fit in window</translation>
    </message>
    <message>
      <source>&amp;50%</source>
      <translation>&amp;50%</translation>
    </message>
    <message>
      <source>&amp;75%</source>
      <translation>&amp;75%</translation>
    </message>
    <message>
      <source>&amp;100%</source>
      <translation>&amp;100%</translation>
    </message>
    <message>
      <source>&amp;200%</source>
      <translation>&amp;200%</translation>
    </message>
    <message>
      <source>&amp;Thumbnails</source>
      <translation>&amp;Thumbnails</translation>
    </message>
    <message>
      <source>Show &amp;Margins</source>
      <translation>Show &amp;Margins</translation>
    </message>
    <message>
      <source>Show &amp;Frames</source>
      <translation>Show &amp;Frames</translation>
    </message>
    <message>
      <source>Show &amp;Images</source>
      <translation>Show &amp;Images</translation>
    </message>
    <message>
      <source>Show &amp;Grid</source>
      <translation>Show &amp;Grid</translation>
    </message>
    <message>
      <source>Show G&amp;uides</source>
      <translation>Show G&amp;uides</translation>
    </message>
    <message>
      <source>Show &amp;Baseline Grid</source>
      <translation>Show &amp;Baseline Grid</translation>
    </message>
    <message>
      <source>Show &amp;Text Chain</source>
      <translation>Show &amp;Text Chain</translation>
    </message>
    <message>
      <source>Sn&amp;ap to Grid</source>
      <translation>Sn&amp;ap to Grid</translation>
    </message>
    <message>
      <source>Sna&amp;p to Guides</source>
      <translation>Sna&amp;p to Guides</translation>
    </message>
    <message>
      <source>&amp;Properties</source>
      <translation>&amp;Properties</translation>
    </message>
    <message>
      <source>&amp;Scrapbook</source>
      <translation>&amp;Scrapbook</translation>
    </message>
    <message>
      <source>&amp;Layers</source>
      <translation>&amp;Layers</translation>
    </message>
    <message>
      <source>&amp;Bookmarks</source>
      <translation>&amp;Bookmarks</translation>
    </message>
    <message>
      <source>&amp;Measurements</source>
      <translation>&amp;Measurements</translation>
    </message>
    <message>
      <source>Action &amp;History</source>
      <translation>Action &amp;History</translation>
    </message>
    <message>
      <source>Preflight &amp;Verifier</source>
      <translation>Preflight &amp;Verifier</translation>
    </message>
    <message>
      <source>&amp;Tools</source>
      <translation>&amp;Tools</translation>
    </message>
    <message>
      <source>P&amp;DF Tools</source>
      <translation>P&amp;DF Tools</translation>
    </message>
    <message>
      <source>Select Item</source>
      <translation>Select Item</translation>
    </message>
    <message>
      <source>T&amp;able</source>
      <translation>T&amp;able</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Shape</translation>
    </message>
    <message>
      <source>&amp;Line</source>
      <translation>&amp;Line</translation>
    </message>
    <message>
      <source>&amp;Freehand Line</source>
      <translation>&amp;Freehand Line</translation>
    </message>
    <message>
      <source>Rotate Item</source>
      <translation>Rotate Item</translation>
    </message>
    <message>
      <source>Zoom in or out</source>
      <translation>Zoom in or out</translation>
    </message>
    <message>
      <source>Zoom in</source>
      <translation>Zoom in</translation>
    </message>
    <message>
      <source>Zoom out</source>
      <translation>Zoom out</translation>
    </message>
    <message>
      <source>Edit Contents of Frame</source>
      <translation>Edit Contents of Frame</translation>
    </message>
    <message>
      <source>Link Text Frames</source>
      <translation>Link Text Frames</translation>
    </message>
    <message>
      <source>Unlink Text Frames</source>
      <translation>Unlink Text Frames</translation>
    </message>
    <message>
      <source>&amp;Eye Dropper</source>
      <translation>&amp;Eye Dropper</translation>
    </message>
    <message>
      <source>Copy Item Properties</source>
      <translation>Copy Item Properties</translation>
    </message>
    <message>
      <source>Edit the text with the Story Editor</source>
      <translation>Edit the text with the Story Editor</translation>
    </message>
    <message>
      <source>Insert Text Frame</source>
      <translation>Insert Text Frame</translation>
    </message>
    <message>
      <source>Insert Image Frame</source>
      <translation>Insert Image Frame</translation>
    </message>
    <message>
      <source>Insert Table</source>
      <translation>Insert Table</translation>
    </message>
    <message>
      <source>Insert Shape</source>
      <translation>Insert Shape</translation>
    </message>
    <message>
      <source>Insert Polygon</source>
      <translation>Insert Polygon</translation>
    </message>
    <message>
      <source>Insert Line</source>
      <translation>Insert Line</translation>
    </message>
    <message>
      <source>Insert Bezier Curve</source>
      <translation>Insert Bezier Curve</translation>
    </message>
    <message>
      <source>Insert Freehand Line</source>
      <translation>Insert Freehand Line</translation>
    </message>
    <message>
      <source>&amp;Manage Pictures</source>
      <translation>&amp;Manage Pictures</translation>
    </message>
    <message>
      <source>&amp;Hyphenate Text</source>
      <translation>&amp;Hyphenate Text</translation>
    </message>
    <message>
      <source>&amp;Generate Table Of Contents</source>
      <translation>&amp;Generate Table Of Contents</translation>
    </message>
    <message>
      <source>&amp;About Scribus</source>
      <translation>&amp;About Scribus</translation>
    </message>
    <message>
      <source>About &amp;Qt</source>
      <translation>About &amp;Qt</translation>
    </message>
    <message>
      <source>Toolti&amp;ps</source>
      <translation>Toolti&amp;ps</translation>
    </message>
    <message>
      <source>Scribus &amp;Manual...</source>
      <translation>Scribus &amp;Manual...</translation>
    </message>
    <message>
      <source>Smart &amp;Hyphen</source>
      <translation>Smart &amp;Hyphen</translation>
    </message>
    <message>
      <source>Non Breaking &amp;Space</source>
      <translation>Non Breaking &amp;Space</translation>
    </message>
    <message>
      <source>Page &amp;Number</source>
      <translation>Page &amp;Number</translation>
    </message>
    <message>
      <source>Copyright</source>
      <translation>Copyright</translation>
    </message>
    <message>
      <source>Registered Trademark</source>
      <translation>Registered Trademark</translation>
    </message>
    <message>
      <source>Trademark</source>
      <translation>Trademark</translation>
    </message>
    <message>
      <source>Bullet</source>
      <translation>Bullet</translation>
    </message>
    <message>
      <source>Em Dash</source>
      <translation>Em Dash</translation>
    </message>
    <message>
      <source>En Dash</source>
      <translation>En Dash</translation>
    </message>
    <message>
      <source>Figure Dash</source>
      <translation>Figure Dash</translation>
    </message>
    <message>
      <source>Quotation Dash</source>
      <translation>Quotation Dash</translation>
    </message>
    <message>
      <source>Apostrophe</source>
      <translation>Apostrophe</translation>
    </message>
    <message>
      <source>Straight Double</source>
      <translation>Straight Double</translation>
    </message>
    <message>
      <source>Single Left</source>
      <translation>Single Left</translation>
    </message>
    <message>
      <source>Single Right</source>
      <translation>Single Right</translation>
    </message>
    <message>
      <source>Double Left</source>
      <translation>Double Left</translation>
    </message>
    <message>
      <source>Double Right</source>
      <translation>Double Right</translation>
    </message>
    <message>
      <source>Single Reversed</source>
      <translation>Single Reversed</translation>
    </message>
    <message>
      <source>Double Reversed</source>
      <translation>Double Reversed</translation>
    </message>
    <message>
      <source>Single Left Guillemet</source>
      <translation>Single Left Guillemet</translation>
    </message>
    <message>
      <source>Single Right Guillemet</source>
      <translation>Single Right Guillemet</translation>
    </message>
    <message>
      <source>Double Left Guillemet</source>
      <translation>Double Left Guillemet</translation>
    </message>
    <message>
      <source>Double Right Guillemet</source>
      <translation>Double Right Guillemet</translation>
    </message>
    <message>
      <source>Low Single Comma</source>
      <translation>Low Single Comma</translation>
    </message>
    <message>
      <source>Low Double Comma</source>
      <translation>Low Double Comma</translation>
    </message>
    <message>
      <source>Double Turned Comma</source>
      <translation>Double Turned Comma</translation>
    </message>
    <message>
      <source>CJK Single Left</source>
      <translation>CJK Single Left</translation>
    </message>
    <message>
      <source>CJK Single Right</source>
      <translation>CJK Single Right</translation>
    </message>
    <message>
      <source>CJK Double Left</source>
      <translation>CJK Double Left</translation>
    </message>
    <message>
      <source>CJK Double Right</source>
      <translation>CJK Double Right</translation>
    </message>
    <message>
      <source>Toggle Palettes</source>
      <translation>Toggle Palettes</translation>
    </message>
    <message>
      <source>S&amp;hadow</source>
      <translation>S&amp;hadow</translation>
    </message>
    <message>
      <source>&amp;Low Resolution</source>
      <translation>&amp;Low Resolution</translation>
    </message>
    <message>
      <source>&amp;Normal Resolution</source>
      <translation>&amp;Normal Resolution</translation>
    </message>
    <message>
      <source>&amp;Full Resolution</source>
      <translation>&amp;Full Resolution</translation>
    </message>
    <message>
      <source>Edit Image...</source>
      <translation>Edit Image...</translation>
    </message>
    <message>
      <source>&amp;Update Image</source>
      <translation>&amp;Update Image</translation>
    </message>
    <message>
      <source>Adjust Frame to Image</source>
      <translation>Adjust Frame to Image</translation>
    </message>
    <message>
      <source>Extended Image Properties</source>
      <translation>Extended Image Properties</translation>
    </message>
    <message>
      <source>Show Control Characters</source>
      <translation>Show Control Characters</translation>
    </message>
    <message>
      <source>&amp;Align and Distribute</source>
      <translation>&amp;Align and Distribute</translation>
    </message>
    <message>
      <source>Edit Text...</source>
      <translation>Edit Text...</translation>
    </message>
    <message>
      <source>New Line</source>
      <translation>New Line</translation>
    </message>
    <message>
      <source>Frame Break</source>
      <translation>Frame Break</translation>
    </message>
    <message>
      <source>Column Break</source>
      <translation>Column Break</translation>
    </message>
    <message>
      <source>Non Breaking Dash</source>
      <translation>Non Breaking Dash</translation>
    </message>
    <message>
      <source>Toggle Guides</source>
      <translation>Toggle Guides</translation>
    </message>
    <message>
      <source>&amp;Arrange Pages</source>
      <translation>&amp;Arrange Pages</translation>
    </message>
    <message>
      <source>Dehyphenate Text</source>
      <translation>Dehyphenate Text</translation>
    </message>
    <message>
      <source>Manage Page Properties...</source>
      <translation>Manage Page Properties...</translation>
    </message>
    <message>
      <source>Rulers relative to Page</source>
      <translation>Rulers relative to Page</translation>
    </message>
  </context>
  <context>
    <name>AdvOptions</name>
    <message>
      <source>Advanced Options</source>
      <translation>Advanced Options</translation>
    </message>
    <message>
      <source>Creates PostScript Level 3</source>
      <translation>Creates PostScript Level 3</translation>
    </message>
    <message>
      <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
      <translation>Creates PostScript Level 2 only, beware,
this can create huge files</translation>
    </message>
    <message>
      <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
      <translation>Creates PostScript Level 1 only, beware,
this can create huge files</translation>
    </message>
    <message>
      <source>Mirror Page(s) &amp;Horizontal</source>
      <translation>Mirror Page(s) &amp;Horizontal</translation>
    </message>
    <message>
      <source>Mirror Page(s) &amp;Vertical</source>
      <translation>Mirror Page(s) &amp;Vertical</translation>
    </message>
    <message>
      <source>Apply &amp;ICC Profiles</source>
      <translation>Apply &amp;ICC Profiles</translation>
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
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Apply Under Color &amp;Removal</source>
      <translation>Apply Under Colour &amp;Removal</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>A way of switching off some of the grey shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the grey. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation>Set Media Size</translation>
    </message>
  </context>
  <context>
    <name>Align</name>
    <message>
      <source>Distribute/Align</source>
      <translation>Distribute/Align</translation>
    </message>
    <message>
      <source>Align</source>
      <translation>Align</translation>
    </message>
    <message>
      <source>Horizontal</source>
      <translation>Horizontal</translation>
    </message>
    <message>
      <source>Left Sides</source>
      <translation>Left Sides</translation>
    </message>
    <message>
      <source>Middles</source>
      <translation>Middles</translation>
    </message>
    <message>
      <source>Right Sides</source>
      <translation>Right Sides</translation>
    </message>
    <message>
      <source>Vertical</source>
      <translation>Vertical</translation>
    </message>
    <message>
      <source>Top Sides</source>
      <translation>Top Sides</translation>
    </message>
    <message>
      <source>Bottom Sides</source>
      <translation>Bottom Sides</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>&amp;Apply</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;Between:</source>
      <translation>&amp;Between:</translation>
    </message>
    <message>
      <source>A&amp;lign</source>
      <translation>A&amp;lign</translation>
    </message>
    <message>
      <source>Di&amp;splacement</source>
      <translation>Di&amp;splacement</translation>
    </message>
    <message>
      <source>Distribute &amp;Evenly</source>
      <translation>Distribute &amp;Evenly</translation>
    </message>
    <message>
      <source>Bet&amp;ween:</source>
      <translation>Bet&amp;ween:</translation>
    </message>
    <message>
      <source>Do &amp;Not Change</source>
      <translation>Do &amp;Not Change</translation>
    </message>
    <message>
      <source>Al&amp;ign</source>
      <translation>Al&amp;ign</translation>
    </message>
    <message>
      <source>Dis&amp;placement</source>
      <translation>Dis&amp;placement</translation>
    </message>
    <message>
      <source>Distribute E&amp;venly</source>
      <translation>Distribute E&amp;venly</translation>
    </message>
    <message>
      <source>&amp;Do Not Change</source>
      <translation>&amp;Do Not Change</translation>
    </message>
  </context>
  <context>
    <name>AlignDistributePalette</name>
    <message>
      <source>Align and Distribute</source>
      <translation>Align and Distribute</translation>
    </message>
    <message>
      <source>Align</source>
      <translation>Align</translation>
    </message>
    <message>
      <source>&amp;Relative to:</source>
      <translation>&amp;Relative to:</translation>
    </message>
    <message>
      <source>First Selected</source>
      <translation>First Selected</translation>
    </message>
    <message>
      <source>Last Selected</source>
      <translation>Last Selected</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Page</translation>
    </message>
    <message>
      <source>Margins</source>
      <translation>Margins</translation>
    </message>
    <message>
      <source>Selection</source>
      <translation>Selection</translation>
    </message>
    <message>
      <source>Align right sides of objects to left side of anchor</source>
      <translation>Align right sides of objects to left side of anchor</translation>
    </message>
    <message>
      <source>Align left sides of objects to right side of anchor</source>
      <translation>Align left sides of objects to right side of anchor</translation>
    </message>
    <message>
      <source>Align bottoms</source>
      <translation>Align bottoms</translation>
    </message>
    <message>
      <source>Align right sides</source>
      <translation>Align right sides</translation>
    </message>
    <message>
      <source>Align tops of objects to bottom of anchor</source>
      <translation>Align tops of objects to bottom of anchor</translation>
    </message>
    <message>
      <source>Center on vertical axis</source>
      <translation>Centre on vertical axis</translation>
    </message>
    <message>
      <source>Align left sides</source>
      <translation>Align left sides</translation>
    </message>
    <message>
      <source>Center on horizontal axis</source>
      <translation>Centre on horizontal axis</translation>
    </message>
    <message>
      <source>Align bottoms of objects to top of anchor</source>
      <translation>Align bottoms of objects to top of anchor</translation>
    </message>
    <message>
      <source>Align tops</source>
      <translation>Align tops</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>Distribute</translation>
    </message>
    <message>
      <source>Distribute left sides equidistantly</source>
      <translation>Distribute left sides equidistantly</translation>
    </message>
    <message>
      <source>Distribute bottoms equidistantly</source>
      <translation>Distribute bottoms equidistantly</translation>
    </message>
    <message>
      <source>Distribute centers equidistantly horizontally</source>
      <translation>Distribute centres equidistantly horizontally</translation>
    </message>
    <message>
      <source>Make vertical gaps between objects equal</source>
      <translation>Make vertical gaps between objects equal</translation>
    </message>
    <message>
      <source>Distribute centers equidistantly vertically</source>
      <translation>Distribute centres equidistantly vertically</translation>
    </message>
    <message>
      <source>Distribute tops equidistantly</source>
      <translation>Distribute tops equidistantly</translation>
    </message>
    <message>
      <source>Distribute right sides equidistantly</source>
      <translation>Distribute right sides equidistantly</translation>
    </message>
    <message>
      <source>Distance</source>
      <translation>Distance</translation>
    </message>
  </context>
  <context>
    <name>AlignSelect</name>
    <message>
      <source>Align Text Left</source>
      <translation>Align Text Left</translation>
    </message>
    <message>
      <source>Align Text Right</source>
      <translation>Align Text Right</translation>
    </message>
    <message>
      <source>Align Text Center</source>
      <translation>Align Text Centre</translation>
    </message>
    <message>
      <source>Align Text Justified</source>
      <translation>Align Text Justified</translation>
    </message>
    <message>
      <source>Align Text Forced Justified</source>
      <translation>Align Text Forced Justified</translation>
    </message>
  </context>
  <context>
    <name>Annot</name>
    <message>
      <source>Field Properties</source>
      <translation>Field Properties</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>Type:</translation>
    </message>
    <message>
      <source>Button</source>
      <translation>Button</translation>
    </message>
    <message>
      <source>Text Field</source>
      <translation>Text Field</translation>
    </message>
    <message>
      <source>Check Box</source>
      <translation>Check Box</translation>
    </message>
    <message>
      <source>Combo Box</source>
      <translation>Combo Box</translation>
    </message>
    <message>
      <source>List Box</source>
      <translation>List Box</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>Properties</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Name:</translation>
    </message>
    <message>
      <source>Tool-Tip:</source>
      <translation>Tool-Tip:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Border</source>
      <translation>Border</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Colour:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>Width:</translation>
    </message>
    <message>
      <source>Thin</source>
      <translation>Thin</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Wide</source>
      <translation>Wide</translation>
    </message>
    <message>
      <source>Style:</source>
      <translation>Style:</translation>
    </message>
    <message>
      <source>Solid</source>
      <translation>Solid</translation>
    </message>
    <message>
      <source>Dashed</source>
      <translation>Dashed</translation>
    </message>
    <message>
      <source>Underline</source>
      <translation>Underline</translation>
    </message>
    <message>
      <source>Beveled</source>
      <translation>Beveled</translation>
    </message>
    <message>
      <source>Inset</source>
      <translation>Inset</translation>
    </message>
    <message>
      <source>Other</source>
      <translation>Other</translation>
    </message>
    <message>
      <source>Read Only</source>
      <translation>Read Only</translation>
    </message>
    <message>
      <source>Required</source>
      <translation>Required</translation>
    </message>
    <message>
      <source>Don't Export Value</source>
      <translation>Don't Export Value</translation>
    </message>
    <message>
      <source>Visibility:</source>
      <translation>Visibility:</translation>
    </message>
    <message>
      <source>Visible</source>
      <translation>Visible</translation>
    </message>
    <message>
      <source>Hidden</source>
      <translation>Hidden</translation>
    </message>
    <message>
      <source>No Print</source>
      <translation>No Print</translation>
    </message>
    <message>
      <source>No View</source>
      <translation>No View</translation>
    </message>
    <message>
      <source>Appearance</source>
      <translation>Appearance</translation>
    </message>
    <message>
      <source>Text for Button Down</source>
      <translation>Text for Button Down</translation>
    </message>
    <message>
      <source>Text for Roll Over</source>
      <translation>Text for Roll Over</translation>
    </message>
    <message>
      <source>Icons</source>
      <translation>Icons</translation>
    </message>
    <message>
      <source>Use Icons</source>
      <translation>Use Icons</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>Remove</translation>
    </message>
    <message>
      <source>Pressed</source>
      <translation>Pressed</translation>
    </message>
    <message>
      <source>Roll Over</source>
      <translation>Roll Over</translation>
    </message>
    <message>
      <source>Icon Placement...</source>
      <translation>Icon Placement...</translation>
    </message>
    <message>
      <source>Highlight</source>
      <translation>Highlight</translation>
    </message>
    <message>
      <source>Invert</source>
      <translation>Invert</translation>
    </message>
    <message>
      <source>Outlined</source>
      <translation>Outlined</translation>
    </message>
    <message>
      <source>Push</source>
      <translation>Push</translation>
    </message>
    <message>
      <source>Multi-Line</source>
      <translation>Multi-Line</translation>
    </message>
    <message>
      <source>Password</source>
      <translation>Password</translation>
    </message>
    <message>
      <source>Limit of</source>
      <translation>Limit of</translation>
    </message>
    <message>
      <source>Characters</source>
      <translation>Characters</translation>
    </message>
    <message>
      <source>Do Not Scroll</source>
      <translation>Do Not Scroll</translation>
    </message>
    <message>
      <source>Do Not Spell Check</source>
      <translation>Do Not Spell Check</translation>
    </message>
    <message>
      <source>Check Style:</source>
      <translation>Check Style:</translation>
    </message>
    <message>
      <source>Check</source>
      <translation>Check</translation>
    </message>
    <message>
      <source>Cross</source>
      <translation>Cross</translation>
    </message>
    <message>
      <source>Diamond</source>
      <translation>Diamond</translation>
    </message>
    <message>
      <source>Circle</source>
      <translation>Circle</translation>
    </message>
    <message>
      <source>Star</source>
      <translation>Star</translation>
    </message>
    <message>
      <source>Square</source>
      <translation>Square</translation>
    </message>
    <message>
      <source>Default is Checked</source>
      <translation>Default is Checked</translation>
    </message>
    <message>
      <source>Editable</source>
      <translation>Editable</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>Java Script</source>
      <translation>Java Script</translation>
    </message>
    <message>
      <source>Go To</source>
      <translation>Go To</translation>
    </message>
    <message>
      <source>Submit Form</source>
      <translation>Submit Form</translation>
    </message>
    <message>
      <source>Reset Form</source>
      <translation>Reset Form</translation>
    </message>
    <message>
      <source>Import Data</source>
      <translation>Import Data</translation>
    </message>
    <message>
      <source>Event:</source>
      <translation>Event:</translation>
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
      <translation>Script:</translation>
    </message>
    <message>
      <source>Edit...</source>
      <translation>Edit...</translation>
    </message>
    <message>
      <source>Submit to URL:</source>
      <translation>Submit to URL:</translation>
    </message>
    <message>
      <source>Submit Data as HTML</source>
      <translation>Submit Data as HTML</translation>
    </message>
    <message>
      <source>Import Data from:</source>
      <translation>Import Data from:</translation>
    </message>
    <message>
      <source>Destination</source>
      <translation>Destination</translation>
    </message>
    <message>
      <source>To File:</source>
      <translation>To File:</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>Change...</translation>
    </message>
    <message>
      <source>Page:</source>
      <translation>Page:</translation>
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
      <translation>Action</translation>
    </message>
    <message>
      <source>Field is formatted as:</source>
      <translation>Field is formatted as:</translation>
    </message>
    <message>
      <source>Plain</source>
      <translation>Plain</translation>
    </message>
    <message>
      <source>Number</source>
      <translation>Number</translation>
    </message>
    <message>
      <source>Percentage</source>
      <translation>Percentage</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>Date</translation>
    </message>
    <message>
      <source>Time</source>
      <translation>Time</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Number Format</source>
      <translation>Number Format</translation>
    </message>
    <message>
      <source>Decimals:</source>
      <translation>Decimals:</translation>
    </message>
    <message>
      <source>Use Currency Symbol</source>
      <translation>Use Currency Symbol</translation>
    </message>
    <message>
      <source>Prepend Currency Symbol</source>
      <translation>Prepend Currency Symbol</translation>
    </message>
    <message>
      <source>Formatting</source>
      <translation>Formatting</translation>
    </message>
    <message>
      <source>Percent Format</source>
      <translation>Percent Format</translation>
    </message>
    <message>
      <source>Date Format</source>
      <translation>Date Format</translation>
    </message>
    <message>
      <source>Time Format</source>
      <translation>Time Format</translation>
    </message>
    <message>
      <source>Custom Scripts</source>
      <translation>Custom Scripts</translation>
    </message>
    <message>
      <source>Format:</source>
      <translation>Format:</translation>
    </message>
    <message>
      <source>Keystroke:</source>
      <translation>Keystroke:</translation>
    </message>
    <message>
      <source>Format</source>
      <translation>Format</translation>
    </message>
    <message>
      <source>Value is not validated</source>
      <translation>Value is not validated</translation>
    </message>
    <message>
      <source>Value must be greater than or equal to:</source>
      <translation>Value must be greater than or equal to:</translation>
    </message>
    <message>
      <source>and less or equal to:</source>
      <translation>and less or equal to:</translation>
    </message>
    <message>
      <source>Custom validate script:</source>
      <translation>Custom validate script:</translation>
    </message>
    <message>
      <source>Validate</source>
      <translation>Validate</translation>
    </message>
    <message>
      <source>Value is not calculated</source>
      <translation>Value is not calculated</translation>
    </message>
    <message>
      <source>Value is the</source>
      <translation>Value is the</translation>
    </message>
    <message>
      <source>sum</source>
      <translation>sum</translation>
    </message>
    <message>
      <source>product</source>
      <translation>product</translation>
    </message>
    <message>
      <source>average</source>
      <translation>average</translation>
    </message>
    <message>
      <source>minimum</source>
      <translation>minimum</translation>
    </message>
    <message>
      <source>maximum</source>
      <translation>maximum</translation>
    </message>
    <message>
      <source>of the following fields:</source>
      <translation>of the following fields:</translation>
    </message>
    <message>
      <source>Pick...</source>
      <translation>Pick...</translation>
    </message>
    <message>
      <source>Custom calculation script:</source>
      <translation>Custom calculation script:</translation>
    </message>
    <message>
      <source>Calculate</source>
      <translation>Calculate</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
    <message>
      <source>Enter a comma separated list of fields here</source>
      <translation>Enter a comma separated list of fields here</translation>
    </message>
    <message>
      <source>You need at least the Icon for Normal to use Icons for Buttons</source>
      <translation>You need at least the Icon for Normal to use Icons for Buttons</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
      <translation>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</translation>
    </message>
    <message>
      <source>Example:</source>
      <translation>Example:</translation>
    </message>
    <message>
      <source>Selection Change</source>
      <translation>Selection Change</translation>
    </message>
    <message>
      <source>Font for use with PDF 1.3:</source>
      <translation>Font for use with PDF 1.3:</translation>
    </message>
    <message>
      <source>Flag is ignored for PDF 1.3</source>
      <translation>Flag is ignored for PDF 1.3</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDF Files (*.pdf);;All Files (*)</translation>
    </message>
  </context>
  <context>
    <name>Annota</name>
    <message>
      <source>Annotation Properties</source>
      <translation>Annotation Properties</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Link</source>
      <translation>Link</translation>
    </message>
    <message>
      <source>External Link</source>
      <translation>External Link</translation>
    </message>
    <message>
      <source>External Web-Link</source>
      <translation>External Web-Link</translation>
    </message>
    <message>
      <source>Destination</source>
      <translation>Destination</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>PDF-Documents (*.pdf);;All Files (*)</source>
      <translation>PDF-Documents (*.pdf);;All Files (*)</translation>
    </message>
    <message>
      <source>&amp;Type:</source>
      <translation>&amp;Type:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>C&amp;hange...</translation>
    </message>
    <message>
      <source>&amp;Page:</source>
      <translation>&amp;Page:</translation>
    </message>
    <message>
      <source>&amp;X-Pos</source>
      <translation>&amp;X-Pos</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>ApplyMasterPageDialog</name>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Apply Master Page</source>
      <translation>Apply Master Page</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>&amp;Master Page:</translation>
    </message>
    <message>
      <source>Apply To</source>
      <translation>Apply To</translation>
    </message>
    <message>
      <source>Current &amp;page</source>
      <translation>Current &amp;page</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>&amp;Even pages</source>
      <translation>&amp;Even pages</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>O&amp;dd pages</source>
      <translation>O&amp;dd pages</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>&amp;All pages</source>
      <translation>&amp;All pages</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Within range</source>
      <translation>&amp;Within range</translation>
    </message>
    <message>
      <source>Alt+W</source>
      <translation>Alt+W</translation>
    </message>
    <message>
      <source>&lt;qt>Apply the selected template to even, odd or all pages within the following range&lt;/qt></source>
      <translation>&lt;qt>Apply the selected template to even, odd or all pages within the following range&lt;/qt></translation>
    </message>
    <message>
      <source>to</source>
      <translation>to</translation>
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
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
  </context>
  <context>
    <name>ArrowChooser</name>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
  </context>
  <context>
    <name>Biblio</name>
    <message>
      <source>Scrapbook</source>
      <translation>Scrapbook</translation>
    </message>
    <message>
      <source>Scrapbooks (*.scs);;All Files (*)</source>
      <translation>Scrapbooks (*.scs);;All Files (*)</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>Delete</translation>
    </message>
    <message>
      <source>Object</source>
      <translation>Object</translation>
    </message>
    <message>
      <source>New Entry</source>
      <translation>New Entry</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation>Rename</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.
Please choose another.</source>
      <translation>Name &quot;%1&quot; isn't unique.
Please choose another.</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Load...</source>
      <translation>&amp;Load...</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Save &amp;As...</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>&amp;Small</source>
      <translation>&amp;Small</translation>
    </message>
    <message>
      <source>&amp;Medium</source>
      <translation>&amp;Medium</translation>
    </message>
    <message>
      <source>&amp;Large</source>
      <translation>&amp;Large</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
    </message>
    <message>
      <source>&amp;Preview</source>
      <translation>&amp;Preview</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
  </context>
  <context>
    <name>BookMView</name>
    <message>
      <source>Bookmarks</source>
      <translation>Bookmarks</translation>
    </message>
    <message>
      <source>Move Bookmark</source>
      <translation>Move Bookmark</translation>
    </message>
    <message>
      <source>Insert Bookmark</source>
      <translation>Insert Bookmark</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
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
      <source>Icon Placement</source>
      <translation>Icon Placement</translation>
    </message>
    <message>
      <source>Layout:</source>
      <translation>Layout:</translation>
    </message>
    <message>
      <source>Caption only</source>
      <translation>Caption only</translation>
    </message>
    <message>
      <source>Icon only</source>
      <translation>Icon only</translation>
    </message>
    <message>
      <source>Caption below Icon</source>
      <translation>Caption below Icon</translation>
    </message>
    <message>
      <source>Caption above Icon</source>
      <translation>Caption above Icon</translation>
    </message>
    <message>
      <source>Caption right to Icon</source>
      <translation>Caption right to Icon</translation>
    </message>
    <message>
      <source>Caption left to Icon</source>
      <translation>Caption left to Icon</translation>
    </message>
    <message>
      <source>Caption overlays Icon</source>
      <translation>Caption overlays Icon</translation>
    </message>
    <message>
      <source>Scale:</source>
      <translation>Scale:</translation>
    </message>
    <message>
      <source>Always</source>
      <translation>Always</translation>
    </message>
    <message>
      <source>When Icon is too big</source>
      <translation>When Icon is too big</translation>
    </message>
    <message>
      <source>Never</source>
      <translation>Never</translation>
    </message>
    <message>
      <source>Scale How:</source>
      <translation>Scale How:</translation>
    </message>
    <message>
      <source>Proportional</source>
      <translation>Proportional</translation>
    </message>
    <message>
      <source>Non Proportional</source>
      <translation>Non Proportional</translation>
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
      <translation>Cancel</translation>
    </message>
    <message>
      <source>Reset</source>
      <translation>Reset</translation>
    </message>
    <message>
      <source>When Icon is too small</source>
      <translation>When Icon is too small</translation>
    </message>
  </context>
  <context>
    <name>CMSPrefs</name>
    <message>
      <source>System Profiles</source>
      <translation>System Profiles</translation>
    </message>
    <message>
      <source>Rendering Intents</source>
      <translation>Rendering Intents</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>Perceptual</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>Relative Colourimetric</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturation</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>Absolute Colourimetric</translation>
    </message>
    <message>
      <source>Default color profile for imported images</source>
      <translation>Default colour profile for imported images</translation>
    </message>
    <message>
      <source>Default color profile for solid colors on the page</source>
      <translation>Default colour profile for solid colours on the page</translation>
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
      <translation>Black Point Compensation is a method of improving contrast in photos.
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
      <source>Enable 'soft proofing' of how your document colors will print,
based on the chosen printer profile.</source>
      <translation>Enable 'soft proofing' of how your document colours will print,
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
      <source>&amp;Pictures:</source>
      <translation>&amp;Pictures:</translation>
    </message>
    <message>
      <source>&amp;Solid Colors:</source>
      <translation>&amp;Solid Colours:</translation>
    </message>
    <message>
      <source>&amp;Monitor:</source>
      <translation>&amp;Monitor:</translation>
    </message>
    <message>
      <source>P&amp;rinter:</source>
      <translation>P&amp;rinter:</translation>
    </message>
    <message>
      <source>M&amp;onitor:</source>
      <translation>M&amp;onitor:</translation>
    </message>
    <message>
      <source>Pr&amp;inter:</source>
      <translation>Pr&amp;inter:</translation>
    </message>
    <message>
      <source>Sim&amp;ulate Printer on the Screen</source>
      <translation>Sim&amp;ulate Printer on the Screen</translation>
    </message>
    <message>
      <source>Mark Colors out of &amp;Gamut</source>
      <translation>Mark Colours out of &amp;Gamut</translation>
    </message>
    <message>
      <source>Use &amp;Blackpoint Compensation</source>
      <translation>Use &amp;Blackpoint Compensation</translation>
    </message>
  </context>
  <context>
    <name>CMYKChoose</name>
    <message>
      <source>Edit Color</source>
      <translation>Edit Colour</translation>
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
      <translation>Web Safe RGB</translation>
    </message>
    <message>
      <source>New</source>
      <translation>New</translation>
    </message>
    <message>
      <source>Old</source>
      <translation>Old</translation>
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
      <translation>Dynamic Colour Bars</translation>
    </message>
    <message>
      <source>Static Color Bars</source>
      <translation>Static Colour Bars</translation>
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
      <translation>Warning</translation>
    </message>
    <message>
      <source>Name of the Color is not unique</source>
      <translation>Name of the colour is not unique</translation>
    </message>
    <message>
      <source>HSV-Colormap</source>
      <translation>HSV-Colourmap</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>Color &amp;Model</source>
      <translation>Colour &amp;Model</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>You cannot create a color named &quot;%1&quot;.
It's a reserved name for transparent color</source>
      <translation>You cannot create a colour named &quot;%1&quot;.
It's a reserved name for transparent colour</translation>
    </message>
  </context>
  <context>
    <name>ChTable</name>
    <message>
      <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
      <translation>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</translation>
    </message>
  </context>
  <context>
    <name>CharSelect</name>
    <message>
      <source>Select Character:</source>
      <translation>Select Character:</translation>
    </message>
    <message>
      <source>Font:</source>
      <translation>Font:</translation>
    </message>
    <message>
      <source>Character Class:</source>
      <translation>Character Class:</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>&amp;Insert</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>Insert the characters at the cursor in the text</source>
      <translation>Insert the characters at the cursor in the text</translation>
    </message>
    <message>
      <source>Delete the current selection(s).</source>
      <translation>Delete the current selection(s).</translation>
    </message>
    <message>
      <source>Close this dialog and return to text editing.</source>
      <translation>Close this dialog and return to text editing.</translation>
    </message>
    <message>
      <source>Full Character Set</source>
      <translation>Full Character Set</translation>
    </message>
    <message>
      <source>Basic Latin</source>
      <translation>Basic Latin</translation>
    </message>
    <message>
      <source>Latin-1 Supplement</source>
      <translation>Latin-1 Supplement</translation>
    </message>
    <message>
      <source>Latin Extended-A</source>
      <translation>Latin Extended-A</translation>
    </message>
    <message>
      <source>Latin Extended-B</source>
      <translation>Latin Extended-B</translation>
    </message>
    <message>
      <source>General Punctuation</source>
      <translation>General Punctuation</translation>
    </message>
    <message>
      <source>Super- and Subscripts</source>
      <translation>Super- and Subscripts</translation>
    </message>
    <message>
      <source>Currency Symbols</source>
      <translation>Currency Symbols</translation>
    </message>
    <message>
      <source>Letterlike Symbols</source>
      <translation>Letterlike Symbols</translation>
    </message>
    <message>
      <source>Number Forms</source>
      <translation>Number Forms</translation>
    </message>
    <message>
      <source>Arrows</source>
      <translation>Arrows</translation>
    </message>
    <message>
      <source>Mathematical Operators</source>
      <translation>Mathematical Operators</translation>
    </message>
    <message>
      <source>Box Drawing</source>
      <translation>Box Drawing</translation>
    </message>
    <message>
      <source>Block Elements</source>
      <translation>Block Elements</translation>
    </message>
    <message>
      <source>Geometric Shapes</source>
      <translation>Geometric Shapes</translation>
    </message>
    <message>
      <source>Miscellaneous Symbols</source>
      <translation>Miscellaneous Symbols</translation>
    </message>
    <message>
      <source>Dingbats</source>
      <translation>Dingbats</translation>
    </message>
    <message>
      <source>Small Form Variants</source>
      <translation>Small Form Variants</translation>
    </message>
    <message>
      <source>Ligatures</source>
      <translation>Ligatures</translation>
    </message>
    <message>
      <source>Specials</source>
      <translation>Specials</translation>
    </message>
    <message>
      <source>Greek</source>
      <translation>Greek</translation>
    </message>
    <message>
      <source>Greek Extended</source>
      <translation>Greek Extended</translation>
    </message>
    <message>
      <source>Cyrillic</source>
      <translation>Cyrillic</translation>
    </message>
    <message>
      <source>Cyrillic Supplement</source>
      <translation>Cyrillic Supplement</translation>
    </message>
    <message>
      <source>Arabic</source>
      <translation>Arabic</translation>
    </message>
    <message>
      <source>Arabic Extended A</source>
      <translation>Arabic Extended A</translation>
    </message>
    <message>
      <source>Arabic Extended B</source>
      <translation>Arabic Extended B</translation>
    </message>
    <message>
      <source>Hebrew</source>
      <translation>Hebrew</translation>
    </message>
  </context>
  <context>
    <name>CheckDocument</name>
    <message>
      <source>Glyphs missing</source>
      <translation>Glyphs missing</translation>
    </message>
    <message>
      <source>Text overflow</source>
      <translation>Text overflow</translation>
    </message>
    <message>
      <source>Object is not on a Page</source>
      <translation>Object is not on a Page</translation>
    </message>
    <message>
      <source>Missing Image</source>
      <translation>Missing Image</translation>
    </message>
    <message>
      <source>Image has a DPI-Value less than %1 DPI</source>
      <translation>Image has a DPI-Value less than %1 DPI</translation>
    </message>
    <message>
      <source>Object has transparency</source>
      <translation>Object has transparency</translation>
    </message>
    <message>
      <source>Object is a PDF Annotation or Field</source>
      <translation>Object is a PDF Annotation or Field</translation>
    </message>
    <message>
      <source>Object is a placed PDF</source>
      <translation>Object is a placed PDF</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Document</translation>
    </message>
    <message>
      <source>No Problems found</source>
      <translation>No Problems found</translation>
    </message>
    <message>
      <source>Page </source>
      <translation>Page </translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation>Free Objects</translation>
    </message>
    <message>
      <source>Problems found</source>
      <translation>Problems found</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Preflight Verifier</translation>
    </message>
    <message>
      <source>Items</source>
      <translation>Items</translation>
    </message>
    <message>
      <source>Problems</source>
      <translation>Problems</translation>
    </message>
    <message>
      <source>Current Profile:</source>
      <translation>Current Profile:</translation>
    </message>
    <message>
      <source>&amp;Ignore Errors</source>
      <translation>&amp;Ignore Errors</translation>
    </message>
  </context>
  <context>
    <name>ChooseStyles</name>
    <message>
      <source>Choose Styles</source>
      <translation>Choose Styles</translation>
    </message>
    <message>
      <source>Available Styles</source>
      <translation>Available Styles</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
  </context>
  <context>
    <name>ColorWheel</name>
    <message>
      <source>Monochromatic</source>
      <translation>Monochromatic</translation>
    </message>
    <message>
      <source>Analogous</source>
      <translation>Analogous</translation>
    </message>
    <message>
      <source>Complementary</source>
      <translation>Complementary</translation>
    </message>
    <message>
      <source>Split Complementary</source>
      <translation>Split Complementary</translation>
    </message>
    <message>
      <source>Triadic</source>
      <translation>Triadic</translation>
    </message>
    <message>
      <source>Tetradic (Double Complementary)</source>
      <translation>Tetradic (Double Complementary)</translation>
    </message>
    <message>
      <source>Base Color</source>
      <translation>Base Colour</translation>
    </message>
    <message>
      <source>Monochromatic Light</source>
      <translation>Monochromatic Light</translation>
    </message>
    <message>
      <source>Monochromatic Dark</source>
      <translation>Monochromatic Dark</translation>
    </message>
    <message>
      <source>1st. Analogous</source>
      <translation>1st. Analogous</translation>
    </message>
    <message>
      <source>2nd. Analogous</source>
      <translation>2nd. Analogous</translation>
    </message>
    <message>
      <source>1st. Split</source>
      <translation>1st. Split</translation>
    </message>
    <message>
      <source>2nd. Split</source>
      <translation>2nd. Split</translation>
    </message>
    <message>
      <source>3rd. Split</source>
      <translation>3rd. Split</translation>
    </message>
    <message>
      <source>4th. Split</source>
      <translation>4th. Split</translation>
    </message>
    <message>
      <source>1st. Triadic</source>
      <translation>1st. Triadic</translation>
    </message>
    <message>
      <source>2nd. Triadic</source>
      <translation>2nd. Triadic</translation>
    </message>
    <message>
      <source>1st. Tetradic (base opposite)</source>
      <translation>1st. Tetradic (base opposite)</translation>
    </message>
    <message>
      <source>2nd. Tetradic (angle)</source>
      <translation>2nd. Tetradic (angle)</translation>
    </message>
    <message>
      <source>3rd. Tetradic (angle opposite)</source>
      <translation>3rd. Tetradic (angle opposite)</translation>
    </message>
  </context>
  <context>
    <name>ColorWheelDialog</name>
    <message>
      <source>Color Wheel</source>
      <translation>Colour Wheel</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Colour</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>C</source>
      <translation>C</translation>
    </message>
    <message>
      <source>M</source>
      <translation>M</translation>
    </message>
    <message>
      <source>Y</source>
      <translation>Y</translation>
    </message>
    <message>
      <source>K</source>
      <translation>K</translation>
    </message>
    <message>
      <source>Select Method:</source>
      <translation>Select Method:</translation>
    </message>
    <message>
      <source>Angle (0 - 90 degrees):</source>
      <translation>Angle (0 - 90 degrees):</translation>
    </message>
    <message>
      <source>&amp;Merge Colors</source>
      <translation>&amp;Merge Colours</translation>
    </message>
    <message>
      <source>&amp;Replace Colors</source>
      <translation>&amp;Replace Colours</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
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
      <source>Difference between selected value and counted ones. See documentation for more info</source>
      <translation>Difference between selected value and counted ones. See documentation for more info</translation>
    </message>
    <message>
      <source>Click the wheel to get base color</source>
      <translation>Click the wheel to get base colour</translation>
    </message>
    <message>
      <source>Here you have the sample color schema</source>
      <translation>Here you have the sample colour schema</translation>
    </message>
    <message>
      <source>Select one of the method to create color schema. See documentation for more info</source>
      <translation>Select one of the method to create colour schema. See documentation for more info</translation>
    </message>
    <message>
      <source>Here you have the color of your chosen color schema</source>
      <translation>Here you have the colour of your chosen colour schema</translation>
    </message>
    <message>
      <source>Merging colors</source>
      <translation>Merging colours</translation>
    </message>
    <message>
      <source>Error: </source>
      <translation>Error: </translation>
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
      <translation>Normal Vision</translation>
    </message>
    <message>
      <source>Protanopy</source>
      <translation>Protanopy</translation>
    </message>
    <message>
      <source>Deuteranopy</source>
      <translation>Deuteranopy</translation>
    </message>
    <message>
      <source>Full Color Blindness</source>
      <translation>Full Colour Blindness</translation>
    </message>
    <message>
      <source>Vision Defect:</source>
      <translation>Vision Defect:</translation>
    </message>
    <message>
      <source>You can simulate common vision defects here. Just select type of the defect</source>
      <translation>You can simulate common vision defects here. Just select type of the defect</translation>
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
      <translation>Horizontal Gradient</translation>
    </message>
    <message>
      <source>Vertical Gradient</source>
      <translation>Vertical Gradient</translation>
    </message>
    <message>
      <source>Diagonal Gradient</source>
      <translation>Diagonal Gradient</translation>
    </message>
    <message>
      <source>Cross Diagonal Gradient</source>
      <translation>Cross Diagonal Gradient</translation>
    </message>
    <message>
      <source>Radial Gradient</source>
      <translation>Radial Gradient</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>Opacity:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Shade:</source>
      <translation>Shade:</translation>
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
      <translation>Normal or gradient fill method</translation>
    </message>
    <message>
      <source>Set the transparency for the color selected</source>
      <translation>Set the transparency for the colour selected</translation>
    </message>
    <message>
      <source>Color of selected object</source>
      <translation>Colour of selected object</translation>
    </message>
    <message>
      <source>Free linear Gradient</source>
      <translation>Free linear Gradient</translation>
    </message>
    <message>
      <source>Free radial Gradient</source>
      <translation>Free radial Gradient</translation>
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
      <source>Move Vector</source>
      <translation>Move Vector</translation>
    </message>
    <message>
      <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
      <translation>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</translation>
    </message>
  </context>
  <context>
    <name>CsvDialog</name>
    <message>
      <source>CSV Importer Options</source>
      <translation>CSV Importer Options</translation>
    </message>
    <message>
      <source>Field delimiter:</source>
      <translation>Field delimiter:</translation>
    </message>
    <message>
      <source>(TAB)</source>
      <translation>(TAB)</translation>
    </message>
    <message>
      <source>Value delimiter:</source>
      <translation>Value delimiter:</translation>
    </message>
    <message>
      <source>First row is a header</source>
      <translation>First row is a header</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
  </context>
  <context>
    <name>CupsOptions</name>
    <message>
      <source>Printer Options</source>
      <translation>Printer Options</translation>
    </message>
    <message>
      <source>Option</source>
      <translation>Option</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>Value</translation>
    </message>
    <message>
      <source>Page Set</source>
      <translation>Page Set</translation>
    </message>
    <message>
      <source>All Pages</source>
      <translation>All Pages</translation>
    </message>
    <message>
      <source>Even Pages only</source>
      <translation>Even Pages only</translation>
    </message>
    <message>
      <source>Odd Pages only</source>
      <translation>Odd Pages only</translation>
    </message>
    <message>
      <source>Mirror</source>
      <translation>Mirror</translation>
    </message>
    <message>
      <source>No</source>
      <translation>No</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Yes</translation>
    </message>
    <message>
      <source>Orientation</source>
      <translation>Orientation</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
    <message>
      <source>N-Up Printing</source>
      <translation>N-Up Printing</translation>
    </message>
    <message>
      <source>Page per Sheet</source>
      <translation>Page per Sheet</translation>
    </message>
    <message>
      <source>Pages per Sheet</source>
      <translation>Pages per Sheet</translation>
    </message>
    <message>
      <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help > About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
      <translation>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help > About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>CustomFDialog</name>
    <message>
      <source>Encoding:</source>
      <translation>Encoding:</translation>
    </message>
    <message>
      <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
      <translation>Moves to your Document Directory.
This can be set in the Preferences.</translation>
    </message>
    <message>
      <source>&amp;Compress File</source>
      <translation>&amp;Compress File</translation>
    </message>
    <message>
      <source>&amp;Include Fonts</source>
      <translation>&amp;Include Fonts</translation>
    </message>
  </context>
  <context>
    <name>DeferredTask</name>
    <message>
      <source>Cancelled by user</source>
      <translation>Cancelled by user</translation>
    </message>
  </context>
  <context>
    <name>DelColor</name>
    <message>
      <source>Delete Color</source>
      <translation>Delete Colour</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Delete Color:</source>
      <translation>Delete Colour:</translation>
    </message>
    <message>
      <source>Replace With:</source>
      <translation>Replace With:</translation>
    </message>
  </context>
  <context>
    <name>DelPages</name>
    <message>
      <source>Delete Pages</source>
      <translation>Delete Pages</translation>
    </message>
    <message>
      <source>to:</source>
      <translation>to:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Delete From:</source>
      <translation>Delete From:</translation>
    </message>
  </context>
  <context>
    <name>DocInfos</name>
    <message>
      <source>Document Information</source>
      <translation>Document Information</translation>
    </message>
    <message>
      <source>&amp;Title:</source>
      <translation>&amp;Title:</translation>
    </message>
    <message>
      <source>&amp;Author:</source>
      <translation>&amp;Author:</translation>
    </message>
    <message>
      <source>&amp;Keywords:</source>
      <translation>&amp;Keywords:</translation>
    </message>
    <message>
      <source>Descri&amp;ption:</source>
      <translation>Descri&amp;ption:</translation>
    </message>
    <message>
      <source>P&amp;ublisher:</source>
      <translation>P&amp;ublisher:</translation>
    </message>
    <message>
      <source>&amp;Contributors:</source>
      <translation>&amp;Contributors:</translation>
    </message>
    <message>
      <source>Dat&amp;e:</source>
      <translation>Dat&amp;e:</translation>
    </message>
    <message>
      <source>T&amp;ype:</source>
      <translation>T&amp;ype:</translation>
    </message>
    <message>
      <source>F&amp;ormat:</source>
      <translation>F&amp;ormat:</translation>
    </message>
    <message>
      <source>Identi&amp;fier:</source>
      <translation>Identi&amp;fier:</translation>
    </message>
    <message>
      <source>&amp;Source:</source>
      <translation>&amp;Source:</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Language:</translation>
    </message>
    <message>
      <source>&amp;Relation:</source>
      <translation>&amp;Relation:</translation>
    </message>
    <message>
      <source>Co&amp;verage:</source>
      <translation>Co&amp;verage:</translation>
    </message>
    <message>
      <source>Ri&amp;ghts:</source>
      <translation>Ri&amp;ghts:</translation>
    </message>
    <message>
      <source>Further &amp;Information</source>
      <translation>Further &amp;Information</translation>
    </message>
    <message>
      <source>A person or organisation responsible for making the document available</source>
      <translation>A person or organisation responsible for making the document available</translation>
    </message>
    <message>
      <source>A person or organisation responsible for making contributions to the content of the document</source>
      <translation>A person or organisation responsible for making contributions to the content of the document</translation>
    </message>
    <message>
      <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
      <translation>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</translation>
    </message>
    <message>
      <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
      <translation>The nature or genre of the content of the document, eg. categories, functions, genres, etc</translation>
    </message>
    <message>
      <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
      <translation>An unambiguous reference to the document within a given context such as ISBN or URI</translation>
    </message>
    <message>
      <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
      <translation>A reference to a document from which the present document is derived, eg. ISBN or URI</translation>
    </message>
    <message>
      <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
      <translation>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</translation>
    </message>
    <message>
      <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
      <translation>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</translation>
    </message>
    <message>
      <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
      <translation>Information about rights held in and over the document, eg. copyright, patent or trademark</translation>
    </message>
    <message>
      <source>Documen&amp;t</source>
      <translation>Documen&amp;t</translation>
    </message>
    <message>
      <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
      <translation>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</translation>
    </message>
    <message>
      <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
      <translation>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</translation>
    </message>
    <message>
      <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
      <translation>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</translation>
    </message>
    <message>
      <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
      <translation>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</translation>
    </message>
    <message>
      <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
      <translation>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</translation>
    </message>
    <message>
      <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
      <translation>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</translation>
    </message>
  </context>
  <context>
    <name>DocumentItemAttributes</name>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Relates To</source>
      <translation>Relates To</translation>
    </message>
    <message>
      <source>Is Parent Of</source>
      <translation>Is Parent Of</translation>
    </message>
    <message>
      <source>Is Child Of</source>
      <translation>Is Child Of</translation>
    </message>
    <message>
      <source>Text Frames</source>
      <translation>Text Frames</translation>
    </message>
    <message>
      <source>Image Frames</source>
      <translation>Image Frames</translation>
    </message>
    <message>
      <source>Boolean</source>
      <translation>Boolean</translation>
    </message>
    <message>
      <source>Integer</source>
      <translation>Integer</translation>
    </message>
    <message>
      <source>String</source>
      <translation>String</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>Document Item Attributes</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Type</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>Value</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation>Parameter</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation>Relationship</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation>Relationship To</translation>
    </message>
    <message>
      <source>Auto Add To</source>
      <translation>Auto Add To</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Add</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
  </context>
  <context>
    <name>Druck</name>
    <message>
      <source>Setup Printer</source>
      <translation>Setup Printer</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
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
      <translation>Yellow</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>Black</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation>Print Destination</translation>
    </message>
    <message>
      <source>&amp;Options...</source>
      <translation>&amp;Options...</translation>
    </message>
    <message>
      <source>&amp;File:</source>
      <translation>&amp;File:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>C&amp;hange...</translation>
    </message>
    <message>
      <source>A&amp;lternative Printer Command</source>
      <translation>A&amp;lternative Printer Command</translation>
    </message>
    <message>
      <source>Co&amp;mmand:</source>
      <translation>Co&amp;mmand:</translation>
    </message>
    <message>
      <source>Range</source>
      <translation>Range</translation>
    </message>
    <message>
      <source>Print &amp;All</source>
      <translation>Print &amp;All</translation>
    </message>
    <message>
      <source>Print Current Pa&amp;ge</source>
      <translation>Print Current Pa&amp;ge</translation>
    </message>
    <message>
      <source>Print &amp;Range</source>
      <translation>Print &amp;Range</translation>
    </message>
    <message>
      <source>N&amp;umber of Copies:</source>
      <translation>N&amp;umber of Copies:</translation>
    </message>
    <message>
      <source>Print &amp;Normal</source>
      <translation>Print &amp;Normal</translation>
    </message>
    <message>
      <source>Print &amp;Separations</source>
      <translation>Print &amp;Separations</translation>
    </message>
    <message>
      <source>Pr&amp;int In Color If Available</source>
      <translation>Pr&amp;int In Colour If Available</translation>
    </message>
    <message>
      <source>Print In Gra&amp;yscale</source>
      <translation>Print In Gre&amp;yscale</translation>
    </message>
    <message>
      <source>Ad&amp;vanced Options...</source>
      <translation>Ad&amp;vanced Options...</translation>
    </message>
    <message>
      <source>&amp;Print</source>
      <translation>&amp;Print</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
      <translation>Use an alternative print manager, such as kprinter or gtklp,
to utilise additional printing options</translation>
    </message>
    <message>
      <source>Postscript Files (*.ps);;All Files (*)</source>
      <translation>Postscript Files (*.ps);;All Files (*)</translation>
    </message>
  </context>
  <context>
    <name>EPSPlug</name>
    <message>
      <source>Importing File:
%1
failed!</source>
      <translation>Importing File:
%1
failed!</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>Fatal Error</translation>
    </message>
  </context>
  <context>
    <name>EditStyle</name>
    <message>
      <source>Edit Style</source>
      <translation>Edit Style</translation>
    </message>
    <message>
      <source>Character</source>
      <translation>Character</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Line Spacing</source>
      <translation>Line Spacing</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Name of the Style is not unique</source>
      <translation>Name of the Style is not unique</translation>
    </message>
    <message>
      <source>Name of your paragraph style</source>
      <translation>Name of your paragraph style</translation>
    </message>
    <message>
      <source>Font of selected text or object</source>
      <translation>Font of selected text or object</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Font Size</translation>
    </message>
    <message>
      <source>Color of text fill</source>
      <translation>Colour of text fill</translation>
    </message>
    <message>
      <source>Color of text stroke</source>
      <translation>Colour of text stroke</translation>
    </message>
    <message>
      <source>Determines the overall height, in line numbers, of the Drop Caps</source>
      <translation>Determines the overall height, in line numbers, of the Drop Caps</translation>
    </message>
    <message>
      <source>Spacing above the paragraph</source>
      <translation>Spacing above the paragraph</translation>
    </message>
    <message>
      <source>Spacing below the paragraph</source>
      <translation>Spacing below the paragraph</translation>
    </message>
    <message>
      <source>Tabulators and Indentation</source>
      <translation>Tabulators and Indentation</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>&amp;Lines:</source>
      <translation>&amp;Lines:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Distances</source>
      <translation>Distances</translation>
    </message>
    <message>
      <source>Fixed Linespacing</source>
      <translation>Fixed Linespacing</translation>
    </message>
    <message>
      <source>Automatic Linespacing</source>
      <translation>Automatic Linespacing</translation>
    </message>
    <message>
      <source>Align to Baseline Grid</source>
      <translation>Align to Baseline Grid</translation>
    </message>
    <message>
      <source>Drop Caps</source>
      <translation>Drop Caps</translation>
    </message>
    <message>
      <source>Distance from Text:</source>
      <translation>Distance from Text:</translation>
    </message>
    <message>
      <source>Preview of the Paragraph Style</source>
      <translation>Preview of the Paragraph Style</translation>
    </message>
    <message>
      <source>Determines the gap between the DropCaps and the Text</source>
      <translation>Determines the gap between the DropCaps and the Text</translation>
    </message>
    <message>
      <source>Toggles sample text of this paragraph style</source>
      <translation>Toggles sample text of this paragraph style</translation>
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
      <translation>Javascripts (*.js);;All Files (*)</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Open...</source>
      <translation>&amp;Open...</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Save &amp;As...</translation>
    </message>
    <message>
      <source>&amp;Save and Exit</source>
      <translation>&amp;Save and Exit</translation>
    </message>
    <message>
      <source>&amp;Exit without Saving</source>
      <translation>&amp;Exit without Saving</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Undo</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Redo</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>Cu&amp;t</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>&amp;Paste</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>&amp;Get Field Names</source>
      <translation>&amp;Get Field Names</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
  </context>
  <context>
    <name>EffectsDialog</name>
    <message>
      <source>Image Effects</source>
      <translation>Image Effects</translation>
    </message>
    <message>
      <source>Options:</source>
      <translation>Options:</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Colour:</translation>
    </message>
    <message>
      <source>Shade:</source>
      <translation>Shade:</translation>
    </message>
    <message>
      <source>Brightness:</source>
      <translation>Brightness:</translation>
    </message>
    <message>
      <source>Contrast:</source>
      <translation>Contrast:</translation>
    </message>
    <message>
      <source>Radius:</source>
      <translation>Radius:</translation>
    </message>
    <message>
      <source>Value:</source>
      <translation>Value:</translation>
    </message>
    <message>
      <source>Posterize:</source>
      <translation>Posterise:</translation>
    </message>
    <message>
      <source>Available Effects</source>
      <translation>Available Effects</translation>
    </message>
    <message>
      <source>Blur</source>
      <translation>Blur</translation>
    </message>
    <message>
      <source>Brightness</source>
      <translation>Brightness</translation>
    </message>
    <message>
      <source>Colorize</source>
      <translation>Colourise</translation>
    </message>
    <message>
      <source>Contrast</source>
      <translation>Contrast</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>Greyscale</translation>
    </message>
    <message>
      <source>Invert</source>
      <translation>Invert</translation>
    </message>
    <message>
      <source>Posterize</source>
      <translation>Posterise</translation>
    </message>
    <message>
      <source>Sharpen</source>
      <translation>Sharpen</translation>
    </message>
    <message>
      <source>>></source>
      <translation>>></translation>
    </message>
    <message>
      <source>&lt;&lt;</source>
      <translation>&lt;&lt;</translation>
    </message>
    <message>
      <source>Effects in use</source>
      <translation>Effects in use</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
  </context>
  <context>
    <name>ExportForm</name>
    <message>
      <source>&amp;All pages</source>
      <translation>&amp;All pages</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Change the output directory</source>
      <translation>Change the output directory</translation>
    </message>
    <message>
      <source>The output directory - the place to store your images.
Name of the export file will be 'documentname-pagenumber.filetype'</source>
      <translation>The output directory - the place to store your images.
Name of the export file will be 'documentname-pagenumber.filetype'</translation>
    </message>
    <message>
      <source>Export only the current page</source>
      <translation>Export only the current page</translation>
    </message>
    <message>
      <source>Available export formats</source>
      <translation>Available export formats</translation>
    </message>
    <message>
      <source>Choose a Export Directory</source>
      <translation>Choose a Export Directory</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>C&amp;hange...</translation>
    </message>
    <message>
      <source>&amp;Export to Directory:</source>
      <translation>&amp;Export to Directory:</translation>
    </message>
    <message>
      <source>Image &amp;Type:</source>
      <translation>Image &amp;Type:</translation>
    </message>
    <message>
      <source>&amp;Quality:</source>
      <translation>&amp;Quality:</translation>
    </message>
    <message>
      <source>Export as Image(s)</source>
      <translation>Export as Image(s)</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>&amp;Resolution:</source>
      <translation>&amp;Resolution:</translation>
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
      <translation>Range</translation>
    </message>
    <message>
      <source>&amp;Current page</source>
      <translation>&amp;Current page</translation>
    </message>
    <message>
      <source>&amp;Range</source>
      <translation>&amp;Range</translation>
    </message>
    <message>
      <source>C</source>
      <translation>C</translation>
    </message>
    <message>
      <source>Export a range of pages</source>
      <translation>Export a range of pages</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</translation>
    </message>
    <message>
      <source>Export all pages</source>
      <translation>Export all pages</translation>
    </message>
    <message>
      <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
      <translation>Resolution of the Images
Use 72 dpi for Images intended for the Screen</translation>
    </message>
    <message>
      <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
      <translation>The quality of your images - 100% is the best, 1% the lowest quality</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
      <translation>Size of the images. 100% for no changes, 200% for two times larger etc.</translation>
    </message>
  </context>
  <context>
    <name>ExtImageProps</name>
    <message>
      <source>Extended Image Properties</source>
      <translation>Extended Image Properties</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Darken</source>
      <translation>Darken</translation>
    </message>
    <message>
      <source>Lighten</source>
      <translation>Lighten</translation>
    </message>
    <message>
      <source>Hue</source>
      <translation>Hue</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturation</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Colour</translation>
    </message>
    <message>
      <source>Luminosity</source>
      <translation>Luminosity</translation>
    </message>
    <message>
      <source>Multiply</source>
      <translation>Multiply</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Screen</translation>
    </message>
    <message>
      <source>Dissolve</source>
      <translation>Dissolve</translation>
    </message>
    <message>
      <source>Overlay</source>
      <translation>Overlay</translation>
    </message>
    <message>
      <source>Hard Light</source>
      <translation>Hard Light</translation>
    </message>
    <message>
      <source>Soft Light</source>
      <translation>Soft Light</translation>
    </message>
    <message>
      <source>Difference</source>
      <translation>Difference</translation>
    </message>
    <message>
      <source>Exlusion</source>
      <translation>Exclusion</translation>
    </message>
    <message>
      <source>Color Dodge</source>
      <translation>Colour Dodge</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>Colour Burn</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Blend Mode:</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>Opacity:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Background</translation>
    </message>
    <message>
      <source>Layers</source>
      <translation>Layers</translation>
    </message>
    <message>
      <source>Don't use any Path</source>
      <translation>Don't use any Path</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>Paths</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>Exclusion</translation>
    </message>
  </context>
  <context>
    <name>FDialogPreview</name>
    <message>
      <source>Size:</source>
      <translation>Size:</translation>
    </message>
    <message>
      <source>Title:</source>
      <translation>Title:</translation>
    </message>
    <message>
      <source>No Title</source>
      <translation>No Title</translation>
    </message>
    <message>
      <source>Author:</source>
      <translation>Author:</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>Scribus Document</source>
      <translation>Scribus Document</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>Resolution:</translation>
    </message>
    <message>
      <source>DPI</source>
      <translation>DPI</translation>
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
      <source>Grayscale</source>
      <translation>Greyscale</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation>Colourspace:</translation>
    </message>
  </context>
  <context>
    <name>Farbmanager</name>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
    <message>
      <source>Color Sets</source>
      <translation>Colour Sets</translation>
    </message>
    <message>
      <source>Current Color Set:</source>
      <translation>Current Colour Set:</translation>
    </message>
    <message>
      <source>Choose a Name</source>
      <translation>Choose a Name</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Documents (*.sla *.scd);;All Files (*)</translation>
    </message>
    <message>
      <source>New Color</source>
      <translation>New Colour</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Copy of %1</translation>
    </message>
    <message>
      <source>Choose a color set to load</source>
      <translation>Choose a colour set to load</translation>
    </message>
    <message>
      <source>Save the current color set</source>
      <translation>Save the current colour set</translation>
    </message>
    <message>
      <source>Remove unused colors from current document's color set</source>
      <translation>Remove unused colours from current document's colour set</translation>
    </message>
    <message>
      <source>Append colors to the current set from an existing document</source>
      <translation>Append colours to the current set from an existing document</translation>
    </message>
    <message>
      <source>Create a new color within the current set</source>
      <translation>Create a new colour within the current set</translation>
    </message>
    <message>
      <source>Edit the currently selected color</source>
      <translation>Edit the currently selected colour</translation>
    </message>
    <message>
      <source>Make a copy of the currently selected color</source>
      <translation>Make a copy of the currently selected colour</translation>
    </message>
    <message>
      <source>Delete the currently selected color</source>
      <translation>Delete the currently selected colour</translation>
    </message>
    <message>
      <source>Make the current colorset the default color set</source>
      <translation>Make the current colourset the default colour set</translation>
    </message>
    <message>
      <source>&amp;Append</source>
      <translation>&amp;Append</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>D&amp;uplicate</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>&amp;Remove Unused</source>
      <translation>&amp;Remove Unused</translation>
    </message>
    <message>
      <source>&amp;Save Color Set</source>
      <translation>&amp;Save Colour Set</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
  </context>
  <context>
    <name>FontPrefs</name>
    <message>
      <source>Available Fonts</source>
      <translation>Available Fonts</translation>
    </message>
    <message>
      <source>Font Substitutions</source>
      <translation>Font Substitutions</translation>
    </message>
    <message>
      <source>Additional Paths</source>
      <translation>Additional Paths</translation>
    </message>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
    </message>
    <message>
      <source>Font Name</source>
      <translation>Font Name</translation>
    </message>
    <message>
      <source>Replacement</source>
      <translation>Replacement</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>&amp;Available Fonts</source>
      <translation>&amp;Available Fonts</translation>
    </message>
    <message>
      <source>Font &amp;Substitutions</source>
      <translation>Font &amp;Substitutions</translation>
    </message>
    <message>
      <source>Additional &amp;Paths</source>
      <translation>Additional &amp;Paths</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>C&amp;hange...</translation>
    </message>
    <message>
      <source>A&amp;dd...</source>
      <translation>A&amp;dd...</translation>
    </message>
    <message>
      <source>&amp;Remove</source>
      <translation>&amp;Remove</translation>
    </message>
    <message>
      <source>Font Name</source>
      <comment>font preview</comment>
      <translation>Font Name</translation>
    </message>
    <message>
      <source>Use Font</source>
      <comment>font preview</comment>
      <translation>Use Font</translation>
    </message>
    <message>
      <source>Embed in:</source>
      <comment>font preview</comment>
      <translation>Embed in:</translation>
    </message>
    <message>
      <source>Subset</source>
      <comment>font preview</comment>
      <translation>Subset</translation>
    </message>
    <message>
      <source>Path to Font File</source>
      <comment>font preview</comment>
      <translation>Path to Font File</translation>
    </message>
    <message>
      <source>&lt;qt>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit->Settings to change the font search path.&lt;/qt></source>
      <translation>&lt;qt>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit->Settings to change the font search path.&lt;/qt></translation>
    </message>
  </context>
  <context>
    <name>FontPreview</name>
    <message>
      <source>Font Name</source>
      <comment>font preview</comment>
      <translation>Font Name</translation>
    </message>
    <message>
      <source>Doc</source>
      <comment>font preview</comment>
      <translation>Doc</translation>
    </message>
    <message>
      <source>Type</source>
      <comment>font preview</comment>
      <translation>Type</translation>
    </message>
    <message>
      <source>Subset</source>
      <comment>font preview</comment>
      <translation>Subset</translation>
    </message>
    <message>
      <source>Access</source>
      <comment>font preview</comment>
      <translation>Access</translation>
    </message>
    <message>
      <source>Font Size:</source>
      <translation>Font Size:</translation>
    </message>
    <message>
      <source>Fonts Preview</source>
      <comment>font preview</comment>
      <translation>Fonts Preview</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <comment>font preview</comment>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <comment>font preview</comment>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>Quick Search: </source>
      <translation>Quick Search: </translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <comment>font preview</comment>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <comment>font preview</comment>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Append selected font into Style, Font menu</source>
      <comment>font preview</comment>
      <translation>Append selected font into Style, Font menu</translation>
    </message>
    <message>
      <source>Leave preview</source>
      <comment>font preview</comment>
      <translation>Leave preview</translation>
    </message>
    <message>
      <source>Typing the text here provides quick searching in the font names. E.g. 'bold' shows all fonts with Bold in name. Searching is case insensitive.</source>
      <translation>Typing the text here provides quick searching in the font names. E.g. 'bold' shows all fonts with Bold in name. Searching is case insensitive.</translation>
    </message>
    <message>
      <source>Start searching</source>
      <translation>Start searching</translation>
    </message>
    <message>
      <source>Size of the selected font</source>
      <translation>Size of the selected font</translation>
    </message>
    <message>
      <source>Woven silk pyjamas exchanged for blue quartz</source>
      <comment>font preview</comment>
      <translation>Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
      <source>User</source>
      <comment>font preview</comment>
      <translation>User</translation>
    </message>
    <message>
      <source>System</source>
      <comment>font preview</comment>
      <translation>System</translation>
    </message>
  </context>
  <context>
    <name>FontReplaceDialog</name>
    <message>
      <source>Font Substitution</source>
      <translation>Font Substitution</translation>
    </message>
    <message>
      <source>Original Font</source>
      <translation>Original Font</translation>
    </message>
    <message>
      <source>Substitution Font</source>
      <translation>Substitution Font</translation>
    </message>
    <message>
      <source>Make these substitutions permanent</source>
      <translation>Make these substitutions permanent</translation>
    </message>
    <message>
      <source>This document contains some fonts that are not installed on your system,
please choose a suitable replacement for them.</source>
      <translation>This document contains some fonts that are not installed on your system,
please choose a suitable replacement for them.</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
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
      <translation>Here you can add, change or remove Colour-Stops.</translation>
    </message>
  </context>
  <context>
    <name>GuideManager</name>
    <message>
      <source>Manage Guides</source>
      <translation>Manage Guides</translation>
    </message>
    <message>
      <source>Horizontal Guides</source>
      <translation>Horizontal Guides</translation>
    </message>
    <message>
      <source>Vertical Guides</source>
      <translation>Vertical Guides</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Add</translation>
    </message>
    <message>
      <source>D&amp;elete</source>
      <translation>D&amp;elete</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>&amp;X-Pos:</translation>
    </message>
    <message>
      <source>A&amp;dd</source>
      <translation>A&amp;dd</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation>De&amp;lete</translation>
    </message>
    <message>
      <source>&amp;Lock Guides</source>
      <translation>&amp;Lock Guides</translation>
    </message>
    <message>
      <source>&amp;Rows:</source>
      <translation>&amp;Rows:</translation>
    </message>
    <message>
      <source>C&amp;olumns:</source>
      <translation>C&amp;olumns:</translation>
    </message>
    <message>
      <source>Refer to:</source>
      <translation>Refer to:</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>&amp;Page</translation>
    </message>
    <message>
      <source>&amp;Margins</source>
      <translation>&amp;Margins</translation>
    </message>
    <message>
      <source>&amp;Selection</source>
      <translation>&amp;Selection</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>Rows and Columns - Automatic Guides</source>
      <translation>Rows and Columns - Automatic Guides</translation>
    </message>
    <message>
      <source>Row &amp;Gap</source>
      <translation>Row &amp;Gap</translation>
    </message>
    <message>
      <source>Colum&amp;n Gap</source>
      <translation>Colum&amp;n Gap</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Update</source>
      <translation>&amp;Update</translation>
    </message>
    <message>
      <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
      <comment>guide manager</comment>
      <translation>Set the guides in document. Guide manager is still opened but the changes are persistant</translation>
    </message>
  </context>
  <context>
    <name>HelpBrowser</name>
    <message>
      <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
      <translation>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>Contents</translation>
    </message>
    <message>
      <source>Link</source>
      <translation>Link</translation>
    </message>
    <message>
      <source>Scribus Online Help</source>
      <translation>Scribus Online Help</translation>
    </message>
    <message>
      <source>&amp;Contents</source>
      <translation>&amp;Contents</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
    <message>
      <source>unknown</source>
      <translation>unknown</translation>
    </message>
    <message>
      <source>Find</source>
      <translation>Find</translation>
    </message>
    <message>
      <source>Search Term:</source>
      <translation>Search Term:</translation>
    </message>
    <message>
      <source>Se&amp;arch</source>
      <translation>Se&amp;arch</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>De&amp;lete All</source>
      <translation>De&amp;lete All</translation>
    </message>
    <message>
      <source>Book&amp;marks</source>
      <translation>Book&amp;marks</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>&amp;Print...</translation>
    </message>
    <message>
      <source>E&amp;xit</source>
      <translation>E&amp;xit</translation>
    </message>
    <message>
      <source>Searching is case unsensitive</source>
      <translation>Searching is case unsensitive</translation>
    </message>
    <message>
      <source>New Bookmark</source>
      <translation>New Bookmark</translation>
    </message>
    <message>
      <source>New Bookmark's Title:</source>
      <translation>New Bookmark's Title:</translation>
    </message>
  </context>
  <context>
    <name>HyAsk</name>
    <message>
      <source>Possible Hyphenation</source>
      <translation>Possible Hyphenation</translation>
    </message>
    <message>
      <source>Accept</source>
      <translation>Accept</translation>
    </message>
    <message>
      <source>Skip</source>
      <translation>Skip</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
  </context>
  <context>
    <name>HySettings</name>
    <message>
      <source>Length of the smallest word to be hyphenated.</source>
      <translation>Length of the smallest word to be hyphenated.</translation>
    </message>
    <message>
      <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
      <translation>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Language:</translation>
    </message>
    <message>
      <source>&amp;Smallest Word:</source>
      <translation>&amp;Smallest Word:</translation>
    </message>
    <message>
      <source>&amp;Hyphenation Suggestions</source>
      <translation>&amp;Hyphenation Suggestions</translation>
    </message>
    <message>
      <source>Hyphenate Text Automatically &amp;During Typing</source>
      <translation>Hyphenate Text Automatically &amp;During Typing</translation>
    </message>
    <message>
      <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
      <translation>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</translation>
    </message>
    <message>
      <source>Enables automatic hyphenation of your text while typing.</source>
      <translation>Enables automatic hyphenation of your text while typing.</translation>
    </message>
    <message>
      <source>Consecutive Hyphenations &amp;Allowed:</source>
      <translation>Consecutive Hyphenations &amp;Allowed:</translation>
    </message>
  </context>
  <context>
    <name>InsPage</name>
    <message>
      <source>Insert Page</source>
      <translation>Insert Page</translation>
    </message>
    <message>
      <source>before Page</source>
      <translation>before Page</translation>
    </message>
    <message>
      <source>after Page</source>
      <translation>after Page</translation>
    </message>
    <message>
      <source>at End</source>
      <translation>at End</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Page(s)</source>
      <translation>Page(s)</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>&amp;Insert</translation>
    </message>
    <message>
      <source>Master Page (&amp;Left Page):</source>
      <translation>Master Page (&amp;Left Page):</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>&amp;Master Page:</translation>
    </message>
    <message>
      <source>Master Page (&amp;Right Page):</source>
      <translation>Master Page (&amp;Right Page):</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>Orie&amp;ntation:</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Width:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Height:</translation>
    </message>
    <message>
      <source>Move Objects with their Page</source>
      <translation>Move Objects with their Page</translation>
    </message>
  </context>
  <context>
    <name>InsertTable</name>
    <message>
      <source>Insert Table</source>
      <translation>Insert Table</translation>
    </message>
    <message>
      <source>Number of rows:</source>
      <translation>Number of rows:</translation>
    </message>
    <message>
      <source>Number of columns:</source>
      <translation>Number of columns:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>JavaDocs</name>
    <message>
      <source>New Script</source>
      <translation>New Script</translation>
    </message>
    <message>
      <source>Edit JavaScripts</source>
      <translation>Edit JavaScripts</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>&amp;Edit...</source>
      <translation>&amp;Edit...</translation>
    </message>
    <message>
      <source>&amp;Add...</source>
      <translation>&amp;Add...</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>&amp;New Script:</source>
      <translation>&amp;New Script:</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;No</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>&amp;Yes</translation>
    </message>
    <message>
      <source>Do you really want to delete this Script?</source>
      <translation>Do you really want to delete this Script?</translation>
    </message>
  </context>
  <context>
    <name>KeyManager</name>
    <message>
      <source>Action</source>
      <translation>Action</translation>
    </message>
    <message>
      <source>Current Key</source>
      <translation>Current Key</translation>
    </message>
    <message>
      <source>Select a Key for this Action</source>
      <translation>Select a Key for this Action</translation>
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
      <translation>Warning</translation>
    </message>
    <message>
      <source>&amp;No Key</source>
      <translation>&amp;No Key</translation>
    </message>
    <message>
      <source>&amp;User Defined Key</source>
      <translation>&amp;User Defined Key</translation>
    </message>
    <message>
      <source>Set &amp;Key</source>
      <translation>Set &amp;Key</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>This Key Sequence is already in use</source>
      <translation>This Key Sequence is already in use</translation>
    </message>
    <message>
      <source>Loadable Shortcut Sets</source>
      <translation>Loadable Shortcut Sets</translation>
    </message>
    <message>
      <source>&amp;Load</source>
      <translation>&amp;Load</translation>
    </message>
    <message>
      <source>&amp;Import...</source>
      <translation>&amp;Import...</translation>
    </message>
    <message>
      <source>&amp;Export...</source>
      <translation>&amp;Export...</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Reset</translation>
    </message>
    <message>
      <source>Keyboard shortcut sets available to load</source>
      <translation>Keyboard shortcut sets available to load</translation>
    </message>
    <message>
      <source>Load the selected shortcut set</source>
      <translation>Load the selected shortcut set</translation>
    </message>
    <message>
      <source>Import a shortcut set into the current configuration</source>
      <translation>Import a shortcut set into the current configuration</translation>
    </message>
    <message>
      <source>Export the current shortcuts into an importable file</source>
      <translation>Export the current shortcuts into an importable file</translation>
    </message>
    <message>
      <source>Reload the default Scribus shortcuts</source>
      <translation>Reload the default Scribus shortcuts</translation>
    </message>
    <message>
      <source>Key Set XML Files (*.ksxml)</source>
      <translation>Key Set XML Files (*.ksxml)</translation>
    </message>
  </context>
  <context>
    <name>LayerPalette</name>
    <message>
      <source>Layers</source>
      <translation>Layers</translation>
    </message>
    <message>
      <source>Delete Layer</source>
      <translation>Delete Layer</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Do you want to delete all objects on this layer too?</source>
      <translation>Do you want to delete all objects on this layer too?</translation>
    </message>
    <message>
      <source>Add a new layer</source>
      <translation>Add a new layer</translation>
    </message>
    <message>
      <source>Delete layer</source>
      <translation>Delete layer</translation>
    </message>
    <message>
      <source>Raise layer</source>
      <translation>Raise layer</translation>
    </message>
    <message>
      <source>Lower layer</source>
      <translation>Lower layer</translation>
    </message>
  </context>
  <context>
    <name>LineFormate</name>
    <message>
      <source>Edit Line Styles</source>
      <translation>Edit Line Styles</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Copy of %1</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>New Style</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Documents (*.sla *.scd);;All Files (*)</translation>
    </message>
    <message>
      <source>&amp;Append</source>
      <translation>&amp;Append</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>D&amp;uplicate</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;No</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>&amp;Yes</translation>
    </message>
    <message>
      <source>Do you really want to delete this Style?</source>
      <translation>Do you really want to delete this Style?</translation>
    </message>
  </context>
  <context>
    <name>LoremManager</name>
    <message>
      <source>Select Lorem Ipsum</source>
      <translation>Select Lorem Ipsum</translation>
    </message>
    <message>
      <source>Author:</source>
      <translation>Author:</translation>
    </message>
    <message>
      <source>Get More:</source>
      <translation>Get More:</translation>
    </message>
    <message>
      <source>XML File:</source>
      <translation>XML File:</translation>
    </message>
    <message>
      <source>Lorem Ipsum</source>
      <translation>Lorem Ipsum</translation>
    </message>
    <message>
      <source>Paragraphs:</source>
      <translation>Paragraphs:</translation>
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
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
  </context>
  <context>
    <name>MarginDialog</name>
    <message>
      <source>Margin Guides</source>
      <translation>Margin Guides</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Manage Page Properties</source>
      <translation>Manage Page Properties</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>Orie&amp;ntation:</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Width:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Height:</translation>
    </message>
    <message>
      <source>Move Objects with their Page</source>
      <translation>Move Objects with their Page</translation>
    </message>
  </context>
  <context>
    <name>MarginWidget</name>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Bottom:</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Top:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Right:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Left:</translation>
    </message>
    <message>
      <source>Distance between the top margin guide and the edge of the page</source>
      <translation>Distance between the top margin guide and the edge of the page</translation>
    </message>
    <message>
      <source>Distance between the bottom margin guide and the edge of the page</source>
      <translation>Distance between the bottom margin guide and the edge of the page</translation>
    </message>
    <message>
      <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</translation>
    </message>
    <message>
      <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</translation>
    </message>
    <message>
      <source>&amp;Inside:</source>
      <translation>&amp;Inside:</translation>
    </message>
    <message>
      <source>O&amp;utside:</source>
      <translation>O&amp;utside:</translation>
    </message>
  </context>
  <context>
    <name>MasterPagesPalette</name>
    <message>
      <source>Edit Master Pages</source>
      <translation>Edit Master Pages</translation>
    </message>
    <message>
      <source>Duplicates the selected master page</source>
      <translation>Duplicates the selected master page</translation>
    </message>
    <message>
      <source>Deletes the selected master page</source>
      <translation>Deletes the selected master page</translation>
    </message>
    <message>
      <source>Adds a new master page</source>
      <translation>Adds a new master page</translation>
    </message>
    <message>
      <source>Imports master pages from another document</source>
      <translation>Imports master pages from another document</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Do you really want to delete this master page?</source>
      <translation>Do you really want to delete this master page?</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;No</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>&amp;Yes</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>New Master Page</source>
      <translation>New Master Page</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Copy of %1</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Name:</translation>
    </message>
    <message>
      <source>New MasterPage</source>
      <translation>New MasterPage</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Copy #%1 of </translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
  </context>
  <context>
    <name>Mdup</name>
    <message>
      <source>Multiple Duplicate</source>
      <translation>Multiple Duplicate</translation>
    </message>
    <message>
      <source>&amp;Number of Copies:</source>
      <translation>&amp;Number of Copies:</translation>
    </message>
    <message>
      <source>&amp;Horizontal Shift:</source>
      <translation>&amp;Horizontal Shift:</translation>
    </message>
    <message>
      <source>&amp;Vertical Shift:</source>
      <translation>&amp;Vertical Shift:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>Measurements</name>
    <message>
      <source>Distances</source>
      <translation>Distances</translation>
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
      <translation>Angle:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation>Length:</translation>
    </message>
    <message>
      <source>pt</source>
      <translation>pt</translation>
    </message>
  </context>
  <context>
    <name>MergeDoc</name>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Documents (*.sla *.scd);;All Files (*)</translation>
    </message>
    <message>
      <source>Import Page(s)</source>
      <translation>Import Page(s)</translation>
    </message>
    <message>
      <source> from 0</source>
      <translation> from 0</translation>
    </message>
    <message>
      <source>Create Page(s)</source>
      <translation>Create Page(s)</translation>
    </message>
    <message>
      <source> from %1</source>
      <translation> from %1</translation>
    </message>
    <message>
      <source>Import Master Page</source>
      <translation>Import Master Page</translation>
    </message>
    <message>
      <source>&amp;From Document:</source>
      <translation>&amp;From Document:</translation>
    </message>
    <message>
      <source>Chan&amp;ge...</source>
      <translation>Chan&amp;ge...</translation>
    </message>
    <message>
      <source>&amp;Import Page(s):</source>
      <translation>&amp;Import Page(s):</translation>
    </message>
    <message>
      <source>&amp;Import Master Page</source>
      <translation>&amp;Import Master Page</translation>
    </message>
    <message>
      <source>Before Page</source>
      <translation>Before Page</translation>
    </message>
    <message>
      <source>After Page</source>
      <translation>After Page</translation>
    </message>
    <message>
      <source>At End</source>
      <translation>At End</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>&amp;Import</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
      <translation>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</translation>
    </message>
  </context>
  <context>
    <name>MissingFont</name>
    <message>
      <source>Missing Font</source>
      <translation>Missing Font</translation>
    </message>
    <message>
      <source>The Font %1 is not installed.</source>
      <translation>The Font %1 is not installed.</translation>
    </message>
    <message>
      <source>Use</source>
      <translation>Use</translation>
    </message>
    <message>
      <source>instead</source>
      <translation>instead</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
  </context>
  <context>
    <name>MovePages</name>
    <message>
      <source>Move Pages</source>
      <translation>Move Pages</translation>
    </message>
    <message>
      <source>Copy Page</source>
      <translation>Copy Page</translation>
    </message>
    <message>
      <source>Move Page(s):</source>
      <translation>Move Page(s):</translation>
    </message>
    <message>
      <source>to:</source>
      <translation>to:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Move Page(s)</source>
      <translation>Move Page(s)</translation>
    </message>
    <message>
      <source>Before Page</source>
      <translation>Before Page</translation>
    </message>
    <message>
      <source>After Page</source>
      <translation>After Page</translation>
    </message>
    <message>
      <source>At End</source>
      <translation>At End</translation>
    </message>
  </context>
  <context>
    <name>Mpalette</name>
    <message>
      <source>Properties</source>
      <translation>Properties</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Geometry</source>
      <translation>Geometry</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Basepoint:</source>
      <translation>Basepoint:</translation>
    </message>
    <message>
      <source>Level</source>
      <translation>Level</translation>
    </message>
    <message>
      <source>Shape:</source>
      <translation>Shape:</translation>
    </message>
    <message>
      <source>Distance of Text</source>
      <translation>Distance of Text</translation>
    </message>
    <message>
      <source>Show Curve</source>
      <translation>Show Curve</translation>
    </message>
    <message>
      <source>Start Offset:</source>
      <translation>Start Offset:</translation>
    </message>
    <message>
      <source>Distance from Curve:</source>
      <translation>Distance from Curve:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Input Profile:</source>
      <translation>Input Profile:</translation>
    </message>
    <message>
      <source>Rendering Intent:</source>
      <translation>Rendering Intent:</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>Perceptual</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>Relative Colourimetric</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturation</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>Absolute Colourimetric</translation>
    </message>
    <message>
      <source>Left Point</source>
      <translation>Left Point</translation>
    </message>
    <message>
      <source>End Points</source>
      <translation>End Points</translation>
    </message>
    <message>
      <source>Miter Join</source>
      <translation>Mitre Join</translation>
    </message>
    <message>
      <source>Bevel Join</source>
      <translation>Bevel Join</translation>
    </message>
    <message>
      <source>Round Join</source>
      <translation>Round Join</translation>
    </message>
    <message>
      <source>Flat Cap</source>
      <translation>Flat Cap</translation>
    </message>
    <message>
      <source>Square Cap</source>
      <translation>Square Cap</translation>
    </message>
    <message>
      <source>Round Cap</source>
      <translation>Round Cap</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Font Size</translation>
    </message>
    <message>
      <source>Line Spacing</source>
      <translation>Line Spacing</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.
Please choose another.</source>
      <translation>Name &quot;%1&quot; isn't unique.
Please choose another.</translation>
    </message>
    <message>
      <source>Name of selected object</source>
      <translation>Name of selected object</translation>
    </message>
    <message>
      <source>Horizontal position of current basepoint</source>
      <translation>Horizontal position of current basepoint</translation>
    </message>
    <message>
      <source>Vertical position of current basepoint</source>
      <translation>Vertical position of current basepoint</translation>
    </message>
    <message>
      <source>Width</source>
      <translation>Width</translation>
    </message>
    <message>
      <source>Height</source>
      <translation>Height</translation>
    </message>
    <message>
      <source>Rotation of object at current basepoint</source>
      <translation>Rotation of object at current basepoint</translation>
    </message>
    <message>
      <source>Point from which measurements or rotation angles are referenced</source>
      <translation>Point from which measurements or rotation angles are referenced</translation>
    </message>
    <message>
      <source>Select top left for basepoint</source>
      <translation>Select top left for basepoint</translation>
    </message>
    <message>
      <source>Select top right for basepoint</source>
      <translation>Select top right for basepoint</translation>
    </message>
    <message>
      <source>Select bottom left for basepoint</source>
      <translation>Select bottom left for basepoint</translation>
    </message>
    <message>
      <source>Select bottom right for basepoint</source>
      <translation>Select bottom right for basepoint</translation>
    </message>
    <message>
      <source>Select center for basepoint</source>
      <translation>Select centre for basepoint</translation>
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
      <translation>Move one level up</translation>
    </message>
    <message>
      <source>Move one level down</source>
      <translation>Move one level down</translation>
    </message>
    <message>
      <source>Move to front</source>
      <translation>Move to front</translation>
    </message>
    <message>
      <source>Move to back</source>
      <translation>Move to back</translation>
    </message>
    <message>
      <source>Lock or unlock the object</source>
      <translation>Lock or unlock the object</translation>
    </message>
    <message>
      <source>Lock or unlock the size of the object</source>
      <translation>Lock or unlock the size of the object</translation>
    </message>
    <message>
      <source>Enable or disable printing of the object</source>
      <translation>Enable or disable printing of the object</translation>
    </message>
    <message>
      <source>Font of selected text or object</source>
      <translation>Font of selected text or object</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>Scaling width of characters</translation>
    </message>
    <message>
      <source>Color of text stroke</source>
      <translation>Colour of text stroke</translation>
    </message>
    <message>
      <source>Color of text fill</source>
      <translation>Colour of text fill</translation>
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
      <translation>Style of current paragraph</translation>
    </message>
    <message>
      <source>Change settings for left or end points</source>
      <translation>Change settings for left or end points</translation>
    </message>
    <message>
      <source>Pattern of line</source>
      <translation>Pattern of line</translation>
    </message>
    <message>
      <source>Thickness of line</source>
      <translation>Thickness of line</translation>
    </message>
    <message>
      <source>Type of line joins</source>
      <translation>Type of line joins</translation>
    </message>
    <message>
      <source>Type of line end</source>
      <translation>Type of line end</translation>
    </message>
    <message>
      <source>Line style of current object</source>
      <translation>Line style of current object</translation>
    </message>
    <message>
      <source>Choose the shape of frame...</source>
      <translation>Choose the shape of frame...</translation>
    </message>
    <message>
      <source>Edit shape of the frame...</source>
      <translation>Edit shape of the frame...</translation>
    </message>
    <message>
      <source>Set radius of corner rounding</source>
      <translation>Set radius of corner rounding</translation>
    </message>
    <message>
      <source>Number of columns in text frame</source>
      <translation>Number of columns in text frame</translation>
    </message>
    <message>
      <source>Distance between columns</source>
      <translation>Distance between columns</translation>
    </message>
    <message>
      <source>Distance of text from top of frame</source>
      <translation>Distance of text from top of frame</translation>
    </message>
    <message>
      <source>Distance of text from bottom of frame</source>
      <translation>Distance of text from bottom of frame</translation>
    </message>
    <message>
      <source>Distance of text from left of frame</source>
      <translation>Distance of text from left of frame</translation>
    </message>
    <message>
      <source>Distance of text from right of frame</source>
      <translation>Distance of text from right of frame</translation>
    </message>
    <message>
      <source>Edit tab settings of text frame...</source>
      <translation>Edit tab settings of text frame...</translation>
    </message>
    <message>
      <source>Allow the image to be a different size to the frame</source>
      <translation>Allow the image to be a different size to the frame</translation>
    </message>
    <message>
      <source>Horizontal offset of image within frame</source>
      <translation>Horizontal offset of image within frame</translation>
    </message>
    <message>
      <source>Vertical offset of image within frame</source>
      <translation>Vertical offset of image within frame</translation>
    </message>
    <message>
      <source>Resize the image horizontally</source>
      <translation>Resize the image horizontally</translation>
    </message>
    <message>
      <source>Resize the image vertically</source>
      <translation>Resize the image vertically</translation>
    </message>
    <message>
      <source>Keep the X and Y scaling the same</source>
      <translation>Keep the X and Y scaling the same</translation>
    </message>
    <message>
      <source>Make the image fit within the size of the frame</source>
      <translation>Make the image fit within the size of the frame</translation>
    </message>
    <message>
      <source>Use image proportions rather than those of the frame</source>
      <translation>Use image proportions rather than those of the frame</translation>
    </message>
    <message>
      <source>Cell Lines</source>
      <translation>Cell Lines</translation>
    </message>
    <message>
      <source>Line at Top</source>
      <translation>Line at Top</translation>
    </message>
    <message>
      <source>Line at the Left</source>
      <translation>Line at the Left</translation>
    </message>
    <message>
      <source>Line at the Right </source>
      <translation>Line at the Right </translation>
    </message>
    <message>
      <source>Line at Bottom</source>
      <translation>Line at Bottom</translation>
    </message>
    <message>
      <source>Keep the aspect ratio</source>
      <translation>Keep the aspect ratio</translation>
    </message>
    <message>
      <source>Source profile of the image</source>
      <translation>Source profile of the image</translation>
    </message>
    <message>
      <source>Rendering intent for the image</source>
      <translation>Rendering intent for the image</translation>
    </message>
    <message>
      <source>Path Text Properties</source>
      <translation>Path Text Properties</translation>
    </message>
    <message>
      <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
      <translation>Indicates the level the object is on, 0 means the object is at the bottom</translation>
    </message>
    <message>
      <source>Make text in lower frames flow around the object shape</source>
      <translation>Make text in lower frames flow around the object shape</translation>
    </message>
    <message>
      <source>Switches between Gap or Column width</source>
      <translation>Switches between Gap or Column width</translation>
    </message>
    <message>
      <source>Column width</source>
      <translation>Column width</translation>
    </message>
    <message>
      <source>X, Y, &amp;Z</source>
      <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Shape</translation>
    </message>
    <message>
      <source>&amp;Text</source>
      <translation>&amp;Text</translation>
    </message>
    <message>
      <source>&amp;Image</source>
      <translation>&amp;Image</translation>
    </message>
    <message>
      <source>&amp;Line</source>
      <translation>&amp;Line</translation>
    </message>
    <message>
      <source>&amp;Colors</source>
      <translation>&amp;Colours</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>&amp;X-Pos:</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Width:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Height:</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>&amp;Rotation:</translation>
    </message>
    <message>
      <source>&amp;Edit Shape...</source>
      <translation>&amp;Edit Shape...</translation>
    </message>
    <message>
      <source>R&amp;ound
Corners:</source>
      <translation>R&amp;ound
Corners:</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>Colu&amp;mns:</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>&amp;Gap:</translation>
    </message>
    <message>
      <source>To&amp;p:</source>
      <translation>To&amp;p:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Bottom:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Left:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Right:</translation>
    </message>
    <message>
      <source>T&amp;abulators...</source>
      <translation>T&amp;abulators...</translation>
    </message>
    <message>
      <source>Text &amp;Flows Around Frame</source>
      <translation>Text &amp;Flows Around Frame</translation>
    </message>
    <message>
      <source>Use &amp;Bounding Box</source>
      <translation>Use &amp;Bounding Box</translation>
    </message>
    <message>
      <source>&amp;Use Contour Line</source>
      <translation>&amp;Use Contour Line</translation>
    </message>
    <message>
      <source>St&amp;yle:</source>
      <translation>St&amp;yle:</translation>
    </message>
    <message>
      <source>Lan&amp;guage:</source>
      <translation>Lan&amp;guage:</translation>
    </message>
    <message>
      <source>&amp;Free Scaling</source>
      <translation>&amp;Free Scaling</translation>
    </message>
    <message>
      <source>X-Sc&amp;ale:</source>
      <translation>X-Sc&amp;ale:</translation>
    </message>
    <message>
      <source>Y-Scal&amp;e:</source>
      <translation>Y-Scal&amp;e:</translation>
    </message>
    <message>
      <source>Scale &amp;To Frame Size</source>
      <translation>Scale &amp;To Frame Size</translation>
    </message>
    <message>
      <source>P&amp;roportional</source>
      <translation>P&amp;roportional</translation>
    </message>
    <message>
      <source>&amp;Basepoint:</source>
      <translation>&amp;Basepoint:</translation>
    </message>
    <message>
      <source>T&amp;ype of Line:</source>
      <translation>T&amp;ype of Line:</translation>
    </message>
    <message>
      <source>Line &amp;Width:</source>
      <translation>Line &amp;Width:</translation>
    </message>
    <message>
      <source>Ed&amp;ges:</source>
      <translation>Ed&amp;ges:</translation>
    </message>
    <message>
      <source>&amp;Endings:</source>
      <translation>&amp;Endings:</translation>
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
      <source>Use a surrounding box instead of the frame's shape for text flow</source>
      <translation>Use a surrounding box instead of the frame's shape for text flow</translation>
    </message>
    <message>
      <source>Use a second line originally based on the frame's shape for text flow</source>
      <translation>Use a second line originally based on the frame's shape for text flow</translation>
    </message>
    <message>
      <source>Hyphenation language of frame</source>
      <translation>Hyphenation language of frame</translation>
    </message>
    <message>
      <source>Right to Left Writing</source>
      <translation>Right to Left Writing</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>Manual Tracking</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Start Arrow:</source>
      <translation>Start Arrow:</translation>
    </message>
    <message>
      <source>End Arrow:</source>
      <translation>End Arrow:</translation>
    </message>
    <message>
      <source>Offset to baseline of characters</source>
      <translation>Offset to baseline of characters</translation>
    </message>
    <message>
      <source>Scaling height of characters</source>
      <translation>Scaling height of characters</translation>
    </message>
    <message>
      <source>Fixed Linespacing</source>
      <translation>Fixed Linespacing</translation>
    </message>
    <message>
      <source>Automatic Linespacing</source>
      <translation>Automatic Linespacing</translation>
    </message>
    <message>
      <source>Align to Baseline Grid</source>
      <translation>Align to Baseline Grid</translation>
    </message>
    <message>
      <source>Actual X-DPI:</source>
      <translation>Actual X-DPI:</translation>
    </message>
    <message>
      <source>Actual Y-DPI:</source>
      <translation>Actual Y-DPI:</translation>
    </message>
  </context>
  <context>
    <name>MultiLine</name>
    <message>
      <source>Edit Style</source>
      <translation>Edit Style</translation>
    </message>
    <message>
      <source>Flat Cap</source>
      <translation>Flat Cap</translation>
    </message>
    <message>
      <source>Square Cap</source>
      <translation>Square Cap</translation>
    </message>
    <message>
      <source>Round Cap</source>
      <translation>Round Cap</translation>
    </message>
    <message>
      <source>Miter Join</source>
      <translation>Mitre Join</translation>
    </message>
    <message>
      <source>Bevel Join</source>
      <translation>Bevel Join</translation>
    </message>
    <message>
      <source>Round Join</source>
      <translation>Round Join</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>Line Width:</translation>
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
      <source> pt </source>
      <translation> pt </translation>
    </message>
    <message>
      <source>Solid Line</source>
      <translation>Solid Line</translation>
    </message>
    <message>
      <source>Dashed Line</source>
      <translation>Dashed Line</translation>
    </message>
    <message>
      <source>Dotted Line</source>
      <translation>Dotted Line</translation>
    </message>
    <message>
      <source>Dash Dot Line</source>
      <translation>Dash Dot Line</translation>
    </message>
    <message>
      <source>Dash Dot Dot Line</source>
      <translation>Dash Dot Dot Line</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.
Please choose another.</source>
      <translation>Name &quot;%1&quot; isn't unique.
Please choose another.</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>NewDoc</name>
    <message>
      <source>New Document</source>
      <translation>New Document</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Margin Guides</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>Document page size, either a standard size or a custom size</source>
      <translation>Document page size, either a standard size or a custom size</translation>
    </message>
    <message>
      <source>Orientation of the document's pages</source>
      <translation>Orientation of the document's pages</translation>
    </message>
    <message>
      <source>Width of the document's pages, editable if you have chosen a custom page size</source>
      <translation>Width of the document's pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
      <source>Height of the document's pages, editable if you have chosen a custom page size</source>
      <translation>Height of the document's pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
      <source>First page number of the document</source>
      <translation>First page number of the document</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>Default unit of measurement for document editing</translation>
    </message>
    <message>
      <source>Create text frames automatically when new pages are added</source>
      <translation>Create text frames automatically when new pages are added</translation>
    </message>
    <message>
      <source>Distance between automatically created columns</source>
      <translation>Distance between automatically created columns</translation>
    </message>
    <message>
      <source>Number of columns to create in automatically created text frames</source>
      <translation>Number of columns to create in automatically created text frames</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>Orie&amp;ntation:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Width:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Height:</translation>
    </message>
    <message>
      <source>F&amp;irst Page Number:</source>
      <translation>F&amp;irst Page Number:</translation>
    </message>
    <message>
      <source>&amp;Default Unit:</source>
      <translation>&amp;Default Unit:</translation>
    </message>
    <message>
      <source>&amp;Automatic Text Frames</source>
      <translation>&amp;Automatic Text Frames</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>&amp;Gap:</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>Colu&amp;mns:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Open Document</source>
      <translation>Open Document</translation>
    </message>
    <message>
      <source>Recent Documents</source>
      <translation>Recent Documents</translation>
    </message>
    <message>
      <source>Do not show this dialog again</source>
      <translation>Do not show this dialog again</translation>
    </message>
    <message>
      <source>Initial number of pages of the document</source>
      <translation>Initial number of pages of the document</translation>
    </message>
    <message>
      <source>N&amp;umber of Pages:</source>
      <translation>N&amp;umber of Pages:</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
      <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;</source>
      <translation>Documents (*.sla *.scd);;</translation>
    </message>
    <message>
      <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
      <translation>Postscript Files (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
      <source>SVG Images (*.svg *.svgz);;</source>
      <translation>SVG Images (*.svg *.svgz);;</translation>
    </message>
    <message>
      <source>SVG Images (*.svg);;</source>
      <translation>SVG Images (*.svg);;</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw (*.sxd);;</source>
      <translation>OpenOffice.org Draw (*.sxd);;</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
  </context>
  <context>
    <name>NewTm</name>
    <message>
      <source>Left Page</source>
      <translation>Left Page</translation>
    </message>
    <message>
      <source>Right Page</source>
      <translation>Right Page</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>NodePalette</name>
    <message>
      <source>Nodes</source>
      <translation>Nodes</translation>
    </message>
    <message>
      <source>Move Nodes</source>
      <translation>Move Nodes</translation>
    </message>
    <message>
      <source>Move Control Points</source>
      <translation>Move Control Points</translation>
    </message>
    <message>
      <source>Add Nodes</source>
      <translation>Add Nodes</translation>
    </message>
    <message>
      <source>Delete Nodes</source>
      <translation>Delete Nodes</translation>
    </message>
    <message>
      <source>Reset Control Points</source>
      <translation>Reset Control Points</translation>
    </message>
    <message>
      <source>Reset this Control Point</source>
      <translation>Reset this Control Point</translation>
    </message>
    <message>
      <source>&amp;Absolute Coordinates</source>
      <translation>&amp;Absolute Coordinates</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>&amp;X-Pos:</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
      <source>Edit &amp;Contour Line</source>
      <translation>Edit &amp;Contour Line</translation>
    </message>
    <message>
      <source>&amp;Reset Contour Line</source>
      <translation>&amp;Reset Contour Line</translation>
    </message>
    <message>
      <source>&amp;End Editing</source>
      <translation>&amp;End Editing</translation>
    </message>
    <message>
      <source>Move Control Points Independently</source>
      <translation>Move Control Points Independently</translation>
    </message>
    <message>
      <source>Move Control Points Symmetrical</source>
      <translation>Move Control Points Symmetrical</translation>
    </message>
    <message>
      <source>Open a Polygon or Cuts a Bezier Curve</source>
      <translation>Open a Polygon or Cuts a Bezier Curve</translation>
    </message>
    <message>
      <source>Close this Bezier Curve</source>
      <translation>Close this Bezier Curve</translation>
    </message>
    <message>
      <source>Mirror the Path Horizontally</source>
      <translation>Mirror the Path Horizontally</translation>
    </message>
    <message>
      <source>Mirror the Path Vertically</source>
      <translation>Mirror the Path Vertically</translation>
    </message>
    <message>
      <source>Shear the Path Horizontally to the Left</source>
      <translation>Shear the Path Horizontally to the Left</translation>
    </message>
    <message>
      <source>Shear the Path Vertically Up</source>
      <translation>Shear the Path Vertically Up</translation>
    </message>
    <message>
      <source>Shear the Path Vertically Down</source>
      <translation>Shear the Path Vertically Down</translation>
    </message>
    <message>
      <source>Rotate the Path Counter-Clockwise</source>
      <translation>Rotate the Path Counter-Clockwise</translation>
    </message>
    <message>
      <source>Rotate the Path Clockwise</source>
      <translation>Rotate the Path Clockwise</translation>
    </message>
    <message>
      <source>Reduce the Size of the Path by shown %</source>
      <translation>Reduce the Size of the Path by shown %</translation>
    </message>
    <message>
      <source>Enlarge the Size of the Path by shown %</source>
      <translation>Enlarge the Size of the Path by shown %</translation>
    </message>
    <message>
      <source>Angle of Rotation</source>
      <translation>Angle of Rotation</translation>
    </message>
    <message>
      <source>% to Enlarge or Reduce By</source>
      <translation>% to Enlarge or Reduce By</translation>
    </message>
    <message>
      <source>Activate Contour Line Editing Mode</source>
      <translation>Activate Contour Line Editing Mode</translation>
    </message>
    <message>
      <source>Reset the Contour Line to the Original Shape of the Frame</source>
      <translation>Reset the Contour Line to the Original Shape of the Frame</translation>
    </message>
    <message>
      <source>Shear the Path Horizontally to the Right</source>
      <translation>Shear the Path Horizontally to the Right</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
      <translation>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</translation>
    </message>
  </context>
  <context>
    <name>OdtDialog</name>
    <message>
      <source>Use document name as a prefix for paragraph styles</source>
      <translation>Use document name as a prefix for paragraph styles</translation>
    </message>
    <message>
      <source>Do not ask again</source>
      <translation>Do not ask again</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Enabling this will overwrite existing styles in the current Scribus document</source>
      <translation>Enabling this will overwrite existing styles in the current Scribus document</translation>
    </message>
    <message>
      <source>Merge Paragraph Styles</source>
      <translation>Merge Paragraph Styles</translation>
    </message>
    <message>
      <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</source>
      <translation>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</translation>
    </message>
    <message>
      <source>Prepend the document name to the paragraph style name in Scribus.</source>
      <translation>Prepend the document name to the paragraph style name in Scribus.</translation>
    </message>
    <message>
      <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
      <translation>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</translation>
    </message>
    <message>
      <source>Overwrite Paragraph Styles</source>
      <translation>Overwrite Paragraph Styles</translation>
    </message>
    <message>
      <source>OpenDocument Importer Options</source>
      <translation>OpenDocument Importer Options</translation>
    </message>
  </context>
  <context>
    <name>OutlineValues</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>Linewidth</translation>
    </message>
  </context>
  <context>
    <name>PDF_Opts</name>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>O&amp;utput to File:</source>
      <translation>O&amp;utput to File:</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>Cha&amp;nge...</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDF Files (*.pdf);;All Files (*)</translation>
    </message>
    <message>
      <source>Save as PDF</source>
      <translation>Save as PDF</translation>
    </message>
  </context>
  <context>
    <name>PPreview</name>
    <message>
      <source>Print Preview</source>
      <translation>Print Preview</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
      <translation>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</translation>
    </message>
    <message>
      <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
      <translation>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</translation>
    </message>
    <message>
      <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
      <translation>Gives a print preview using simulations of generic CMYK inks, instead of RGB colours</translation>
    </message>
    <message>
      <source>Enable/disable the C (Cyan) ink plate</source>
      <translation>Enable/disable the C (Cyan) ink plate</translation>
    </message>
    <message>
      <source>Enable/disable the M (Magenta) ink plate</source>
      <translation>Enable/disable the M (Magenta) ink plate</translation>
    </message>
    <message>
      <source>Enable/disable the Y (Yellow) ink plate</source>
      <translation>Enable/disable the Y (Yellow) ink plate</translation>
    </message>
    <message>
      <source>Enable/disable the K (Black) ink plate</source>
      <translation>Enable/disable the K (Black) ink plate</translation>
    </message>
    <message>
      <source>Anti-alias &amp;Text</source>
      <translation>Anti-alias &amp;Text</translation>
    </message>
    <message>
      <source>Anti-alias &amp;Graphics</source>
      <translation>Anti-alias &amp;Graphics</translation>
    </message>
    <message>
      <source>Display Trans&amp;parency</source>
      <translation>Display Trans&amp;parency</translation>
    </message>
    <message>
      <source>&amp;Display CMYK</source>
      <translation>&amp;Display CMYK</translation>
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
      <translation>&amp;Under Colour Removal</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>A way of switching off some of the grey shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the grey. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
      <translation>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
      <translation>Detected some Errors.
Consider using the Preflight Checker to correct them</translation>
    </message>
    <message>
      <source>Abort</source>
      <translation>Abort</translation>
    </message>
    <message>
      <source>Ignore</source>
      <translation>Ignore</translation>
    </message>
  </context>
  <context>
    <name>PageItem</name>
    <message>
      <source>Image</source>
      <translation>Image</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>Line</translation>
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
      <translation>PathText</translation>
    </message>
    <message>
      <source>Copy of</source>
      <translation>Copy of</translation>
    </message>
  </context>
  <context>
    <name>PageItemAttributes</name>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Relates To</source>
      <translation>Relates To</translation>
    </message>
    <message>
      <source>Is Parent Of</source>
      <translation>Is Parent Of</translation>
    </message>
    <message>
      <source>Is Child Of</source>
      <translation>Is Child Of</translation>
    </message>
    <message>
      <source>Page Item Attributes</source>
      <translation>Page Item Attributes</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Type</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>Value</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation>Parameter</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation>Relationship</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation>Relationship To</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Add</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>PageLayouts</name>
    <message>
      <source>Page Layout</source>
      <translation>Page Layout</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>Single Page</translation>
    </message>
    <message>
      <source>Double sided</source>
      <translation>Double sided</translation>
    </message>
    <message>
      <source>3-Fold</source>
      <translation>3-Fold</translation>
    </message>
    <message>
      <source>4-Fold</source>
      <translation>4-Fold</translation>
    </message>
    <message>
      <source>Page #</source>
      <translation>Page #</translation>
    </message>
    <message>
      <source>is the first Page.</source>
      <translation>is the first Page.</translation>
    </message>
  </context>
  <context>
    <name>PageSelector</name>
    <message>
      <source>Page </source>
      <translation>Page </translation>
    </message>
    <message>
      <source> of %1</source>
      <translation> of %1</translation>
    </message>
  </context>
  <context>
    <name>PageSize</name>
    <message>
      <source>Quarto</source>
      <translation>Quarto</translation>
    </message>
    <message>
      <source>Foolscap</source>
      <translation>Foolscap</translation>
    </message>
    <message>
      <source>Letter</source>
      <translation>Letter</translation>
    </message>
    <message>
      <source>Government Letter</source>
      <translation>Government Letter</translation>
    </message>
    <message>
      <source>Legal</source>
      <translation>Legal</translation>
    </message>
    <message>
      <source>Ledger</source>
      <translation>Ledger</translation>
    </message>
    <message>
      <source>Executive</source>
      <translation>Executive</translation>
    </message>
    <message>
      <source>Post</source>
      <translation>Post</translation>
    </message>
    <message>
      <source>Crown</source>
      <translation>Crown</translation>
    </message>
    <message>
      <source>Large Post</source>
      <translation>Large Post</translation>
    </message>
    <message>
      <source>Demy</source>
      <translation>Demy</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>Medium</translation>
    </message>
    <message>
      <source>Royal</source>
      <translation>Royal</translation>
    </message>
    <message>
      <source>Elephant</source>
      <translation>Elephant</translation>
    </message>
    <message>
      <source>Double Demy</source>
      <translation>Double Demy</translation>
    </message>
    <message>
      <source>Quad Demy</source>
      <translation>Quad Demy</translation>
    </message>
    <message>
      <source>STMT</source>
      <translation>STMT</translation>
    </message>
    <message>
      <source>A</source>
      <translation>A</translation>
    </message>
    <message>
      <source>B</source>
      <translation>B</translation>
    </message>
    <message>
      <source>C</source>
      <translation>C</translation>
    </message>
    <message>
      <source>D</source>
      <translation>D</translation>
    </message>
    <message>
      <source>E</source>
      <translation>E</translation>
    </message>
  </context>
  <context>
    <name>PicSearch</name>
    <message>
      <source>Result</source>
      <translation>Result</translation>
    </message>
    <message>
      <source>Search Results for: </source>
      <translation>Search Results for: </translation>
    </message>
    <message>
      <source>Preview</source>
      <translation>Preview</translation>
    </message>
    <message>
      <source>Select</source>
      <translation>Select</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
  </context>
  <context>
    <name>PicStatus</name>
    <message>
      <source>Goto</source>
      <translation>Goto</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Yes</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Missing</source>
      <translation>Missing</translation>
    </message>
    <message>
      <source>Search</source>
      <translation>Search</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Path</source>
      <translation>Path</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Page</translation>
    </message>
    <message>
      <source>Print</source>
      <translation>Print</translation>
    </message>
    <message>
      <source>Status</source>
      <translation>Status</translation>
    </message>
    <message>
      <source>Cancel Search</source>
      <translation>Cancel Search</translation>
    </message>
    <message>
      <source>Manage Pictures</source>
      <translation>Manage Pictures</translation>
    </message>
    <message>
      <source>Scribus - Image Search</source>
      <translation>Scribus - Image Search</translation>
    </message>
    <message>
      <source>The search failed: %1</source>
      <translation>The search failed: %1</translation>
    </message>
    <message>
      <source>No images named &quot;%1&quot; were found.</source>
      <translation>No images named &quot;%1&quot; were found.</translation>
    </message>
  </context>
  <context>
    <name>PluginManager</name>
    <message>
      <source>Persistent</source>
      <comment>plugin manager</comment>
      <translation>Persistent</translation>
    </message>
    <message>
      <source>Import</source>
      <comment>plugin manager</comment>
      <translation>Import</translation>
    </message>
    <message>
      <source>Standard</source>
      <comment>plugin manager</comment>
      <translation>Standard</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>plugin manager</comment>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>Cannot find plugin</source>
      <comment>plugin manager</comment>
      <translation>Cannot find plugin</translation>
    </message>
    <message>
      <source>unknown error</source>
      <comment>plugin manager</comment>
      <translation>unknown error</translation>
    </message>
    <message>
      <source>Cannot find symbol (%1)</source>
      <comment>plugin manager</comment>
      <translation>Cannot find symbol (%1)</translation>
    </message>
  </context>
  <context>
    <name>PolygonProps</name>
    <message>
      <source>Polygon Properties</source>
      <translation>Polygon Properties</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>PolygonWidget</name>
    <message>
      <source>Corn&amp;ers:</source>
      <translation>Corn&amp;ers:</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>&amp;Rotation:</translation>
    </message>
    <message>
      <source>Apply &amp;Factor</source>
      <translation>Apply &amp;Factor</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Factor:</source>
      <translation>&amp;Factor:</translation>
    </message>
    <message>
      <source>Number of corners for polygons</source>
      <translation>Number of corners for polygons</translation>
    </message>
    <message>
      <source>Degrees of rotation for polygons</source>
      <translation>Degrees of rotation for polygons</translation>
    </message>
    <message>
      <source>Apply Convex/Concave Factor to change shape of Polygons</source>
      <translation>Apply Convex/Concave Factor to change shape of Polygons</translation>
    </message>
    <message>
      <source>Sample Polygon</source>
      <translation>Sample Polygon</translation>
    </message>
    <message>
      <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
      <translation>A negative value will make the polygon concave (or star shaped),
a positive value will make it convex</translation>
    </message>
  </context>
  <context>
    <name>Preferences</name>
    <message>
      <source>Preferences</source>
      <translation>Preferences</translation>
    </message>
    <message>
      <source>General</source>
      <translation>General</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Document</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>Guides</translation>
    </message>
    <message>
      <source>Typography</source>
      <translation>Typography</translation>
    </message>
    <message>
      <source>Tools</source>
      <translation>Tools</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>Scrapbook</translation>
    </message>
    <message>
      <source>Display</source>
      <translation>Display</translation>
    </message>
    <message>
      <source>GUI</source>
      <translation>GUI</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>Paths</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Margin Guides</translation>
    </message>
    <message>
      <source>Autosave</source>
      <translation>Autosave</translation>
    </message>
    <message>
      <source>min</source>
      <translation>min</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Other Options</source>
      <translation>Other Options</translation>
    </message>
    <message>
      <source>Preview</source>
      <translation>Preview</translation>
    </message>
    <message>
      <source>Small</source>
      <translation>Small</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>Medium</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>External Tools</source>
      <translation>External Tools</translation>
    </message>
    <message>
      <source>Postscript Interpreter</source>
      <translation>Postscript Interpreter</translation>
    </message>
    <message>
      <source>Image Processing Tool</source>
      <translation>Image Processing Tool</translation>
    </message>
    <message>
      <source>Printing</source>
      <translation>Printing</translation>
    </message>
    <message>
      <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
      <translation>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</translation>
    </message>
    <message>
      <source>Default font size for the menus and windows</source>
      <translation>Default font size for the menus and windows</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>Default unit of measurement for document editing</translation>
    </message>
    <message>
      <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
      <translation>Number of lines Scribus will scroll for each move of the mouse wheel</translation>
    </message>
    <message>
      <source>Number of recently edited documents to show in the File menu</source>
      <translation>Number of recently edited documents to show in the File menu</translation>
    </message>
    <message>
      <source>Default documents directory</source>
      <translation>Default documents directory</translation>
    </message>
    <message>
      <source>Default Scripter scripts directory</source>
      <translation>Default Scripter scripts directory</translation>
    </message>
    <message>
      <source>Default page size, either a standard size or a custom size</source>
      <translation>Default page size, either a standard size or a custom size</translation>
    </message>
    <message>
      <source>Default orientation of document pages</source>
      <translation>Default orientation of document pages</translation>
    </message>
    <message>
      <source>Width of document pages, editable if you have chosen a custom page size</source>
      <translation>Width of document pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
      <source>Height of document pages, editable if you have chosen a custom page size</source>
      <translation>Height of document pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
      <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
      <translation>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</translation>
    </message>
    <message>
      <source>Time period between saving automatically</source>
      <translation>Time period between saving automatically</translation>
    </message>
    <message>
      <source>Choose the size of the preview in the scrapbook palette</source>
      <translation>Choose the size of the preview in the scrapbook palette</translation>
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
      <translation>Set the default zoom level</translation>
    </message>
    <message>
      <source>Antialias text for EPS and PDF onscreen rendering</source>
      <translation>Antialias text for EPS and PDF onscreen rendering</translation>
    </message>
    <message>
      <source>Antialias graphics for EPS and PDF onscreen rendering</source>
      <translation>Antialias graphics for EPS and PDF onscreen rendering</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins on the printed page or exported file</source>
      <translation>Do not show objects outside the margins on the printed page or exported file</translation>
    </message>
    <message>
      <source>Save the scrapbook contents everytime after a change</source>
      <translation>Save the scrapbook contents everytime after a change</translation>
    </message>
    <message>
      <source>&amp;Theme:</source>
      <translation>&amp;Theme:</translation>
    </message>
    <message>
      <source>&amp;Font Size:</source>
      <translation>&amp;Font Size:</translation>
    </message>
    <message>
      <source>&amp;Wheel Jump:</source>
      <translation>&amp;Wheel Jump:</translation>
    </message>
    <message>
      <source>&amp;Recent Documents:</source>
      <translation>&amp;Recent Documents:</translation>
    </message>
    <message>
      <source>&amp;Documents:</source>
      <translation>&amp;Documents:</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation>&amp;Change...</translation>
    </message>
    <message>
      <source>&amp;ICC Profiles:</source>
      <translation>&amp;ICC Profiles:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>C&amp;hange...</translation>
    </message>
    <message>
      <source>&amp;Scripts:</source>
      <translation>&amp;Scripts:</translation>
    </message>
    <message>
      <source>Ch&amp;ange...</source>
      <translation>Ch&amp;ange...</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>Orie&amp;ntation:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Width:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Height:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Bottom:</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Top:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Right:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Left:</translation>
    </message>
    <message>
      <source>&amp;Enabled</source>
      <translation>&amp;Enabled</translation>
    </message>
    <message>
      <source>&amp;Interval:</source>
      <translation>&amp;Interval:</translation>
    </message>
    <message>
      <source>Sa&amp;ve Contents on Changes</source>
      <translation>Sa&amp;ve Contents on Changes</translation>
    </message>
    <message>
      <source>Large</source>
      <translation>Large</translation>
    </message>
    <message>
      <source>Display &amp;Unprintable Area in Margin Color</source>
      <translation>Display &amp;Unprintable Area in Margin Colour</translation>
    </message>
    <message>
      <source>&amp;Adjust Display Size</source>
      <translation>&amp;Adjust Display Size</translation>
    </message>
    <message>
      <source>&amp;Name of Executable:</source>
      <translation>&amp;Name of Executable:</translation>
    </message>
    <message>
      <source>Antialias &amp;Text</source>
      <translation>Antialias &amp;Text</translation>
    </message>
    <message>
      <source>Antialias &amp;Graphics</source>
      <translation>Antialias &amp;Graphics</translation>
    </message>
    <message>
      <source>Name of &amp;Executable:</source>
      <translation>Name of &amp;Executable:</translation>
    </message>
    <message>
      <source>Clip to Page &amp;Margins</source>
      <translation>Clip to Page &amp;Margins</translation>
    </message>
    <message>
      <source>Apply &amp;Under Color Removal</source>
      <translation>Apply &amp;Under Colour Removal</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>Cha&amp;nge...</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>A way of switching off some of the grey shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the grey. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
      <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend 'gimp-remote',
as it allows you to edit the image in an already running
instance of gimp.</source>
      <translation>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend 'gimp-remote',
as it allows you to edit the image in an already running
instance of gimp.</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Language:</translation>
    </message>
    <message>
      <source>Document T&amp;emplates:</source>
      <translation>Document T&amp;emplates:</translation>
    </message>
    <message>
      <source>Units:</source>
      <translation>Units:</translation>
    </message>
    <message>
      <source>Undo/Redo</source>
      <translation>Undo/Redo</translation>
    </message>
    <message>
      <source>Action history length</source>
      <translation>Action history length</translation>
    </message>
    <message>
      <source>Hyphenator</source>
      <translation>Hyphenator</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>Fonts</translation>
    </message>
    <message>
      <source>Color Management</source>
      <translation>Colour Management</translation>
    </message>
    <message>
      <source>PDF Export</source>
      <translation>PDF Export</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>Document Item Attributes</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Table of Contents and Indexes</translation>
    </message>
    <message>
      <source>Keyboard Shortcuts</source>
      <translation>Keyboard Shortcuts</translation>
    </message>
    <message>
      <source>Page Display</source>
      <translation>Page Display</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Colour:</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Show Pictures</source>
      <translation>Show Pictures</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation>Show Text Chains</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation>Show Frames</translation>
    </message>
    <message>
      <source>Scratch Space</source>
      <translation>Scratch Space</translation>
    </message>
    <message>
      <source>Miscellaneous</source>
      <translation>Miscellaneous</translation>
    </message>
    <message>
      <source>Plugin Manager</source>
      <translation>Plugin Manager</translation>
    </message>
    <message>
      <source>Plugin</source>
      <translation>Plugin</translation>
    </message>
    <message>
      <source>How to run</source>
      <translation>How to run</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Type</translation>
    </message>
    <message>
      <source>Load it?</source>
      <translation>Load it?</translation>
    </message>
    <message>
      <source>Plugin ID</source>
      <translation>Plugin ID</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Yes</translation>
    </message>
    <message>
      <source>No</source>
      <translation>No</translation>
    </message>
    <message>
      <source>You need to restart the application to apply the changes.</source>
      <translation>You need to restart the application to apply the changes.</translation>
    </message>
    <message>
      <source>Plugins</source>
      <translation>Plugins</translation>
    </message>
    <message>
      <source>Turns the display of linked frames on or off</source>
      <translation>Turns the display of linked frames on or off</translation>
    </message>
    <message>
      <source>Turns the display of frames on or off</source>
      <translation>Turns the display of frames on or off</translation>
    </message>
    <message>
      <source>Turns the display of pictures on or off</source>
      <translation>Turns the display of pictures on or off</translation>
    </message>
    <message>
      <source>Select your default language for Scribus to run with.
Leave this blank to choose based on environment variables.
You can still override this by passing a command line option when starting Scribus</source>
      <translation>Select your default language for Scribus to run with.
Leave this blank to choose based on environment variables.
You can still override this by passing a command line option when starting Scribus</translation>
    </message>
    <message>
      <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
      <translation>Default ICC profiles directory. This cannot
be changed with a document open.</translation>
    </message>
    <message>
      <source>Additional directory for document templates</source>
      <translation>Additional directory for document templates</translation>
    </message>
    <message>
      <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
      <translation>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</translation>
    </message>
    <message>
      <source>Filesystem location for the Ghostscript interpreter</source>
      <translation>Filesystem location for the Ghostscript interpreter</translation>
    </message>
    <message>
      <source>Always ask before fonts are replaced when loading a document</source>
      <translation>Always ask before fonts are replaced when loading a document</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation>Show Text Control Characters</translation>
    </message>
    <message>
      <source>Preview of current Paragraph Style visible when editing Styles</source>
      <translation>Preview of current Paragraph Style visible when editing Styles</translation>
    </message>
    <message>
      <source>To adjust the display drag the ruler below with the slider.</source>
      <translation>To adjust the display drag the ruler below with the slider.</translation>
    </message>
    <message>
      <source>Display non-printing characters such as paragraph markers in text frames</source>
      <translation>Display non-printing characters such as paragraph markers in text frames</translation>
    </message>
    <message>
      <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
      <translation>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</translation>
    </message>
    <message>
      <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
      <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
      <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
      <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Preflight Verifier</translation>
    </message>
    <message>
      <source>dpi</source>
      <translation>dpi</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>Resolution:</translation>
    </message>
    <message>
      <source>Lorem Ipsum</source>
      <translation>Lorem Ipsum</translation>
    </message>
    <message>
      <source>Always use standard Lorem Ipsum</source>
      <translation>Always use standard Lorem Ipsum</translation>
    </message>
    <message>
      <source>Count of the Paragraphs:</source>
      <translation>Count of the Paragraphs:</translation>
    </message>
    <message>
      <source>Rulers relative to Page</source>
      <translation>Rulers relative to Page</translation>
    </message>
    <message>
      <source>Gaps between Pages</source>
      <translation>Gaps between Pages</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation>Horizontal:</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation>Vertical:</translation>
    </message>
    <message>
      <source>Show Startup Dialog</source>
      <translation>Show Startup Dialog</translation>
    </message>
    <message>
      <source>Locate Ghostscript</source>
      <translation>Locate Ghostscript</translation>
    </message>
    <message>
      <source>Locate your image editor</source>
      <translation>Locate your image editor</translation>
    </message>
  </context>
  <context>
    <name>PrefsDialogBase</name>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;Defaults</source>
      <translation>&amp;Defaults</translation>
    </message>
  </context>
  <context>
    <name>PrefsManager</name>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
    </message>
    <message>
      <source>PDF 1.3</source>
      <translation>PDF 1.3</translation>
    </message>
    <message>
      <source>PDF 1.4</source>
      <translation>PDF 1.4</translation>
    </message>
    <message>
      <source>PDF/X-3</source>
      <translation>PDF/X-3</translation>
    </message>
    <message>
      <source>Migrate Old Scribus Settings?</source>
      <translation>Migrate Old Scribus Settings?</translation>
    </message>
    <message>
      <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
      <translation>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</translation>
    </message>
  </context>
  <context>
    <name>PythonConsole</name>
    <message>
      <source>&amp;Open...</source>
      <translation>&amp;Open...</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>&amp;Exit</source>
      <translation>&amp;Exit</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
    </message>
    <message>
      <source>&amp;Run</source>
      <translation>&amp;Run</translation>
    </message>
    <message>
      <source>&amp;Save Output...</source>
      <translation>&amp;Save Output...</translation>
    </message>
    <message>
      <source>&amp;Script</source>
      <translation>&amp;Script</translation>
    </message>
    <message>
      <source>Script Console</source>
      <translation>Script Console</translation>
    </message>
    <message>
      <source>Write your commands here. A selection is processed as script</source>
      <translation>Write your commands here. A selection is processed as script</translation>
    </message>
    <message>
      <source>Output of your script</source>
      <translation>Output of your script</translation>
    </message>
    <message>
      <source>Python Scripts (*.py)</source>
      <translation>Python Scripts (*.py)</translation>
    </message>
    <message>
      <source>Open File With Python Commands</source>
      <translation>Open File With Python Commands</translation>
    </message>
    <message>
      <source>Save the Python Commands in File</source>
      <translation>Save the Python Commands in File</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Text Files (*.txt)</source>
      <translation>Text Files (*.txt)</translation>
    </message>
    <message>
      <source>Save Current Output</source>
      <translation>Save Current Output</translation>
    </message>
    <message>
      <source>&amp;Save As...</source>
      <translation>&amp;Save As...</translation>
    </message>
    <message>
      <source>&amp;Run As Console</source>
      <translation>&amp;Run As Console</translation>
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
      <translation>A&amp;lpha channel:</translation>
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
      <source>&amp;Define Custom Colors >></source>
      <translation>&amp;Define Custom Colours >></translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
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
      <translation>Copy or Move a File</translation>
    </message>
    <message>
      <source>Read: %1</source>
      <translation>Read: %1</translation>
    </message>
    <message>
      <source>Write: %1</source>
      <translation>Write: %1</translation>
    </message>
    <message>
      <source>File &amp;name:</source>
      <translation>File &amp;name:</translation>
    </message>
    <message>
      <source>File &amp;type:</source>
      <translation>File &amp;type:</translation>
    </message>
    <message>
      <source>One directory up</source>
      <translation>One directory up</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Size</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Type</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>Date</translation>
    </message>
    <message>
      <source>Attributes</source>
      <translation>Attributes</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Look &amp;in:</source>
      <translation>Look &amp;in:</translation>
    </message>
    <message>
      <source>Back</source>
      <translation>Back</translation>
    </message>
    <message>
      <source>Create New Folder</source>
      <translation>Create New Folder</translation>
    </message>
    <message>
      <source>List View</source>
      <translation>List View</translation>
    </message>
    <message>
      <source>Detail View</source>
      <translation>Detail View</translation>
    </message>
    <message>
      <source>Preview File Info</source>
      <translation>Preview File Info</translation>
    </message>
    <message>
      <source>Preview File Contents</source>
      <translation>Preview File Contents</translation>
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
      <translation>Inaccessible</translation>
    </message>
    <message>
      <source>Symlink to File</source>
      <translation>Symlink to File</translation>
    </message>
    <message>
      <source>Symlink to Directory</source>
      <translation>Symlink to Directory</translation>
    </message>
    <message>
      <source>Symlink to Special</source>
      <translation>Symlink to Special</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Dir</source>
      <translation>Dir</translation>
    </message>
    <message>
      <source>Special</source>
      <translation>Special</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Save As</source>
      <translation>Save As</translation>
    </message>
    <message>
      <source>&amp;Open</source>
      <translation>&amp;Open</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>&amp;Rename</source>
      <translation>&amp;Rename</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>R&amp;eload</source>
      <translation>R&amp;eload</translation>
    </message>
    <message>
      <source>Sort by &amp;Name</source>
      <translation>Sort by &amp;Name</translation>
    </message>
    <message>
      <source>Sort by &amp;Size</source>
      <translation>Sort by &amp;Size</translation>
    </message>
    <message>
      <source>Sort by &amp;Date</source>
      <translation>Sort by &amp;Date</translation>
    </message>
    <message>
      <source>&amp;Unsorted</source>
      <translation>&amp;Unsorted</translation>
    </message>
    <message>
      <source>Sort</source>
      <translation>Sort</translation>
    </message>
    <message>
      <source>Show &amp;hidden files</source>
      <translation>Show &amp;hidden files</translation>
    </message>
    <message>
      <source>the file</source>
      <translation>the file</translation>
    </message>
    <message>
      <source>the directory</source>
      <translation>the directory</translation>
    </message>
    <message>
      <source>the symlink</source>
      <translation>the symlink</translation>
    </message>
    <message>
      <source>Delete %1</source>
      <translation>Delete %1</translation>
    </message>
    <message>
      <source>&lt;qt>Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt></source>
      <translation>&lt;qt>Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt></translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>&amp;Yes</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;No</translation>
    </message>
    <message>
      <source>New Folder 1</source>
      <translation>New Folder 1</translation>
    </message>
    <message>
      <source>New Folder</source>
      <translation>New Folder</translation>
    </message>
    <message>
      <source>New Folder %1</source>
      <translation>New Folder %1</translation>
    </message>
    <message>
      <source>Find Directory</source>
      <translation>Find Directory</translation>
    </message>
    <message>
      <source>Directories</source>
      <translation>Directories</translation>
    </message>
    <message>
      <source>Save</source>
      <translation>Save</translation>
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
File not found.
Check path and filename.</translation>
    </message>
    <message>
      <source>All Files (*.*)</source>
      <translation>All Files (*.*)</translation>
    </message>
    <message>
      <source>Select a Directory</source>
      <translation>Select a Directory</translation>
    </message>
    <message>
      <source>Directory:</source>
      <translation>Directory:</translation>
    </message>
  </context>
  <context>
    <name>QFontDialog</name>
    <message>
      <source>&amp;Font</source>
      <translation>&amp;Font</translation>
    </message>
    <message>
      <source>Font st&amp;yle</source>
      <translation>Font st&amp;yle</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>&amp;Size</translation>
    </message>
    <message>
      <source>Effects</source>
      <translation>Effects</translation>
    </message>
    <message>
      <source>Stri&amp;keout</source>
      <translation>Stri&amp;keout</translation>
    </message>
    <message>
      <source>&amp;Underline</source>
      <translation>&amp;Underline</translation>
    </message>
    <message>
      <source>&amp;Color</source>
      <translation>&amp;Colour</translation>
    </message>
    <message>
      <source>Sample</source>
      <translation>Sample</translation>
    </message>
    <message>
      <source>Scr&amp;ipt</source>
      <translation>Scr&amp;ipt</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Apply</source>
      <translation>Apply</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Close</translation>
    </message>
    <message>
      <source>Select Font</source>
      <translation>Select Font</translation>
    </message>
  </context>
  <context>
    <name>QLineEdit</name>
    <message>
      <source>Clear</source>
      <translation>Clear</translation>
    </message>
    <message>
      <source>Select All</source>
      <translation>Select All</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Undo</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Redo</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>Cu&amp;t</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>&amp;Paste</translation>
    </message>
  </context>
  <context>
    <name>QMainWindow</name>
    <message>
      <source>Line up</source>
      <translation>Line up</translation>
    </message>
    <message>
      <source>Customize...</source>
      <translation>Customise...</translation>
    </message>
  </context>
  <context>
    <name>QMessageBox</name>
    <message>
      <source>&lt;h3>About Qt&lt;/h3>&lt;p>This program uses Qt version %1.&lt;/p>&lt;p>Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p>&lt;p>Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br>Qt is also available for embedded devices.&lt;/p>&lt;p>Qt is a Trolltech product. See &lt;tt>http://www.trolltech.com/qt/&lt;/tt> for more information.&lt;/p></source>
      <translation>&lt;h3>About Qt&lt;/h3>&lt;p>This program uses Qt version %1.&lt;/p>&lt;p>Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p>&lt;p>Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br>Qt is also available for embedded devices.&lt;/p>&lt;p>Qt is a Trolltech product. See &lt;tt>http://www.trolltech.com/qt/&lt;/tt> for more information.&lt;/p></translation>
    </message>
  </context>
  <context>
    <name>QObject</name>
    <message>
      <source>Initializing...</source>
      <translation>Initialising...</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Document</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Background</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Do you really want to overwrite the File:
%1 ?</source>
      <translation>Do you really want to overwrite the File:
%1 ?</translation>
    </message>
    <message>
      <source>Online Reference</source>
      <translation>Online Reference</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
      <translation>SVG-Images (*.svg *.svgz);;All Files (*)</translation>
    </message>
    <message>
      <source>SVG-Images (*.svg);;All Files (*)</source>
      <translation>SVG-Images (*.svg);;All Files (*)</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Yes</translation>
    </message>
    <message>
      <source>No</source>
      <translation>No</translation>
    </message>
    <message>
      <source>Save as Image</source>
      <translation>Save as Image</translation>
    </message>
    <message>
      <source>Error writing the output file(s).</source>
      <translation>Error writing the output file(s).</translation>
    </message>
    <message>
      <source>Export successful.</source>
      <translation>Export successful.</translation>
    </message>
    <message>
      <source>File exists. Overwrite?</source>
      <translation>File exists. Overwrite?</translation>
    </message>
    <message>
      <source>exists already. Overwrite?</source>
      <translation>exists already. Overwrite?</translation>
    </message>
    <message>
      <source>Yes all</source>
      <translation>Yes all</translation>
    </message>
    <message>
      <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
      <translation>All Supported Formats (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
    </message>
    <message>
      <source>New &amp;from Template...</source>
      <translation>New &amp;from Template...</translation>
    </message>
    <message>
      <source>Newsletters</source>
      <translation>Newsletters</translation>
    </message>
    <message>
      <source>Brochures</source>
      <translation>Brochures</translation>
    </message>
    <message>
      <source>Catalogs</source>
      <translation>Catalogues</translation>
    </message>
    <message>
      <source>Flyers</source>
      <translation>Flyers</translation>
    </message>
    <message>
      <source>Signs</source>
      <translation>Signs</translation>
    </message>
    <message>
      <source>Cards</source>
      <translation>Cards</translation>
    </message>
    <message>
      <source>Letterheads</source>
      <translation>Letterheads</translation>
    </message>
    <message>
      <source>Envelopes</source>
      <translation>Envelopes</translation>
    </message>
    <message>
      <source>Business Cards</source>
      <translation>Business Cards</translation>
    </message>
    <message>
      <source>Calendars</source>
      <translation>Calendars</translation>
    </message>
    <message>
      <source>Advertisements</source>
      <translation>Advertisements</translation>
    </message>
    <message>
      <source>Labels</source>
      <translation>Labels</translation>
    </message>
    <message>
      <source>Menus</source>
      <translation>Menus</translation>
    </message>
    <message>
      <source>Programs</source>
      <translation>Programs</translation>
    </message>
    <message>
      <source>PDF Forms</source>
      <translation>PDF Forms</translation>
    </message>
    <message>
      <source>PDF Presentations</source>
      <translation>PDF Presentations</translation>
    </message>
    <message>
      <source>Magazines</source>
      <translation>Magazines</translation>
    </message>
    <message>
      <source>Posters</source>
      <translation>Posters</translation>
    </message>
    <message>
      <source>Announcements</source>
      <translation>Announcements</translation>
    </message>
    <message>
      <source>Text Documents</source>
      <translation>Text Documents</translation>
    </message>
    <message>
      <source>Folds</source>
      <translation>Folds</translation>
    </message>
    <message>
      <source>Own Templates</source>
      <translation>Own Templates</translation>
    </message>
    <message>
      <source>Save as &amp;Image...</source>
      <translation>Save as &amp;Image...</translation>
    </message>
    <message>
      <source>Print Previe&amp;w</source>
      <translation>Print Previe&amp;w</translation>
    </message>
    <message>
      <source>Import &amp;EPS/PS...</source>
      <translation>Import &amp;EPS/PS...</translation>
    </message>
    <message>
      <source>Save as &amp;Template...</source>
      <translation>Save as &amp;Template...</translation>
    </message>
    <message>
      <source>S&amp;cripter Manual...</source>
      <translation>S&amp;cripter Manual...</translation>
    </message>
    <message>
      <source>Save Page as &amp;SVG...</source>
      <translation>Save Page as &amp;SVG...</translation>
    </message>
    <message>
      <source>Import &amp;SVG...</source>
      <translation>Import &amp;SVG...</translation>
    </message>
    <message>
      <source>Importing text</source>
      <translation>Importing text</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>All Supported Formats</translation>
    </message>
    <message>
      <source>HTML Files</source>
      <translation>HTML Files</translation>
    </message>
    <message>
      <source>html</source>
      <translation>html</translation>
    </message>
    <message>
      <source>Text Files</source>
      <translation>Text Files</translation>
    </message>
    <message>
      <source>Comma Separated Value Files</source>
      <translation>Comma Separated Value Files</translation>
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
      <translation>Font %1 is broken, discarding it</translation>
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
      <translation>Text Filters</translation>
    </message>
    <message>
      <source>Media Cases</source>
      <translation>Media Cases</translation>
    </message>
    <message>
      <source>Albanian</source>
      <translation>Albanian</translation>
    </message>
    <message>
      <source>Basque</source>
      <translation>Basque</translation>
    </message>
    <message>
      <source>Bulgarian</source>
      <translation>Bulgarian</translation>
    </message>
    <message>
      <source>Brazilian</source>
      <translation>Brazilian</translation>
    </message>
    <message>
      <source>Catalan</source>
      <translation>Catalan</translation>
    </message>
    <message>
      <source>Chinese</source>
      <translation>Chinese</translation>
    </message>
    <message>
      <source>Czech</source>
      <translation>Czech</translation>
    </message>
    <message>
      <source>Danish</source>
      <translation>Danish</translation>
    </message>
    <message>
      <source>Dutch</source>
      <translation>Dutch</translation>
    </message>
    <message>
      <source>English</source>
      <translation>English</translation>
    </message>
    <message>
      <source>English (British)</source>
      <translation>English (British)</translation>
    </message>
    <message>
      <source>Esperanto</source>
      <translation>Esperanto</translation>
    </message>
    <message>
      <source>German</source>
      <translation>German</translation>
    </message>
    <message>
      <source>Finnish</source>
      <translation>Finnish</translation>
    </message>
    <message>
      <source>French</source>
      <translation>French</translation>
    </message>
    <message>
      <source>Galician</source>
      <translation>Galician</translation>
    </message>
    <message>
      <source>Greek</source>
      <translation>Greek</translation>
    </message>
    <message>
      <source>Hungarian</source>
      <translation>Hungarian</translation>
    </message>
    <message>
      <source>Indonesian</source>
      <translation>Indonesian</translation>
    </message>
    <message>
      <source>Italian</source>
      <translation>Italian</translation>
    </message>
    <message>
      <source>Korean</source>
      <translation>Korean</translation>
    </message>
    <message>
      <source>Lithuanian</source>
      <translation>Lithuanian</translation>
    </message>
    <message>
      <source>Norwegian (Bokmaal)</source>
      <translation>Norwegian (Bokmaal)</translation>
    </message>
    <message>
      <source>Norwegian (Nnyorsk)</source>
      <translation>Norwegian (Nnyorsk)</translation>
    </message>
    <message>
      <source>Norwegian</source>
      <translation>Norwegian</translation>
    </message>
    <message>
      <source>Polish</source>
      <translation>Polish</translation>
    </message>
    <message>
      <source>Russian</source>
      <translation>Russian</translation>
    </message>
    <message>
      <source>Swedish</source>
      <translation>Swedish</translation>
    </message>
    <message>
      <source>Spanish</source>
      <translation>Spanish</translation>
    </message>
    <message>
      <source>Spanish (Latin)</source>
      <translation>Spanish (Latin)</translation>
    </message>
    <message>
      <source>Slovak</source>
      <translation>Slovak</translation>
    </message>
    <message>
      <source>Slovenian</source>
      <translation>Slovenian</translation>
    </message>
    <message>
      <source>Serbian</source>
      <translation>Serbian</translation>
    </message>
    <message>
      <source>Cannot get font size of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get font size of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot get font of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get font of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot get text size of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get text size of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot get column count of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get column count of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot get line space of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get line space of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot get column gap of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get column gap of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot get text of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get text of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot set text of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set text of non-text frame.</translation>
    </message>
    <message>
      <source>Cannot insert text into non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot insert text into non-text frame.</translation>
    </message>
    <message>
      <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
      <comment>python error</comment>
      <translation>Alignment out of range. Use one of the scribus.ALIGN* constants.</translation>
    </message>
    <message>
      <source>Selection index out of bounds</source>
      <comment>python error</comment>
      <translation>Selection index out of bounds</translation>
    </message>
    <message>
      <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
      <comment>python error</comment>
      <translation>Unit out of range. Use one of the scribus.UNIT_* constants.</translation>
    </message>
    <message>
      <source>Target is not an image frame.</source>
      <comment>python error</comment>
      <translation>Target is not an image frame.</translation>
    </message>
    <message>
      <source>Corner radius must be a positive number.</source>
      <comment>python error</comment>
      <translation>Corner radius must be a positive number.</translation>
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
      <source>Import &amp;OpenOffice.org Draw...</source>
      <translation>Import &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
      <translation>OpenOffice.org Draw (*.sxd);;All Files (*)</translation>
    </message>
    <message>
      <source>OpenOffice.org Writer Documents</source>
      <translation>OpenOffice.org Writer Documents</translation>
    </message>
    <message>
      <source>Color not found - python error</source>
      <comment>python error</comment>
      <translation>Colour not found - python error</translation>
    </message>
    <message>
      <source>Custom (optional) configuration: </source>
      <comment>short words plugin</comment>
      <translation>Custom (optional) configuration: </translation>
    </message>
    <message>
      <source>Standard configuration: </source>
      <comment>short words plugin</comment>
      <translation>Standard configuration: </translation>
    </message>
    <message>
      <source>Short &amp;Words...</source>
      <comment>short words plugin</comment>
      <translation>Short &amp;Words...</translation>
    </message>
    <message>
      <source>Short Words processing. Wait please...</source>
      <comment>short words plugin</comment>
      <translation>Short Words processing. Wait please...</translation>
    </message>
    <message>
      <source>Short Words processing. Done.</source>
      <comment>short words plugin</comment>
      <translation>Short Words processing. Done.</translation>
    </message>
    <message>
      <source>Afrikaans</source>
      <translation>Afrikaans</translation>
    </message>
    <message>
      <source>Turkish</source>
      <translation>Turkish</translation>
    </message>
    <message>
      <source>Ukranian</source>
      <translation>Ukranian</translation>
    </message>
    <message>
      <source>Welsh</source>
      <translation>Welsh</translation>
    </message>
    <message>
      <source>The filename must be a string.</source>
      <comment>python error</comment>
      <translation>The filename must be a string.</translation>
    </message>
    <message>
      <source>Cannot delete image type settings.</source>
      <comment>python error</comment>
      <translation>Cannot delete image type settings.</translation>
    </message>
    <message>
      <source>The image type must be a string.</source>
      <comment>python error</comment>
      <translation>The image type must be a string.</translation>
    </message>
    <message>
      <source>'allTypes' attribute is READ-ONLY</source>
      <comment>python error</comment>
      <translation>'allTypes' attribute is READ-ONLY</translation>
    </message>
    <message>
      <source>Failed to export image</source>
      <comment>python error</comment>
      <translation>Failed to export image</translation>
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
      <translation>Cannot scale by 0%.</translation>
    </message>
    <message>
      <source>Specified item not an image frame.</source>
      <comment>python error</comment>
      <translation>Specified item not an image frame.</translation>
    </message>
    <message>
      <source>Font not found.</source>
      <comment>python error</comment>
      <translation>Font not found.</translation>
    </message>
    <message>
      <source>Cannot render an empty sample.</source>
      <comment>python error</comment>
      <translation>Cannot render an empty sample.</translation>
    </message>
    <message>
      <source>Cannot have an empty layer name.</source>
      <comment>python error</comment>
      <translation>Cannot have an empty layer name.</translation>
    </message>
    <message>
      <source>Layer not found.</source>
      <comment>python error</comment>
      <translation>Layer not found.</translation>
    </message>
    <message>
      <source>Cannot remove the last layer.</source>
      <comment>python error</comment>
      <translation>Cannot remove the last layer.</translation>
    </message>
    <message>
      <source>Cannot create layer without a name.</source>
      <comment>python error</comment>
      <translation>Cannot create layer without a name.</translation>
    </message>
    <message>
      <source>Insert index out of bounds.</source>
      <comment>python error</comment>
      <translation>Insert index out of bounds.</translation>
    </message>
    <message>
      <source>Cannot set text alignment on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set text alignment on a non-text frame.</translation>
    </message>
    <message>
      <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
      <comment>python error</comment>
      <translation>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</translation>
    </message>
    <message>
      <source>Cannot set font size on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set font size on a non-text frame.</translation>
    </message>
    <message>
      <source>Cannot set font on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set font on a non-text frame.</translation>
    </message>
    <message>
      <source>Line space out of bounds, must be >= 0.1.</source>
      <comment>python error</comment>
      <translation>Line space out of bounds, must be >= 0.1.</translation>
    </message>
    <message>
      <source>Cannot set line spacing on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set line spacing on a non-text frame.</translation>
    </message>
    <message>
      <source>Column gap out of bounds, must be positive.</source>
      <comment>python error</comment>
      <translation>Column gap out of bounds, must be positive.</translation>
    </message>
    <message>
      <source>Cannot set column gap on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set column gap on a non-text frame.</translation>
    </message>
    <message>
      <source>Column count out of bounds, must be > 1.</source>
      <comment>python error</comment>
      <translation>Column count out of bounds, must be > 1.</translation>
    </message>
    <message>
      <source>Cannot set number of columns on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set number of columns on a non-text frame.</translation>
    </message>
    <message>
      <source>Cannot select text in a non-text frame</source>
      <comment>python error</comment>
      <translation>Cannot select text in a non-text frame</translation>
    </message>
    <message>
      <source>Cannot delete text from a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot delete text from a non-text frame.</translation>
    </message>
    <message>
      <source>Cannot set text fill on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set text fill on a non-text frame.</translation>
    </message>
    <message>
      <source>Cannot set text stroke on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set text stroke on a non-text frame.</translation>
    </message>
    <message>
      <source>Cannot set text shade on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set text shade on a non-text frame.</translation>
    </message>
    <message>
      <source>Can only link text frames.</source>
      <comment>python error</comment>
      <translation>Can only link text frames.</translation>
    </message>
    <message>
      <source>Target frame must be empty.</source>
      <comment>python error</comment>
      <translation>Target frame must be empty.</translation>
    </message>
    <message>
      <source>Target frame links to another frame.</source>
      <comment>python error</comment>
      <translation>Target frame links to another frame.</translation>
    </message>
    <message>
      <source>Target frame is linked to by another frame.</source>
      <comment>python error</comment>
      <translation>Target frame is linked to by another frame.</translation>
    </message>
    <message>
      <source>Source and target are the same object.</source>
      <comment>python error</comment>
      <translation>Source and target are the same object.</translation>
    </message>
    <message>
      <source>Cannot unlink a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot unlink a non-text frame.</translation>
    </message>
    <message>
      <source>Cannot convert a non-text frame to outlines.</source>
      <comment>python error</comment>
      <translation>Cannot convert a non-text frame to outlines.</translation>
    </message>
    <message>
      <source>Can't set bookmark on a non-text frame</source>
      <comment>python error</comment>
      <translation>Can't set bookmark on a non-text frame</translation>
    </message>
    <message>
      <source>Can't get info from a non-text frame</source>
      <comment>python error</comment>
      <translation>Can't get info from a non-text frame</translation>
    </message>
    <message>
      <source>OpenDocument Text Documents</source>
      <translation>OpenDocument Text Documents</translation>
    </message>
    <message>
      <source>Croatian</source>
      <translation>Croatian</translation>
    </message>
    <message>
      <source>Portuguese</source>
      <translation>Portuguese</translation>
    </message>
    <message>
      <source>Portuguese (BR)</source>
      <translation>Portuguese (BR)</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Page</translation>
    </message>
    <message>
      <source>Master Page </source>
      <translation>Master Page </translation>
    </message>
    <message>
      <source>4A0</source>
      <translation>4A0</translation>
    </message>
    <message>
      <source>2A0</source>
      <translation>2A0</translation>
    </message>
    <message>
      <source>Comm10E</source>
      <translation>Comm10E</translation>
    </message>
    <message>
      <source>DLE</source>
      <translation>DLE</translation>
    </message>
    <message>
      <source>Output stream not writeable</source>
      <translation>Output stream not writeable</translation>
    </message>
    <message>
      <source>Verification of settings failed: %1</source>
      <translation>Verification of settings failed: %1</translation>
    </message>
    <message>
      <source>Unable to read settings XML:</source>
      <translation>Unable to read settings XML:</translation>
    </message>
    <message>
      <source>%1 (line %2 col %3)</source>
      <comment>Load PDF settings</comment>
      <translation>%1 (line %2 col %3)</translation>
    </message>
    <message>
      <source>Unable to read settings XML: %1</source>
      <translation>Unable to read settings XML: %1</translation>
    </message>
    <message>
      <source>null root node</source>
      <comment>Load PDF settings</comment>
      <translation>null root node</translation>
    </message>
    <message>
      <source>&lt;pdfVersion> invalid</source>
      <comment>Load PDF settings</comment>
      <translation>&lt;pdfVersion> invalid</translation>
    </message>
    <message>
      <source>found %1 &lt;%2> nodes, need 1.</source>
      <comment>Load PDF settings</comment>
      <translation>found %1 &lt;%2> nodes, need 1.</translation>
    </message>
    <message>
      <source>unexpected null &lt;%2> node</source>
      <comment>Load PDF settings</comment>
      <translation>unexpected null &lt;%2> node</translation>
    </message>
    <message>
      <source>node &lt;%1> not an element</source>
      <comment>Load PDF settings</comment>
      <translation>node &lt;%1> not an element</translation>
    </message>
    <message>
      <source>element &lt;%1> lacks `value' attribute</source>
      <comment>Load PDF settings</comment>
      <translation>element &lt;%1> lacks `value' attribute</translation>
    </message>
    <message>
      <source>element &lt;%1> value must be `true' or `false'</source>
      <comment>Load PDF settings</comment>
      <translation>element &lt;%1> value must be `true' or `false'</translation>
    </message>
    <message>
      <source>element &lt;lpiSettingsEntry> lacks `name' attribute</source>
      <comment>Load PDF settings</comment>
      <translation>element &lt;lpiSettingsEntry> lacks `name' attribute</translation>
    </message>
    <message>
      <source>Font %1 (found using fontconfig) is broken, discarding it</source>
      <translation>Font %1 (found using fontconfig) is broken, discarding it</translation>
    </message>
    <message>
      <source>Scribus Development Version</source>
      <translation>Scribus Development Version</translation>
    </message>
    <message>
      <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File->Save As. Are you sure you wish to proceed with this operation?</source>
      <translation>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File->Save As. Are you sure you wish to proceed with this operation?</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;Proceed</source>
      <translation>&amp;Proceed</translation>
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
      <source> cm</source>
      <translation> cm</translation>
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
      <source>cm</source>
      <translation>cm</translation>
    </message>
    <message>
      <source>Points (pt)</source>
      <translation>Points (pt)</translation>
    </message>
    <message>
      <source>Millimeters (mm)</source>
      <translation>Millimetres (mm)</translation>
    </message>
    <message>
      <source>Inches (in)</source>
      <translation>Inches (in)</translation>
    </message>
    <message>
      <source>Picas (p)</source>
      <translation>Picas (p)</translation>
    </message>
    <message>
      <source>Centimeters (cm)</source>
      <translation>Centimetres (cm)</translation>
    </message>
    <message>
      <source>File exists</source>
      <translation>File exists</translation>
    </message>
    <message>
      <source>A file named '%1' already exists.
Do you want to replace it with the file you are saving?</source>
      <translation>A file named '%1' already exists.
Do you want to replace it with the file you are saving?</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>&amp;Replace</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>&amp;Color Wheel...</source>
      <translation>&amp;Colour Wheel...</translation>
    </message>
    <message>
      <source>&amp;Fonts Preview...</source>
      <translation>&amp;Fonts Preview...</translation>
    </message>
    <message>
      <source>Document Template: </source>
      <translation>Document Template: </translation>
    </message>
    <message>
      <source>Failed to open document.</source>
      <comment>python error</comment>
      <translation>Failed to open document.</translation>
    </message>
    <message>
      <source>Failed to save document.</source>
      <comment>python error</comment>
      <translation>Failed to save document.</translation>
    </message>
    <message>
      <source>Argument must be page item name, or PyCObject instance</source>
      <translation>Argument must be page item name, or PyCObject instance</translation>
    </message>
    <message>
      <source>Property not found</source>
      <translation>Property not found</translation>
    </message>
    <message>
      <source>Child not found</source>
      <translation>Child not found</translation>
    </message>
    <message>
      <source>Couldn't convert result type '%1'.</source>
      <translation>Couldn't convert result type '%1'.</translation>
    </message>
    <message>
      <source>Property type '%1' not supported</source>
      <translation>Property type '%1' not supported</translation>
    </message>
    <message>
      <source>Couldn't convert '%1' to property type '%2'</source>
      <translation>Couldn't convert '%1' to property type '%2'</translation>
    </message>
    <message>
      <source>Types matched, but setting property failed.</source>
      <translation>Types matched, but setting property failed.</translation>
    </message>
    <message>
      <source>Unable to save pixmap</source>
      <comment>scripter error</comment>
      <translation>Unable to save pixmap</translation>
    </message>
    <message>
      <source>An object with the requested name already exists.</source>
      <comment>python error</comment>
      <translation>An object with the requested name already exists.</translation>
    </message>
    <message>
      <source>Point list must contain at least two points (four values).</source>
      <comment>python error</comment>
      <translation>Point list must contain at least two points (four values).</translation>
    </message>
    <message>
      <source>Point list must contain an even number of values.</source>
      <comment>python error</comment>
      <translation>Point list must contain an even number of values.</translation>
    </message>
    <message>
      <source>Point list must contain at least three points (six values).</source>
      <comment>python error</comment>
      <translation>Point list must contain at least three points (six values).</translation>
    </message>
    <message>
      <source>Point list must contain at least four points (eight values).</source>
      <comment>python error</comment>
      <translation>Point list must contain at least four points (eight values).</translation>
    </message>
    <message>
      <source>Point list must have a multiple of six values.</source>
      <comment>python error</comment>
      <translation>Point list must have a multiple of six values.</translation>
    </message>
    <message>
      <source>Object not found.</source>
      <comment>python error</comment>
      <translation>Object not found.</translation>
    </message>
    <message>
      <source>Style not found.</source>
      <comment>python error</comment>
      <translation>Style not found.</translation>
    </message>
    <message>
      <source>Cannot set style on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot set style on a non-text frame.</translation>
    </message>
    <message>
      <source>Failed to save EPS.</source>
      <comment>python error</comment>
      <translation>Failed to save EPS.</translation>
    </message>
    <message>
      <source>Page number out of range.</source>
      <comment>python error</comment>
      <translation>Page number out of range.</translation>
    </message>
    <message>
      <source>argument is not list: must be list of float values.</source>
      <comment>python error</comment>
      <translation>argument is not list: must be list of float values.</translation>
    </message>
    <message>
      <source>argument contains non-numeric values: must be list of float values.</source>
      <comment>python error</comment>
      <translation>argument contains non-numeric values: must be list of float values.</translation>
    </message>
    <message>
      <source>argument contains no-numeric values: must be list of float values.</source>
      <comment>python error</comment>
      <translation>argument contains no-numeric values: must be list of float values.</translation>
    </message>
    <message>
      <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
      <comment>python error</comment>
      <translation>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</translation>
    </message>
    <message>
      <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
      <comment>python error</comment>
      <translation>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
      <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
      <comment>python error</comment>
      <translation>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
      <source>Line style not found.</source>
      <comment>python error</comment>
      <translation>Line style not found.</translation>
    </message>
    <message>
      <source>Object is not a linked text frame, can't unlink.</source>
      <comment>python error</comment>
      <translation>Object is not a linked text frame, can't unlink.</translation>
    </message>
    <message>
      <source>Object the last frame in a series, can't unlink. Unlink the previous frame instead.</source>
      <comment>python error</comment>
      <translation>Object the last frame in a series, can't unlink. Unlink the previous frame instead.</translation>
    </message>
    <message>
      <source>Only text frames can be checked for overflowing</source>
      <comment>python error</comment>
      <translation>Only text frames can be checked for overflowing</translation>
    </message>
    <message>
      <source>&amp;Execute Script...</source>
      <translation>&amp;Execute Script...</translation>
    </message>
    <message>
      <source>Show &amp;Console</source>
      <translation>Show &amp;Console</translation>
    </message>
    <message>
      <source>&amp;About Script...</source>
      <translation>&amp;About Script...</translation>
    </message>
    <message>
      <source>&amp;Script</source>
      <translation>&amp;Script</translation>
    </message>
    <message>
      <source>&amp;Scribus Scripts</source>
      <translation>&amp;Scribus Scripts</translation>
    </message>
    <message>
      <source>&amp;Recent Scripts</source>
      <translation>&amp;Recent Scripts</translation>
    </message>
    <message>
      <source>About Script</source>
      <translation>About Script</translation>
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
raise a NoValidObjectError if it can't find anything to operate
on.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text color on a graphics frame doesn't make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function's documentation, though as with most Python code this list
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
raise a NoValidObjectError if it can't find anything to operate
on.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text colour on a graphics frame doesn't make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function's documentation, though as with most Python code this list
is not exhaustive due to exceptions from called functions.
</translation>
    </message>
    <message>
      <source>Short Words Manual</source>
      <translation>Short Words Manual</translation>
    </message>
    <message>
      <source>Could not open output file %1</source>
      <translation>Could not open output file %1</translation>
    </message>
    <message>
      <source>Could not open input file %1</source>
      <translation>Could not open input file %1</translation>
    </message>
    <message>
      <source>Invalid argument: </source>
      <translation>Invalid argument: </translation>
    </message>
    <message>
      <source>File %1 does not exist, aborting.</source>
      <translation>File %1 does not exist, aborting.</translation>
    </message>
    <message>
      <source>Usage: scribus [option ... ] [file]</source>
      <translation>Usage: scribus [option ... ] [file]</translation>
    </message>
    <message>
      <source>Options:</source>
      <translation>Options:</translation>
    </message>
    <message>
      <source>Print help (this message) and exit</source>
      <translation>Print help (this message) and exit</translation>
    </message>
    <message>
      <source>Uses xx as shortcut for a language</source>
      <translation>Uses xx as shortcut for a language</translation>
    </message>
    <message>
      <source>Do not show the splashscreen on startup</source>
      <translation>Do not show the splashscreen on startup</translation>
    </message>
    <message>
      <source>Output version information and exit</source>
      <translation>Output version information and exit</translation>
    </message>
    <message>
      <source>To override the default language choice:</source>
      <translation>To override the default language choice:</translation>
    </message>
    <message>
      <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
      <translation>scribus -l xx or scribus --lang xx, where xx is the language of choice.</translation>
    </message>
    <message>
      <source>Scribus Version </source>
      <translation>Scribus Version </translation>
    </message>
    <message>
      <source>Scribus, Open Source Desktop Publishing</source>
      <translation>Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
      <source>---------------------------------------</source>
      <translation>---------------------------------------</translation>
    </message>
    <message>
      <source>Homepage:       http://www.scribus.net </source>
      <translation>Homepage:       http://www.scribus.net </translation>
    </message>
    <message>
      <source>Documentation:  http://docs.scribus.net</source>
      <translation>Documentation:  http://docs.scribus.net</translation>
    </message>
    <message>
      <source>Issues:         http://bugs.scribus.net</source>
      <translation>Issues:         http://bugs.scribus.net</translation>
    </message>
    <message>
      <source>Installed interface languages for Scribus are as follows:</source>
      <translation>Installed interface languages for Scribus are as follows:</translation>
    </message>
    <message>
      <source>Freetype2 library not available</source>
      <translation>Freetype2 library not available</translation>
    </message>
    <message>
      <source>Font %1 is broken, no embedding</source>
      <translation>Font %1 is broken, no embedding</translation>
    </message>
    <message>
      <source>Font %1 is broken (read stream), no embedding</source>
      <translation>Font %1 is broken (read stream), no embedding</translation>
    </message>
    <message>
      <source>Font %1 is broken (FreeType2), discarding it</source>
      <translation>Font %1 is broken (FreeType2), discarding it</translation>
    </message>
    <message>
      <source>Font %1 is broken (no Face), discarding it</source>
      <translation>Font %1 is broken (no Face), discarding it</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2 (charcode %3)</source>
      <translation>Font %1 has broken glyph %2 (charcode %3)</translation>
    </message>
    <message>
      <source>Font %1 is broken and will be discarded</source>
      <translation>Font %1 is broken and will be discarded</translation>
    </message>
    <message>
      <source>Font %1 cannot be read, no embedding</source>
      <translation>Font %1 cannot be read, no embedding</translation>
    </message>
    <message>
      <source>Failed to load font %1 - font type unknown</source>
      <translation>Failed to load font %1 - font type unknown</translation>
    </message>
    <message>
      <source>Font %1 loaded from %2(%3)</source>
      <translation>Font %1 loaded from %2(%3)</translation>
    </message>
    <message>
      <source>Font %1(%2) is duplicate of %3</source>
      <translation>Font %1(%2) is duplicate of %3</translation>
    </message>
    <message>
      <source>Loading font %1 (found using fontconfig)</source>
      <translation>Loading font %1 (found using fontconfig)</translation>
    </message>
    <message>
      <source>Failed to load a font - freetype2 couldn't find the font file</source>
      <translation>Failed to load a font - freetype2 couldn't find the font file</translation>
    </message>
    <message>
      <source>Font %1 is broken (FreeType), discarding it</source>
      <translation>Font %1 is broken (FreeType), discarding it</translation>
    </message>
    <message>
      <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
      <translation>Font %1  has invalid glyph %2 (charcode %3), discarding it</translation>
    </message>
    <message>
      <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
      <translation>extracting face %1 from font %2 (offset=%3, nTables=%4)</translation>
    </message>
    <message>
      <source>memcpy header: %1 %2 %3</source>
      <translation>memcpy header: %1 %2 %3</translation>
    </message>
    <message>
      <source>table '%1'</source>
      <translation>table '%1'</translation>
    </message>
    <message>
      <source>memcpy table: %1 %2 %3</source>
      <translation>memcpy table: %1 %2 %3</translation>
    </message>
    <message>
      <source>memcpy offset: %1 %2 %3</source>
      <translation>memcpy offset: %1 %2 %3</translation>
    </message>
    <message>
      <source>List the currently installed interface languages</source>
      <translation>List the currently installed interface languages</translation>
    </message>
    <message>
      <source>Show information on the console when fonts are being loaded</source>
      <translation>Show information on the console when fonts are being loaded</translation>
    </message>
    <message>
      <source>Scribus Crash</source>
      <translation>Scribus Crash</translation>
    </message>
    <message>
      <source>Scribus crashes due to Signal #%1</source>
      <translation>Scribus crashes due to Signal #%1</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Can't group less than two items</source>
      <comment>python error</comment>
      <translation>Can't group less than two items</translation>
    </message>
    <message>
      <source>Need selection or argument list of items to group</source>
      <comment>python error</comment>
      <translation>Need selection or argument list of items to group</translation>
    </message>
    <message>
      <source>Cannot group less than two items</source>
      <comment>python error</comment>
      <translation>Cannot group less than two items</translation>
    </message>
    <message>
      <source> c</source>
      <translation> c</translation>
    </message>
    <message>
      <source>c</source>
      <translation>c</translation>
    </message>
    <message>
      <source>Cicero (c)</source>
      <translation>Cicero (c)</translation>
    </message>
    <message>
      <source>The filename should not be empty string.</source>
      <comment>python error</comment>
      <translation>The filename should not be empty string.</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Wiki:           http://wiki.scribus.net</source>
      <translation>Wiki:           http://wiki.scribus.net</translation>
    </message>
  </context>
  <context>
    <name>QTextEdit</name>
    <message>
      <source>Clear</source>
      <translation>Clear</translation>
    </message>
    <message>
      <source>Select All</source>
      <translation>Select All</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Undo</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Redo</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>Cu&amp;t</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>&amp;Paste</translation>
    </message>
  </context>
  <context>
    <name>QTitleBar</name>
    <message>
      <source>System Menu</source>
      <translation>System Menu</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Shade</translation>
    </message>
    <message>
      <source>Unshade</source>
      <translation>Unshade</translation>
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
      <translation>Close</translation>
    </message>
  </context>
  <context>
    <name>QWorkspace</name>
    <message>
      <source>&amp;Restore</source>
      <translation>&amp;Restore</translation>
    </message>
    <message>
      <source>&amp;Move</source>
      <translation>&amp;Move</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>&amp;Size</translation>
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
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>Stay on &amp;Top</source>
      <translation>Stay on &amp;Top</translation>
    </message>
    <message>
      <source>Minimize</source>
      <translation>Minimise</translation>
    </message>
    <message>
      <source>Restore Down</source>
      <translation>Restore Down</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Close</translation>
    </message>
    <message>
      <source>Sh&amp;ade</source>
      <translation>Sh&amp;ade</translation>
    </message>
    <message>
      <source>%1 - [%2]</source>
      <translation>%1 - [%2]</translation>
    </message>
    <message>
      <source>&amp;Unshade</source>
      <translation>&amp;Unshade</translation>
    </message>
  </context>
  <context>
    <name>Query</name>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>ReformDoc</name>
    <message>
      <source>Document Setup</source>
      <translation>Document Setup</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Margin Guides</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Top:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Left:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Bottom:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Right:</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
    <message>
      <source>F&amp;irst Page Number:</source>
      <translation>F&amp;irst Page Number:</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>Orie&amp;ntation:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Width:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Height:</translation>
    </message>
    <message>
      <source>&amp;Unit:</source>
      <translation>&amp;Unit:</translation>
    </message>
    <message>
      <source>Autosave</source>
      <translation>Autosave</translation>
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
      <source>Document</source>
      <translation>Document</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>Guides</translation>
    </message>
    <message>
      <source>Page Display</source>
      <translation>Page Display</translation>
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
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Show Pictures</source>
      <translation>Show Pictures</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation>Show Text Chains</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation>Show Frames</translation>
    </message>
    <message>
      <source>Display</source>
      <translation>Display</translation>
    </message>
    <message>
      <source>Typography</source>
      <translation>Typography</translation>
    </message>
    <message>
      <source>Tools</source>
      <translation>Tools</translation>
    </message>
    <message>
      <source>Hyphenator</source>
      <translation>Hyphenator</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>Fonts</translation>
    </message>
    <message>
      <source>PDF Export</source>
      <translation>PDF Export</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>Document Item Attributes</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Table of Contents and Indexes</translation>
    </message>
    <message>
      <source>Color Management</source>
      <translation>Colour Management</translation>
    </message>
    <message>
      <source>Turns the of linked frames on or off</source>
      <translation>Turns the of linked frames on or off</translation>
    </message>
    <message>
      <source>Turns the display of frames on or off</source>
      <translation>Turns the display of frames on or off</translation>
    </message>
    <message>
      <source>Turns the display of pictures on or off</source>
      <translation>Turns the display of pictures on or off</translation>
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
      <translation>Document Information</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation>Show Text Control Characters</translation>
    </message>
    <message>
      <source>Display non-printing characters such as paragraph markers in text frames</source>
      <translation>Display non-printing characters such as paragraph markers in text frames</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Preflight Verifier</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>Apply size settings to all Pages</source>
      <translation>Apply size settings to all Pages</translation>
    </message>
    <message>
      <source>Apply margin settings to all Pages</source>
      <translation>Apply margin settings to all Pages</translation>
    </message>
    <message>
      <source>Rulers relative to Page</source>
      <translation>Rulers relative to Page</translation>
    </message>
    <message>
      <source>Minimum Scratch Space</source>
      <translation>Minimum Scratch Space</translation>
    </message>
    <message>
      <source>Gaps between Pages</source>
      <translation>Gaps between Pages</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation>Horizontal:</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation>Vertical:</translation>
    </message>
  </context>
  <context>
    <name>RunScriptDialog</name>
    <message>
      <source>Python Scripts (*.py);; All Files (*)</source>
      <translation>Python Scripts (*.py);; All Files (*)</translation>
    </message>
    <message>
      <source>Run as Extension Script</source>
      <comment>run script dialog</comment>
      <translation>Run as Extension Script</translation>
    </message>
  </context>
  <context>
    <name>SToolBAlign</name>
    <message>
      <source>Style of current paragraph</source>
      <translation>Style of current paragraph</translation>
    </message>
    <message>
      <source>Style Settings</source>
      <translation>Style Settings</translation>
    </message>
  </context>
  <context>
    <name>SToolBColorF</name>
    <message>
      <source>None</source>
      <translation>None</translation>
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
      <translation>None</translation>
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
      <translation> pt</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Font of selected text</source>
      <translation>Font of selected text</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Font Size</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>Scaling width of characters</translation>
    </message>
    <message>
      <source>Font Settings</source>
      <translation>Font Settings</translation>
    </message>
    <message>
      <source>Scaling height of characters</source>
      <translation>Scaling height of characters</translation>
    </message>
  </context>
  <context>
    <name>SToolBStyle</name>
    <message>
      <source>Character Settings</source>
      <translation>Character Settings</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>Manual Tracking</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
  </context>
  <context>
    <name>ScriXmlDoc</name>
    <message>
      <source>Copy #%1 of </source>
      <translation>Copy #%1 of </translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Background</translation>
    </message>
  </context>
  <context>
    <name>ScribusApp</name>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>Searching for Fonts</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>Fatal Error</translation>
    </message>
    <message>
      <source>Reading Preferences</source>
      <translation>Reading Preferences</translation>
    </message>
    <message>
      <source>Setting up Shortcuts</source>
      <translation>Setting up Shortcuts</translation>
    </message>
    <message>
      <source>Reading Scrapbook</source>
      <translation>Reading Scrapbook</translation>
    </message>
    <message>
      <source>Initializing Plugins</source>
      <translation>Initialising Plugins</translation>
    </message>
    <message>
      <source>About Qt</source>
      <translation>About Qt</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
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
      <translation>Ready</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Size</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Shade</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Documents (*.sla *.scd);;All Files (*)</translation>
    </message>
    <message>
      <source>Loading...</source>
      <translation>Loading...</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>All Supported Formats</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>Text Files (*.txt);;All Files(*)</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
      <translation>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Saving...</source>
      <translation>Saving...</translation>
    </message>
    <message>
      <source>Printing...</source>
      <translation>Printing...</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Document</translation>
    </message>
    <message>
      <source>Printing failed!</source>
      <translation>Printing failed!</translation>
    </message>
    <message>
      <source>Scribus Manual</source>
      <translation>Scribus Manual</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>Adjusting Colours</translation>
    </message>
    <message>
      <source>English</source>
      <translation>English</translation>
    </message>
    <message>
      <source>German</source>
      <translation>German</translation>
    </message>
    <message>
      <source>Spanish</source>
      <translation>Spanish</translation>
    </message>
    <message>
      <source>Italian</source>
      <translation>Italian</translation>
    </message>
    <message>
      <source>French</source>
      <translation>French</translation>
    </message>
    <message>
      <source>Russian</source>
      <translation>Russian</translation>
    </message>
    <message>
      <source>Danish</source>
      <translation>Danish</translation>
    </message>
    <message>
      <source>Slovak</source>
      <translation>Slovak</translation>
    </message>
    <message>
      <source>Hungarian</source>
      <translation>Hungarian</translation>
    </message>
    <message>
      <source>Czech</source>
      <translation>Czech</translation>
    </message>
    <message>
      <source>Dutch</source>
      <translation>Dutch</translation>
    </message>
    <message>
      <source>Portuguese</source>
      <translation>Portuguese</translation>
    </message>
    <message>
      <source>Ukrainian</source>
      <translation>Ukrainian</translation>
    </message>
    <message>
      <source>Polish</source>
      <translation>Polish</translation>
    </message>
    <message>
      <source>Greek</source>
      <translation>Greek</translation>
    </message>
    <message>
      <source>Catalan</source>
      <translation>Catalan</translation>
    </message>
    <message>
      <source>Finnish</source>
      <translation>Finnish</translation>
    </message>
    <message>
      <source>Irish</source>
      <translation>Irish</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>&amp;Item</source>
      <translation>&amp;Item</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>&amp;Page</translation>
    </message>
    <message>
      <source>&amp;View</source>
      <translation>&amp;View</translation>
    </message>
    <message>
      <source>&amp;Tools</source>
      <translation>&amp;Tools</translation>
    </message>
    <message>
      <source>E&amp;xtras</source>
      <translation>E&amp;xtras</translation>
    </message>
    <message>
      <source>&amp;Windows</source>
      <translation>&amp;Windows</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation>&amp;Help</translation>
    </message>
    <message>
      <source>Some Objects are locked.</source>
      <translation>Some Objects are locked.</translation>
    </message>
    <message>
      <source>Lithuanian</source>
      <translation>Lithuanian</translation>
    </message>
    <message>
      <source>Swedish</source>
      <translation>Swedish</translation>
    </message>
    <message>
      <source>Slovenian</source>
      <translation>Slovenian</translation>
    </message>
    <message>
      <source>Open &amp;Recent</source>
      <translation>Open &amp;Recent</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>&amp;Import</translation>
    </message>
    <message>
      <source>&amp;Export</source>
      <translation>&amp;Export</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Shape</translation>
    </message>
    <message>
      <source>St&amp;yle</source>
      <translation>St&amp;yle</translation>
    </message>
    <message>
      <source>&amp;Cascade</source>
      <translation>&amp;Cascade</translation>
    </message>
    <message>
      <source>&amp;Tile</source>
      <translation>&amp;Tile</translation>
    </message>
    <message>
      <source>&amp;Color</source>
      <translation>&amp;Colour</translation>
    </message>
    <message>
      <source>&amp;Font</source>
      <translation>&amp;Font</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>&amp;Size</translation>
    </message>
    <message>
      <source>&amp;Effects</source>
      <translation>&amp;Effects</translation>
    </message>
    <message>
      <source>&amp;Alignment</source>
      <translation>&amp;Alignment</translation>
    </message>
    <message>
      <source>&amp;Shade</source>
      <translation>&amp;Shade</translation>
    </message>
    <message>
      <source>Importing Pages...</source>
      <translation>Importing Pages...</translation>
    </message>
    <message>
      <source>Import Page(s)</source>
      <translation>Import Page(s)</translation>
    </message>
    <message>
      <source>&lt;p>You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p>Choose one of the following:&lt;br>&lt;ul>&lt;li>&lt;b>Create&lt;/b> missing pages&lt;/li>&lt;li>&lt;b>Import&lt;/b> pages until the last page&lt;/li>&lt;li>&lt;b>Cancel&lt;/b>&lt;/li>&lt;/ul>&lt;br></source>
      <translation>&lt;p>You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p>Choose one of the following:&lt;br>&lt;ul>&lt;li>&lt;b>Create&lt;/b> missing pages&lt;/li>&lt;li>&lt;b>Import&lt;/b> pages until the last page&lt;/li>&lt;li>&lt;b>Cancel&lt;/b>&lt;/li>&lt;/ul>&lt;br></translation>
    </message>
    <message>
      <source>Create</source>
      <translation>Create</translation>
    </message>
    <message>
      <source>Import</source>
      <translation>Import</translation>
    </message>
    <message>
      <source>Import done</source>
      <translation>Import done</translation>
    </message>
    <message>
      <source>Found nothing to import</source>
      <translation>Found nothing to import</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>&amp;Shade:</translation>
    </message>
    <message>
      <source>Afrikaans</source>
      <translation>Afrikaans</translation>
    </message>
    <message>
      <source>Font System Initialized</source>
      <translation>Font System Initialised</translation>
    </message>
    <message>
      <source>Portuguese (BR)</source>
      <translation>Portuguese (BR)</translation>
    </message>
    <message>
      <source>Croatian</source>
      <translation>Croatian</translation>
    </message>
    <message>
      <source>Reading ICC Profiles</source>
      <translation>Reading ICC Profiles</translation>
    </message>
    <message>
      <source>Initializing Hyphenator</source>
      <translation>Initialising Hyphenator</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>Edit</translation>
    </message>
    <message>
      <source>There are no fonts found on your system.</source>
      <translation>There are no fonts found on your system.</translation>
    </message>
    <message>
      <source>Exiting now.</source>
      <translation>Exiting now.</translation>
    </message>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
    </message>
    <message>
      <source>PDF 1.3</source>
      <translation>PDF 1.3</translation>
    </message>
    <message>
      <source>PDF 1.4</source>
      <translation>PDF 1.4</translation>
    </message>
    <message>
      <source>PDF/X-3</source>
      <translation>PDF/X-3</translation>
    </message>
    <message>
      <source>&amp;Level</source>
      <translation>&amp;Level</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>&amp;PDF Options</translation>
    </message>
    <message>
      <source>C&amp;onvert To</source>
      <translation>C&amp;onvert To</translation>
    </message>
    <message>
      <source>I&amp;nsert</source>
      <translation>I&amp;nsert</translation>
    </message>
    <message>
      <source>Character</source>
      <translation>Character</translation>
    </message>
    <message>
      <source>Quote</source>
      <translation>Quote</translation>
    </message>
    <message>
      <source>Space</source>
      <translation>Space</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
      <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;</source>
      <translation>Documents (*.sla *.scd);;</translation>
    </message>
    <message>
      <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
      <translation>Postscript Files (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
      <source>SVG Images (*.svg *.svgz);;</source>
      <translation>SVG Images (*.svg *.svgz);;</translation>
    </message>
    <message>
      <source>SVG Images (*.svg);;</source>
      <translation>SVG Images (*.svg);;</translation>
    </message>
    <message>
      <source>File %1 
is not in an acceptable format</source>
      <translation>File %1 
is not in an acceptable format</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Some fonts used by this document have been substituted:</source>
      <translation>Some fonts used by this document have been substituted:</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation> was replaced by: </translation>
    </message>
    <message>
      <source>Some ICC profiles used by this document are not installed:</source>
      <translation>Some ICC profiles used by this document are not installed:</translation>
    </message>
    <message>
      <source>(converted)</source>
      <translation>(converted)</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>Cannot write the file: 
%1</translation>
    </message>
    <message>
      <source>Save As</source>
      <translation>Save As</translation>
    </message>
    <message>
      <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
      <translation>Detected some Errors.
Consider using the Preflight Checker to correct them</translation>
    </message>
    <message>
      <source>Abort</source>
      <translation>Abort</translation>
    </message>
    <message>
      <source>Ignore</source>
      <translation>Ignore</translation>
    </message>
    <message>
      <source>The following programs are missing:</source>
      <translation>The following programs are missing:</translation>
    </message>
    <message>
      <source>EPS Files (*.eps);;All Files (*)</source>
      <translation>EPS Files (*.eps);;All Files (*)</translation>
    </message>
    <message>
      <source>Detected some Errors.
Consider using the Preflight Verifier to correct them</source>
      <translation>Detected some Errors.
Consider using the Preflight Verifier to correct them</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;Lock All</source>
      <translation>&amp;Lock All</translation>
    </message>
    <message>
      <source>&amp;Unlock All</source>
      <translation>&amp;Unlock All</translation>
    </message>
    <message>
      <source>Bulgarian</source>
      <translation>Bulgarian</translation>
    </message>
    <message>
      <source>The program</source>
      <translation>The program</translation>
    </message>
    <message>
      <source>is already running!</source>
      <translation>is already running!</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>Information</translation>
    </message>
    <message>
      <source>is missing!</source>
      <translation>is missing!</translation>
    </message>
    <message>
      <source>Ghostscript : You cannot use EPS images or Print Preview</source>
      <translation>Ghostscript : You cannot use EPS images or Print Preview</translation>
    </message>
    <message>
      <source>The selected color does not exist in the document's color set. Please enter a name for this new color.</source>
      <translation>The selected colour does not exist in the document's colour set. Please enter a name for this new colour.</translation>
    </message>
    <message>
      <source>Color Not Found</source>
      <translation>Colour Not Found</translation>
    </message>
    <message>
      <source>The name you have selected already exists. Please enter a different name for this new color.</source>
      <translation>The name you have selected already exists. Please enter a different name for this new colour.</translation>
    </message>
    <message>
      <source>Initializing Story Editor</source>
      <translation>Initialising Story Editor</translation>
    </message>
    <message>
      <source>Cannot Cut In-Use Item</source>
      <translation>Cannot Cut In-Use Item</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
      <translation>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</translation>
    </message>
    <message>
      <source>Send to Layer</source>
      <translation>Send to Layer</translation>
    </message>
    <message>
      <source>Previe&amp;w Settings</source>
      <translation>Previe&amp;w Settings</translation>
    </message>
    <message>
      <source>Initializing Keyboard Shortcuts</source>
      <translation>Initialising Keyboard Shortcuts</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw (*.sxd);;</source>
      <translation>OpenOffice.org Draw (*.sxd);;</translation>
    </message>
  </context>
  <context>
    <name>ScribusDoc</name>
    <message>
      <source>New Layer</source>
      <translation>New Layer</translation>
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
      <translation>Layer</translation>
    </message>
    <message>
      <source>Copy Here</source>
      <translation>Copy Here</translation>
    </message>
    <message>
      <source>Move Here</source>
      <translation>Move Here</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>&amp;Paste</translation>
    </message>
    <message>
      <source>Picture</source>
      <translation>Picture</translation>
    </message>
    <message>
      <source>File: </source>
      <translation>File </translation>
    </message>
    <message>
      <source>Original PPI: </source>
      <translation>Original PPI: </translation>
    </message>
    <message>
      <source>Actual PPI: </source>
      <translation>Actual PPI: </translation>
    </message>
    <message>
      <source>Linked Text</source>
      <translation>Linked Text</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation>Text Frame</translation>
    </message>
    <message>
      <source>Text on a Path</source>
      <translation>Text on a Path</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>Paragraphs: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>Words: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>Chars: </translation>
    </message>
    <message>
      <source>Print: </source>
      <translation>Print: </translation>
    </message>
    <message>
      <source>Enabled</source>
      <translation>Enabled</translation>
    </message>
    <message>
      <source>Disabled</source>
      <translation>Disabled</translation>
    </message>
    <message>
      <source>In&amp;fo</source>
      <translation>In&amp;fo</translation>
    </message>
    <message>
      <source>Preview Settings</source>
      <translation>Preview Settings</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>&amp;PDF Options</translation>
    </message>
    <message>
      <source>Send to La&amp;yer</source>
      <translation>Send to La&amp;yer</translation>
    </message>
    <message>
      <source>Le&amp;vel</source>
      <translation>Le&amp;vel</translation>
    </message>
    <message>
      <source>Conve&amp;rt to</source>
      <translation>Conve&amp;rt to</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Do you really want to clear all your Text?</source>
      <translation>Do you really want to clear all your Text?</translation>
    </message>
    <message>
      <source>Cannot Delete In-Use Item</source>
      <translation>Cannot Delete In-Use Item</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
      <translation>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</translation>
    </message>
    <message>
      <source>Unit</source>
      <translation>Unit</translation>
    </message>
  </context>
  <context>
    <name>ScribusWin</name>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Document:</source>
      <translation>Document:</translation>
    </message>
    <message>
      <source>has been changed since the last save.</source>
      <translation>has been changed since the last save.</translation>
    </message>
    <message>
      <source>&amp;Leave Anyway</source>
      <translation>&amp;Leave Anyway</translation>
    </message>
    <message>
      <source>C&amp;lose Anyway</source>
      <translation>C&amp;lose Anyway</translation>
    </message>
    <message>
      <source>&amp;Save Now</source>
      <translation>&amp;Save Now</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>ScripterCore</name>
    <message>
      <source>Script error</source>
      <translation>Script error</translation>
    </message>
    <message>
      <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;>bugs.scribus.net&lt;/a> please.</source>
      <translation>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;>bugs.scribus.net&lt;/a> please.</translation>
    </message>
    <message>
      <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
      <translation>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</translation>
    </message>
    <message>
      <source>Examine Script</source>
      <translation>Examine Script</translation>
    </message>
    <message>
      <source>Python Scripts (*.py)</source>
      <translation>Python Scripts (*.py)</translation>
    </message>
    <message>
      <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
      <translation>There was an internal error while trying the command you entered. Details were printed to stderr. </translation>
    </message>
    <message>
      <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
      <translation>Setting up the Python plugin failed. Error details were printed to stderr. </translation>
    </message>
  </context>
  <context>
    <name>SeList</name>
    <message>
      <source>Show Page Previews</source>
      <translation>Show Page Previews</translation>
    </message>
  </context>
  <context>
    <name>SeView</name>
    <message>
      <source>Show Master Page Names</source>
      <translation>Show Master Page Names</translation>
    </message>
  </context>
  <context>
    <name>SearchReplace</name>
    <message>
      <source>Search/Replace</source>
      <translation>Search/Replace</translation>
    </message>
    <message>
      <source>Search for:</source>
      <translation>Search for:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Paragraph Style</source>
      <translation>Paragraph Style</translation>
    </message>
    <message>
      <source>Font</source>
      <translation>Font</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Font Size</translation>
    </message>
    <message>
      <source>Font Effects</source>
      <translation>Font Effects</translation>
    </message>
    <message>
      <source>Fill Color</source>
      <translation>Fill Colour</translation>
    </message>
    <message>
      <source>Fill Shade</source>
      <translation>Fill Shade</translation>
    </message>
    <message>
      <source>Stroke Color</source>
      <translation>Stroke Colour</translation>
    </message>
    <message>
      <source>Stroke Shade</source>
      <translation>Stroke Shade</translation>
    </message>
    <message>
      <source>Left</source>
      <translation>Left</translation>
    </message>
    <message>
      <source>Center</source>
      <translation>Centre</translation>
    </message>
    <message>
      <source>Right</source>
      <translation>Right</translation>
    </message>
    <message>
      <source>Block</source>
      <translation>Block</translation>
    </message>
    <message>
      <source>Forced</source>
      <translation>Forced</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Replace with:</source>
      <translation>Replace with:</translation>
    </message>
    <message>
      <source>Search finished</source>
      <translation>Search finished</translation>
    </message>
    <message>
      <source>&amp;Whole Word</source>
      <translation>&amp;Whole Word</translation>
    </message>
    <message>
      <source>&amp;Ignore Case</source>
      <translation>&amp;Ignore Case</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>&amp;Replace</translation>
    </message>
    <message>
      <source>Replace &amp;All</source>
      <translation>Replace &amp;All</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Close</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
  </context>
  <context>
    <name>SeitenPal</name>
    <message>
      <source>Arrange Pages</source>
      <translation>Arrange Pages</translation>
    </message>
    <message>
      <source>Document Pages:</source>
      <translation>Document Pages:</translation>
    </message>
    <message>
      <source>Facing Pages</source>
      <translation>Facing Pages</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Drag pages or master pages onto the trashbin to delete them</source>
      <translation>Drag pages or master pages onto the trashbin to delete them</translation>
    </message>
    <message>
      <source>Previews all the pages of your document</source>
      <translation>Previews all the pages of your document</translation>
    </message>
    <message>
      <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
      <translation>Here are all your master pages. To create a new page, drag a master page to the page view below</translation>
    </message>
    <message>
      <source>Available Master Pages:</source>
      <translation>Available Master Pages:</translation>
    </message>
    <message>
      <source>Left Page First</source>
      <translation>Left Page First</translation>
    </message>
  </context>
  <context>
    <name>SelectFields</name>
    <message>
      <source>Select Fields</source>
      <translation>Select Fields</translation>
    </message>
    <message>
      <source>Available Fields</source>
      <translation>Available Fields</translation>
    </message>
    <message>
      <source>Selected Fields</source>
      <translation>Selected Fields</translation>
    </message>
    <message>
      <source>&amp;>></source>
      <translation>&amp;>></translation>
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
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>ShadeButton</name>
    <message>
      <source>Other...</source>
      <translation>Other...</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Shade</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>&amp;Shade:</translation>
    </message>
  </context>
  <context>
    <name>ShadowValues</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>X-Offset</source>
      <translation>X-Offset (ENGB)</translation>
    </message>
    <message>
      <source>Y-Offset</source>
      <translation>Y-Offset</translation>
    </message>
  </context>
  <context>
    <name>SideBar</name>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
    <message>
      <source>Edit Styles...</source>
      <translation>Edit Styles...</translation>
    </message>
  </context>
  <context>
    <name>Spalette</name>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
  </context>
  <context>
    <name>StilFormate</name>
    <message>
      <source>Edit Styles</source>
      <translation>Edit Styles</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Copy of %1</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>New Style</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>No</source>
      <translation>No</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Yes</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Documents (*.sla *.scd);;All Files (*)</translation>
    </message>
    <message>
      <source>&amp;Append</source>
      <translation>&amp;Append</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>D&amp;uplicate</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Do you really want to delete this Style?</source>
      <translation>Do you really want to delete this Style?</translation>
    </message>
  </context>
  <context>
    <name>StoryEditor</name>
    <message>
      <source>Story Editor</source>
      <translation>Story Editor</translation>
    </message>
    <message>
      <source>Current Paragraph:</source>
      <translation>Current Paragraph:</translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>Words: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>Chars: </translation>
    </message>
    <message>
      <source>Totals:</source>
      <translation>Totals:</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>Paragraphs: </translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>Text Files (*.txt);;All Files(*)</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>Do you want to save your changes?</source>
      <translation>Do you want to save your changes?</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Reload Text from Frame</source>
      <translation>&amp;Reload Text from Frame</translation>
    </message>
    <message>
      <source>&amp;Save to File...</source>
      <translation>&amp;Save to File...</translation>
    </message>
    <message>
      <source>&amp;Load from File...</source>
      <translation>&amp;Load from File...</translation>
    </message>
    <message>
      <source>Save &amp;Document</source>
      <translation>Save &amp;Document</translation>
    </message>
    <message>
      <source>&amp;Update Text Frame and Exit</source>
      <translation>&amp;Update Text Frame and Exit</translation>
    </message>
    <message>
      <source>&amp;Exit Without Updating Text Frame</source>
      <translation>&amp;Exit Without Updating Text Frame</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>Cu&amp;t</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Copy</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>&amp;Paste</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>&amp;Update Text Frame</source>
      <translation>&amp;Update Text Frame</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>Select &amp;All</source>
      <translation>Select &amp;All</translation>
    </message>
    <message>
      <source>&amp;Edit Styles...</source>
      <translation>&amp;Edit Styles...</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Load Text from File</source>
      <translation>Load Text from File</translation>
    </message>
    <message>
      <source>Save Text to File</source>
      <translation>Save Text to File</translation>
    </message>
    <message>
      <source>Update Text Frame and Exit</source>
      <translation>Update Text Frame and Exit</translation>
    </message>
    <message>
      <source>Exit Without Updating Text Frame</source>
      <translation>Exit Without Updating Text Frame</translation>
    </message>
    <message>
      <source>Reload Text from Frame</source>
      <translation>Reload Text from Frame</translation>
    </message>
    <message>
      <source>Update Text Frame</source>
      <translation>Update Text Frame</translation>
    </message>
    <message>
      <source>&amp;Search/Replace...</source>
      <translation>&amp;Search/Replace...</translation>
    </message>
    <message>
      <source>&amp;Fonts Preview...</source>
      <translation>&amp;Fonts Preview...</translation>
    </message>
    <message>
      <source>&amp;Background...</source>
      <translation>&amp;Background...</translation>
    </message>
    <message>
      <source>&amp;Display Font...</source>
      <translation>&amp;Display Font...</translation>
    </message>
    <message>
      <source>&amp;Settings</source>
      <translation>&amp;Settings</translation>
    </message>
    <message>
      <source>Search/Replace</source>
      <translation>Search/Replace</translation>
    </message>
    <message>
      <source>&amp;Smart text selection</source>
      <translation>&amp;Smart text selection</translation>
    </message>
    <message>
      <source>&amp;Insert Glyph...</source>
      <translation>&amp;Insert Glyph...</translation>
    </message>
    <message>
      <source>Clear All Text</source>
      <translation>Clear All Text</translation>
    </message>
    <message>
      <source>Story Editor - %1</source>
      <translation>Story Editor - %1</translation>
    </message>
    <message>
      <source>Do you really want to lose all your changes?</source>
      <translation>Do you really want to lose all your changes?</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>Do you really want to clear all your text?</translation>
    </message>
  </context>
  <context>
    <name>StrikeValues</name>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Displacement</source>
      <translation>Displacement</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>Linewidth</translation>
    </message>
  </context>
  <context>
    <name>StyleSelect</name>
    <message>
      <source>Underline</source>
      <translation>Underline</translation>
    </message>
    <message>
      <source>Small Caps</source>
      <translation>Small Caps</translation>
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
      <translation>Strike Out</translation>
    </message>
    <message>
      <source>All Caps</source>
      <translation>All Caps</translation>
    </message>
    <message>
      <source>Outline</source>
      <translation>Outline</translation>
    </message>
    <message>
      <source>Shadow</source>
      <translation>Shadow</translation>
    </message>
    <message>
      <source>Underline Words Only</source>
      <translation>Underline Words Only</translation>
    </message>
  </context>
  <context>
    <name>SxwDialog</name>
    <message>
      <source>Use document name as a prefix for paragraph styles</source>
      <translation>Use document name as a prefix for paragraph styles</translation>
    </message>
    <message>
      <source>Do not ask again</source>
      <translation>Do not ask again</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>OpenOffice.org Writer Importer Options</source>
      <translation>OpenOffice.org Writer Importer Options</translation>
    </message>
    <message>
      <source>Enabling this will overwrite existing styles in the current Scribus document</source>
      <translation>Enabling this will overwrite existing styles in the current Scribus document</translation>
    </message>
    <message>
      <source>Merge Paragraph Styles</source>
      <translation>Merge Paragraph Styles</translation>
    </message>
    <message>
      <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</source>
      <translation>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</translation>
    </message>
    <message>
      <source>Prepend the document name to the paragraph style name in Scribus.</source>
      <translation>Prepend the document name to the paragraph style name in Scribus.</translation>
    </message>
    <message>
      <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
      <translation>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</translation>
    </message>
    <message>
      <source>Overwrite Paragraph Styles</source>
      <translation>Overwrite Paragraph Styles</translation>
    </message>
  </context>
  <context>
    <name>TOCIndexPrefs</name>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Table of Contents and Indexes</translation>
    </message>
    <message>
      <source>Table Of Contents</source>
      <translation>Table Of Contents</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Add</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Item Attribute Name:</source>
      <translation>Item Attribute Name:</translation>
    </message>
    <message>
      <source>Destination Frame:</source>
      <translation>Destination Frame:</translation>
    </message>
    <message>
      <source>At the beginning</source>
      <translation>At the beginning</translation>
    </message>
    <message>
      <source>At the end</source>
      <translation>At the end</translation>
    </message>
    <message>
      <source>Not Shown</source>
      <translation>Not Shown</translation>
    </message>
    <message>
      <source>The frame the table of contents will be placed into</source>
      <translation>The frame the table of contents will be placed into</translation>
    </message>
    <message>
      <source>Page Numbers Placed:</source>
      <translation>Page Numbers Placed:</translation>
    </message>
    <message>
      <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
      <translation>The Item Attribute that will be set on frames used as a basis for creation of the entries</translation>
    </message>
    <message>
      <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
      <translation>Place page numbers of the entries at the beginning or the end of the line, or not at all</translation>
    </message>
    <message>
      <source>List Non-Printing Entries</source>
      <translation>List Non-Printing Entries</translation>
    </message>
    <message>
      <source>Include frames that are set to not print as well</source>
      <translation>Include frames that are set to not print as well</translation>
    </message>
    <message>
      <source>The paragraph style used for the entry lines</source>
      <translation>The paragraph style used for the entry lines</translation>
    </message>
    <message>
      <source>Paragraph Style:</source>
      <translation>Paragraph Style:</translation>
    </message>
    <message>
      <source>Inde&amp;x</source>
      <translation>Inde&amp;x</translation>
    </message>
  </context>
  <context>
    <name>TabCheckDoc</name>
    <message>
      <source>Ignore all errors</source>
      <translation>Ignore all errors</translation>
    </message>
    <message>
      <source>Automatic check before printing or exporting</source>
      <translation>Automatic check before printing or exporting</translation>
    </message>
    <message>
      <source>Check for missing glyphs</source>
      <translation>Check for missing glyphs</translation>
    </message>
    <message>
      <source>Check for objects not on a page</source>
      <translation>Check for objects not on a page</translation>
    </message>
    <message>
      <source>Check for overflow in text frames</source>
      <translation>Check for overflow in text frames</translation>
    </message>
    <message>
      <source>Check for transparencies used</source>
      <translation>Check for transparencies used</translation>
    </message>
    <message>
      <source>Check for missing images</source>
      <translation>Check for missing images</translation>
    </message>
    <message>
      <source>Check image resolution</source>
      <translation>Check image resolution</translation>
    </message>
    <message>
      <source>Lowest allowed resolution</source>
      <translation>Lowest allowed resolution</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>Check for placed PDF Files</source>
      <translation>Check for placed PDF Files</translation>
    </message>
    <message>
      <source>Check for PDF Annotations and Fields</source>
      <translation>Check for PDF Annotations and Fields</translation>
    </message>
    <message>
      <source>Add Profile</source>
      <translation>Add Profile</translation>
    </message>
    <message>
      <source>Remove Profile</source>
      <translation>Remove Profile</translation>
    </message>
  </context>
  <context>
    <name>TabGuides</name>
    <message>
      <source>Common Settings</source>
      <translation>Common Settings</translation>
    </message>
    <message>
      <source>Placing in Documents</source>
      <translation>Placing in Documents</translation>
    </message>
    <message>
      <source>In the Background</source>
      <translation>In the Background</translation>
    </message>
    <message>
      <source>In the Foreground</source>
      <translation>In the Foreground</translation>
    </message>
    <message>
      <source>Snapping</source>
      <translation>Snapping</translation>
    </message>
    <message>
      <source>Snap Distance:</source>
      <translation>Snap Distance:</translation>
    </message>
    <message>
      <source>Grab Radius:</source>
      <translation>Grab Radius:</translation>
    </message>
    <message>
      <source> px</source>
      <translation> px</translation>
    </message>
    <message>
      <source>Show Guides</source>
      <translation>Show Guides</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Colour:</translation>
    </message>
    <message>
      <source>Show Margins</source>
      <translation>Show Margins</translation>
    </message>
    <message>
      <source>Show Page Grid</source>
      <translation>Show Page Grid</translation>
    </message>
    <message>
      <source>Major Grid</source>
      <translation>Major Grid</translation>
    </message>
    <message>
      <source>Spacing:</source>
      <translation>Spacing:</translation>
    </message>
    <message>
      <source>Minor Grid</source>
      <translation>Minor Grid</translation>
    </message>
    <message>
      <source>Show Baseline Grid</source>
      <translation>Show Baseline Grid</translation>
    </message>
    <message>
      <source>Baseline Settings</source>
      <translation>Baseline Settings</translation>
    </message>
    <message>
      <source>Baseline &amp;Grid:</source>
      <translation>Baseline &amp;Grid:</translation>
    </message>
    <message>
      <source>Baseline &amp;Offset:</source>
      <translation>Baseline &amp;Offset:</translation>
    </message>
    <message>
      <source>Distance between the minor grid lines</source>
      <translation>Distance between the minor grid lines</translation>
    </message>
    <message>
      <source>Distance between the major grid lines</source>
      <translation>Distance between the major grid lines</translation>
    </message>
    <message>
      <source>Distance within which an object will snap to your placed guides</source>
      <translation>Distance within which an object will snap to your placed guides</translation>
    </message>
    <message>
      <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
      <translation>Radius of the area where Scribus will allow you to grab an objects handles</translation>
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
      <translation>Turns the basegrid on or off</translation>
    </message>
    <message>
      <source>Turns the gridlines on or off</source>
      <translation>Turns the gridlines on or off</translation>
    </message>
    <message>
      <source>Turns the guides on or off</source>
      <translation>Turns the guides on or off</translation>
    </message>
    <message>
      <source>Turns the margins on or off</source>
      <translation>Turns the margins on or off</translation>
    </message>
    <message>
      <source>Guides are not visible through objects on the page</source>
      <translation>Guides are not visible through objects on the page</translation>
    </message>
    <message>
      <source>Guides are visible above all objects on the page</source>
      <translation>Guides are visible above all objects on the page</translation>
    </message>
    <message>
      <source>Distance between the lines of the baseline grid</source>
      <translation>Distance between the lines of the baseline grid</translation>
    </message>
    <message>
      <source>Distance from the top of the page for the first baseline</source>
      <translation>Distance from the top of the page for the first baseline</translation>
    </message>
  </context>
  <context>
    <name>TabManager</name>
    <message>
      <source>Manage Tabulators</source>
      <translation>Manage Tabulators</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>TabPDFOptions</name>
    <message>
      <source>Export Range</source>
      <translation>Export Range</translation>
    </message>
    <message>
      <source>&amp;All Pages</source>
      <translation>&amp;All Pages</translation>
    </message>
    <message>
      <source>C&amp;hoose Pages</source>
      <translation>C&amp;hoose Pages</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>&amp;Rotation:</translation>
    </message>
    <message>
      <source>File Options</source>
      <translation>File Options</translation>
    </message>
    <message>
      <source>Compatibilit&amp;y:</source>
      <translation>Compatibilit&amp;y:</translation>
    </message>
    <message>
      <source>&amp;Binding:</source>
      <translation>&amp;Binding:</translation>
    </message>
    <message>
      <source>Left Margin</source>
      <translation>Left Margin</translation>
    </message>
    <message>
      <source>Right Margin</source>
      <translation>Right Margin</translation>
    </message>
    <message>
      <source>Generate &amp;Thumbnails</source>
      <translation>Generate &amp;Thumbnails</translation>
    </message>
    <message>
      <source>Save &amp;Linked Text Frames as PDF Articles</source>
      <translation>Save &amp;Linked Text Frames as PDF Articles</translation>
    </message>
    <message>
      <source>&amp;Include Bookmarks</source>
      <translation>&amp;Include Bookmarks</translation>
    </message>
    <message>
      <source>Include Layers</source>
      <translation>Include Layers</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>&amp;Resolution for EPS Graphics:</source>
      <translation>&amp;Resolution for EPS Graphics:</translation>
    </message>
    <message>
      <source>Com&amp;press Text and Vector Graphics</source>
      <translation>Com&amp;press Text and Vector Graphics</translation>
    </message>
    <message>
      <source>Image Settings</source>
      <translation>Image Settings</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>Automatic</translation>
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
      <translation>None</translation>
    </message>
    <message>
      <source>&amp;Method:</source>
      <translation>&amp;Method:</translation>
    </message>
    <message>
      <source>&amp;Quality:</source>
      <translation>&amp;Quality:</translation>
    </message>
    <message>
      <source>Maximum</source>
      <translation>Maximum</translation>
    </message>
    <message>
      <source>High</source>
      <translation>High</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>Medium</translation>
    </message>
    <message>
      <source>Low</source>
      <translation>Low</translation>
    </message>
    <message>
      <source>Minimum</source>
      <translation>Minimum</translation>
    </message>
    <message>
      <source>Resample Images to:</source>
      <translation>Resample Images to:</translation>
    </message>
    <message>
      <source>&amp;General</source>
      <translation>&amp;General</translation>
    </message>
    <message>
      <source>&amp;Embed all Fonts</source>
      <translation>&amp;Embed all Fonts</translation>
    </message>
    <message>
      <source>&amp;Subset all Fonts</source>
      <translation>&amp;Subset all Fonts</translation>
    </message>
    <message>
      <source>Embedding</source>
      <translation>Embedding</translation>
    </message>
    <message>
      <source>Available Fonts:</source>
      <translation>Available Fonts:</translation>
    </message>
    <message>
      <source>&amp;>></source>
      <translation>&amp;>></translation>
    </message>
    <message>
      <source>&amp;&lt;&lt;</source>
      <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
      <source>Fonts to embed:</source>
      <translation>Fonts to embed:</translation>
    </message>
    <message>
      <source>Fonts to subset:</source>
      <translation>Fonts to subset:</translation>
    </message>
    <message>
      <source>&amp;Fonts</source>
      <translation>&amp;Fonts</translation>
    </message>
    <message>
      <source>Enable &amp;Presentation Effects</source>
      <translation>Enable &amp;Presentation Effects</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Page</translation>
    </message>
    <message>
      <source>Show Page Pre&amp;views</source>
      <translation>Show Page Pre&amp;views</translation>
    </message>
    <message>
      <source>Effects</source>
      <translation>Effects</translation>
    </message>
    <message>
      <source>&amp;Display Duration:</source>
      <translation>&amp;Display Duration:</translation>
    </message>
    <message>
      <source>Effec&amp;t Duration:</source>
      <translation>Effec&amp;t Duration:</translation>
    </message>
    <message>
      <source>Effect T&amp;ype:</source>
      <translation>Effect T&amp;ype:</translation>
    </message>
    <message>
      <source>&amp;Moving Lines:</source>
      <translation>&amp;Moving Lines:</translation>
    </message>
    <message>
      <source>F&amp;rom the:</source>
      <translation>F&amp;rom the:</translation>
    </message>
    <message>
      <source>D&amp;irection:</source>
      <translation>D&amp;irection:</translation>
    </message>
    <message>
      <source> sec</source>
      <translation> sec</translation>
    </message>
    <message>
      <source>No Effect</source>
      <translation>No Effect</translation>
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
      <translation>Glitter</translation>
    </message>
    <message>
      <source>Split</source>
      <translation>Split</translation>
    </message>
    <message>
      <source>Wipe</source>
      <translation>Wipe</translation>
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
      <translation>Inside</translation>
    </message>
    <message>
      <source>Outside</source>
      <translation>Outside</translation>
    </message>
    <message>
      <source>Left to Right</source>
      <translation>Left to Right</translation>
    </message>
    <message>
      <source>Top to Bottom</source>
      <translation>Top to Bottom</translation>
    </message>
    <message>
      <source>Bottom to Top</source>
      <translation>Bottom to Top</translation>
    </message>
    <message>
      <source>Right to Left</source>
      <translation>Right to Left</translation>
    </message>
    <message>
      <source>Top-left to Bottom-Right</source>
      <translation>Top-left to Bottom-Right</translation>
    </message>
    <message>
      <source>&amp;Apply Effect on all Pages</source>
      <translation>&amp;Apply Effect on all Pages</translation>
    </message>
    <message>
      <source>E&amp;xtras</source>
      <translation>E&amp;xtras</translation>
    </message>
    <message>
      <source>&amp;Use Encryption</source>
      <translation>&amp;Use Encryption</translation>
    </message>
    <message>
      <source>Passwords</source>
      <translation>Passwords</translation>
    </message>
    <message>
      <source>&amp;User:</source>
      <translation>&amp;User:</translation>
    </message>
    <message>
      <source>&amp;Owner:</source>
      <translation>&amp;Owner:</translation>
    </message>
    <message>
      <source>Settings</source>
      <translation>Settings</translation>
    </message>
    <message>
      <source>Allow &amp;Printing the Document</source>
      <translation>Allow &amp;Printing the Document</translation>
    </message>
    <message>
      <source>Allow &amp;Changing the Document</source>
      <translation>Allow &amp;Changing the Document</translation>
    </message>
    <message>
      <source>Allow Cop&amp;ying Text and Graphics</source>
      <translation>Allow Cop&amp;ying Text and Graphics</translation>
    </message>
    <message>
      <source>Allow Adding &amp;Annotations and Fields</source>
      <translation>Allow Adding &amp;Annotations and Fields</translation>
    </message>
    <message>
      <source>S&amp;ecurity</source>
      <translation>S&amp;ecurity</translation>
    </message>
    <message>
      <source>General</source>
      <translation>General</translation>
    </message>
    <message>
      <source>Output &amp;Intended For:</source>
      <translation>Output &amp;Intended For:</translation>
    </message>
    <message>
      <source>Screen / Web</source>
      <translation>Screen / Web</translation>
    </message>
    <message>
      <source>Printer</source>
      <translation>Printer</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>Greyscale</translation>
    </message>
    <message>
      <source>&amp;Use Custom Rendering Settings</source>
      <translation>&amp;Use Custom Rendering Settings</translation>
    </message>
    <message>
      <source>Rendering Settings</source>
      <translation>Rendering Settings</translation>
    </message>
    <message>
      <source>Fre&amp;quency:</source>
      <translation>Fre&amp;quency:</translation>
    </message>
    <message>
      <source>&amp;Angle:</source>
      <translation>&amp;Angle:</translation>
    </message>
    <message>
      <source>S&amp;pot Function:</source>
      <translation>S&amp;pot Function:</translation>
    </message>
    <message>
      <source>Simple Dot</source>
      <translation>Simple Dot</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>Line</translation>
    </message>
    <message>
      <source>Round</source>
      <translation>Round</translation>
    </message>
    <message>
      <source>Ellipse</source>
      <translation>Ellipse</translation>
    </message>
    <message>
      <source>Solid Colors:</source>
      <translation>Solid Colours:</translation>
    </message>
    <message>
      <source>Use ICC Profile</source>
      <translation>Use ICC Profile</translation>
    </message>
    <message>
      <source>Profile:</source>
      <translation>Profile:</translation>
    </message>
    <message>
      <source>Rendering-Intent:</source>
      <translation>Rendering-Intent:</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>Perceptual</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>Relative Colourimetric</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturation</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>Absolute Colourimetric</translation>
    </message>
    <message>
      <source>Images:</source>
      <translation>Images:</translation>
    </message>
    <message>
      <source>Don't use embedded ICC profiles</source>
      <translation>Don't use embedded ICC profiles</translation>
    </message>
    <message>
      <source>C&amp;olor</source>
      <translation>C&amp;olour</translation>
    </message>
    <message>
      <source>PDF/X-3 Output Intent</source>
      <translation>PDF/X-3 Output Intent</translation>
    </message>
    <message>
      <source>&amp;Info String:</source>
      <translation>&amp;Info String:</translation>
    </message>
    <message>
      <source>Output &amp;Profile:</source>
      <translation>Output &amp;Profile:</translation>
    </message>
    <message>
      <source>Trim Box</source>
      <translation>Trim Box</translation>
    </message>
    <message>
      <source>PDF/X-&amp;3</source>
      <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
      <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
      <translation>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</translation>
    </message>
    <message>
      <source>Show page previews of each page listed above.</source>
      <translation>Show page previews of each page listed above.</translation>
    </message>
    <message>
      <source>Length of time the page is shown before the presentation starts on the selected page.</source>
      <translation>Length of time the page is shown before the presentation starts on the selected page.</translation>
    </message>
    <message>
      <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
      <translation>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</translation>
    </message>
    <message>
      <source>Type of the display effect.</source>
      <translation>Type of the display effect.</translation>
    </message>
    <message>
      <source>Direction of the effect of moving lines for the split and blind effects.</source>
      <translation>Direction of the effect of moving lines for the split and blind effects.</translation>
    </message>
    <message>
      <source>Starting position for the box and split effects.</source>
      <translation>Starting position for the box and split effects.</translation>
    </message>
    <message>
      <source>Direction of the glitter or wipe effects.</source>
      <translation>Direction of the glitter or wipe effects.</translation>
    </message>
    <message>
      <source>Apply the selected effect to all pages.</source>
      <translation>Apply the selected effect to all pages.</translation>
    </message>
    <message>
      <source>Export all pages to PDF</source>
      <translation>Export all pages to PDF</translation>
    </message>
    <message>
      <source>Export a range of pages to PDF</source>
      <translation>Export a range of pages to PDF</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</translation>
    </message>
    <message>
      <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
      <translation>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</translation>
    </message>
    <message>
      <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
      <translation>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</translation>
    </message>
    <message>
      <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
      <translation>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</translation>
    </message>
    <message>
      <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
      <translation>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</translation>
    </message>
    <message>
      <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
      <translation>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</translation>
    </message>
    <message>
      <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
      <translation>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</translation>
    </message>
    <message>
      <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
      <translation>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</translation>
    </message>
    <message>
      <source>DPI (Dots Per Inch) for image export.</source>
      <translation>DPI (Dots Per Inch) for image export.</translation>
    </message>
    <message>
      <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
      <translation>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</translation>
    </message>
    <message>
      <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
      <translation>Choose a master password which enables or disables all the
security features in your exported PDF</translation>
    </message>
    <message>
      <source>Choose a password for users to be able to read your PDF.</source>
      <translation>Choose a password for users to be able to read your PDF.</translation>
    </message>
    <message>
      <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
      <translation>Allow printing of the PDF. If un-checked, printing is prevented. </translation>
    </message>
    <message>
      <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
      <translation>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</translation>
    </message>
    <message>
      <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
      <translation>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</translation>
    </message>
    <message>
      <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
      <translation>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</translation>
    </message>
    <message>
      <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
      <translation>Colour model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 colour CMYK printer.</translation>
    </message>
    <message>
      <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
      <translation>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</translation>
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
      <translation>Rendering intent for images</translation>
    </message>
    <message>
      <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
      <translation>Output profile for printing. If possible, get some guidance from your printer on profile selection.</translation>
    </message>
    <message>
      <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
      <translation>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation>Distance for bleed from the top of the physical page</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation>Distance for bleed from the bottom of the physical page</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation>Distance for bleed from the left of the physical page</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation>Distance for bleed from the right of the physical page</translation>
    </message>
    <message>
      <source>Mirror Page(s) horizontally</source>
      <translation>Mirror Page(s) horizontally</translation>
    </message>
    <message>
      <source>Mirror Page(s) vertically</source>
      <translation>Mirror Page(s) vertically</translation>
    </message>
    <message>
      <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
      <translation>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</translation>
    </message>
    <message>
      <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
      <translation>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want colour managed RGB for commercial printing
 and is selectable when you have activated colour management. 
Use only when advised by your printer or in some cases printing to a 4 colour digital colour laser printer.</translation>
    </message>
    <message>
      <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
      <translation>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</translation>
    </message>
    <message>
      <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
      <translation>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colours. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</translation>
    </message>
    <message>
      <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
      <translation>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</translation>
    </message>
  </context>
  <context>
    <name>TabTools</name>
    <message>
      <source>Font:</source>
      <translation>Font:</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Size:</source>
      <translation>Size:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
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
      <translation>Colu&amp;mns:</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>&amp;Gap:</translation>
    </message>
    <message>
      <source>Woven silk pyjamas exchanged for blue quartz</source>
      <translation>Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
      <source>&amp;Line Color:</source>
      <translation>&amp;Line Colour:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Shading:</source>
      <translation>&amp;Shading:</translation>
    </message>
    <message>
      <source>&amp;Fill Color:</source>
      <translation>&amp;Fill Colour:</translation>
    </message>
    <message>
      <source>S&amp;hading:</source>
      <translation>S&amp;hading:</translation>
    </message>
    <message>
      <source>Line Style:</source>
      <translation>Line Style:</translation>
    </message>
    <message>
      <source>Line &amp;Width:</source>
      <translation>Line &amp;Width:</translation>
    </message>
    <message>
      <source>Line S&amp;tyle:</source>
      <translation>Line S&amp;tyle:</translation>
    </message>
    <message>
      <source>Arrows:</source>
      <translation>Arrows:</translation>
    </message>
    <message>
      <source>Start:</source>
      <translation>Start:</translation>
    </message>
    <message>
      <source>End:</source>
      <translation>End:</translation>
    </message>
    <message>
      <source>&amp;Free Scaling</source>
      <translation>&amp;Free Scaling</translation>
    </message>
    <message>
      <source>&amp;Horizontal Scaling:</source>
      <translation>&amp;Horizontal Scaling:</translation>
    </message>
    <message>
      <source>&amp;Vertical Scaling:</source>
      <translation>&amp;Vertical Scaling:</translation>
    </message>
    <message>
      <source>&amp;Scale Picture to Frame Size</source>
      <translation>&amp;Scale Picture to Frame Size</translation>
    </message>
    <message>
      <source>Keep Aspect &amp;Ratio</source>
      <translation>Keep Aspect &amp;Ratio</translation>
    </message>
    <message>
      <source>F&amp;ill Color:</source>
      <translation>F&amp;ill Colour:</translation>
    </message>
    <message>
      <source>Use embedded Clipping Path</source>
      <translation>Use embedded Clipping Path</translation>
    </message>
    <message>
      <source>On Screen Preview</source>
      <translation>On Screen Preview</translation>
    </message>
    <message>
      <source>Full Resolution Preview</source>
      <translation>Full Resolution Preview</translation>
    </message>
    <message>
      <source>Normal Resolution Preview</source>
      <translation>Normal Resolution Preview</translation>
    </message>
    <message>
      <source>Low Resolution Preview</source>
      <translation>Low Resolution Preview</translation>
    </message>
    <message>
      <source>Mi&amp;nimum:</source>
      <translation>Mi&amp;nimum:</translation>
    </message>
    <message>
      <source>Ma&amp;ximum:</source>
      <translation>Ma&amp;ximum:</translation>
    </message>
    <message>
      <source>&amp;Stepping:</source>
      <translation>&amp;Stepping:</translation>
    </message>
    <message>
      <source>Text Frame Properties</source>
      <translation>Text Frame Properties</translation>
    </message>
    <message>
      <source>Picture Frame Properties</source>
      <translation>Picture Frame Properties</translation>
    </message>
    <message>
      <source>Shape Drawing Properties</source>
      <translation>Shape Drawing Properties</translation>
    </message>
    <message>
      <source>Magnification Level Defaults</source>
      <translation>Magnification Level Defaults</translation>
    </message>
    <message>
      <source>Line Drawing Properties</source>
      <translation>Line Drawing Properties</translation>
    </message>
    <message>
      <source>Polygon Drawing Properties</source>
      <translation>Polygon Drawing Properties</translation>
    </message>
    <message>
      <source>Font for new text frames</source>
      <translation>Font for new text frames</translation>
    </message>
    <message>
      <source>Size of font for new text frames</source>
      <translation>Size of font for new text frames</translation>
    </message>
    <message>
      <source>Color of font</source>
      <translation>Colour of font</translation>
    </message>
    <message>
      <source>Number of columns in a text frame</source>
      <translation>Number of columns in a text frame</translation>
    </message>
    <message>
      <source>Gap between text frame columns</source>
      <translation>Gap between text frame columns</translation>
    </message>
    <message>
      <source>Sample of your font</source>
      <translation>Sample of your font</translation>
    </message>
    <message>
      <source>Picture frames allow pictures to scale to any size</source>
      <translation>Picture frames allow pictures to scale to any size</translation>
    </message>
    <message>
      <source>Horizontal scaling of images</source>
      <translation>Horizontal scaling of images</translation>
    </message>
    <message>
      <source>Vertical scaling of images</source>
      <translation>Vertical scaling of images</translation>
    </message>
    <message>
      <source>Keep horizontal and vertical scaling the same</source>
      <translation>Keep horizontal and vertical scaling the same</translation>
    </message>
    <message>
      <source>Pictures in picture frames are scaled to the size of the frame</source>
      <translation>Pictures in picture frames are scaled to the size of the frame</translation>
    </message>
    <message>
      <source>Automatically scaled pictures keep their original proportions</source>
      <translation>Automatically scaled pictures keep their original proportions</translation>
    </message>
    <message>
      <source>Fill color of picture frames</source>
      <translation>Fill colour of picture frames</translation>
    </message>
    <message>
      <source>Saturation of color of fill</source>
      <translation>Saturation of colour of fill</translation>
    </message>
    <message>
      <source>Line color of shapes</source>
      <translation>Line colour of shapes</translation>
    </message>
    <message>
      <source>Saturation of color of lines</source>
      <translation>Saturation of colour of lines</translation>
    </message>
    <message>
      <source>Fill color of shapes</source>
      <translation>Fill colour of shapes</translation>
    </message>
    <message>
      <source>Line style of shapes</source>
      <translation>Line style of shapes</translation>
    </message>
    <message>
      <source>Line width of shapes</source>
      <translation>Line width of shapes</translation>
    </message>
    <message>
      <source>Minimum magnification allowed</source>
      <translation>Minimum magnification allowed</translation>
    </message>
    <message>
      <source>Maximum magnification allowed</source>
      <translation>Maximum magnification allowed</translation>
    </message>
    <message>
      <source>Change in magnification for each zoom operation</source>
      <translation>Change in magnification for each zoom operation</translation>
    </message>
    <message>
      <source>Color of lines</source>
      <translation>Colour of lines</translation>
    </message>
    <message>
      <source>Saturation of color</source>
      <translation>Saturation of colour</translation>
    </message>
    <message>
      <source>Style of lines</source>
      <translation>Style of lines</translation>
    </message>
    <message>
      <source>Width of lines</source>
      <translation>Width of lines</translation>
    </message>
    <message>
      <source>Tab Fill Character:</source>
      <translation>Tab Fill Character:</translation>
    </message>
    <message>
      <source>Tab Width:</source>
      <translation>Tab Width:</translation>
    </message>
    <message>
      <source>Custom:</source>
      <translation>Custom:</translation>
    </message>
    <message>
      <source>Custom: </source>
      <translation>Custom: </translation>
    </message>
  </context>
  <context>
    <name>TabTypograpy</name>
    <message>
      <source>Subscript</source>
      <translation>Subscript</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Displacement:</source>
      <translation>&amp;Displacement:</translation>
    </message>
    <message>
      <source>&amp;Scaling:</source>
      <translation>&amp;Scaling:</translation>
    </message>
    <message>
      <source>Superscript</source>
      <translation>Superscript</translation>
    </message>
    <message>
      <source>D&amp;isplacement:</source>
      <translation>D&amp;isplacement:</translation>
    </message>
    <message>
      <source>S&amp;caling:</source>
      <translation>S&amp;caling:</translation>
    </message>
    <message>
      <source>Small Caps</source>
      <translation>Small Caps</translation>
    </message>
    <message>
      <source>Sc&amp;aling:</source>
      <translation>Sc&amp;aling:</translation>
    </message>
    <message>
      <source>Underline</source>
      <translation>Underline</translation>
    </message>
    <message>
      <source>Displacement:</source>
      <translation>Displacement:</translation>
    </message>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>Line Width:</translation>
    </message>
    <message>
      <source>Strikethru</source>
      <translation>Strikeout</translation>
    </message>
    <message>
      <source>Automatic &amp;Line Spacing</source>
      <translation>Automatic &amp;Line Spacing</translation>
    </message>
    <message>
      <source>Line Spacing:</source>
      <translation>Line Spacing:</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the font on a line</source>
      <translation>Displacement above the baseline of the font on a line</translation>
    </message>
    <message>
      <source>Relative size of the superscript compared to the normal font</source>
      <translation>Relative size of the superscript compared to the normal font</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font on a line</source>
      <translation>Displacement below the baseline of the normal font on a line</translation>
    </message>
    <message>
      <source>Relative size of the subscript compared to the normal font</source>
      <translation>Relative size of the subscript compared to the normal font</translation>
    </message>
    <message>
      <source>Relative size of the small caps font compared to the normal font</source>
      <translation>Relative size of the small caps font compared to the normal font</translation>
    </message>
    <message>
      <source>Percentage increase over the font size for the line spacing</source>
      <translation>Percentage increase over the font size for the line spacing</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
      <translation>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</translation>
    </message>
    <message>
      <source>Line width expressed as a percentage of the font size</source>
      <translation>Line width expressed as a percentage of the font size</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
      <translation>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</translation>
    </message>
  </context>
  <context>
    <name>Tabruler</name>
    <message>
      <source>Left</source>
      <translation>Left</translation>
    </message>
    <message>
      <source>Right</source>
      <translation>Right</translation>
    </message>
    <message>
      <source>Full Stop</source>
      <translation>Full Stop</translation>
    </message>
    <message>
      <source>Comma</source>
      <translation>Comma</translation>
    </message>
    <message>
      <source>Center</source>
      <translation>Centre</translation>
    </message>
    <message>
      <source>Delete All</source>
      <translation>Delete All</translation>
    </message>
    <message>
      <source>Indentation for first line of the paragraph</source>
      <translation>Indentation for first line of the paragraph</translation>
    </message>
    <message>
      <source>Indentation from the left for the whole paragraph</source>
      <translation>Indentation from the left for the whole paragraph</translation>
    </message>
    <message>
      <source>Delete all Tabulators</source>
      <translation>Delete all Tabulators</translation>
    </message>
    <message>
      <source>&amp;Position:</source>
      <translation>&amp;Position:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>Fill Char:</source>
      <translation>Fill Char:</translation>
    </message>
    <message>
      <source>Custom:</source>
      <translation>Custom:</translation>
    </message>
    <message>
      <source>Custom: </source>
      <translation>Custom: </translation>
    </message>
    <message>
      <source>Dot</source>
      <translation>Dot</translation>
    </message>
    <message>
      <source>Hyphen</source>
      <translation>Hyphen</translation>
    </message>
    <message>
      <source>Underscore</source>
      <translation>Underscore</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Custom</translation>
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
      <source>Group </source>
      <translation>Group </translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation>Free Objects</translation>
    </message>
    <message>
      <source>Page </source>
      <translation>Page </translation>
    </message>
  </context>
  <context>
    <name>UnderlineValues</name>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Displacement</source>
      <translation>Displacement</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>Linewidth</translation>
    </message>
  </context>
  <context>
    <name>UndoManager</name>
    <message>
      <source>Add vertical guide</source>
      <translation>Add vertical guide</translation>
    </message>
    <message>
      <source>Add horizontal guide</source>
      <translation>Add horizontal guide</translation>
    </message>
    <message>
      <source>Remove vertical guide</source>
      <translation>Remove vertical guide</translation>
    </message>
    <message>
      <source>Remove horizontal guide</source>
      <translation>Remove horizontal guide</translation>
    </message>
    <message>
      <source>Move vertical guide</source>
      <translation>Move vertical guide</translation>
    </message>
    <message>
      <source>Move horizontal guide</source>
      <translation>Move horizontal guide</translation>
    </message>
    <message>
      <source>Lock guides</source>
      <translation>Lock guides</translation>
    </message>
    <message>
      <source>Unlock guides</source>
      <translation>Unlock guides</translation>
    </message>
    <message>
      <source>Move</source>
      <translation>Move</translation>
    </message>
    <message>
      <source>Resize</source>
      <translation>Resize</translation>
    </message>
    <message>
      <source>Rotate</source>
      <translation>Rotate</translation>
    </message>
    <message>
      <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
      <translation>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
      <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
      <translation>W1: %1, H1: %2
W2: %3, H2: %4</translation>
    </message>
    <message>
      <source>Selection</source>
      <translation>Selection</translation>
    </message>
    <message>
      <source>Group</source>
      <translation>Group</translation>
    </message>
    <message>
      <source>Selection/Group</source>
      <translation>Selection/Group</translation>
    </message>
    <message>
      <source>Create</source>
      <translation>Create</translation>
    </message>
    <message>
      <source>X: %1, Y: %2
W: %3, H: %4</source>
      <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
      <source>Align/Distribute</source>
      <translation>Align/Distribute</translation>
    </message>
    <message>
      <source>Items involved</source>
      <translation>Items involved</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
    <message>
      <source>Set fill color</source>
      <translation>Set fill colour</translation>
    </message>
    <message>
      <source>Color1: %1, Color2: %2</source>
      <translation>Colour1: %1, Colour2: %2</translation>
    </message>
    <message>
      <source>Set fill color shade</source>
      <translation>Set fill colour shade</translation>
    </message>
    <message>
      <source>Set line color</source>
      <translation>Set line colour</translation>
    </message>
    <message>
      <source>Set line color shade</source>
      <translation>Set line colour shade</translation>
    </message>
    <message>
      <source>Flip horizontally</source>
      <translation>Flip horizontally</translation>
    </message>
    <message>
      <source>Flip vertically</source>
      <translation>Flip vertically</translation>
    </message>
    <message>
      <source>Lock</source>
      <translation>Lock</translation>
    </message>
    <message>
      <source>Unlock</source>
      <translation>Unlock</translation>
    </message>
    <message>
      <source>Lock size</source>
      <translation>Lock size</translation>
    </message>
    <message>
      <source>Unlock size</source>
      <translation>Unlock size</translation>
    </message>
    <message>
      <source>Ungroup</source>
      <translation>Ungroup</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>Delete</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation>Rename</translation>
    </message>
    <message>
      <source>From %1
to %2</source>
      <translation>From %1
to %2</translation>
    </message>
    <message>
      <source>Apply Master Page</source>
      <translation>Apply Master Page</translation>
    </message>
    <message>
      <source>Paste</source>
      <translation>Paste</translation>
    </message>
    <message>
      <source>Cut</source>
      <translation>Cut</translation>
    </message>
    <message>
      <source>Set fill color transparency</source>
      <translation>Set fill colour transparency</translation>
    </message>
    <message>
      <source>Set line color transparency</source>
      <translation>Set line colour transparency</translation>
    </message>
    <message>
      <source>Set line style</source>
      <translation>Set line style</translation>
    </message>
    <message>
      <source>Set the style of line end</source>
      <translation>Set the style of line end</translation>
    </message>
    <message>
      <source>Set the style of line join</source>
      <translation>Set the style of line join</translation>
    </message>
    <message>
      <source>Set line width</source>
      <translation>Set line width</translation>
    </message>
    <message>
      <source>No style</source>
      <translation>No style</translation>
    </message>
    <message>
      <source>Set custom line style</source>
      <translation>Set custom line style</translation>
    </message>
    <message>
      <source>Do not use custom line style</source>
      <translation>Do not use custom line style</translation>
    </message>
    <message>
      <source>Set start arrow</source>
      <translation>Set start arrow</translation>
    </message>
    <message>
      <source>Set end arrow</source>
      <translation>Set end arrow</translation>
    </message>
    <message>
      <source>Create table</source>
      <translation>Create table</translation>
    </message>
    <message>
      <source>Rows: %1, Cols: %2</source>
      <translation>Rows: %1, Cols: %2</translation>
    </message>
    <message>
      <source>Set font</source>
      <translation>Set font</translation>
    </message>
    <message>
      <source>Set font size</source>
      <translation>Set font size</translation>
    </message>
    <message>
      <source>Set font width</source>
      <translation>Set font width</translation>
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
      <translation>Set kerning</translation>
    </message>
    <message>
      <source>Set line spacing</source>
      <translation>Set line spacing</translation>
    </message>
    <message>
      <source>Set paragraph style</source>
      <translation>Set paragraph style</translation>
    </message>
    <message>
      <source>Set language</source>
      <translation>Set language</translation>
    </message>
    <message>
      <source>Align text</source>
      <translation>Align text</translation>
    </message>
    <message>
      <source>Set font effect</source>
      <translation>Set font effect</translation>
    </message>
    <message>
      <source>Image frame</source>
      <translation>Image frame</translation>
    </message>
    <message>
      <source>Text frame</source>
      <translation>Text frame</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation>Polygon</translation>
    </message>
    <message>
      <source>Bezier curve</source>
      <translation>Bezier curve</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>Polyline</translation>
    </message>
    <message>
      <source>Convert to</source>
      <translation>Convert to</translation>
    </message>
    <message>
      <source>Import SVG image</source>
      <translation>Import SVG image</translation>
    </message>
    <message>
      <source>Import EPS image</source>
      <translation>Import EPS image</translation>
    </message>
    <message>
      <source>Scratch space</source>
      <translation>Scratch space</translation>
    </message>
    <message>
      <source>Text flows around the frame</source>
      <translation>Text flows around the frame</translation>
    </message>
    <message>
      <source>Text flows around bounding box</source>
      <translation>Text flows around bounding box</translation>
    </message>
    <message>
      <source>Text flows around contour line</source>
      <translation>Text flows around contour line</translation>
    </message>
    <message>
      <source>No text flow</source>
      <translation>No text flow</translation>
    </message>
    <message>
      <source>No bounding box</source>
      <translation>No bounding box</translation>
    </message>
    <message>
      <source>No contour line</source>
      <translation>No contour line</translation>
    </message>
    <message>
      <source>Page %1</source>
      <translation>Page %1</translation>
    </message>
    <message>
      <source>Set image scaling</source>
      <translation>Set image scaling</translation>
    </message>
    <message>
      <source>Frame size</source>
      <translation>Frame size</translation>
    </message>
    <message>
      <source>Free scaling</source>
      <translation>Free scaling</translation>
    </message>
    <message>
      <source>Keep aspect ratio</source>
      <translation>Keep aspect ratio</translation>
    </message>
    <message>
      <source>Break aspect ratio</source>
      <translation>Break aspect ratio</translation>
    </message>
    <message>
      <source>Edit contour line</source>
      <translation>Edit contour line</translation>
    </message>
    <message>
      <source>Edit shape</source>
      <translation>Edit shape</translation>
    </message>
    <message>
      <source>Reset contour line</source>
      <translation>Reset contour line</translation>
    </message>
    <message>
      <source>Add page</source>
      <translation>Add page</translation>
    </message>
    <message>
      <source>Add pages</source>
      <translation>Add pages</translation>
    </message>
    <message>
      <source>Delete page</source>
      <translation>Delete page</translation>
    </message>
    <message>
      <source>Delete pages</source>
      <translation>Delete pages</translation>
    </message>
    <message>
      <source>Add layer</source>
      <translation>Add layer</translation>
    </message>
    <message>
      <source>Delete layer</source>
      <translation>Delete layer</translation>
    </message>
    <message>
      <source>Rename layer</source>
      <translation>Rename layer</translation>
    </message>
    <message>
      <source>Raise layer</source>
      <translation>Raise layer</translation>
    </message>
    <message>
      <source>Lower layer</source>
      <translation>Lower layer</translation>
    </message>
    <message>
      <source>Send to layer</source>
      <translation>Send to layer</translation>
    </message>
    <message>
      <source>Enable printing of layer</source>
      <translation>Enable printing of layer</translation>
    </message>
    <message>
      <source>Disable printing of layer</source>
      <translation>Disable printing of layer</translation>
    </message>
    <message>
      <source>Change name of the layer</source>
      <translation>Change name of the layer</translation>
    </message>
    <message>
      <source>Get image</source>
      <translation>Get image</translation>
    </message>
    <message>
      <source>Import OpenOffice.org Draw image</source>
      <translation>Import OpenOffice.org Draw image</translation>
    </message>
    <message>
      <source>Set font height</source>
      <translation>Set font height</translation>
    </message>
  </context>
  <context>
    <name>UndoPalette</name>
    <message>
      <source>Initial State</source>
      <translation>Initial State</translation>
    </message>
    <message>
      <source>Action History</source>
      <translation>Action History</translation>
    </message>
    <message>
      <source>Show selected object only</source>
      <translation>Show selected object only</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Undo</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Redo</translation>
    </message>
  </context>
  <context>
    <name>UndoWidget</name>
    <message>
      <source>%1: %2</source>
      <comment>undo target: action (f.e. Text frame: Resize)</comment>
      <translation>%1: %2</translation>
    </message>
  </context>
  <context>
    <name>ValueDialog</name>
    <message>
      <source>Insert value</source>
      <translation>Insert value</translation>
    </message>
    <message>
      <source>Enter a value then press OK.</source>
      <translation>Enter a value then press OK.</translation>
    </message>
    <message>
      <source>Enter a value then press OK</source>
      <translation>Enter a value then press OK</translation>
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
      <translation>Send your value to the script</translation>
    </message>
  </context>
  <context>
    <name>VlnaDialog</name>
    <message>
      <source>Short Words</source>
      <comment>short words plugin</comment>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>Apply unbreakable space on:</source>
      <comment>short words plugin</comment>
      <translation>Apply unbreakable space on:</translation>
    </message>
    <message>
      <source>&amp;Selected frames</source>
      <comment>short words plugin</comment>
      <translation>&amp;Selected frames</translation>
    </message>
    <message>
      <source>Active &amp;page</source>
      <comment>short words plugin</comment>
      <translation>Active &amp;page</translation>
    </message>
    <message>
      <source>&amp;All items</source>
      <comment>short words plugin</comment>
      <translation>&amp;All items</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <comment>short words plugin</comment>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <comment>short words plugin</comment>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Replace defaults by user config</source>
      <comment>short words plugin</comment>
      <translation>Replace defaults by user config</translation>
    </message>
    <message>
      <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
      <comment>short words plugin</comment>
      <translation>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</translation>
    </message>
    <message>
      <source>Only selected frames processed.</source>
      <comment>short words plugin</comment>
      <translation>Only selected frames processed.</translation>
    </message>
    <message>
      <source>Only actual page processed.</source>
      <comment>short words plugin</comment>
      <translation>Only actual page processed.</translation>
    </message>
    <message>
      <source>All items in document processed.</source>
      <comment>short words plugin</comment>
      <translation>All items in document processed.</translation>
    </message>
    <message>
      <source>Short Words for Scribus</source>
      <comment>short words plugin</comment>
      <translation>Short Words for Scribus</translation>
    </message>
    <message>
      <source>Available in the following languages</source>
      <comment>short words plugin</comment>
      <translation>Available in the following languages</translation>
    </message>
    <message>
      <source>About Short Words</source>
      <comment>short words plugin</comment>
      <translation>About Short Words</translation>
    </message>
    <message>
      <source>Edit &amp;system configuration...</source>
      <translation>Edit &amp;system configuration...</translation>
    </message>
    <message>
      <source>Edit &amp;user configuration...</source>
      <translation>Edit &amp;user configuration...</translation>
    </message>
    <message>
      <source>S&amp;etup editor...</source>
      <translation>S&amp;etup editor...</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>&amp;Info and Languages...</source>
      <translation>&amp;Info and Languages...</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation>&amp;Help</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>You are starting to edit read-only file.
%1</source>
      <translation>You are starting to edit read-only file.
%1</translation>
    </message>
    <message>
      <source>Application '%1' error. Cannot be started.</source>
      <translation>Application '%1' error. Cannot be started.</translation>
    </message>
    <message>
      <source>Short Words setup</source>
      <translation>Short Words setup</translation>
    </message>
    <message>
      <source>Enter name of the plain text editor executable:</source>
      <translation>Enter name of the plain text editor executable:</translation>
    </message>
  </context>
  <context>
    <name>WerkToolB</name>
    <message>
      <source>Tools</source>
      <translation>Tools</translation>
    </message>
    <message>
      <source>Properties...</source>
      <translation>Properties...</translation>
    </message>
  </context>
  <context>
    <name>WerkToolBP</name>
    <message>
      <source>Button</source>
      <translation>Button</translation>
    </message>
    <message>
      <source>Text Field</source>
      <translation>Text Field</translation>
    </message>
    <message>
      <source>Check Box</source>
      <translation>Check Box</translation>
    </message>
    <message>
      <source>Combo Box</source>
      <translation>Combo Box</translation>
    </message>
    <message>
      <source>List Box</source>
      <translation>List Box</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Link</source>
      <translation>Link</translation>
    </message>
    <message>
      <source>PDF Tools</source>
      <translation>PDF Tools</translation>
    </message>
    <message>
      <source>Insert PDF Fields</source>
      <translation>Insert PDF Fields</translation>
    </message>
    <message>
      <source>Insert PDF Annotations</source>
      <translation>Insert PDF Annotations</translation>
    </message>
  </context>
  <context>
    <name>gtFileDialog</name>
    <message>
      <source>Choose the importer to use</source>
      <translation>Choose the importer to use</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>Automatic</translation>
    </message>
    <message>
      <source>Import text without any formatting</source>
      <translation>Import text without any formatting</translation>
    </message>
    <message>
      <source>Importer:</source>
      <translation>Importer:</translation>
    </message>
    <message>
      <source>Encoding:</source>
      <translation>Encoding:</translation>
    </message>
    <message>
      <source>Import Text Only</source>
      <translation>Import Text Only</translation>
    </message>
  </context>
  <context>
    <name>gtImporterDialog</name>
    <message>
      <source>Choose the importer to use</source>
      <translation>Choose the importer to use</translation>
    </message>
    <message>
      <source></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remember association</source>
      <translation>Remember association</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
      <translation>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</translation>
    </message>
  </context>
  <context>
    <name>nftdialog</name>
    <message>
      <source>New From Template</source>
      <translation>New From Template</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
    <message>
      <source>Description</source>
      <translation>Description</translation>
    </message>
    <message>
      <source>Usage</source>
      <translation>Usage</translation>
    </message>
    <message>
      <source>Created with</source>
      <translation>Created with</translation>
    </message>
    <message>
      <source>Author</source>
      <translation>Author</translation>
    </message>
    <message>
      <source>&amp;Remove</source>
      <translation>&amp;Remove</translation>
    </message>
    <message>
      <source>&amp;Open</source>
      <translation>&amp;Open</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Downloading Templates</source>
      <translation>Downloading Templates</translation>
    </message>
    <message>
      <source>Installing Templates</source>
      <translation>Installing Templates</translation>
    </message>
    <message>
      <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
      <translation>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</translation>
    </message>
    <message>
      <source>Preparing a template</source>
      <translation>Preparing a template</translation>
    </message>
    <message>
      <source>Removing a template</source>
      <translation>Removing a template</translation>
    </message>
    <message>
      <source>Translating template.xml</source>
      <translation>Translating template.xml</translation>
    </message>
    <message>
      <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
      <translation>Document templates can be found at http://www.scribus.net/ in the Downloads section.</translation>
    </message>
    <message>
      <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
      <translation>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</translation>
    </message>
    <message>
      <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
      <translation>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</translation>
    </message>
    <message>
      <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
      <translation>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</translation>
    </message>
    <message>
      <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
      <translation>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>Date</translation>
    </message>
  </context>
  <context>
    <name>satdialog</name>
    <message>
      <source>Save as Template</source>
      <translation>Save as Template</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Category</source>
      <translation>Category</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
    <message>
      <source>Description</source>
      <translation>Description</translation>
    </message>
    <message>
      <source>Usage</source>
      <translation>Usage</translation>
    </message>
    <message>
      <source>Author</source>
      <translation>Author</translation>
    </message>
    <message>
      <source>Email</source>
      <translation>Email</translation>
    </message>
    <message>
      <source>More Details</source>
      <translation>More Details</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Less Details</source>
      <translation>Less Details</translation>
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
      <source>landscape</source>
      <translation>landscape</translation>
    </message>
    <message>
      <source>portrait</source>
      <translation>portrait</translation>
    </message>
    <message>
      <source>custom</source>
      <translation>custom</translation>
    </message>
  </context>
  <context>
    <name>tfDia</name>
    <message>
      <source>Create filter</source>
      <translation>Create filter</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Choose a previously saved filter</source>
      <translation>Choose a previously saved filter</translation>
    </message>
    <message>
      <source>Give a name to this filter for saving</source>
      <translation>Give a name to this filter for saving</translation>
    </message>
    <message>
      <source>Give a name for saving</source>
      <translation>Give a name for saving</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
  </context>
  <context>
    <name>tfFilter</name>
    <message>
      <source>Disable or enable this filter row</source>
      <translation>Disable or enable this filter row</translation>
    </message>
    <message>
      <source>Remove this filter row</source>
      <translation>Remove this filter row</translation>
    </message>
    <message>
      <source>Add a new filter row</source>
      <translation>Add a new filter row</translation>
    </message>
    <message>
      <source>to</source>
      <translation>to</translation>
    </message>
    <message>
      <source>and</source>
      <translation>and</translation>
    </message>
    <message>
      <source>remove match</source>
      <translation>remove match</translation>
    </message>
    <message>
      <source>do not remove match</source>
      <translation>do not remove match</translation>
    </message>
    <message>
      <source>words</source>
      <translation>words</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>Remove</translation>
    </message>
    <message>
      <source>Replace</source>
      <translation>Replace</translation>
    </message>
    <message>
      <source>Apply</source>
      <translation>Apply</translation>
    </message>
    <message>
      <source>Value at the left is a regular expression</source>
      <translation>Value at the left is a regular expression</translation>
    </message>
    <message>
      <source>with</source>
      <translation>with</translation>
    </message>
    <message>
      <source>paragraph style</source>
      <translation>paragraph style</translation>
    </message>
    <message>
      <source>all instances of</source>
      <translation>all instances of</translation>
    </message>
    <message>
      <source>all paragraphs</source>
      <translation>all paragraphs</translation>
    </message>
    <message>
      <source>paragraphs starting with</source>
      <translation>paragraphs starting with</translation>
    </message>
    <message>
      <source>paragraphs with less than</source>
      <translation>paragraphs with less than</translation>
    </message>
    <message>
      <source>paragraphs with more than</source>
      <translation>paragraphs with more than</translation>
    </message>
  </context>
</TS>
