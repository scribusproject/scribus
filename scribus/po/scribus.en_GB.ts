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
      <source>newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder, numPages) -> bool

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
      <translation>newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder, numPages) -> bool

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
</translation>
    </message>
    <message>
      <source>placeSVG(&quot;filename&quot;, x, y)

Places the SVG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SVG placed on the page

If loading was successful, the selection contains the imported SVG
</source>
      <translation>placeSVG(&quot;filename&quot;, x, y)

Places the SVG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SVG placed on the page

If loading was successful, the selection contains the imported SVG
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
    <message>
      <source>newStyleDialog() -> string

Shows 'Create new paragraph style' dialog. Function returns real
style name or None when user cancels the dialog.
</source>
      <translation>newStyleDialog() -> string

Shows 'Create new paragraph style' dialog. Function returns real
style name or None when user cancels the dialog.
</translation>
    </message>
    <message>
      <source>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -> bool

WARNING: Obsolete procedure! Use newDocument instead.

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

WARNING: Obsolete procedure! Use newDocument instead.

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
      <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
      <translation>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</translation>
    </message>
    <message>
      <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
      <translation>masterPageNames()

Returns a list of the names of all master pages in the document.
</translation>
    </message>
    <message>
      <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
      <translation>editMasterPage(pageName)

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

Creates a new master page named pageName and opens it for
editing.
</translation>
    </message>
    <message>
      <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
      <translation>deleteMasterPage(pageName)

Delete the named master page.
</translation>
    </message>
    <message>
      <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
      <translation>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</translation>
    </message>
    <message>
      <source>getFillTransparency([&quot;name&quot;]) -> float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getFillTransparency([&quot;name&quot;]) -> float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getFillBlendmode([&quot;name&quot;]) -> integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getFillBlendmode([&quot;name&quot;]) -> integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getLineTransparency([&quot;name&quot;]) -> float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getLineTransparency([&quot;name&quot;]) -> float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getLineBlendmode([&quot;name&quot;]) -> integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getLineBlendmode([&quot;name&quot;]) -> integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerLocked(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerLocked(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerOutlined(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerOutlined(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerFlow(&quot;layer&quot;) -> bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerFlow(&quot;layer&quot;) -> bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>getLayerBlendmode(&quot;layer&quot;) -> int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>getLayerBlendmode(&quot;layer&quot;) -> int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>getLayerTransparency(&quot;layer&quot;) -> float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>getLayerTransparency(&quot;layer&quot;) -> float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>textFlowMode(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional int &quot;state&quot; (0 &lt;= state &lt;= 3).
Setting &quot;state&quot; to 0 will disable text flow.
Setting &quot;state&quot; to 1 will make text flow around object frame.
Setting &quot;state&quot; to 2 will make text flow around bounding box.
Setting &quot;state&quot; to 3 will make text flow around contour line.
If &quot;state&quot; is not passed, text flow is toggled.
</source>
      <translation>textFlowMode(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional int &quot;state&quot; (0 &lt;= state &lt;= 3).
Setting &quot;state&quot; to 0 will disable text flow.
Setting &quot;state&quot; to 1 will make text flow around object frame.
Setting &quot;state&quot; to 2 will make text flow around bounding box.
Setting &quot;state&quot; to 3 will make text flow around contour line.
If &quot;state&quot; is not passed, text flow is toggled.
</translation>
    </message>
    <message>
      <source>duplicateObject([&quot;name&quot;]) -> string

creates a Duplicate of the selected Object (or Selection Group).
</source>
      <translation>duplicateObject([&quot;name&quot;]) -> string

creates a Duplicate of the selected Object (or Selection Group).
</translation>
    </message>
    <message>
      <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
      <translation>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</translation>
    </message>
    <message>
      <source>getPageType() -> integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</source>
      <translation>getPageType() -> integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</translation>
    </message>
    <message>
      <source>getTextLines([&quot;name&quot;]) -> integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getTextLines([&quot;name&quot;]) -> integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
      <translation>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new colour &quot;name&quot;. The colour Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Colour components should be in
the range from 0 to 255.

May raise ValueError if an invalid colour name is specified.
</translation>
    </message>
    <message>
      <source>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
      <source>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</source>
      <translation>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</translation>
    </message>
    <message>
      <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
      <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the
printable set to false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the
printable set to false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerVisible(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerVisible(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting text at position -1 appends it to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
      <translation>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting text at position -1 appends it to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</translation>
    </message>
    <message>
      <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Qt::DockLeft(lr), Qt::DockRight(rr), Qt::DockTop(tr) and Qt::DockBottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</source>
      <translation>setMargins(lr, rr, tr, br)

Sets the margins of the document, Qt::DockLeft(lr), Qt::DockRight(rr), Qt::DockTop(tr) and Qt::DockBottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</translation>
    </message>
    <message>
      <source>setLayerOutlined(&quot;layer&quot;, outline)

Sets the layer &quot;layer&quot; to be locked or not. If outline is set to
true the layer will be displayed outlined.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerOutlined(&quot;layer&quot;, outline)

Sets the layer &quot;layer&quot; to be locked or not. If outline is set to
true the layer will be displayed outlined.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>setLayerFlow(&quot;layer&quot;, flow)

Sets the layers &quot;layer&quot;  flowcontrol to flow. If flow is set to
true text in layers above this one will flow around objects on this layer.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerFlow(&quot;layer&quot;, flow)

Sets the layers &quot;layer&quot;  flowcontrol to flow. If flow is set to
true text in layers above this one will flow around objects on this layer.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>setLayerBlendmode(&quot;layer&quot;, blend)

Sets the layers &quot;layer&quot;  blendmode to blend.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerBlendmode(&quot;layer&quot;, blend)

Sets the layers &quot;layer&quot;  blendmode to blend.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>setLayerTransparency(&quot;layer&quot;, trans)

Sets the layers &quot;layer&quot;  transparency to trans.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerTransparency(&quot;layer&quot;, trans)

Sets the layers &quot;layer&quot;  transparency to trans.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>setFillBlendmode(blendmode, [&quot;name&quot;])

Sets the fill blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>setFillBlendmode(blendmode, [&quot;name&quot;])

Sets the fill blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>setLineBlendmode(blendmode, [&quot;name&quot;])

Sets the line blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>setLineBlendmode(blendmode, [&quot;name&quot;])

Sets the line blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>placeEPS(&quot;filename&quot;, x, y)

Places the EPS &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the EPS placed on the page

If loading was successful, the selection contains the imported EPS
</source>
      <translation>placeEPS(&quot;filename&quot;, x, y)

Places the EPS &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the EPS placed on the page

If loading was successful, the selection contains the imported EPS
</translation>
    </message>
    <message>
      <source>placeSXD(&quot;filename&quot;, x, y)

Places the SXD &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SXD placed on the page

If loading was successful, the selection contains the imported SXD
</source>
      <translation>placeSXD(&quot;filename&quot;, x, y)

Places the SXD &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SXD placed on the page

If loading was successful, the selection contains the imported SXD
</translation>
    </message>
    <message>
      <source>placeODG(&quot;filename&quot;, x, y)

Places the ODG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the ODG placed on the page

If loading was successful, the selection contains the imported ODG
</source>
      <translation>placeODG(&quot;filename&quot;, x, y)

Places the ODG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the ODG placed on the page

If loading was successful, the selection contains the imported ODG
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
optional parameters is False.

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
optional parameters is False.

The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.
For example 'Images (*.png *.xpm *.jpg)'.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog('Open input', 'CSV files (*.csv)')
Example: fileDialog('Save report', defaultname='report.txt', issave=True)
</translation>
    </message>
    <message>
      <source>getLineJoin([&quot;name&quot;]) -> integer (see constants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
      <translation>getLineJoin([&quot;name&quot;]) -> integer (see constants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</translation>
    </message>
    <message>
      <source>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertex at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertex at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (currently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
      <translation>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (currently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</translation>
    </message>
    <message>
      <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets whether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets whether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</translation>
    </message>
  </context>
  <context>
    <name>AIPlug</name>
    <message>
      <source>Importing: %1</source>
      <translation>Importing: %1</translation>
    </message>
    <message>
      <source>Analyzing File:</source>
      <translation>Analysing File:</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>Generating Items</translation>
    </message>
  </context>
  <context>
    <name>About</name>
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
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>%1 %2 %3</source>
      <translation>%1 %2 %3</translation>
    </message>
    <message>
      <source>%3-%2-%1 %4 %5</source>
      <translation>%3-%2-%1 %4 %5</translation>
    </message>
    <message>
      <source>Using Ghostscript version %1</source>
      <translation>Using Ghostscript version %1</translation>
    </message>
    <message>
      <source>No Ghostscript version available</source>
      <translation>No Ghostscript version available</translation>
    </message>
    <message>
      <source>Build ID:</source>
      <translation>Build ID:</translation>
    </message>
    <message>
      <source>Mac OS&amp;#174; X Aqua Port:</source>
      <translation>Mac OS&amp;#174; X Aqua Port:</translation>
    </message>
    <message>
      <source>Windows&amp;#174; Port:</source>
      <translation>Windows&amp;#174; Port:</translation>
    </message>
    <message>
      <source>Tango Project Icons:</source>
      <translation>Tango Project Icons:</translation>
    </message>
    <message>
      <source>&amp;Updates</source>
      <translation>&amp;Updates</translation>
    </message>
    <message>
      <source>Check for &amp;Updates</source>
      <translation>Check for &amp;Updates</translation>
    </message>
    <message>
      <source>Check for updates to Scribus. No data from your machine will be transferred off it.</source>
      <translation>Check for updates to Scribus. No data from your machine will be transferred off it.</translation>
    </message>
    <message>
      <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
      <translation>OS/2&amp;#174;/eComStation&amp;#8482; Port:</translation>
    </message>
    <message>
      <source>Splash Screen:</source>
      <translation>Splash Screen:</translation>
    </message>
    <message>
      <source>This panel shows the version, build date and compiled in library support in Scribus.</source>
      <translation>This panel shows the version, build date and compiled in library support in Scribus.</translation>
    </message>
    <message>
      <source>The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.Last Letter is the renderer C=cairo or Q=Qt</source>
      <translation>The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.Last Letter is the renderer C=cairo or Q=Qt</translation>
    </message>
    <message>
      <source>Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
      <translation>Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</translation>
    </message>
    <message>
      <source>The Windows version does not use fontconfig or CUPS libraries.</source>
      <translation>The Windows version does not use fontconfig or CUPS libraries.</translation>
    </message>
    <message>
      <source>&lt;p align=&quot;center&quot;>&lt;b>%1 %2&lt;/b>&lt;/p>&lt;p align=&quot;center&quot;>%3&lt;br>%4 %5&lt;br>%6&lt;/p></source>
      <translation>&lt;p align=&quot;center&quot;>&lt;b>%1 %2&lt;/b>&lt;/p>&lt;p align=&quot;center&quot;>%3&lt;br>%4 %5&lt;br>%6&lt;/p></translation>
    </message>
    <message>
      <source>Scribus Version</source>
      <translation>Scribus Version</translation>
    </message>
  </context>
  <context>
    <name>AboutPlugins</name>
    <message>
      <source>Filename:</source>
      <translation>Filename:</translation>
    </message>
    <message>
      <source>Version:</source>
      <translation>Version:</translation>
    </message>
    <message>
      <source>Enabled:</source>
      <translation>Enabled:</translation>
    </message>
    <message>
      <source>Release Date:</source>
      <translation>Release Date:</translation>
    </message>
    <message>
      <source>Description:</source>
      <translation>Description:</translation>
    </message>
    <message>
      <source>Author(s):</source>
      <translation>Author(s):</translation>
    </message>
    <message>
      <source>Copyright:</source>
      <translation>Copyright:</translation>
    </message>
    <message>
      <source>License:</source>
      <translation>License:</translation>
    </message>
    <message>
      <source>Scribus: About Plug-ins</source>
      <translation>Scribus: About Plug-ins</translation>
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
      <source>&amp;Master Pages...</source>
      <translation>&amp;Master Pages...</translation>
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
    <message>
      <source>Print Previe&amp;w</source>
      <translation>Print Previe&amp;w</translation>
    </message>
    <message>
      <source>&amp;JavaScripts...</source>
      <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
      <source>Convert to Master Page...</source>
      <translation>Convert to Master Page...</translation>
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
      <source>&amp;About Plug-ins</source>
      <translation>&amp;About Plug-ins</translation>
    </message>
    <message>
      <source>More Info...</source>
      <translation>More Info...</translation>
    </message>
    <message>
      <source>&amp;Printing Enabled</source>
      <translation>&amp;Printing Enabled</translation>
    </message>
    <message>
      <source>&amp;Flip Horizontally</source>
      <translation>&amp;Flip Horizontally</translation>
    </message>
    <message>
      <source>&amp;Flip Vertically</source>
      <translation>&amp;Flip Vertically</translation>
    </message>
    <message>
      <source>Show Rulers</source>
      <translation>Show Rulers</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <comment>Document Outline Palette</comment>
      <translation>&amp;Outline</translation>
    </message>
    <message>
      <source>Solidus</source>
      <translation>Solidus</translation>
    </message>
    <message>
      <source>Middle Dot</source>
      <translation>Middle Dot</translation>
    </message>
    <message>
      <source>En Space</source>
      <translation>En Space</translation>
    </message>
    <message>
      <source>Em Space</source>
      <translation>Em Space</translation>
    </message>
    <message>
      <source>Thin Space</source>
      <translation>Thin Space</translation>
    </message>
    <message>
      <source>Thick Space</source>
      <translation>Thick Space</translation>
    </message>
    <message>
      <source>Mid Space</source>
      <translation>Mid Space</translation>
    </message>
    <message>
      <source>Hair Space</source>
      <translation>Hair Space</translation>
    </message>
    <message>
      <source>ff</source>
      <translation>ff</translation>
    </message>
    <message>
      <source>fi</source>
      <translation>fi</translation>
    </message>
    <message>
      <source>fl</source>
      <translation>fl</translation>
    </message>
    <message>
      <source>ffi</source>
      <translation>ffi</translation>
    </message>
    <message>
      <source>ffl</source>
      <translation>ffl</translation>
    </message>
    <message>
      <source>ft</source>
      <translation>ft</translation>
    </message>
    <message>
      <source>st</source>
      <translation>st</translation>
    </message>
    <message>
      <source>S&amp;tyles...</source>
      <translation>S&amp;tyles...</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <comment>type effect</comment>
      <translation>&amp;Outline</translation>
    </message>
    <message>
      <source>&amp;Outlines</source>
      <comment>Convert to oulines</comment>
      <translation>&amp;Outlines</translation>
    </message>
    <message>
      <source>Paste (&amp;Absolute)</source>
      <translation>Paste (&amp;Absolute)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>C&amp;lear</translation>
    </message>
    <message>
      <source>Insert PDF Push Button</source>
      <translation>Insert PDF Push Button</translation>
    </message>
    <message>
      <source>Insert PDF Text Field</source>
      <translation>Insert PDF Text Field</translation>
    </message>
    <message>
      <source>Insert PDF Check Box</source>
      <translation>Insert PDF Check Box</translation>
    </message>
    <message>
      <source>Insert PDF Combo Box</source>
      <translation>Insert PDF Combo Box</translation>
    </message>
    <message>
      <source>Insert PDF List Box</source>
      <translation>Insert PDF List Box</translation>
    </message>
    <message>
      <source>Insert Text Annotation</source>
      <translation>Insert Text Annotation</translation>
    </message>
    <message>
      <source>Insert Link Annotation</source>
      <translation>Insert Link Annotation</translation>
    </message>
    <message>
      <source>Save as &amp;EPS...</source>
      <translation>Save as &amp;EPS...</translation>
    </message>
    <message>
      <source>Show Text Frame Columns</source>
      <translation>Show Text Frame Columns</translation>
    </message>
    <message>
      <source>&amp;Frame...</source>
      <translation>&amp;Frame...</translation>
    </message>
    <message>
      <source>Preview Mode</source>
      <translation>Preview Mode</translation>
    </message>
    <message>
      <source>Show Layer Indicators</source>
      <translation>Show Layer Indicators</translation>
    </message>
    <message>
      <source>Patterns...</source>
      <translation>Patterns...</translation>
    </message>
    <message>
      <source>Send to Patterns</source>
      <translation>Send to Patterns</translation>
    </message>
    <message>
      <source>Sticky Tools</source>
      <translation>Sticky Tools</translation>
    </message>
    <message>
      <source>&amp;Fit to Height</source>
      <translation>&amp;Fit to Height</translation>
    </message>
    <message>
      <source>Fit to Width</source>
      <translation>Fit to Width</translation>
    </message>
    <message>
      <source>Show Bleeds</source>
      <translation>Show Bleeds</translation>
    </message>
    <message>
      <source>&amp;Zero Width Space</source>
      <translation>&amp;Zero Width Space</translation>
    </message>
    <message>
      <source>Zero Width NB Space</source>
      <translation>Zero Width NB Space</translation>
    </message>
    <message>
      <source>Apostrophe</source>
      <comment>Unicode 0x0027</comment>
      <translation>Apostrophe</translation>
    </message>
    <message>
      <source>Straight Double</source>
      <comment>Unicode 0x0022</comment>
      <translation>Straight Double</translation>
    </message>
    <message>
      <source>Single Left</source>
      <comment>Unicode 0x2018</comment>
      <translation>Single Left</translation>
    </message>
    <message>
      <source>Single Right</source>
      <comment>Unicode 0x2019</comment>
      <translation>Single Right</translation>
    </message>
    <message>
      <source>Double Left</source>
      <comment>Unicode 0x201C</comment>
      <translation>Double Left</translation>
    </message>
    <message>
      <source>Double Right</source>
      <comment>Unicode 0x201D</comment>
      <translation>Double Right</translation>
    </message>
    <message>
      <source>Single Reversed</source>
      <comment>Unicode 0x201B</comment>
      <translation>Single Reversed</translation>
    </message>
    <message>
      <source>Double Reversed</source>
      <comment>Unicode 0x201F</comment>
      <translation>Double Reversed</translation>
    </message>
    <message>
      <source>Single Left Guillemet</source>
      <comment>Unicode 0x2039</comment>
      <translation>Single Left Guillemet</translation>
    </message>
    <message>
      <source>Single Right Guillemet</source>
      <comment>Unicode 0x203A</comment>
      <translation>Single Right Guillemet</translation>
    </message>
    <message>
      <source>Double Left Guillemet</source>
      <comment>Unicode 0x00AB</comment>
      <translation>Double Left Guillemet</translation>
    </message>
    <message>
      <source>Double Right Guillemet</source>
      <comment>Unicode 0x00BB</comment>
      <translation>Double Right Guillemet</translation>
    </message>
    <message>
      <source>Low Single Comma</source>
      <comment>Unicode 0x201A</comment>
      <translation>Low Single Comma</translation>
    </message>
    <message>
      <source>Low Double Comma</source>
      <comment>Unicode 0x201E</comment>
      <translation>Low Double Comma</translation>
    </message>
    <message>
      <source>CJK Single Left</source>
      <comment>Unicode 0x300C</comment>
      <translation>CJK Single Left</translation>
    </message>
    <message>
      <source>CJK Single Right</source>
      <comment>Unicode 0x300D</comment>
      <translation>CJK Single Right</translation>
    </message>
    <message>
      <source>CJK Double Left</source>
      <comment>Unicode 0x300E</comment>
      <translation>CJK Double Left</translation>
    </message>
    <message>
      <source>CJK Double Right</source>
      <comment>Unicode 0x300F</comment>
      <translation>CJK Double Right</translation>
    </message>
    <message>
      <source>&amp;400%</source>
      <translation>&amp;400%</translation>
    </message>
    <message>
      <source>Edit Latex Source...</source>
      <translation>Edit Latex Source...</translation>
    </message>
    <message>
      <source>Insert &amp;Text Frame</source>
      <translation>Insert &amp;Text Frame</translation>
    </message>
    <message>
      <source>Insert &amp;Image Frame</source>
      <translation>Insert &amp;Image Frame</translation>
    </message>
    <message>
      <source>Insert &amp;Latex Frame</source>
      <translation>Insert &amp;Latex Frame</translation>
    </message>
    <message>
      <source>Insert T&amp;able</source>
      <translation>Insert T&amp;able</translation>
    </message>
    <message>
      <source>Insert &amp;Shape</source>
      <translation>Insert &amp;Shape</translation>
    </message>
    <message>
      <source>Insert &amp;Polygon</source>
      <translation>Insert &amp;Polygon</translation>
    </message>
    <message>
      <source>Insert &amp;Line</source>
      <translation>Insert &amp;Line</translation>
    </message>
    <message>
      <source>Insert &amp;Bezier Curve</source>
      <translation>Insert &amp;Bezier Curve</translation>
    </message>
    <message>
      <source>Insert &amp;Freehand Line</source>
      <translation>Insert &amp;Freehand Line</translation>
    </message>
    <message>
      <source>Scribus Homepage</source>
      <translation>Scribus Homepage</translation>
    </message>
    <message>
      <source>Scribus Online Documentation</source>
      <translation>Scribus Online Documentation</translation>
    </message>
    <message>
      <source>Scribus Wiki</source>
      <translation>Scribus Wiki</translation>
    </message>
    <message>
      <source>Getting Started with Scribus</source>
      <translation>Getting Started with Scribus</translation>
    </message>
  </context>
  <context>
    <name>AlignDistribute</name>
    <message>
      <source>Align</source>
      <translation>Align</translation>
    </message>
    <message>
      <source>&amp;Selected Guide:</source>
      <translation>&amp;Selected Guide:</translation>
    </message>
    <message>
      <source>&amp;Relative To:</source>
      <translation>&amp;Relative To:</translation>
    </message>
    <message>
      <source>...</source>
      <translation>...</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>Distribute</translation>
    </message>
    <message>
      <source>&amp;Distance:</source>
      <translation>&amp;Distance:</translation>
    </message>
  </context>
  <context>
    <name>AlignDistributePalette</name>
    <message>
      <source>Align and Distribute</source>
      <translation>Align and Distribute</translation>
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
      <source>Align bottoms</source>
      <translation>Align bottoms</translation>
    </message>
    <message>
      <source>Align right sides</source>
      <translation>Align right sides</translation>
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
      <source>Align tops</source>
      <translation>Align tops</translation>
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
      <source>Guide</source>
      <translation>Guide</translation>
    </message>
    <message>
      <source>&amp;Selected Guide:</source>
      <translation>&amp;Selected Guide:</translation>
    </message>
    <message>
      <source>&amp;Distance:</source>
      <translation>&amp;Distance:</translation>
    </message>
    <message>
      <source>Distribute the items with the distance specified</source>
      <translation>Distribute the items with the distance specified</translation>
    </message>
    <message>
      <source>None Selected</source>
      <translation>None Selected</translation>
    </message>
    <message>
      <source>Y: %1%2</source>
      <translation>Y: %1%2</translation>
    </message>
    <message>
      <source>X: %1%2</source>
      <translation>X: %1%2</translation>
    </message>
    <message>
      <source>Align right sides of items to left side of anchor</source>
      <translation>Align right sides of items to left side of anchor</translation>
    </message>
    <message>
      <source>Align left sides of items to right side of anchor</source>
      <translation>Align left sides of items to right side of anchor</translation>
    </message>
    <message>
      <source>Align tops of items to bottom of anchor</source>
      <translation>Align tops of items to bottom of anchor</translation>
    </message>
    <message>
      <source>Align bottoms of items to top of anchor</source>
      <translation>Align bottoms of items to top of anchor</translation>
    </message>
    <message>
      <source>Make horizontal gaps between items equal</source>
      <translation>Make horizontal gaps between items equal</translation>
    </message>
    <message>
      <source>Make horizontal gaps between items equal to the value specified</source>
      <translation>Make horizontal gaps between items equal to the value specified</translation>
    </message>
    <message>
      <source>Make vertical gaps between items equal</source>
      <translation>Make vertical gaps between items equal</translation>
    </message>
    <message>
      <source>Make vertical gaps between items equal to the value specified</source>
      <translation>Make vertical gaps between items equal to the value specified</translation>
    </message>
    <message>
      <source>Make horizontal gaps between items and sides of page equal</source>
      <translation>Make horizontal gaps between items and sides of page equal</translation>
    </message>
    <message>
      <source>Make vertical gaps between items and the top and bottom of page margins equal</source>
      <translation>Make vertical gaps between items and the top and bottom of page margins equal</translation>
    </message>
    <message>
      <source>Make horizontal gaps between items and sides of page margins equal</source>
      <translation>Make horizontal gaps between items and sides of page margins equal</translation>
    </message>
    <message>
      <source>Make vertical gaps between items and the top and bottom of page equal</source>
      <translation>Make vertical gaps between items and the top and bottom of page equal</translation>
    </message>
    <message>
      <source>Align</source>
      <translation>Align</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>Distribute</translation>
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
    <message>
      <source>JavaScript</source>
      <translation>JavaScript</translation>
    </message>
    <message>
      <source>None</source>
      <comment>highlight</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>None</source>
      <comment>action</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Tooltip:</source>
      <translation>Tooltip:</translation>
    </message>
    <message>
      <source>Do Not Export Value</source>
      <translation>Do Not Export Value</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)</source>
      <translation>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;%1;;All Files (*)</source>
      <translation>Images (*.tif *.png *.jpg *.xpm);;%1;;All Files (*)</translation>
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
      <source>%1;;All Files (*)</source>
      <translation>%1;;All Files (*)</translation>
    </message>
  </context>
  <context>
    <name>ApplyMasterPageDialog</name>
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
      <source>to</source>
      <translation>to</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Apply the selected master page to even, odd or all pages within the following range</source>
      <translation>Apply the selected master page to even, odd or all pages within the following range</translation>
    </message>
    <message>
      <source>Possible Hyphenation</source>
      <translation>Possible Hyphenation</translation>
    </message>
  </context>
  <context>
    <name>Barcode</name>
    <message>
      <source>&amp;Barcode Generator...</source>
      <translation>&amp;Barcode Generator...</translation>
    </message>
    <message>
      <source>Scribus frontend for Pure Postscript Barcode Writer</source>
      <translation>Scribus frontend for Pure Postscript Barcode Writer</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGenerator</name>
    <message>
      <source>Error opening file: %1</source>
      <translation>Error opening file: %1</translation>
    </message>
    <message>
      <source>12 or 13 digits</source>
      <translation>12 or 13 digits</translation>
    </message>
    <message>
      <source>8 digits</source>
      <translation>8 digits</translation>
    </message>
    <message>
      <source>11 or 12 digits</source>
      <translation>11 or 12 digits</translation>
    </message>
    <message>
      <source>7 or 8 digits</source>
      <translation>7 or 8 digits</translation>
    </message>
    <message>
      <source>5 digits</source>
      <translation>5 digits</translation>
    </message>
    <message>
      <source>2 digits</source>
      <translation>2 digits</translation>
    </message>
    <message>
      <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
      <translation>Variable number of characters, digits and any of the symbols -. *$/+%.</translation>
    </message>
    <message>
      <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
      <translation>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</translation>
    </message>
    <message>
      <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
      <translation>Variable number of digits and any of the symbols -$:/.+ABCD.</translation>
    </message>
    <message>
      <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
      <translation>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</translation>
    </message>
    <message>
      <source>Variable number of digits</source>
      <translation>Variable number of digits</translation>
    </message>
    <message>
      <source>Variable number of digits and capital letters</source>
      <translation>Variable number of digits and capital letters</translation>
    </message>
    <message>
      <source>Variable number of hexadecimal characters</source>
      <translation>Variable number of hexadecimal characters</translation>
    </message>
    <message>
      <source>Barcode incomplete</source>
      <translation>Barcode incomplete</translation>
    </message>
    <message>
      <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -> 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
      <translation>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -> 978-1-56592-479. The final check-digit will be calculated automatically.)</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGeneratorBase</name>
    <message>
      <source>Barcode Creator</source>
      <translation>Barcode Creator</translation>
    </message>
    <message>
      <source>Barcode</source>
      <translation>Barcode</translation>
    </message>
    <message>
      <source>&amp;Type:</source>
      <translation>&amp;Type:</translation>
    </message>
    <message>
      <source>Select one of the available barcode type here</source>
      <translation>Select one of the available barcode type here</translation>
    </message>
    <message>
      <source>The numeric representation of the code itself. See the help message below</source>
      <translation>The numeric representation of the code itself. See the help message below</translation>
    </message>
    <message>
      <source>Reset the barcode samples</source>
      <translation>Reset the barcode samples</translation>
    </message>
    <message>
      <source>&amp;Include text in barcode</source>
      <translation>&amp;Include text in barcode</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation>Alt+I</translation>
    </message>
    <message>
      <source>If checked, there will be numbers in the barcode too</source>
      <translation>If checked, there will be numbers in the barcode too</translation>
    </message>
    <message>
      <source>&amp;Guard whitespace</source>
      <translation>&amp;Guard whitespace</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Draw arrows to be sure of space next the code</source>
      <translation>Draw arrows to be sure of space next the code</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
    <message>
      <source>&amp;Background</source>
      <translation>&amp;Background</translation>
    </message>
    <message>
      <source>Alt+B</source>
      <translation>Alt+B</translation>
    </message>
    <message>
      <source>Background color - under the code lines</source>
      <translation>Background colour - under the code lines</translation>
    </message>
    <message>
      <source>&amp;Lines</source>
      <translation>&amp;Lines</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Color of the lines in barcode</source>
      <translation>Colour of the lines in barcode</translation>
    </message>
    <message>
      <source>&amp;Text</source>
      <translation>&amp;Text</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Color of the text and numbers</source>
      <translation>Colour of the text and numbers</translation>
    </message>
    <message>
      <source>Hints and help is shown here</source>
      <translation>Hints and help is shown here</translation>
    </message>
    <message>
      <source>Preview of the result. 72dpi sample.</source>
      <translation>Preview of the result. 72dpi sample.</translation>
    </message>
    <message>
      <source>Co&amp;de:</source>
      <translation>Co&amp;de:</translation>
    </message>
    <message>
      <source>I&amp;nclude checksum</source>
      <translation>I&amp;nclude checksum</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Generate and include a checksum in barcode</source>
      <translation>Generate and include a checksum in barcode</translation>
    </message>
    <message>
      <source>Incl&amp;ude checksum digit</source>
      <translation>Incl&amp;ude checksum digit</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Include the checksum digit in the barcode text</source>
      <translation>Include the checksum digit in the barcode text</translation>
    </message>
  </context>
  <context>
    <name>Biblio</name>
    <message>
      <source>Scrapbook</source>
      <translation>Scrapbook</translation>
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
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
      <translation>Name &quot;%1&quot; is not unique.
Please choose another.</translation>
    </message>
    <message>
      <source>Choose a Scrapbook Directory</source>
      <translation>Choose a Scrapbook Directory</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>Choose a scrapbook file to import</source>
      <translation>Choose a scrapbook file to import</translation>
    </message>
    <message>
      <source>Create a new scrapbook page</source>
      <translation>Create a new scrapbook page</translation>
    </message>
    <message>
      <source>Load an existing scrapbook</source>
      <translation>Load an existing scrapbook</translation>
    </message>
    <message>
      <source>Save the selected scrapbook</source>
      <translation>Save the selected scrapbook</translation>
    </message>
    <message>
      <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
      <translation>Import an scrapbook file from Scribus &lt;=1.3.2</translation>
    </message>
    <message>
      <source>Close the selected scrapbook</source>
      <translation>Close the selected scrapbook</translation>
    </message>
    <message>
      <source>Copy To:</source>
      <translation>Copy To:</translation>
    </message>
    <message>
      <source>Move To:</source>
      <translation>Move To:</translation>
    </message>
    <message>
      <source>Main</source>
      <translation>Main</translation>
    </message>
    <message>
      <source>Copied Items</source>
      <translation>Copied Items</translation>
    </message>
    <message>
      <source>New Name</source>
      <translation>New Name</translation>
    </message>
    <message>
      <source>Scrapbook (*.scs *.SCS)</source>
      <translation>Scrapbook (*.scs *.SCS)</translation>
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
      <source>&amp;Monitor:</source>
      <translation>&amp;Monitor:</translation>
    </message>
    <message>
      <source>P&amp;rinter:</source>
      <translation>P&amp;rinter:</translation>
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
    <message>
      <source>&amp;RGB Pictures:</source>
      <translation>&amp;RGB Pictures:</translation>
    </message>
    <message>
      <source>&amp;CMYK Pictures:</source>
      <translation>&amp;CMYK Pictures:</translation>
    </message>
    <message>
      <source>Default color profile for imported CMYK images</source>
      <translation>Default colour profile for imported CMYK images</translation>
    </message>
    <message>
      <source>Default color profile for imported RGB images</source>
      <translation>Default colour profile for imported RGB images</translation>
    </message>
    <message>
      <source>&amp;RGB Solid Colors:</source>
      <translation>&amp;RGB Solid Colours:</translation>
    </message>
    <message>
      <source>&amp;CMYK Solid Colors:</source>
      <translation>&amp;CMYK Solid Colours:</translation>
    </message>
    <message>
      <source>Pictures:</source>
      <translation>Pictures:</translation>
    </message>
    <message>
      <source>Sol&amp;id Colors:</source>
      <translation>Sol&amp;id Colours:</translation>
    </message>
    <message>
      <source>Convert all colors to printer space</source>
      <translation>Convert all colours to printer space</translation>
    </message>
    <message>
      <source>Default color profile for solid RGB colors on the page</source>
      <translation>Default colour profile for solid RGB colours on the page</translation>
    </message>
    <message>
      <source>Default color profile for solid CMYK colors on the page</source>
      <translation>Default colour profile for solid CMYK colours on the page</translation>
    </message>
    <message>
      <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation>Default rendering intent for solid colours. Unless you know why to change it,
Relative Colourimetric or Perceptual should be chosen.</translation>
    </message>
    <message>
      <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation>Default rendering intent for images. Unless you know why to change it,
Relative Colourimetric or Perceptual should be chosen.</translation>
    </message>
    <message>
      <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
      <translation>Simulate a full colour managed environment :
all colours, rgb or cmyk, are converted to printer colour space.</translation>
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
      <source>Is Spot Color</source>
      <translation>Is Spot Colour</translation>
    </message>
    <message>
      <source>Is Registration Color</source>
      <translation>Is Registration Colour</translation>
    </message>
    <message>
      <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
      <translation>You cannot create a colour named &quot;%1&quot;.
It is a reserved name for transparent colour</translation>
    </message>
    <message>
      <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
      <translation>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</translation>
    </message>
    <message>
      <source>The name of the color already exists,
please choose another one.</source>
      <translation>The name of the colour already exists,
please choose another one.</translation>
    </message>
    <message>
      <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
      <translation>Choosing this will make this colour a spot colour, thus creating another spot when creating plates or separations. This is used most often when a logo or other colour needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</translation>
    </message>
    <message>
      <source>If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management.</source>
      <translation>If colour management is enabled, a triangle warning indicator is a warning that the colour maybe outside of the colour gamut of the current printer profile selected. What this means is the colour may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Colour Management.</translation>
    </message>
  </context>
  <context>
    <name>CWDialog</name>
    <message>
      <source>Merging colors</source>
      <translation>Merging colours</translation>
    </message>
    <message>
      <source>Error: </source>
      <translation>Error: </translation>
    </message>
    <message>
      <source>Color %1 exists already!</source>
      <translation>Colour %1 exists already!</translation>
    </message>
    <message>
      <source>Color %1 appended.</source>
      <translation>Colour %1 appended.</translation>
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
      <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
      <translation>Unable to find the requested colour. You have probably selected black, grey or white. There is no way to process this colour.</translation>
    </message>
    <message>
      <source>Color Wheel</source>
      <translation>Colour Wheel</translation>
    </message>
    <message>
      <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
      <translation>Click the wheel to get the base colour. Its colour model depends on the chosen tab.</translation>
    </message>
    <message>
      <source>Result Colors</source>
      <translation>Result Colors</translation>
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
      <source>HSV</source>
      <translation>HSV</translation>
    </message>
    <message>
      <source>Colors of your chosen color scheme.</source>
      <translation>Colours of your chosen colour scheme.</translation>
    </message>
    <message>
      <source>Color Scheme Method</source>
      <translation>Colour Scheme Method</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation>Angle:</translation>
    </message>
    <message>
      <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
      <translation>Difference between the selected value and the counted ones. Refer to documentation for more information.</translation>
    </message>
    <message>
      <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
      <translation>Select one of the methods to create a colour scheme. Refer to documentation for more information.</translation>
    </message>
    <message>
      <source>Merge created colors into the document colors</source>
      <translation>Merge created colours into the document colours</translation>
    </message>
    <message>
      <source>&amp;Merge</source>
      <translation>&amp;Merge</translation>
    </message>
    <message>
      <source>Alt+M</source>
      <translation>Alt+M</translation>
    </message>
    <message>
      <source>Replace created colors in the document colors</source>
      <translation>Replace created colours in the document colours</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>&amp;Replace</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Leave colors untouched</source>
      <translation>Leave colours untouched</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Preview:</source>
      <translation>Preview:</translation>
    </message>
    <message>
      <source>Sample color scheme.</source>
      <translation>Sample colour scheme.</translation>
    </message>
    <message>
      <source>Simulate common vision defects here. Select type of the defect.</source>
      <translation>Simulate common vision defects here. Select type of the defect.</translation>
    </message>
    <message>
      <source>Vision Defect Type:</source>
      <translation>Vision Defect Type:</translation>
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
      <source>RGB:</source>
      <translation>RGB:</translation>
    </message>
    <message>
      <source>HSV:</source>
      <translation>HSV:</translation>
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
      <source>CMYK:</source>
      <translation>CMYK:</translation>
    </message>
    <message>
      <source>H:</source>
      <translation>H:</translation>
    </message>
    <message>
      <source>S:</source>
      <translation>S:</translation>
    </message>
    <message>
      <source>V:</source>
      <translation>V:</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Document</translation>
    </message>
  </context>
  <context>
    <name>CharSelect</name>
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
      <source>Insert the characters at the cursor in the text</source>
      <translation>Insert the characters at the cursor in the text</translation>
    </message>
    <message>
      <source>Delete the current selection(s).</source>
      <translation>Delete the current selection(s).</translation>
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
    <message>
      <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
      <translation>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</translation>
    </message>
    <message>
      <source>Scribus Char Palette (*.ucp);;All Files (*)</source>
      <translation>Scribus Char Palette (*.ucp);;All Files (*)</translation>
    </message>
    <message>
      <source>Enhanced Palette</source>
      <translation>Enhanced Palette</translation>
    </message>
    <message>
      <source>Quick Palette</source>
      <translation>Quick Palette</translation>
    </message>
    <message>
      <source>Hide Enhanced</source>
      <translation>Hide Enhanced</translation>
    </message>
    <message>
      <source>Choose a filename to open</source>
      <translation>Choose a filename to open</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>Error</translation>
    </message>
    <message>
      <source>Error reading file %1 - file is corrupted propably.</source>
      <translation>Error reading file %1 - file is corrupted propably.</translation>
    </message>
    <message>
      <source>Choose a filename to save under</source>
      <translation>Choose a filename to save under</translation>
    </message>
    <message>
      <source>Cannot write file %1</source>
      <translation>Cannot write file %1</translation>
    </message>
    <message>
      <source>Clean the Palette?</source>
      <translation>Clean the Palette?</translation>
    </message>
    <message>
      <source>You will clean all characters from this palette. Are you sure?</source>
      <translation>You will clean all characters from this palette. Are you sure?</translation>
    </message>
    <message>
      <source>Character Palette</source>
      <translation>Character Palette</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
  </context>
  <context>
    <name>CharStyleComboBox</name>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
  </context>
  <context>
    <name>CharTableView</name>
    <message>
      <source>Delete</source>
      <translation>Delete</translation>
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
    <message>
      <source>Check again</source>
      <translation>Check again</translation>
    </message>
    <message>
      <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
      <translation>Image resolution below %1 DPI, currently %2 x %3 DPI</translation>
    </message>
    <message>
      <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
      <translation>Image resolution above %1 DPI, currently %2 x %3 DPI</translation>
    </message>
    <message>
      <source>Image is GIF</source>
      <translation>Image is GIF</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Transparency used</source>
      <translation>Transparency used</translation>
    </message>
    <message>
      <source>Blendmode used</source>
      <translation>Blendmode used</translation>
    </message>
    <message>
      <source>Layer &quot;%1&quot;</source>
      <translation>Layer &quot;%1&quot;</translation>
    </message>
    <message>
      <source>Annotation uses a non TrueType font</source>
      <translation>Annotation uses a non TrueType font</translation>
    </message>
  </context>
  <context>
    <name>CollectForOutput</name>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>Collecting...</source>
      <translation>Collecting...</translation>
    </message>
    <message>
      <source>Cannot collect all files for output for file:
%1</source>
      <translation>Cannot collect all files for output for file:
%1</translation>
    </message>
    <message>
      <source>Cannot collect the file: 
%1</source>
      <translation>Cannot collect the file: 
%1</translation>
    </message>
  </context>
  <context>
    <name>ColorManager</name>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>&amp;Import</translation>
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
      <source>Color Sets</source>
      <translation>Colour Sets</translation>
    </message>
    <message>
      <source>Current Color Set:</source>
      <translation>Current Colour Set:</translation>
    </message>
    <message>
      <source>&amp;Save Color Set</source>
      <translation>&amp;Save Colour Set</translation>
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
      <source>Import colors to the current set from an existing document</source>
      <translation>Import colours to the current set from an existing document</translation>
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
      <source>&amp;Name:</source>
      <translation>&amp;Name:</translation>
    </message>
    <message>
      <source>Choose a Name</source>
      <translation>Choose a Name</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Copy of %1</translation>
    </message>
    <message>
      <source>New Color</source>
      <translation>New Colour</translation>
    </message>
    <message>
      <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
      <translation>If colour management is enabled, a triangle warning indicator is a warning the the colour maybe outside of the colour gamut of the current printer profile selected.What this means is the colour may not print exactly as indicated on screen. Spot colours are indicated by a red circle. Registration colours will have a registration mark next to the colour. More hints about gamut warnings are in the online help under Colour Management.</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>Information</translation>
    </message>
    <message>
      <source>The file %1 does not contain colors which can be imported.
If the file was a PostScript-based, try to import it with File -&amp;gt; Import. 
Not all files have DSC conformant comments where the color descriptions are located.
 This prevents importing colors from some files.
See the Edit Colors section of the documentation for more details.</source>
      <translation>The file %1 does not contain colours which can be imported.
If the file was a PostScript-based, try to import it with File -&amp;gt; Import. 
Not all files have DSC conformant comments where the colour descriptions are located.
 This prevents importing colours from some files.
See the Edit Colours section of the documentation for more details.</translation>
    </message>
    <message>
      <source>Import</source>
      <translation>Import</translation>
    </message>
    <message>
      <source>Documents (%1);;Other Files (%2);;All Files (*)</source>
      <translation>Documents (%1);;Other Files (%2);;All Files (*)</translation>
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
    <name>ColorWheelPlugin</name>
    <message>
      <source>&amp;Color Wheel...</source>
      <translation>&amp;Colour Wheel...</translation>
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
      <translation>&amp;Apply</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Warning</translation>
    </message>
    <message>
      <source>None</source>
      <comment>color name</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Custom</source>
      <comment>CommonStrings, custom page size</comment>
      <translation>Custom</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>Single Page</translation>
    </message>
    <message>
      <source>Double Sided</source>
      <translation>Double Sided</translation>
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
      <source>Monday</source>
      <translation>Monday</translation>
    </message>
    <message>
      <source>Tuesday</source>
      <translation>Tuesday</translation>
    </message>
    <message>
      <source>Wednesday</source>
      <translation>Wednesday</translation>
    </message>
    <message>
      <source>Thursday</source>
      <translation>Thursday</translation>
    </message>
    <message>
      <source>Friday</source>
      <translation>Friday</translation>
    </message>
    <message>
      <source>Saturday</source>
      <translation>Saturday</translation>
    </message>
    <message>
      <source>Sunday</source>
      <translation>Sunday</translation>
    </message>
    <message>
      <source>January</source>
      <translation>January</translation>
    </message>
    <message>
      <source>February</source>
      <translation>February</translation>
    </message>
    <message>
      <source>March</source>
      <translation>March</translation>
    </message>
    <message>
      <source>April</source>
      <translation>April</translation>
    </message>
    <message>
      <source>May</source>
      <translation>May</translation>
    </message>
    <message>
      <source>June</source>
      <translation>June</translation>
    </message>
    <message>
      <source>July</source>
      <translation>July</translation>
    </message>
    <message>
      <source>August</source>
      <translation>August</translation>
    </message>
    <message>
      <source>September</source>
      <translation>September</translation>
    </message>
    <message>
      <source>October</source>
      <translation>October</translation>
    </message>
    <message>
      <source>November</source>
      <translation>November</translation>
    </message>
    <message>
      <source>December</source>
      <translation>December</translation>
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
      <source>&amp;Yes</source>
      <translation>&amp;Yes</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;No</translation>
    </message>
    <message>
      <source>Left Page</source>
      <comment>Left page location</comment>
      <translation>Left Page</translation>
    </message>
    <message>
      <source>Middle</source>
      <comment>Middle page location</comment>
      <translation>Middle</translation>
    </message>
    <message>
      <source>Middle Left</source>
      <comment>Middle Left page location</comment>
      <translation>Middle Left</translation>
    </message>
    <message>
      <source>Middle Right</source>
      <comment>Middle Right page location</comment>
      <translation>Middle Right</translation>
    </message>
    <message>
      <source>Right Page</source>
      <comment>Right page location</comment>
      <translation>Right Page</translation>
    </message>
    <message>
      <source>Normal</source>
      <comment>Default single master page</comment>
      <translation>Normal</translation>
    </message>
    <message>
      <source>Normal Left</source>
      <comment>Default left master page</comment>
      <translation>Normal Left</translation>
    </message>
    <message>
      <source>Normal Middle</source>
      <comment>Default middle master page</comment>
      <translation>Normal Middle</translation>
    </message>
    <message>
      <source>Normal Right</source>
      <comment>Default right master page</comment>
      <translation>Normal Right</translation>
    </message>
    <message>
      <source>Normal Vision</source>
      <comment>Color Blindness - Normal Vision</comment>
      <translation>Normal Vision</translation>
    </message>
    <message>
      <source>Protanopia (Red)</source>
      <comment>Color Blindness - Red Color Blind</comment>
      <translation>Protanopia (Red)</translation>
    </message>
    <message>
      <source>Deuteranopia (Green)</source>
      <comment>Color Blindness - Greed Color Blind</comment>
      <translation>Deuteranopia (Green)</translation>
    </message>
    <message>
      <source>Tritanopia (Blue)</source>
      <comment>Color Blindness - Blue Color Blind</comment>
      <translation>Tritanopia (Blue)</translation>
    </message>
    <message>
      <source>Full Color Blindness</source>
      <comment>Color Blindness - Full Color Blindness</comment>
      <translation>Full Colour Blindness</translation>
    </message>
    <message>
      <source>Custom: </source>
      <comment>Custom Tab Fill Option</comment>
      <translation>Custom: </translation>
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
      <source>None</source>
      <comment>Optical Margin Setting</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Left Protruding</source>
      <comment>Optical Margin Setting</comment>
      <translation>Left Protruding</translation>
    </message>
    <message>
      <source>Right Protruding</source>
      <comment>Optical Margin Setting</comment>
      <translation>Right Protruding</translation>
    </message>
    <message>
      <source>Left Hanging Punctuation</source>
      <comment>Optical Margin Setting</comment>
      <translation>Left Hanging Punctuation</translation>
    </message>
    <message>
      <source>Right Hanging Punctuation</source>
      <comment>Optical Margin Setting</comment>
      <translation>Right Hanging Punctuation</translation>
    </message>
    <message>
      <source>Default</source>
      <comment>Optical Margin Setting</comment>
      <translation>Default</translation>
    </message>
    <message>
      <source>Min. Word Tracking</source>
      <translation>Min. Word Tracking</translation>
    </message>
    <message>
      <source>Max. Word Tracking</source>
      <translation>Max. Word Tracking</translation>
    </message>
    <message>
      <source>Min. Glyph Extension</source>
      <translation>Min. Glyph Extension</translation>
    </message>
    <message>
      <source>Max. Glyph Extension</source>
      <translation>Max. Glyph Extension</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
    </message>
    <message>
      <source>RGB</source>
      <comment>Colorspace</comment>
      <translation>RGB</translation>
    </message>
    <message>
      <source>CMYK</source>
      <comment>Colorspace</comment>
      <translation>CMYK</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <comment>Colorspace</comment>
      <translation>Greyscale</translation>
    </message>
    <message>
      <source>Duotone</source>
      <comment>Colorspace</comment>
      <translation>Duotone</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>Colorspace (Unknown)</comment>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
  </context>
  <context>
    <name>CopyPageToMasterPageBase</name>
    <message>
      <source>Convert Page to Master Page</source>
      <translation>Convert Page to Master Page</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Name:</translation>
    </message>
    <message>
      <source>Copy Applied Master Page Items</source>
      <translation>Copy Applied Master Page Items</translation>
    </message>
  </context>
  <context>
    <name>CopyPageToMasterPageDialog</name>
    <message>
      <source>New Master Page %1</source>
      <translation>New Master Page %1</translation>
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
    <message>
      <source>Transparency Settings</source>
      <translation>Transparency Settings</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Blend Mode:</translation>
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
      <source>Multiply</source>
      <translation>Multiply</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Screen</translation>
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
      <source>Color Dodge</source>
      <translation>Colour Dodge</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>Colour Burn</translation>
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
      <source>Offsets</source>
      <translation>Offsets</translation>
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
      <source>Scaling</source>
      <translation>Scaling</translation>
    </message>
    <message>
      <source>Rotation</source>
      <translation>Rotation</translation>
    </message>
    <message>
      <source>Angle</source>
      <translation>Angle</translation>
    </message>
    <message>
      <source>Pattern</source>
      <translation>Pattern</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>Exclusion</translation>
    </message>
    <message>
      <source>X-Scale:</source>
      <translation>X-Scale:</translation>
    </message>
    <message>
      <source>Y-Scale:</source>
      <translation>Y-Scale:</translation>
    </message>
  </context>
  <context>
    <name>CreateRange</name>
    <message>
      <source>Create Range</source>
      <translation>Create Range</translation>
    </message>
    <message>
      <source>Number of Pages in Document:</source>
      <translation>Number of Pages in Document:</translation>
    </message>
    <message>
      <source>Doc Page Range</source>
      <translation>Doc Page Range</translation>
    </message>
    <message>
      <source>Basic Range Selection</source>
      <translation>Basic Range Selection</translation>
    </message>
    <message>
      <source>Range of Pages</source>
      <translation>Range of Pages</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation>De&amp;lete</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Move &amp;Down</source>
      <translation>Move &amp;Down</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Move &amp;Up</source>
      <translation>Move &amp;Up</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Add a Range of Pages</source>
      <translation>Add a Range of Pages</translation>
    </message>
    <message>
      <source>Consecutive Pages</source>
      <translation>Consecutive Pages</translation>
    </message>
    <message>
      <source>Even Pages</source>
      <translation>Even Pages</translation>
    </message>
    <message>
      <source>From:</source>
      <translation>From:</translation>
    </message>
    <message>
      <source>To:</source>
      <translation>To:</translation>
    </message>
    <message>
      <source>&amp;Add To Range</source>
      <translation>&amp;Add To Range</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Odd Pages</source>
      <translation>Odd Pages</translation>
    </message>
    <message>
      <source>Comma Separated List</source>
      <translation>Comma Separated List</translation>
    </message>
    <message>
      <source>Advanced Reordering</source>
      <translation>Advanced Reordering</translation>
    </message>
    <message>
      <source>Page Order</source>
      <translation>Page Order</translation>
    </message>
    <message>
      <source>Sample Page Order:</source>
      <translation>Sample Page Order:</translation>
    </message>
    <message>
      <source>Page Group Size:</source>
      <translation>Page Group Size:</translation>
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
      <comment>delimiter</comment>
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
      <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help > About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
      <translation>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help > About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</translation>
    </message>
  </context>
  <context>
    <name>CurveWidget</name>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Curve Files (*.scu);;All Files (*)</source>
      <translation>Curve Files (*.scu);;All Files (*)</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>Cannot write the file: 
%1</translation>
    </message>
    <message>
      <source>Inverts the curve</source>
      <translation>Inverts the curve</translation>
    </message>
    <message>
      <source>Resets the curve</source>
      <translation>Resets the curve</translation>
    </message>
    <message>
      <source>Switches between linear and cubic interpolation of the curve</source>
      <translation>Switches between linear and cubic interpolation of the curve</translation>
    </message>
    <message>
      <source>Loads a curve</source>
      <translation>Loads a curve</translation>
    </message>
    <message>
      <source>Saves this curve</source>
      <translation>Saves this curve</translation>
    </message>
  </context>
  <context>
    <name>CustomFDialog</name>
    <message>
      <source>Encoding:</source>
      <translation>Encoding:</translation>
    </message>
    <message>
      <source>&amp;Compress File</source>
      <translation>&amp;Compress File</translation>
    </message>
    <message>
      <source>&amp;Include Fonts</source>
      <translation>&amp;Include Fonts</translation>
    </message>
    <message>
      <source>Show Preview</source>
      <translation>Show Preview</translation>
    </message>
    <message>
      <source>&amp;Include Color Profiles</source>
      <translation>&amp;Include Colour Profiles</translation>
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
      <source>Delete From:</source>
      <translation>Delete From:</translation>
    </message>
  </context>
  <context>
    <name>DocIm</name>
    <message>
      <source>Importing failed</source>
      <translation>Importing failed</translation>
    </message>
    <message>
      <source>Importing Word document failed 
%1</source>
      <translation>Importing Word document failed 
%1</translation>
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
    <name>DocSections</name>
    <message>
      <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
      <translation>Add a page numbering section to the document. The new section will be added after the currently selected section.</translation>
    </message>
    <message>
      <source>Delete the currently selected section.</source>
      <translation>Delete the currently selected section.</translation>
    </message>
    <message>
      <source>1, 2, 3, ...</source>
      <translation>1, 2, 3, ...</translation>
    </message>
    <message>
      <source>i, ii, iii, ...</source>
      <translation>i, ii, iii, ...</translation>
    </message>
    <message>
      <source>I, II, III, ...</source>
      <translation>I, II, III, ...</translation>
    </message>
    <message>
      <source>a, b, c, ...</source>
      <translation>a, b, c, ...</translation>
    </message>
    <message>
      <source>A, B, C, ...</source>
      <translation>A, B, C, ...</translation>
    </message>
    <message>
      <source>&lt;b>Name:&lt;/b> Optional name for section eg. Index&lt;br/>&lt;b>Shown:&lt;/b> Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/>&lt;b>From:&lt;/b> The page index for this section to start at.&lt;br/>&lt;b>To:&lt;/b> The page index for this section to stop at.&lt;br/>&lt;b>Style:&lt;/b> Select the page number style to be used.&lt;br/>&lt;b>Start:&lt;/b> The index within the Style's range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
      <translation>&lt;b>Name:&lt;/b> Optional name for section eg. Index&lt;br/>&lt;b>Shown:&lt;/b> Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/>&lt;b>From:&lt;/b> The page index for this section to start at.&lt;br/>&lt;b>To:&lt;/b> The page index for this section to stop at.&lt;br/>&lt;b>Style:&lt;/b> Select the page number style to be used.&lt;br/>&lt;b>Start:&lt;/b> The index within the Style's range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</translation>
    </message>
    <message>
      <source>Page Number Out Of Bounds</source>
      <translation>Page Number Out Of Bounds</translation>
    </message>
    <message>
      <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
      <translation>The value you have entered is outside the range of page numbers in the current document (%1-%2).</translation>
    </message>
    <message>
      <source>Document Sections</source>
      <translation>Document Sections</translation>
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
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Shown</source>
      <translation>Shown</translation>
    </message>
    <message>
      <source>From</source>
      <translation>From</translation>
    </message>
    <message>
      <source>To</source>
      <translation>To</translation>
    </message>
    <message>
      <source>Style</source>
      <translation>Style</translation>
    </message>
    <message>
      <source>Start</source>
      <translation>Start</translation>
    </message>
  </context>
  <context>
    <name>DocumentItemAttributes</name>
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
      <source>Real Number</source>
      <translation>Real Number</translation>
    </message>
    <message>
      <source>None</source>
      <comment>relationship</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>None</source>
      <comment>auto add</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>None</source>
      <comment>types</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>Document Item Attributes</translation>
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
  </context>
  <context>
    <name>PrintDialog</name>
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
      <source>&amp;Print</source>
      <translation>&amp;Print</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>Print Normal</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation>Print Separations</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation>Print in Colour if Available</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>Print in Greyscale</translation>
    </message>
    <message>
      <source>PostScript Level 1</source>
      <translation>PostScript Level 1</translation>
    </message>
    <message>
      <source>PostScript Level 2</source>
      <translation>PostScript Level 2</translation>
    </message>
    <message>
      <source>PostScript Level 3</source>
      <translation>PostScript Level 3</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Page</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>Mirror Page(s) Horizontal</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>Mirror Page(s) Vertical</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation>Set Media Size</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Colour</translation>
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation>Apply Under Colour Removal</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>Convert Spot Colours to Process Colours</translation>
    </message>
    <message>
      <source>Advanced Options</source>
      <translation>Advanced Options</translation>
    </message>
    <message>
      <source>Preview...</source>
      <translation>Preview...</translation>
    </message>
    <message>
      <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
      <translation>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</translation>
    </message>
    <message>
      <source>PostScript Files (*.ps);;All Files (*)</source>
      <translation>PostScript Files (*.ps);;All Files (*)</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
      <translation>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>A way of switching off some of the grey shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the grey. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Enables Spot Colours to be converted to composite colours. Unless you are planning to print spot colours at a commercial printer, this is probably best left enabled.</translation>
    </message>
    <message>
      <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
      <translation>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</translation>
    </message>
    <message>
      <source>Failed to retrieve printer settings</source>
      <translation>Failed to retrieve printer settings</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>Clip to Page Margins</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins on the printed page</source>
      <translation>Do not show objects outside the margins on the printed page</translation>
    </message>
    <message>
      <source>Force Overprint Mode</source>
      <translation>Force Overprint Mode</translation>
    </message>
    <message>
      <source>Enables global Overprint Mode for this document, overrides object settings</source>
      <translation>Enables global Overprint Mode for this document, overrides object settings</translation>
    </message>
    <message>
      <source>Printer Marks</source>
      <translation>Printer Marks</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>Crop Marks</translation>
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>Bleed Marks</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>Registration Marks</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>Colour Bars</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>Offset:</translation>
    </message>
    <message>
      <source>Marks</source>
      <translation>Marks</translation>
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation>Bleed Settings</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>Top:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>Bottom:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>Left:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>Right:</translation>
    </message>
    <message>
      <source>Use Document Bleeds</source>
      <translation>Use Document Bleeds</translation>
    </message>
    <message>
      <source>Bleeds</source>
      <translation>Bleeds</translation>
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
      <source>Inside:</source>
      <translation>Inside:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>Outside:</translation>
    </message>
    <message>
      <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</translation>
    </message>
    <message>
      <source>Add registration marks which are added to each separation</source>
      <translation>Add registration marks which are added to each separation</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation>Add colour calibration bars</translation>
    </message>
    <message>
      <source>Indicate the distance offset for the registration marks</source>
      <translation>Indicate the distance offset for the registration marks</translation>
    </message>
    <message>
      <source>Use the existing bleed settings from the document preferences</source>
      <translation>Use the existing bleed settings from the document preferences</translation>
    </message>
    <message>
      <source>Apply Color Profiles</source>
      <translation>Apply Colour Profiles</translation>
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation>Allows you to embed colour profiles in the print stream when colour management is enabled</translation>
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
    <message>
      <source>Error</source>
      <translation>Error</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>Generating Items</translation>
    </message>
    <message>
      <source>Analyzing PostScript:</source>
      <translation>Analysing PostScript:</translation>
    </message>
    <message>
      <source>Converting of %1 images failed!</source>
      <translation>Converting of %1 images failed!</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
    <message>
      <source>Importing: %1</source>
      <translation>Importing: %1</translation>
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
    <message>
      <source>JavaScripts (*.js);;All Files (*)</source>
      <translation>JavaScripts (*.js);;All Files (*)</translation>
    </message>
    <message>
      <source>Ctrl+N</source>
      <translation>Ctrl+N</translation>
    </message>
    <message>
      <source>Ctrl+Z</source>
      <translation>Ctrl+Z</translation>
    </message>
    <message>
      <source>Ctrl+X</source>
      <translation>Ctrl+X</translation>
    </message>
    <message>
      <source>Ctrl+C</source>
      <translation>Ctrl+C</translation>
    </message>
    <message>
      <source>Ctrl-V</source>
      <translation>Ctrl-V</translation>
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
    <message>
      <source>Color 1:</source>
      <translation>Colour 1:</translation>
    </message>
    <message>
      <source>Color 2:</source>
      <translation>Colour 2:</translation>
    </message>
    <message>
      <source>Color 3:</source>
      <translation>Colour 3:</translation>
    </message>
    <message>
      <source>Color 4:</source>
      <translation>Colour 4:</translation>
    </message>
    <message>
      <source>Duotone</source>
      <translation>Duotone</translation>
    </message>
    <message>
      <source>Tritone</source>
      <translation>Tritone</translation>
    </message>
    <message>
      <source>Quadtone</source>
      <translation>Quadtone</translation>
    </message>
    <message>
      <source>Curves</source>
      <translation>Curves</translation>
    </message>
  </context>
  <context>
    <name>ExportBitmap</name>
    <message>
      <source>File exists. Overwrite?</source>
      <translation>File exists. Overwrite?</translation>
    </message>
    <message>
      <source>exists already. Overwrite?</source>
      <translation>exists already. Overwrite?</translation>
    </message>
  </context>
  <context>
    <name>ExportForm</name>
    <message>
      <source>&amp;All pages</source>
      <translation>&amp;All pages</translation>
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
    <message>
      <source>Image size in Pixels</source>
      <translation>Image size in Pixels</translation>
    </message>
    <message>
      <source>TextLabel</source>
      <translation>TextLabel</translation>
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
      <source>Colorspace:</source>
      <translation>Colourspace:</translation>
    </message>
  </context>
  <context>
    <name>FileLoader</name>
    <message>
      <source>Some fonts used by this document have been substituted:</source>
      <translation>Some fonts used by this document have been substituted:</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation> was replaced by: </translation>
    </message>
  </context>
  <context>
    <name>FontComboH</name>
    <message>
      <source>Face:</source>
      <translation>Face:</translation>
    </message>
    <message>
      <source>Style:</source>
      <translation>Style:</translation>
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
      <source>Font search paths can only be set in File > Preferences, and only when there is no document currently open. Close any open documents, then use File > Preferences > Fonts to change the font search path.</source>
      <translation>Font search paths can only be set in File > Preferences, and only when there is no document currently open. Close any open documents, then use File > Preferences > Fonts to change the font search path.</translation>
    </message>
    <message>
      <source>Use Font</source>
      <translation>Use Font</translation>
    </message>
    <message>
      <source>Embed in PostScript</source>
      <translation>Embed in PostScript</translation>
    </message>
    <message>
      <source>Subset</source>
      <translation>Subset</translation>
    </message>
    <message>
      <source>Path to Font File</source>
      <translation>Path to Font File</translation>
    </message>
  </context>
  <context>
    <name>FontPreview</name>
    <message>
      <source>Leave preview</source>
      <comment>font preview</comment>
      <translation>Leave preview</translation>
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
    <message>
      <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
      <translation>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</translation>
    </message>
    <message>
      <source>Font Name</source>
      <translation>Font Name</translation>
    </message>
    <message>
      <source>Doc</source>
      <translation>Doc</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Type</translation>
    </message>
    <message>
      <source>Subset</source>
      <translation>Subset</translation>
    </message>
    <message>
      <source>Access</source>
      <translation>Access</translation>
    </message>
    <message>
      <source>Fonts Preview</source>
      <translation>Fonts Preview</translation>
    </message>
    <message>
      <source>&amp;Quick Search:</source>
      <translation>&amp;Quick Search:</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>&amp;Font Size:</source>
      <translation>&amp;Font Size:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Sample text to display</source>
      <translation>Sample text to display</translation>
    </message>
    <message>
      <source>Se&amp;t</source>
      <translation>Se&amp;t</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Reset the text</source>
      <translation>Reset the text</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>u&amp;Close</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
  </context>
  <context>
    <name>FontPreviewPlugin</name>
    <message>
      <source>&amp;Font Preview...</source>
      <translation>&amp;Font Preview...</translation>
    </message>
    <message>
      <source>Font Preview dialog</source>
      <translation>Font Preview dialog</translation>
    </message>
    <message>
      <source>Sorting, searching and browsing available fonts.</source>
      <translation>Sorting, searching and browsing available fonts.</translation>
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
      <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
      <translation>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</translation>
    </message>
    <message>
      <source>Cancels these font substitutions and stops loading the document.</source>
      <translation>Cancels these font substitutions and stops loading the document.</translation>
    </message>
    <message>
      <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit > Preferences > Fonts.</source>
      <translation>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit > Preferences > Fonts.</translation>
    </message>
    <message>
      <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
      <translation>If you select OK, then save, these substitutions are made permanent in the document.</translation>
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
      <source>Add, change or remove color stops here</source>
      <translation>Add, change or remove colour stops here</translation>
    </message>
  </context>
  <context>
    <name>GuideManager</name>
    <message>
      <source>&amp;Single</source>
      <translation>&amp;Single</translation>
    </message>
    <message>
      <source>Horizontals</source>
      <translation>Horizontals</translation>
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
      <source>D&amp;elete</source>
      <translation>D&amp;elete</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>Verticals</source>
      <translation>Verticals</translation>
    </message>
    <message>
      <source>A&amp;dd</source>
      <translation>A&amp;dd</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation>De&amp;lete</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>&amp;Lock Guides</source>
      <translation>&amp;Lock Guides</translation>
    </message>
    <message>
      <source>Appl&amp;y to All Pages</source>
      <translation>Appl&amp;y to All Pages</translation>
    </message>
    <message>
      <source>Alt+Y</source>
      <translation>Alt+Y</translation>
    </message>
    <message>
      <source>&amp;Column/Row</source>
      <translation>&amp;Column/Row</translation>
    </message>
    <message>
      <source>&amp;Number:</source>
      <translation>&amp;Number:</translation>
    </message>
    <message>
      <source>U&amp;se Gap:</source>
      <translation>U&amp;se Gap:</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>Refer To</source>
      <translation>Refer To</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>&amp;Page</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>M&amp;argins</source>
      <translation>M&amp;argins</translation>
    </message>
    <message>
      <source>S&amp;election</source>
      <translation>S&amp;election</translation>
    </message>
    <message>
      <source>Nu&amp;mber:</source>
      <translation>Nu&amp;mber:</translation>
    </message>
    <message>
      <source>Use &amp;Gap:</source>
      <translation>Use &amp;Gap:</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>&amp;Misc</source>
      <translation>&amp;Misc</translation>
    </message>
    <message>
      <source>Delete all guides from the current page</source>
      <translation>Delete all guides from the current page</translation>
    </message>
    <message>
      <source>Delete Guides from Current &amp;Page</source>
      <translation>Delete Guides from Current &amp;Page</translation>
    </message>
    <message>
      <source>Delete all guides from the current document</source>
      <translation>Delete all guides from the current document</translation>
    </message>
    <message>
      <source>Delete Guides from &amp;All Pages</source>
      <translation>Delete Guides from &amp;All Pages</translation>
    </message>
  </context>
  <context>
    <name>HelpBrowser</name>
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
      <source>Book&amp;marks</source>
      <translation>Book&amp;marks</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>&amp;Print...</translation>
    </message>
    <message>
      <source>New Bookmark</source>
      <translation>New Bookmark</translation>
    </message>
    <message>
      <source>New Bookmark's Title:</source>
      <translation>New Bookmark's Title:</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
    </message>
    <message>
      <source>&amp;Find...</source>
      <translation>&amp;Find...</translation>
    </message>
    <message>
      <source>Find &amp;Next</source>
      <translation>Find &amp;Next</translation>
    </message>
    <message>
      <source>Find &amp;Previous</source>
      <translation>Find &amp;Previous</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>&amp;Add Bookmark</source>
      <translation>&amp;Add Bookmark</translation>
    </message>
    <message>
      <source>D&amp;elete All</source>
      <translation>D&amp;elete All</translation>
    </message>
    <message>
      <source>&amp;Bookmarks</source>
      <translation>&amp;Bookmarks</translation>
    </message>
    <message>
      <source>Scribus Help</source>
      <translation>Scribus Help</translation>
    </message>
    <message>
      <source>Searching is case insensitive</source>
      <translation>Searching is case insensitive</translation>
    </message>
    <message>
      <source>1</source>
      <translation>1</translation>
    </message>
    <message>
      <source>&amp;Exit</source>
      <translation>&amp;Exit</translation>
    </message>
    <message>
      <source>Find &amp;Next...</source>
      <translation>Find &amp;Next...</translation>
    </message>
    <message>
      <source>Find &amp;Previous...</source>
      <translation>Find &amp;Previous...</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Add</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation>&amp;Quit</translation>
    </message>
    <message>
      <source>&lt;h2>&lt;p>Sorry, no manual is installed!&lt;/p>&lt;p>Please see:&lt;/p>&lt;ul>&lt;li>http://docs.scribus.net for updated documentation&lt;/li>&lt;li>http://www.scribus.net for downloads&lt;/li>&lt;/ul>&lt;/h2></source>
      <comment>HTML message for no documentation available to show</comment>
      <translation>&lt;h2>&lt;p>Sorry, no manual is installed!&lt;/p>&lt;p>Please see:&lt;/p>&lt;ul>&lt;li>http://docs.scribus.net for updated documentation&lt;/li>&lt;li>http://www.scribus.net for downloads&lt;/li>&lt;/ul>&lt;/h2></translation>
    </message>
  </context>
  <context>
    <name>HelpBrowser2</name>
    <message>
      <source>Dialog</source>
      <translation>Dialog</translation>
    </message>
    <message>
      <source>&amp;Contents</source>
      <translation>&amp;Contents</translation>
    </message>
    <message>
      <source>1</source>
      <translation>1</translation>
    </message>
    <message>
      <source>Se&amp;arch</source>
      <translation>Se&amp;arch</translation>
    </message>
    <message>
      <source>Searching is case insensitive</source>
      <translation>Searching is case insensitive</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
    <message>
      <source>Book&amp;marks</source>
      <translation>Book&amp;marks</translation>
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
    <message>
      <source>Add to the
Exception List</source>
      <translation>Add to the
Exception List</translation>
    </message>
    <message>
      <source>Add to the
Ignore List</source>
      <translation>Add to the
Ignore List</translation>
    </message>
  </context>
  <context>
    <name>HySettings</name>
    <message>
      <source>Ignore List</source>
      <translation>Ignore List</translation>
    </message>
    <message>
      <source>Add a new Entry</source>
      <translation>Add a new Entry</translation>
    </message>
    <message>
      <source>Edit Entry</source>
      <translation>Edit Entry</translation>
    </message>
    <message>
      <source>Exception List</source>
      <translation>Exception List</translation>
    </message>
  </context>
  <context>
    <name>ImageInfoDialog</name>
    <message>
      <source>Image Info</source>
      <translation>Image Info</translation>
    </message>
    <message>
      <source>General Info</source>
      <translation>General Info</translation>
    </message>
    <message>
      <source>Date / Time:</source>
      <translation>Date / Time:</translation>
    </message>
    <message>
      <source>Has Embedded Profile:</source>
      <translation>Has Embedded Profile:</translation>
    </message>
    <message>
      <source>Profile Name:</source>
      <translation>Profile Name:</translation>
    </message>
    <message>
      <source>Has Embedded Paths:</source>
      <translation>Has Embedded Paths:</translation>
    </message>
    <message>
      <source>Has Layers:</source>
      <translation>Has Layers:</translation>
    </message>
    <message>
      <source>EXIF Info</source>
      <translation>EXIF Info</translation>
    </message>
    <message>
      <source>Artist:</source>
      <translation>Artist:</translation>
    </message>
    <message>
      <source>Comment:</source>
      <translation>Comment:</translation>
    </message>
    <message>
      <source>User Comment:</source>
      <translation>User Comment:</translation>
    </message>
    <message>
      <source>Camera Model:</source>
      <translation>Camera Model:</translation>
    </message>
    <message>
      <source>Camera Manufacturer:</source>
      <translation>Camera Manufacturer:</translation>
    </message>
    <message>
      <source>Description:</source>
      <translation>Description:</translation>
    </message>
    <message>
      <source>Copyright:</source>
      <translation>Copyright:</translation>
    </message>
    <message>
      <source>Scanner Model:</source>
      <translation>Scanner Model:</translation>
    </message>
    <message>
      <source>Scanner Manufacturer:</source>
      <translation>Scanner Manufacturer:</translation>
    </message>
    <message>
      <source>Exposure time</source>
      <translation>Exposure time</translation>
    </message>
    <message>
      <source>Aperture:</source>
      <translation>Aperture:</translation>
    </message>
    <message>
      <source>ISO equiv.:</source>
      <translation>ISO equiv.:</translation>
    </message>
  </context>
  <context>
    <name>ImportAIPlugin</name>
    <message>
      <source>Import AI...</source>
      <translation>Import AI...</translation>
    </message>
    <message>
      <source>Imports Illustrator Files</source>
      <translation>Imports Illustrator Files</translation>
    </message>
    <message>
      <source>Imports most Illustrator files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>Imports most Illustrator files into the current document,
converting their vector data into Scribus objects.</translation>
    </message>
  </context>
  <context>
    <name>ImportPSPlugin</name>
    <message>
      <source>Import PostScript...</source>
      <translation>Import PostScript...</translation>
    </message>
    <message>
      <source>Imports PostScript Files</source>
      <translation>Imports PostScript Files</translation>
    </message>
    <message>
      <source>Imports most PostScript files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>Imports most PostScript files into the current document,
converting their vector data into Scribus objects.</translation>
    </message>
  </context>
  <context>
    <name>Imposition</name>
    <message>
      <source>Portrait</source>
      <translation>Portrait</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Landscape</translation>
    </message>
  </context>
  <context>
    <name>ImpositionBase</name>
    <message>
      <source>Imposition</source>
      <translation>Imposition</translation>
    </message>
    <message>
      <source>Gri&amp;d</source>
      <translation>Gri&amp;d</translation>
    </message>
    <message>
      <source>Copies</source>
      <translation>Copies</translation>
    </message>
    <message>
      <source>Do&amp;uble sided</source>
      <translation>Do&amp;uble sided</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Front side</source>
      <translation>Front side</translation>
    </message>
    <message>
      <source>Back side</source>
      <translation>Back side</translation>
    </message>
    <message>
      <source>&amp;Booklet</source>
      <translation>&amp;Booklet</translation>
    </message>
    <message>
      <source>Pages per sheet</source>
      <translation>Pages per sheet</translation>
    </message>
    <message>
      <source>4</source>
      <translation>4</translation>
    </message>
    <message>
      <source>8</source>
      <translation>8</translation>
    </message>
    <message>
      <source>16</source>
      <translation>16</translation>
    </message>
    <message>
      <source>Pages</source>
      <translation>Pages</translation>
    </message>
    <message>
      <source>&lt;html>&lt;head>&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; />&lt;style type=&quot;text/css&quot;>
p, li { white-space: pre-wrap; }
&lt;/style>&lt;/head>&lt;body style=&quot; font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;&quot;>
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;>Separate pages with a comma, ranges with a hyphen, e.g. 1,4,9-11 to get pages 1,4,9,10,11.&lt;/p>&lt;/body>&lt;/html></source>
      <translation>&lt;html>&lt;head>&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; />&lt;style type=&quot;text/css&quot;>
p, li { white-space: pre-wrap; }
&lt;/style>&lt;/head>&lt;body style=&quot; font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;&quot;>
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;>Separate pages with a comma, ranges with a hyphen, e.g. 1,4,9-11 to get pages 1,4,9,10,11.&lt;/p>&lt;/body>&lt;/html></translation>
    </message>
    <message>
      <source>Fold</source>
      <translation>Fold</translation>
    </message>
    <message>
      <source>Front page from</source>
      <translation>Front page from</translation>
    </message>
    <message>
      <source>Double sided</source>
      <translation>Double sided</translation>
    </message>
    <message>
      <source>Back page from</source>
      <translation>Back page from</translation>
    </message>
    <message>
      <source>Destination page</source>
      <translation>Destination page</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Size</translation>
    </message>
    <message>
      <source>Orientation</source>
      <translation>Orientation</translation>
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
      <source>Preview</source>
      <translation>Preview</translation>
    </message>
    <message>
      <source>Refresh preview</source>
      <translation>Refresh preview</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
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
    <name>ImpositionPlugin</name>
    <message>
      <source>&amp;Imposition...</source>
      <translation>&amp;Imposition...</translation>
    </message>
    <message>
      <source>Imposition dialog</source>
      <translation>Imposition dialog</translation>
    </message>
    <message>
      <source>Imposition on grids, booklets and folds</source>
      <translation>Imposition on grids, booklets and folds</translation>
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
      <source>Page(s)</source>
      <translation>Page(s)</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>&amp;Insert</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>&amp;Master Page:</translation>
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
    <message>
      <source>Master Pages</source>
      <translation>Master Pages</translation>
    </message>
  </context>
  <context>
    <name>InsertAFrame</name>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>&lt;b>Insert a text frame&lt;/b>&lt;br/>A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable format from plain text to OpenOffice.org.&lt;br/>Your text may be edited and formatted on the page directly or in the simple Story Editor.</source>
      <translation>&lt;b>Insert a text frame&lt;/b>&lt;br/>A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable format from plain text to OpenOffice.org.&lt;br/>Your text may be edited and formatted on the page directly or in the simple Story Editor.</translation>
    </message>
    <message>
      <source>&lt;b>Insert an image frame&lt;/b>&lt;br/>An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.</source>
      <translation>&lt;b>Insert an image frame&lt;/b>&lt;br/>An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.</translation>
    </message>
    <message>
      <source>Insert A Frame</source>
      <translation>Insert A Frame</translation>
    </message>
    <message>
      <source>T&amp;ype</source>
      <translation>T&amp;ype</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation>&amp;Text Frame</translation>
    </message>
    <message>
      <source>&amp;Image Frame</source>
      <translation>&amp;Image Frame</translation>
    </message>
    <message>
      <source>&amp;Location</source>
      <translation>&amp;Location</translation>
    </message>
    <message>
      <source>Page Placement</source>
      <translation>Page Placement</translation>
    </message>
    <message>
      <source>Current Page</source>
      <translation>Current Page</translation>
    </message>
    <message>
      <source>All Pages</source>
      <translation>All Pages</translation>
    </message>
    <message>
      <source>...</source>
      <translation>...</translation>
    </message>
    <message>
      <source>Position of Frame</source>
      <translation>Position of Frame</translation>
    </message>
    <message>
      <source>Top Left of Margins</source>
      <translation>Top Left of Margins</translation>
    </message>
    <message>
      <source>Top Left of Page</source>
      <translation>Top Left of Page</translation>
    </message>
    <message>
      <source>Top Left of Bleed</source>
      <translation>Top Left of Bleed</translation>
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
      <source>&amp;Size</source>
      <translation>&amp;Size</translation>
    </message>
    <message>
      <source>Same as the Page Margins</source>
      <translation>Same as the Page Margins</translation>
    </message>
    <message>
      <source>Same as the Page</source>
      <translation>Same as the Page</translation>
    </message>
    <message>
      <source>Same as the Bleed</source>
      <translation>Same as the Bleed</translation>
    </message>
    <message>
      <source>Same as the Imported Image</source>
      <translation>Same as the Imported Image</translation>
    </message>
    <message>
      <source>Height:</source>
      <translation>Height:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>Width:</translation>
    </message>
    <message>
      <source>&amp;Options</source>
      <translation>&amp;Options</translation>
    </message>
    <message>
      <source>Source Image:</source>
      <translation>Source Image:</translation>
    </message>
    <message>
      <source>There are no options for this type of frame</source>
      <translation>There are no options for this type of frame</translation>
    </message>
    <message>
      <source>Columns:</source>
      <translation>Columns:</translation>
    </message>
    <message>
      <source>Gap:</source>
      <translation>Gap:</translation>
    </message>
    <message>
      <source>Link Created Frames</source>
      <translation>Link Created Frames</translation>
    </message>
    <message>
      <source>Source Document:</source>
      <translation>Source Document:</translation>
    </message>
    <message>
      <source>Range of Pages</source>
      <translation>Range of Pages</translation>
    </message>
    <message>
      <source>Custom Position</source>
      <translation>Custom Position</translation>
    </message>
    <message>
      <source>Custom Size</source>
      <translation>Custom Size</translation>
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
      <source>Do you really want to delete this script?</source>
      <translation>Do you really want to delete this script?</translation>
    </message>
    <message>
      <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
      <translation>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</translation>
    </message>
  </context>
  <context>
    <name>LatexEditor</name>
    <message>
      <source>Dialog</source>
      <translation>Dialog</translation>
    </message>
    <message>
      <source>Enter Code:</source>
      <translation>Enter Code:</translation>
    </message>
    <message>
      <source>Update</source>
      <translation>Update</translation>
    </message>
    <message>
      <source>Revert</source>
      <translation>Revert</translation>
    </message>
    <message>
      <source>Program Messages:</source>
      <translation>Program Messages:</translation>
    </message>
    <message>
      <source>&lt;html>&lt;head>&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; />&lt;style type=&quot;text/css&quot;>
p, li { white-space: pre-wrap; }
&lt;/style>&lt;/head>&lt;body style=&quot; font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;&quot;>
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;>&lt;/p>&lt;/body>&lt;/html></source>
      <translation>&lt;html>&lt;head>&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; />&lt;style type=&quot;text/css&quot;>
p, li { white-space: pre-wrap; }
&lt;/style>&lt;/head>&lt;body style=&quot; font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;&quot;>
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;>&lt;/p>&lt;/body>&lt;/html></translation>
    </message>
    <message>
      <source>Status: Unknown</source>
      <translation>Status: Unknown</translation>
    </message>
    <message>
      <source>Kill Program</source>
      <translation>Kill Program</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>Resolution:</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>Automatic</translation>
    </message>
    <message>
      <source> DPI</source>
      <translation> DPI</translation>
    </message>
    <message>
      <source>Program:</source>
      <translation>Program:</translation>
    </message>
    <message>
      <source>LaTeX</source>
      <translation>LaTeX</translation>
    </message>
    <message>
      <source>Use Preamble</source>
      <translation>Use Preamble</translation>
    </message>
    <message>
      <source>Update Application Settings</source>
      <translation>Update Application Settings</translation>
    </message>
    <message>
      <source>Fonts and Colors</source>
      <translation>Fonts and Colours</translation>
    </message>
    <message>
      <source>Will be filled later</source>
      <translation>Will be filled later</translation>
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
    <message>
      <source>Opacity:</source>
      <translation>Opacity:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Blend Mode:</translation>
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
      <source>Multiply</source>
      <translation>Multiply</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Screen</translation>
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
      <source>Color Dodge</source>
      <translation>Colour Dodge</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>Colour Burn</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>Exclusion</translation>
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
      <source>Color of the Layer Indicator - Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color. </source>
      <translation>Colour of the Layer Indicator - Each layer has a colour assigned to display on the canvas when layer indicators are enabled. You can double click to edit the colour. </translation>
    </message>
    <message>
      <source>Make Layer Visible - Uncheck to hide the layer from the display </source>
      <translation>Make Layer Visible - Uncheck to hide the layer from the display </translation>
    </message>
    <message>
      <source>Print Layer - Uncheck to disable printing. </source>
      <translation>Print Layer - Uncheck to disable printing. </translation>
    </message>
    <message>
      <source>Lock or Unlock Layer - Unchecked is unlocked </source>
      <translation>Lock or Unlock Layer - Unchecked is unlocked </translation>
    </message>
    <message>
      <source>Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below</source>
      <translation>Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below</translation>
    </message>
    <message>
      <source>Outline Mode - Toggles the 'wireframe' display of objects to speed the display of very complex objects.</source>
      <translation>Outline Mode - Toggles the 'wireframe' display of objects to speed the display of very complex objects.</translation>
    </message>
    <message>
      <source>Name of the Layer - Double clicking on the name of a layer enabled editing</source>
      <translation>Name of the Layer - Double clicking on the name of a layer enabled editing</translation>
    </message>
    <message>
      <source>Duplicates the current layer</source>
      <translation>Duplicates the current layer</translation>
    </message>
  </context>
  <context>
    <name>LensDialogBase</name>
    <message>
      <source>Optical Lens</source>
      <translation>Optical Lens</translation>
    </message>
    <message>
      <source>Add Lens</source>
      <translation>Add Lens</translation>
    </message>
    <message>
      <source>Remove Lens</source>
      <translation>Remove Lens</translation>
    </message>
    <message>
      <source>+</source>
      <translation>+</translation>
    </message>
    <message>
      <source>-</source>
      <translation>-</translation>
    </message>
    <message>
      <source>Lens Parameters</source>
      <translation>Lens Parameters</translation>
    </message>
    <message>
      <source>X Pos:</source>
      <translation>X Pos:</translation>
    </message>
    <message>
      <source>Y Pos:</source>
      <translation>Y Pos:</translation>
    </message>
    <message>
      <source>Radius:</source>
      <translation>Radius:</translation>
    </message>
    <message>
      <source>Strength:</source>
      <translation>Strength:</translation>
    </message>
  </context>
  <context>
    <name>LoadSavePlugin</name>
    <message>
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
    </message>
    <message>
      <source>No File Loader Plugins Found</source>
      <translation>No File Loader Plugins Found</translation>
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
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Standard Lorem Ipsum</source>
      <translation>Standard Lorem Ipsum</translation>
    </message>
  </context>
  <context>
    <name>MarginDialog</name>
    <message>
      <source>Margin Guides</source>
      <translation>Margin Guides</translation>
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
    <message>
      <source>Type:</source>
      <translation>Type:</translation>
    </message>
    <message>
      <source>Other Settings</source>
      <translation>Other Settings</translation>
    </message>
    <message>
      <source>Master Page:</source>
      <translation>Master Page:</translation>
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
      <source>&amp;Inside:</source>
      <translation>&amp;Inside:</translation>
    </message>
    <message>
      <source>O&amp;utside:</source>
      <translation>O&amp;utside:</translation>
    </message>
    <message>
      <source>Preset Layouts:</source>
      <translation>Preset Layouts:</translation>
    </message>
    <message>
      <source>Apply the margin changes to all existing pages in the document</source>
      <translation>Apply the margin changes to all existing pages in the document</translation>
    </message>
    <message>
      <source>Printer Margins...</source>
      <translation>Printer Margins...</translation>
    </message>
    <message>
      <source>Import the margins for the selected page size from the available printers.</source>
      <translation>Import the margins for the selected page size from the available printers.</translation>
    </message>
    <message>
      <source>Apply settings to:</source>
      <translation>Apply settings to:</translation>
    </message>
    <message>
      <source>All Document Pages</source>
      <translation>All Document Pages</translation>
    </message>
    <message>
      <source>All Master Pages</source>
      <translation>All Master Pages</translation>
    </message>
    <message>
      <source>Apply the margin changes to all existing master pages in the document</source>
      <translation>Apply the margin changes to all existing master pages in the document</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Margin Guides</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>Top:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>Bottom:</translation>
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
      <source>Bleeds</source>
      <translation>Bleeds</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation>Inside:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>Outside:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>Left:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>Right:</translation>
    </message>
    <message>
      <source>Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</translation>
    </message>
    <message>
      <source>Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</translation>
    </message>
  </context>
  <context>
    <name>MasterPagesPalette</name>
    <message>
      <source>Edit Master Pages</source>
      <translation>Edit Master Pages</translation>
    </message>
    <message>
      <source>Do you really want to delete this master page?</source>
      <translation>Do you really want to delete this master page?</translation>
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
      <source>Duplicate the selected master page</source>
      <translation>Duplicate the selected master page</translation>
    </message>
    <message>
      <source>Delete the selected master page</source>
      <translation>Delete the selected master page</translation>
    </message>
    <message>
      <source>Add a new master page</source>
      <translation>Add a new master page</translation>
    </message>
    <message>
      <source>Import master pages from another document</source>
      <translation>Import master pages from another document</translation>
    </message>
    <message>
      <source>New Master Page %1</source>
      <translation>New Master Page %1</translation>
    </message>
    <message>
      <source>Unable to Rename Master Page</source>
      <translation>Unable to Rename Master Page</translation>
    </message>
    <message>
      <source>The Normal page is not allowed to be renamed.</source>
      <translation>The Normal page is not allowed to be renamed.</translation>
    </message>
    <message>
      <source>Rename Master Page</source>
      <translation>Rename Master Page</translation>
    </message>
    <message>
      <source>New Name:</source>
      <translation>New Name:</translation>
    </message>
    <message>
      <source>Copy #%1 of %2</source>
      <translation>Copy #%1 of %2</translation>
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
      <source> °</source>
      <translation> °</translation>
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
  </context>
  <context>
    <name>ModeToolBar</name>
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
    <message>
      <source>To:</source>
      <translation>To:</translation>
    </message>
    <message>
      <source>Number of copies:</source>
      <translation>Number of copies:</translation>
    </message>
  </context>
  <context>
    <name>PropertiesPalette</name>
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
      <source>Font of selected text or object</source>
      <translation>Font of selected text or object</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>Scaling width of characters</translation>
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
      <source>Use &amp;Bounding Box</source>
      <translation>Use &amp;Bounding Box</translation>
    </message>
    <message>
      <source>&amp;Use Contour Line</source>
      <translation>&amp;Use Contour Line</translation>
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
      <source>Right to Left Writing</source>
      <translation>Right to Left Writing</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>Manual Tracking</translation>
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
    <message>
      <source>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</source>
      <translation>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</translation>
    </message>
    <message>
      <source>Fill Rule</source>
      <translation>Fill Rule</translation>
    </message>
    <message>
      <source>Even-Odd</source>
      <translation>Even-Odd</translation>
    </message>
    <message>
      <source>Non Zero</source>
      <translation>Non Zero</translation>
    </message>
    <message>
      <source>Overprinting</source>
      <translation>Overprinting</translation>
    </message>
    <message>
      <source>Knockout</source>
      <translation>Knockout</translation>
    </message>
    <message>
      <source>Overprint</source>
      <translation>Overprint</translation>
    </message>
    <message>
      <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
      <translation>Colour of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same colour.</translation>
    </message>
    <message>
      <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
      <translation>Colour of selected text. If Outline text decoration is enabled, this colour will be the fill colour. If Drop Shadow Text is enabled, then this will be the top most colour.</translation>
    </message>
    <message>
      <source>Gap:</source>
      <translation>Gap:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>Width:</translation>
    </message>
    <message>
      <source>Text &amp;Flow Around Frame</source>
      <translation>Text &amp;Flow Around Frame</translation>
    </message>
    <message>
      <source>Disabled</source>
      <translation>Disabled</translation>
    </message>
    <message>
      <source>Use Frame &amp;Shape</source>
      <translation>Use Frame &amp;Shape</translation>
    </message>
    <message>
      <source>Image Effects</source>
      <translation>Image Effects</translation>
    </message>
    <message>
      <source>Extended Image Properties</source>
      <translation>Extended Image Properties</translation>
    </message>
    <message>
      <source>Disable text flow from lower frames around object</source>
      <translation>Disable text flow from lower frames around object</translation>
    </message>
    <message>
      <source>Use the frame shape for text flow of text frames below the object.</source>
      <translation>Use the frame shape for text flow of text frames below the object.</translation>
    </message>
    <message>
      <source>Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. </source>
      <translation>Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. </translation>
    </message>
    <message>
      <source>Transparency Settings</source>
      <translation>Transparency Settings</translation>
    </message>
    <message>
      <source>&amp;Group</source>
      <translation>&amp;Group</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>Opacity:</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Blend Mode:</translation>
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
      <source>Multiply</source>
      <translation>Multiply</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Screen</translation>
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
      <source>Exclusion</source>
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
      <source>Hue</source>
      <translation>Hue</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Colour</translation>
    </message>
    <message>
      <source>Group the selected objects</source>
      <translation>Group the selected objects</translation>
    </message>
    <message>
      <source>Destroys the selected group</source>
      <translation>Destroys the selected group</translation>
    </message>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source>When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object. T</source>
      <translation>When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object. T</translation>
    </message>
    <message>
      <source>Click and hold down to select the line spacing mode.</source>
      <translation>Click and hold down to select the line spacing mode.</translation>
    </message>
    <message>
      <source>Default</source>
      <translation>Default</translation>
    </message>
    <message>
      <source>Stair Step</source>
      <translation>Stair Step</translation>
    </message>
    <message>
      <source>Skew</source>
      <translation>Skew</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>Type:</translation>
    </message>
    <message>
      <source>Paragraph St&amp;yle:</source>
      <translation>Paragraph St&amp;yle:</translation>
    </message>
    <message>
      <source>Character St&amp;yle:</source>
      <translation>Character St&amp;yle:</translation>
    </message>
    <message>
      <source>Optical Margins:</source>
      <translation>Optical Margins:</translation>
    </message>
    <message>
      <source>Word Tracking</source>
      <translation>Word Tracking</translation>
    </message>
    <message>
      <source>Min:</source>
      <translation>Min:</translation>
    </message>
    <message>
      <source>Max:</source>
      <translation>Max:</translation>
    </message>
    <message>
      <source>Glyph Extension</source>
      <translation>Glyph Extension</translation>
    </message>
    <message>
      <source>Paragraph style of currently selected text or paragraph</source>
      <translation>Paragraph style of currently selected text or paragraph</translation>
    </message>
    <message>
      <source>Character style of currently selected text or paragraph</source>
      <translation>Character style of currently selected text or paragraph</translation>
    </message>
    <message>
      <source>Flip Text</source>
      <translation>Flip Text</translation>
    </message>
    <message>
      <source>Use Image Clip Path</source>
      <translation>Use Image Clip Path</translation>
    </message>
    <message>
      <source>Norm:</source>
      <translation>Norm:</translation>
    </message>
    <message>
      <source>Use the clipping path of the image</source>
      <translation>Use the clipping path of the image</translation>
    </message>
    <message>
      <source>Remove Direct Paragraph Formatting</source>
      <translation>Remove Direct Paragraph Formatting</translation>
    </message>
    <message>
      <source>Remove Direct Character Formatting</source>
      <translation>Remove Direct Character Formatting</translation>
    </message>
    <message>
      <source>Minimal width of spaces between words</source>
      <translation>Minimal width of spaces between words</translation>
    </message>
    <message>
      <source>Normal width of spaces between words</source>
      <translation>Normal width of spaces between words</translation>
    </message>
    <message>
      <source>Minimal shrinkage of glyphs for justification</source>
      <translation>Minimal shrinkage of glyphs for justification</translation>
    </message>
    <message>
      <source>Maximal extension of glyphs for justification</source>
      <translation>Maximal extension of glyphs for justification</translation>
    </message>
    <message>
      <source>Uses hanging punctuation and margin kerning to achieve nicer looking columns</source>
      <translation>Uses hanging punctuation and margin kerning to achieve nicer looking columns</translation>
    </message>
    <message>
      <source>Enable or disable exporting of the object</source>
      <translation>Enable or disable exporting of the object</translation>
    </message>
  </context>
  <context>
    <name>MultiProgressDialog</name>
    <message>
      <source>Progress</source>
      <translation>Progress</translation>
    </message>
    <message>
      <source>Overall Progress:</source>
      <translation>Overall Progress:</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Cancel</translation>
    </message>
    <message>
      <source>%v of %m</source>
      <translation>%v of %m</translation>
    </message>
  </context>
  <context>
    <name>MultipleDuplicate</name>
    <message>
      <source>&amp;Horizontal Shift:</source>
      <translation>&amp;Horizontal Shift:</translation>
    </message>
    <message>
      <source>&amp;Vertical Shift:</source>
      <translation>&amp;Vertical Shift:</translation>
    </message>
    <message>
      <source>&amp;Horizontal Gap:</source>
      <translation>&amp;Horizontal Gap:</translation>
    </message>
    <message>
      <source>&amp;Vertical Gap:</source>
      <translation>&amp;Vertical Gap:</translation>
    </message>
    <message>
      <source>Multiple Duplicate</source>
      <translation>Multiple Duplicate</translation>
    </message>
    <message>
      <source>&amp;By Number of Copies</source>
      <translation>&amp;By Number of Copies</translation>
    </message>
    <message>
      <source>&amp;Number of Copies:</source>
      <translation>&amp;Number of Copies:</translation>
    </message>
    <message>
      <source>&amp;Shift Created Items By</source>
      <translation>&amp;Shift Created Items By</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>Create &amp;Gap Between Items Of</source>
      <translation>Create &amp;Gap Between Items Of</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Rotation:</source>
      <translation>Rotation:</translation>
    </message>
    <message>
      <source>By &amp;Rows &amp;&amp; Columns</source>
      <translation>By &amp;Rows &amp;&amp; Columns</translation>
    </message>
    <message>
      <source>Vertical Gap:</source>
      <translation>Vertical Gap:</translation>
    </message>
    <message>
      <source>Horizontal Gap:</source>
      <translation>Horizontal Gap:</translation>
    </message>
    <message>
      <source>Number of Rows:</source>
      <translation>Number of Rows:</translation>
    </message>
    <message>
      <source>Number of Columns:</source>
      <translation>Number of Columns:</translation>
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
    <name>MyPlugin</name>
    <message>
      <source>My &amp;Plugin</source>
      <translation>My &amp;Plugin</translation>
    </message>
  </context>
  <context>
    <name>MyPluginImpl</name>
    <message>
      <source>Scribus - My Plugin</source>
      <translation>Scribus - My Plugin</translation>
    </message>
    <message>
      <source>The plugin worked!</source>
      <translation>The plugin worked!</translation>
    </message>
  </context>
  <context>
    <name>NewDoc</name>
    <message>
      <source>New Document</source>
      <translation>New Document</translation>
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
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>&amp;New Document</source>
      <translation>&amp;New Document</translation>
    </message>
    <message>
      <source>Open &amp;Existing Document</source>
      <translation>Open &amp;Existing Document</translation>
    </message>
    <message>
      <source>Open Recent &amp;Document</source>
      <translation>Open Recent &amp;Document</translation>
    </message>
    <message>
      <source>First Page is:</source>
      <translation>First Page is:</translation>
    </message>
    <message>
      <source>Show Document Settings After Creation</source>
      <translation>Show Document Settings After Creation</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation>Document Layout</translation>
    </message>
  </context>
  <context>
    <name>NewFromTemplatePlugin</name>
    <message>
      <source>New &amp;from Template...</source>
      <translation>New &amp;from Template...</translation>
    </message>
    <message>
      <source>Load documents with predefined layout</source>
      <translation>Load documents with predefined layout</translation>
    </message>
    <message>
      <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
      <translation>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</translation>
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
      <source>Enlarge the Size of the Path by shown %</source>
      <translation>Enlarge the Size of the Path by shown %</translation>
    </message>
    <message>
      <source>Angle of Rotation</source>
      <translation>Angle of Rotation</translation>
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
    <message>
      <source>Shrink the Size of the Path by shown %</source>
      <translation>Shrink the Size of the Path by shown %</translation>
    </message>
    <message>
      <source>Reduce the Size of the Path by the shown value</source>
      <translation>Reduce the Size of the Path by the shown value</translation>
    </message>
    <message>
      <source>Enlarge the Size of the Path by the shown value</source>
      <translation>Enlarge the Size of the Path by the shown value</translation>
    </message>
    <message>
      <source>% to Enlarge or Shrink By</source>
      <translation>% to Enlarge or Shrink By</translation>
    </message>
    <message>
      <source>Value to Enlarge or Shrink By</source>
      <translation>Value to Enlarge or Shrink By</translation>
    </message>
    <message>
      <source>Set Contour to Image Clip</source>
      <translation>Set Contour to Image Clip</translation>
    </message>
    <message>
      <source>Reset the Contour Line to the Clipping Path of the Image</source>
      <translation>Reset the Contour Line to the Clipping Path of the Image</translation>
    </message>
    <message>
      <source>Lens Effects...</source>
      <translation>Lens Effects...</translation>
    </message>
    <message>
      <source>Apply fancy Lens Effects</source>
      <translation>Apply fancy Lens Effects</translation>
    </message>
  </context>
  <context>
    <name>OODPlug</name>
    <message>
      <source>This document does not seem to be an OpenOffice Draw file.</source>
      <translation>This document does not seem to be an OpenOffice Draw file.</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
  </context>
  <context>
    <name>OODrawImportPlugin</name>
    <message>
      <source>Import &amp;OpenOffice.org Draw...</source>
      <translation>Import &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
      <source>Imports OpenOffice.org Draw Files</source>
      <translation>Imports OpenOffice.org Draw Files</translation>
    </message>
    <message>
      <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
      <translation>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</translation>
    </message>
    <message>
      <source>OpenDocument 1.0 Draw</source>
      <comment>Import/export format name</comment>
      <translation>OpenDocument 1.0 Draw</translation>
    </message>
    <message>
      <source>OpenOffice.org 1.x Draw</source>
      <comment>Import/export format name</comment>
      <translation>OpenOffice.org 1.x Draw</translation>
    </message>
    <message>
      <source>This file contains some unsupported features</source>
      <translation>This file contains some unsupported features</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>The file could not be imported</translation>
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
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
  </context>
  <context>
    <name>OldScribusFormat</name>
    <message>
      <source>Scribus Document</source>
      <translation>Scribus Document</translation>
    </message>
    <message>
      <source>Scribus 1.2.x Document</source>
      <translation>Scribus 1.2.x Document</translation>
    </message>
  </context>
  <context>
    <name>OneClick</name>
    <message>
      <source>Origin</source>
      <translation>Origin</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Size</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>Width:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation>Length:</translation>
    </message>
    <message>
      <source>Height:</source>
      <translation>Height:</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation>Angle:</translation>
    </message>
    <message>
      <source>Remember Values</source>
      <translation>Remember Values</translation>
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
    <name>PDFExportDialog</name>
    <message>
      <source>Save as PDF</source>
      <translation>Save as PDF</translation>
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
      <source>Output one file for eac&amp;h page</source>
      <translation>Output one file for eac&amp;h page</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDF Files (*.pdf);;All Files (*)</translation>
    </message>
    <message>
      <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
      <translation>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</translation>
    </message>
    <message>
      <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
      <translation>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</translation>
    </message>
    <message>
      <source>%1 does not exists and will be created, continue?</source>
      <translation>%1 does not exists and will be created, continue?</translation>
    </message>
    <message>
      <source>Cannot create directory: 
%1</source>
      <translation>Cannot create directory: 
%1</translation>
    </message>
  </context>
  <context>
    <name>PDFLibCore</name>
    <message>
      <source>Saving PDF</source>
      <translation>Saving PDF</translation>
    </message>
    <message>
      <source>Exporting Master Page:</source>
      <translation>Exporting Master Page:</translation>
    </message>
    <message>
      <source>Exporting Page:</source>
      <translation>Exporting Page:</translation>
    </message>
    <message>
      <source>Exporting Items on Current Page:</source>
      <translation>Exporting Items on Current Page:</translation>
    </message>
    <message>
      <source>Page:</source>
      <translation>Page:</translation>
    </message>
    <message>
      <source>Date:</source>
      <translation>Date:</translation>
    </message>
  </context>
  <context>
    <name>PDFToolBar</name>
    <message>
      <source>PDF Tools</source>
      <translation>PDF Tools</translation>
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
      <source>Separation Name</source>
      <translation>Separation Name</translation>
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
      <source>Scaling:</source>
      <translation>Scaling:</translation>
    </message>
    <message>
      <source>Print...</source>
      <translation>Print...</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>A way of switching off some of the grey shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the grey. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
      <source>Resize the scale of the page.</source>
      <translation>Resize the scale of the page.</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Close</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Force Overprint Mode</source>
      <translation>Force Overprint Mode</translation>
    </message>
    <message>
      <source>Enable &amp;Antialiasing</source>
      <translation>Enable &amp;Antialiasing</translation>
    </message>
    <message>
      <source>Fit to Width</source>
      <translation>Fit to Width</translation>
    </message>
    <message>
      <source>Fit to Height</source>
      <translation>Fit to Height</translation>
    </message>
    <message>
      <source>Fit to Page</source>
      <translation>Fit to Page</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
      <translation>Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</translation>
    </message>
    <message>
      <source>Display Settings</source>
      <translation>Display Settings</translation>
    </message>
    <message>
      <source>Print Settings</source>
      <translation>Print Settings</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>Mirror Page(s) Horizontal</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>Mirror Page(s) Vertical</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>Clip to Page Margins</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>Print in Greyscale</translation>
    </message>
    <message>
      <source>Convert Spot Colors</source>
      <translation>Convert Spot Colours</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Enables Spot Colours to be converted to composite colours. Unless you are planning to print spot colours at a commercial printer, this is probably best left enabled.</translation>
    </message>
    <message>
      <source>Enables global Overprint Mode for this document, overrides object settings</source>
      <translation>Enables global Overprint Mode for this document, overrides object settings</translation>
    </message>
    <message>
      <source>Apply Color Profiles</source>
      <translation>Apply Colour Profiles</translation>
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation>Allows you to embed color profiles in the print stream when color management is enabled</translation>
    </message>
  </context>
  <context>
    <name>PSLib</name>
    <message>
      <source>Processing Master Page:</source>
      <translation>Processing Master Page:</translation>
    </message>
    <message>
      <source>Exporting Page:</source>
      <translation>Exporting Page:</translation>
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
      <source>None</source>
      <comment>relationship</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Page Item Attributes</source>
      <translation>Page Item Attributes</translation>
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
  </context>
  <context>
    <name>PageItem_ImageFrame</name>
    <message>
      <source>Picture</source>
      <translation>Picture</translation>
    </message>
    <message>
      <source>Preview Settings</source>
      <translation>Preview Settings</translation>
    </message>
  </context>
  <context>
    <name>PageItem_LatexFrame</name>
    <message>
      <source>Latex</source>
      <translation>Latex</translation>
    </message>
    <message>
      <source>\section*{Manual}
Your \LaTeX-frames setup is working when you can read this text!\\
Placing formulas is very easy:\\
Right click $\Rightarrow$ Edit Latex Source\\
And replace this text with your own. Here is an example:
\begin{verbatim}\[J = \int r^2 \mathrm{d}m\]\end{verbatim}
becomes
\[J = \int r^2 \mathrm{d}m\]
</source>
      <translation>\section*{Manual}
Your \LaTeX-frames setup is working when you can read this text!\\
Placing formulas is very easy:\\
Right click $\Rightarrow$ Edit Latex Source\\
And replace this text with your own. Here is an example:
\begin{verbatim}\[J = \int r^2 \mathrm{d}m\]\end{verbatim}
becomes
\[J = \int r^2 \mathrm{d}m\]
</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>Error</translation>
    </message>
    <message>
      <source>Running the external application failed!</source>
      <translation>Running the external application failed!</translation>
    </message>
    <message>
      <source>Could not create a temporary file to run the application!</source>
      <translation>Could not create a temporary file to run the application!</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>Information</translation>
    </message>
    <message>
      <source>Please specify a latex executable in the preferences!</source>
      <translation>Please specify a latex executable in the preferences!</translation>
    </message>
    <message>
      <source>An editor for this frame is already running!</source>
      <translation>An editor for this frame is already running!</translation>
    </message>
    <message>
      <source>Please specify an editor in the preferences!</source>
      <translation>Please specify an editor in the preferences!</translation>
    </message>
    <message>
      <source>Could not create a temporary file to run the external editor!</source>
      <translation>Could not create a temporary file to run the external editor!</translation>
    </message>
    <message>
      <source>Running the editor failed with exitcode %d!</source>
      <translation>Running the editor failed with exitcode %d!</translation>
    </message>
    <message>
      <source>Running the editor &quot;%1&quot; failed!</source>
      <translation>Running the editor &quot;%1&quot; failed!</translation>
    </message>
    <message>
      <source>Running the application &quot;%1&quot; failed!</source>
      <translation>Running the application &quot;%1&quot; failed!</translation>
    </message>
    <message>
      <source>Latex-Frame</source>
      <translation>Latex-Frame</translation>
    </message>
    <message>
      <source>Command: </source>
      <translation>Command: </translation>
    </message>
    <message>
      <source>DPI: </source>
      <translation>DPI: </translation>
    </message>
    <message>
      <source>Status: </source>
      <translation>Status: </translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Running</source>
      <translation>Running</translation>
    </message>
    <message>
      <source>Errorcode </source>
      <translation>Errorcode </translation>
    </message>
  </context>
  <context>
    <name>PageItem_PathText</name>
    <message>
      <source>Paragraphs: </source>
      <translation>Paragraphs: </translation>
    </message>
    <message>
      <source>Lines: </source>
      <translation>Lines: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>Words: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>Chars: </translation>
    </message>
  </context>
  <context>
    <name>PageItem_TextFrame</name>
    <message>
      <source>Linked Text</source>
      <translation>Linked Text</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation>Text Frame</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>Paragraphs: </translation>
    </message>
    <message>
      <source>Lines: </source>
      <translation>Lines: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>Words: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>Chars: </translation>
    </message>
  </context>
  <context>
    <name>PageLayouts</name>
    <message>
      <source>First Page is:</source>
      <translation>First Page is:</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation>Document Layout</translation>
    </message>
  </context>
  <context>
    <name>PagePalette</name>
    <message>
      <source>Drag pages or master pages onto the trashbin to delete them</source>
      <translation>Drag pages or master pages onto the trashbin to delete them</translation>
    </message>
    <message>
      <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
      <translation>Here are all your master pages. To create a new page, drag a master page to the page view below</translation>
    </message>
    <message>
      <source>Arrange Pages</source>
      <translation>Arrange Pages</translation>
    </message>
    <message>
      <source>Available Master Pages:</source>
      <translation>Available Master Pages:</translation>
    </message>
    <message>
      <source>Document Pages:</source>
      <translation>Document Pages:</translation>
    </message>
  </context>
  <context>
    <name>PageSelector</name>
    <message>
      <source>%1 of %2</source>
      <translation>%1 of %2</translation>
    </message>
  </context>
  <context>
    <name>ParaStyleComboBox</name>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
  </context>
  <context>
    <name>PatternDialog</name>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>Loading Patterns</source>
      <translation>Loading Patterns</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>Patterns</source>
      <translation>Patterns</translation>
    </message>
    <message>
      <source>Load</source>
      <translation>Load</translation>
    </message>
    <message>
      <source>Load Set</source>
      <translation>Load Set</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>Remove</translation>
    </message>
    <message>
      <source>Remove All</source>
      <translation>Remove All</translation>
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
    <name>PicSearch</name>
    <message>
      <source>Size:</source>
      <translation>Size:</translation>
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
      <source>Unknown</source>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation>Colourspace:</translation>
    </message>
    <message>
      <source>Result</source>
      <translation>Result</translation>
    </message>
    <message>
      <source>Search Results for: </source>
      <translation>Search Results for: </translation>
    </message>
    <message>
      <source>&amp;Preview</source>
      <translation>&amp;Preview</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>&amp;Select</source>
      <translation>&amp;Select</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
    </message>
  </context>
  <context>
    <name>PicSearchOptions</name>
    <message>
      <source>The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows</source>
      <translation>The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows</translation>
    </message>
    <message>
      <source>Cancel Search</source>
      <translation>Cancel Search</translation>
    </message>
    <message>
      <source>Start Search</source>
      <translation>Start Search</translation>
    </message>
    <message>
      <source>Select a base directory for search</source>
      <translation>Select a base directory for search</translation>
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
      <source>Search Images</source>
      <translation>Search Images</translation>
    </message>
    <message>
      <source>Search for:</source>
      <translation>Search for:</translation>
    </message>
    <message>
      <source>Start at:</source>
      <translation>Start at:</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>Change...</translation>
    </message>
    <message>
      <source>Searching</source>
      <translation>Searching</translation>
    </message>
    <message>
      <source>Case insensitive search</source>
      <translation>Case insensitive search</translation>
    </message>
    <message>
      <source>Search recursively</source>
      <translation>Search recursively</translation>
    </message>
  </context>
  <context>
    <name>PicStatus</name>
    <message>
      <source>Goto</source>
      <translation>Goto</translation>
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
      <source>No images named &quot;%1&quot; were found.</source>
      <translation>No images named &quot;%1&quot; were found.</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Close</translation>
    </message>
    <message>
      <source>Not on a Page</source>
      <translation>Not on a Page</translation>
    </message>
    <message>
      <source>JPG</source>
      <translation>JPG</translation>
    </message>
    <message>
      <source>TIFF</source>
      <translation>TIFF</translation>
    </message>
    <message>
      <source>PSD</source>
      <translation>PSD</translation>
    </message>
    <message>
      <source>EPS/PS</source>
      <translation>EPS/PS</translation>
    </message>
    <message>
      <source>PDF</source>
      <translation>PDF</translation>
    </message>
    <message>
      <source>JPG2000</source>
      <translation>JPG2000</translation>
    </message>
    <message>
      <source>emb. PSD</source>
      <translation>emb. PSD</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>n/a</source>
      <translation>n/a</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>Information</translation>
    </message>
    <message>
      <source>Path:</source>
      <translation>Path:</translation>
    </message>
    <message>
      <source>Search...</source>
      <translation>Search...</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Name:</translation>
    </message>
    <message>
      <source>Image</source>
      <translation>Image</translation>
    </message>
    <message>
      <source>DPI:</source>
      <translation>DPI:</translation>
    </message>
    <message>
      <source>Format:</source>
      <translation>Format:</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation>Colourspace:</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Size</translation>
    </message>
    <message>
      <source>Pixels:</source>
      <translation>Pixels:</translation>
    </message>
    <message>
      <source>Scale:</source>
      <translation>Scale:</translation>
    </message>
    <message>
      <source>Printed:</source>
      <translation>Printed:</translation>
    </message>
    <message>
      <source>Layout</source>
      <translation>Layout</translation>
    </message>
    <message>
      <source>On Page:</source>
      <translation>On Page:</translation>
    </message>
    <message>
      <source>eff. DPI:</source>
      <translation>eff. DPI:</translation>
    </message>
    <message>
      <source>Object:</source>
      <translation>Object:</translation>
    </message>
    <message>
      <source>Select</source>
      <translation>Select</translation>
    </message>
    <message>
      <source>Image Tools</source>
      <translation>Image Tools</translation>
    </message>
    <message>
      <source>Image Visible</source>
      <translation>Image Visible</translation>
    </message>
    <message>
      <source>Image Effects...</source>
      <translation>Image Effects...</translation>
    </message>
    <message>
      <source>Edit Image...</source>
      <translation>Edit Image...</translation>
    </message>
    <message>
      <source>Print Image</source>
      <translation>Print Image</translation>
    </message>
    <message>
      <source>Extended Image Properties...</source>
      <translation>Extended Image Properties...</translation>
    </message>
  </context>
  <context>
    <name>PixmapExportPlugin</name>
    <message>
      <source>Save as &amp;Image...</source>
      <translation>Save as &amp;Image...</translation>
    </message>
    <message>
      <source>Export As Image</source>
      <translation>Export As Image</translation>
    </message>
    <message>
      <source>Exports selected pages as bitmap images.</source>
      <translation>Exports selected pages as bitmap images.</translation>
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
      <source>Export successful</source>
      <translation>Export successful</translation>
    </message>
  </context>
  <context>
    <name>PluginManager</name>
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
    <message>
      <source>Plugin: loading %1</source>
      <comment>plugin manager</comment>
      <translation>Plugin: loading %1</translation>
    </message>
    <message>
      <source>init failed</source>
      <comment>plugin load error</comment>
      <translation>init failed</translation>
    </message>
    <message>
      <source>unknown plugin type</source>
      <comment>plugin load error</comment>
      <translation>unknown plugin type</translation>
    </message>
    <message>
      <source>Plugin: %1 loaded</source>
      <comment>plugin manager</comment>
      <translation>Plugin: %1 loaded</translation>
    </message>
    <message>
      <source>Plugin: %1 failed to load: %2</source>
      <comment>plugin manager</comment>
      <translation>Plugin: %1 failed to load: %2</translation>
    </message>
    <message>
      <source>Plugin: %1 initialized ok </source>
      <comment>plugin manager</comment>
      <translation>Plugin: %1 initialised ok </translation>
    </message>
    <message>
      <source>Plugin: %1 failed post initialization</source>
      <comment>plugin manager</comment>
      <translation>Plugin: %1 failed post initialisation</translation>
    </message>
    <message>
      <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
      <translation>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</translation>
    </message>
  </context>
  <context>
    <name>PluginManagerPrefsGui</name>
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
      <source>You need to restart the application to apply the changes.</source>
      <translation>You need to restart the application to apply the changes.</translation>
    </message>
    <message>
      <source>Form</source>
      <translation>Form</translation>
    </message>
  </context>
  <context>
    <name>PolygonProps</name>
    <message>
      <source>Polygon Properties</source>
      <translation>Polygon Properties</translation>
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
      <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
      <translation>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</translation>
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
      <source>Display</source>
      <translation>Display</translation>
    </message>
    <message>
      <source>External Tools</source>
      <translation>External Tools</translation>
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
      <source>Miscellaneous</source>
      <translation>Miscellaneous</translation>
    </message>
    <message>
      <source>Plugins</source>
      <translation>Plugins</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Preflight Verifier</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>Scrapbook</translation>
    </message>
    <message>
      <source>Printer</source>
      <translation>Printer</translation>
    </message>
  </context>
  <context>
    <name>PrefsDialogBase</name>
    <message>
      <source>&amp;Defaults</source>
      <translation>&amp;Defaults</translation>
    </message>
    <message>
      <source>Save Preferences</source>
      <translation>Save Preferences</translation>
    </message>
    <message>
      <source>Export...</source>
      <translation>Export...</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>&amp;Apply</translation>
    </message>
    <message>
      <source>All preferences can be reset here</source>
      <translation>All preferences can be reset here</translation>
    </message>
    <message>
      <source>Apply all changes without closing the dialog</source>
      <translation>Apply all changes without closing the dialog</translation>
    </message>
    <message>
      <source>Export current preferences into file</source>
      <translation>Export current preferences into file</translation>
    </message>
  </context>
  <context>
    <name>PrefsManager</name>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
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
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
      <translation>Could not open preferences file &quot;%1&quot; for writing: %2</translation>
    </message>
    <message>
      <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
      <translation>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</translation>
    </message>
    <message>
      <source>Failed to open prefs file &quot;%1&quot;: %2</source>
      <translation>Failed to open prefs file &quot;%1&quot;: %2</translation>
    </message>
    <message>
      <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
      <translation>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</translation>
    </message>
    <message>
      <source>Error Writing Preferences</source>
      <translation>Error Writing Preferences</translation>
    </message>
    <message>
      <source>Scribus was not able to save its preferences:&lt;br>%1&lt;br>Please check file and directory permissions and available disk space.</source>
      <comment>scribus app error</comment>
      <translation>Scribus was not able to save its preferences:&lt;br>%1&lt;br>Please check file and directory permissions and available disk space.</translation>
    </message>
    <message>
      <source>Error Loading Preferences</source>
      <translation>Error Loading Preferences</translation>
    </message>
    <message>
      <source>Scribus was not able to load its preferences:&lt;br>%1&lt;br>Default settings will be loaded.</source>
      <translation>Scribus was not able to load its preferences:&lt;br>%1&lt;br>Default settings will be loaded.</translation>
    </message>
  </context>
  <context>
    <name>PresetLayout</name>
    <message>
      <source>Magazine</source>
      <translation>Magazine</translation>
    </message>
    <message>
      <source>Fibonacci</source>
      <translation>Fibonacci</translation>
    </message>
    <message>
      <source>Golden Mean</source>
      <translation>Golden Mean</translation>
    </message>
    <message>
      <source>Nine Parts</source>
      <translation>Nine Parts</translation>
    </message>
    <message>
      <source>Gutenberg</source>
      <translation>Gutenberg</translation>
    </message>
    <message>
      <source>None</source>
      <comment>layout type</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>You can select a predefined page layout here. 'None' leave margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins for same value. Leading is Left/Inside value.</source>
      <translation>You can select a predefined page layout here. 'None' leave margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins for same value. Leading is Left/Inside value.</translation>
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
      <source>Save the Python Commands in File</source>
      <translation>Save the Python Commands in File</translation>
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
      <source>Save &amp;As...</source>
      <translation>Save &amp;As...</translation>
    </message>
    <message>
      <source>Run As &amp;Console</source>
      <translation>Run As &amp;Console</translation>
    </message>
    <message>
      <source>Scribus Python Console</source>
      <translation>Scribus Python Console</translation>
    </message>
    <message>
      <source>Open Python Script File</source>
      <translation>Open Python Script File</translation>
    </message>
    <message>
      <source>Python Scripts (*.py *.PY)</source>
      <translation>Python Scripts (*.py *.PY)</translation>
    </message>
    <message>
      <source>Col: %1 Row: %2/%3</source>
      <translation>Col: %1 Row: %2/%3</translation>
    </message>
    <message>
      <source>Ctrl+O</source>
      <translation>Ctrl+O</translation>
    </message>
    <message>
      <source>Ctrl+S</source>
      <translation>Ctrl+S</translation>
    </message>
    <message>
      <source>This is derived from standard Python console
so it contains some limitations esp. in the
case of whitespaces. Please consult Scribus
manual for more informations.</source>
      <translation>This is derived from standard Python console
so it contains some limitations esp. in the
case of whitespaces. Please consult Scribus
manual for more informations.</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation>&amp;Quit</translation>
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
      <source>Background</source>
      <translation>Background</translation>
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
      <source>All Files (*)</source>
      <translation>All Files (*)</translation>
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
      <source>Scribus Development Version</source>
      <translation>Scribus Development Version</translation>
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
      <source>All</source>
      <translation>All</translation>
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
      <source>Could not open output file %1</source>
      <translation>Could not open output file %1</translation>
    </message>
    <message>
      <source>Could not open input file %1</source>
      <translation>Could not open input file %1</translation>
    </message>
    <message>
      <source>Freetype2 library not available</source>
      <translation>Freetype2 library not available</translation>
    </message>
    <message>
      <source>Font %1 is broken (read stream), no embedding</source>
      <translation>Font %1 is broken (read stream), no embedding</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2 (charcode %3)</source>
      <translation>Font %1 has broken glyph %2 (charcode %3)</translation>
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
      <source>page</source>
      <comment>page export</comment>
      <translation>page</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Copy #%1 of </translation>
    </message>
    <message>
      <source>Black</source>
      <translation>Black</translation>
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
      <source>Color Wheel</source>
      <translation>Colour Wheel</translation>
    </message>
    <message>
      <source>Font Preview</source>
      <translation>Font Preview</translation>
    </message>
    <message>
      <source>My Plugin</source>
      <translation>My Plugin</translation>
    </message>
    <message>
      <source>New From Template</source>
      <translation>New From Template</translation>
    </message>
    <message>
      <source>Save As Template</source>
      <translation>Save As Template</translation>
    </message>
    <message>
      <source>Scripter</source>
      <translation>Scripter</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>SVG Export</source>
      <translation>SVG Export</translation>
    </message>
    <message>
      <source>SVG Import</source>
      <translation>SVG Import</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw Importer</source>
      <translation>OpenOffice.org Draw Importer</translation>
    </message>
    <message>
      <source>Scribus crashes due to the following exception : %1</source>
      <translation>Scribus crashes due to the following exception : %1</translation>
    </message>
    <message>
      <source>Creating Font Cache</source>
      <translation>Creating Font Cache</translation>
    </message>
    <message>
      <source>New Font found, checking...</source>
      <translation>New Font found, checking...</translation>
    </message>
    <message>
      <source>Modified Font found, checking...</source>
      <translation>Modified Font found, checking...</translation>
    </message>
    <message>
      <source>Reading Font Cache</source>
      <translation>Reading Font Cache</translation>
    </message>
    <message>
      <source>Writing updated Font Cache</source>
      <translation>Writing updated Font Cache</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>Searching for Fonts</translation>
    </message>
    <message>
      <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
      <translation>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</translation>
    </message>
    <message>
      <source>A file named '%1' already exists.&lt;br/>Do you want to replace it with the file you are saving?</source>
      <translation>A file named '%1' already exists.&lt;br/>Do you want to replace it with the file you are saving?</translation>
    </message>
    <message>
      <source>firstPageOrder is bigger than allowed.</source>
      <comment>python error</comment>
      <translation>firstPageOrder is bigger than allowed.</translation>
    </message>
    <message>
      <source>Old .sla format support</source>
      <translation>Old .sla format support</translation>
    </message>
    <message>
      <source>German (Trad.)</source>
      <translation>German (Trad.)</translation>
    </message>
    <message>
      <source>Exporting PostScript File</source>
      <translation>Exporting PostScript File</translation>
    </message>
    <message>
      <source>Printing File</source>
      <translation>Printing File</translation>
    </message>
    <message>
      <source>&lt;p>You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p>Choose one of the following:&lt;br>&lt;ul>&lt;li>&lt;b>Create&lt;/b> missing pages&lt;/li>&lt;li>&lt;b>Import&lt;/b> pages until the last page&lt;/li>&lt;li>&lt;b>Cancel&lt;/b>&lt;/li>&lt;/ul></source>
      <translation>&lt;p>You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p>Choose one of the following:&lt;br>&lt;ul>&lt;li>&lt;b>Create&lt;/b> missing pages&lt;/li>&lt;li>&lt;b>Import&lt;/b> pages until the last page&lt;/li>&lt;li>&lt;b>Cancel&lt;/b>&lt;/li>&lt;/ul></translation>
    </message>
    <message>
      <source>C&amp;reate</source>
      <translation>C&amp;reate</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>&amp;Import</translation>
    </message>
    <message>
      <source>Thai</source>
      <translation>Thai</translation>
    </message>
    <message>
      <source>Barcode Generator</source>
      <translation>Barcode Generator</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
      <translation>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</translation>
    </message>
    <message>
      <source>Word Documents</source>
      <translation>Word Documents</translation>
    </message>
    <message>
      <source>Palm PDB Documents</source>
      <comment>PDB Importer</comment>
      <translation>Palm PDB Documents</translation>
    </message>
    <message>
      <source>PDB_data</source>
      <comment>PDB Importer</comment>
      <translation>PDB_data</translation>
    </message>
    <message>
      <source>PDB Import</source>
      <comment>PDB Importer</comment>
      <translation>PDB Import</translation>
    </message>
    <message>
      <source>Could not open file %1</source>
      <comment>PDB Importer</comment>
      <translation>Could not open file %1</translation>
    </message>
    <message>
      <source>Luxembourgish</source>
      <translation>Luxembourgish</translation>
    </message>
    <message>
      <source>Japanese</source>
      <translation>Japanese</translation>
    </message>
    <message>
      <source>Given master page name does not match any existing.</source>
      <comment>python error</comment>
      <translation>Given master page name does not match any existing.</translation>
    </message>
    <message>
      <source>Font %1(%2) is broken</source>
      <translation>Font %1(%2) is broken</translation>
    </message>
    <message>
      <source>Arabic</source>
      <translation>Arabic</translation>
    </message>
    <message>
      <source>Dzongkha</source>
      <translation>Dzongkha</translation>
    </message>
    <message>
      <source>Estonian</source>
      <translation>Estonian</translation>
    </message>
    <message>
      <source>font %1 </source>
      <translation>font %1 </translation>
    </message>
    <message>
      <source>size %1 </source>
      <translation>size %1 </translation>
    </message>
    <message>
      <source>+style </source>
      <translation>+style </translation>
    </message>
    <message>
      <source>+color </source>
      <translation>+colour </translation>
    </message>
    <message>
      <source>+underline </source>
      <translation>+underline </translation>
    </message>
    <message>
      <source>-underline </source>
      <translation>-underline </translation>
    </message>
    <message>
      <source>+strikeout </source>
      <translation>+strikeout </translation>
    </message>
    <message>
      <source>-strikeout </source>
      <translation>-strikeout </translation>
    </message>
    <message>
      <source>+shadow </source>
      <translation>+shadow </translation>
    </message>
    <message>
      <source>-shadow </source>
      <translation>-shadow </translation>
    </message>
    <message>
      <source>+outline </source>
      <translation>+outline </translation>
    </message>
    <message>
      <source>-outline </source>
      <translation>-outline </translation>
    </message>
    <message>
      <source>-tracking </source>
      <translation>-tracking </translation>
    </message>
    <message>
      <source>+stretch </source>
      <translation>+stretch </translation>
    </message>
    <message>
      <source>parent= %1</source>
      <translation>parent= %1</translation>
    </message>
    <message>
      <source>Font %1 is broken (no Face), discarding it</source>
      <translation>Font %1 is broken (no Face), discarding it</translation>
    </message>
    <message>
      <source>Latin</source>
      <translation>Latin</translation>
    </message>
    <message>
      <source>Icelandic</source>
      <translation>Icelandic</translation>
    </message>
    <message>
      <source>Romanian</source>
      <translation>Romanian</translation>
    </message>
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
      <source>Govt. Letter</source>
      <translation>Govt. Letter</translation>
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
    <message>
      <source>%1 may be corrupted : missing resolution tags</source>
      <translation>%1 may be corrupted : missing resolution tags</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2</source>
      <translation>Font %1 has broken glyph %2</translation>
    </message>
    <message>
      <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
      <comment>python error</comment>
      <translation>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</translation>
    </message>
    <message>
      <source>Blendmode out of bounds, must be 0 &lt;= blendmode &lt;= 15.</source>
      <comment>python error</comment>
      <translation>Blendmode out of bounds, must be 0 &lt;= blendmode &lt;= 15.</translation>
    </message>
    <message>
      <source>Scribus 1.2.x Support</source>
      <translation>Scribus 1.2.x Support</translation>
    </message>
    <message>
      <source>Scribus 1.3.4 Support</source>
      <translation>Scribus 1.3.4 Support</translation>
    </message>
    <message>
      <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
      <comment>PDB Importer</comment>
      <translation>This file is not recognised as a PDB document. Please, report this as a bug if you are sure it is one.</translation>
    </message>
    <message>
      <source>Cannot get number of lines of non-text frame.</source>
      <comment>python error</comment>
      <translation>Cannot get number of lines of non-text frame.</translation>
    </message>
    <message>
      <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</source>
      <translation>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</translation>
    </message>
    <message>
      <source>+tracking %1 </source>
      <translation>+tracking %1 </translation>
    </message>
    <message>
      <source>+baseline %1 </source>
      <translation>+baseline %1 </translation>
    </message>
    <message>
      <source>Breton</source>
      <translation>Breton</translation>
    </message>
    <message>
      <source>English (American)</source>
      <translation>English (American)</translation>
    </message>
    <message>
      <source>%1 may be corrupted : missing or wrong resolution tags</source>
      <translation>%1 may be corrupted : missing or wrong resolution tags</translation>
    </message>
    <message>
      <source>The Font(s):
%1 are not available.
They have been replaced by &quot;Courier&quot;
Therefore the image may be not correct</source>
      <translation>The Font(s):
%1 are not available.
They have been replaced by &quot;Courier&quot;
Therefore the image may be not correct</translation>
    </message>
    <message>
      <source>English (Australian)</source>
      <translation>English (Australian)</translation>
    </message>
    <message>
      <source>German (Swiss)</source>
      <translation>German (Swiss)</translation>
    </message>
    <message>
      <source>Hebrew</source>
      <translation>Hebrew</translation>
    </message>
    <message>
      <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
      <translation>Font %1 has broken metrics in file %2, ignoring metrics</translation>
    </message>
    <message>
      <source>Chinese (Trad.)</source>
      <translation>Chinese (Trad.)</translation>
    </message>
    <message>
      <source>Scribus 1.3.0->1.3.3.x Support</source>
      <translation>Scribus 1.3.0->1.3.3.x Support</translation>
    </message>
    <message>
      <source>Copy of %1 (%2)</source>
      <translation>Copy of %1 (%2)</translation>
    </message>
    <message>
      <source>Image</source>
      <translation>Image</translation>
    </message>
    <message>
      <source>PDF Push Button</source>
      <translation>PDF Push Button</translation>
    </message>
    <message>
      <source>PDF Text Field</source>
      <translation>PDF Text Field</translation>
    </message>
    <message>
      <source>PDF Check Box</source>
      <translation>PDF Check Box</translation>
    </message>
    <message>
      <source>PDF Combo Box</source>
      <translation>PDF Combo Box</translation>
    </message>
    <message>
      <source>PDF List Box</source>
      <translation>PDF List Box</translation>
    </message>
    <message>
      <source>PDF Text Annotation</source>
      <translation>PDF Text Annotation</translation>
    </message>
    <message>
      <source>PDF Link Annotation</source>
      <translation>PDF Link Annotation</translation>
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
      <source>%</source>
      <translation>%</translation>
    </message>
    <message>
      <source>Khmer</source>
      <translation>Khmer</translation>
    </message>
    <message>
      <source>Lao</source>
      <translation>Lao</translation>
    </message>
    <message>
      <source>Vietnamese</source>
      <translation>Vietnamese</translation>
    </message>
    <message>
      <source>An error occurred while initializing icc transforms</source>
      <translation>An error occurred while initializing icc transforms</translation>
    </message>
    <message>
      <source>Output profile is not supported</source>
      <translation>Output profile is not supported</translation>
    </message>
    <message>
      <source>°</source>
      <comment>degrees, unicode 0xB0</comment>
      <translation>°</translation>
    </message>
    <message>
      <source>WMF Import</source>
      <translation>WMF Import</translation>
    </message>
    <message>
      <source>New Layer</source>
      <translation>New Layer</translation>
    </message>
    <message>
      <source>Adobe Illustrator Importer</source>
      <translation>Adobe Illustrator Importer</translation>
    </message>
    <message>
      <source>Imposition</source>
      <translation>Imposition</translation>
    </message>
    <message>
      <source>PostScript Importer</source>
      <translation>PostScript Importer</translation>
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation>%1;;All Files (*)</translation>
    </message>
    <message>
      <source>Do you really want to overwrite the file:
%1 ?</source>
      <translation>Do you really want to overwrite the file:
%1 ?</translation>
    </message>
    <message>
      <source>Encapsulated PostScript</source>
      <translation>Encapsulated PostScript</translation>
    </message>
    <message>
      <source>GIF</source>
      <translation>GIF</translation>
    </message>
    <message>
      <source>JPEG</source>
      <translation>JPEG</translation>
    </message>
    <message>
      <source>Pattern Files</source>
      <translation>Pattern Files</translation>
    </message>
    <message>
      <source>PDF Document</source>
      <translation>PDF Document</translation>
    </message>
    <message>
      <source>PNG</source>
      <translation>PNG</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Adobe Photoshop</source>
      <translation>Adobe Photoshop</translation>
    </message>
    <message>
      <source>TIFF</source>
      <translation>TIFF</translation>
    </message>
    <message>
      <source>XPM</source>
      <translation>XPM</translation>
    </message>
    <message>
      <source>Windows Meta File</source>
      <translation>Windows Meta File</translation>
    </message>
    <message>
      <source>Scalable Vector Graphics</source>
      <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
      <source>Adobe Illustrator</source>
      <translation>Adobe Illustrator</translation>
    </message>
    <message>
      <source>Norwegian (Bokm�l)</source>
      <translation>Norwegian (Bokm�l)</translation>
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
    <name>ReformDoc</name>
    <message>
      <source>Document Setup</source>
      <translation>Document Setup</translation>
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
      <source>Document Information</source>
      <translation>Document Information</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Preflight Verifier</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>Adjusting Colours</translation>
    </message>
    <message>
      <source>Sections</source>
      <translation>Sections</translation>
    </message>
  </context>
  <context>
    <name>RunScriptDialog</name>
    <message>
      <source>Python Scripts (*.py *.PY);; All Files (*)</source>
      <translation>Python Scripts (*.py *.PY);; All Files (*)</translation>
    </message>
    <message>
      <source>Dialog</source>
      <translation>Dialog</translation>
    </message>
    <message>
      <source>Run as Extension Script</source>
      <translation>Run as Extension Script</translation>
    </message>
  </context>
  <context>
    <name>SMAlignSelect</name>
    <message>
      <source>P</source>
      <comment>P as in Parent</comment>
      <translation>P</translation>
    </message>
    <message>
      <source>Use parent style's alignment instead of overriding it</source>
      <translation>Use parent style's alignment instead of overriding it</translation>
    </message>
  </context>
  <context>
    <name>SMCStyleWidget</name>
    <message>
      <source>Parent style</source>
      <translation>Parent style</translation>
    </message>
    <message>
      <source>Font face</source>
      <translation>Font face</translation>
    </message>
    <message>
      <source>Font size</source>
      <translation>Font size</translation>
    </message>
    <message>
      <source>Tracking</source>
      <translation>Tracking</translation>
    </message>
    <message>
      <source>Baseline offset</source>
      <translation>Baseline offset</translation>
    </message>
    <message>
      <source>Horizontal scaling</source>
      <translation>Horizontal scaling</translation>
    </message>
    <message>
      <source>Vertical scaling</source>
      <translation>Vertical scaling</translation>
    </message>
    <message>
      <source>Language</source>
      <translation>Language</translation>
    </message>
    <message>
      <source>Fill color</source>
      <translation>Fill colour</translation>
    </message>
    <message>
      <source>Fill shade</source>
      <translation>Fill shade</translation>
    </message>
    <message>
      <source>Stroke color</source>
      <translation>Stroke colour</translation>
    </message>
    <message>
      <source>Stroke shade</source>
      <translation>Stroke shade</translation>
    </message>
    <message>
      <source>Based On:</source>
      <translation>Based On:</translation>
    </message>
    <message>
      <source>Language:</source>
      <translation>Language:</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Shade</translation>
    </message>
    <message>
      <source>Basic Formatting</source>
      <translation>Basic Formatting</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Advanced Formatting</source>
      <translation>Advanced Formatting</translation>
    </message>
    <message>
      <source>TextLabel</source>
      <translation>TextLabel</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
  </context>
  <context>
    <name>SMCharacterStyle</name>
    <message>
      <source>Properties</source>
      <translation>Properties</translation>
    </message>
    <message>
      <source>Character Styles</source>
      <translation>Character Styles</translation>
    </message>
    <message>
      <source>Character Style</source>
      <translation>Character Style</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>New Style</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>Clone of %1</translation>
    </message>
    <message>
      <source>%1 (%2)</source>
      <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
      <translation>%1 (%2)</translation>
    </message>
  </context>
  <context>
    <name>SMColorCombo</name>
    <message>
      <source>Use Parent Value</source>
      <translation>Use Parent Value</translation>
    </message>
  </context>
  <context>
    <name>SMFontComboH</name>
    <message>
      <source>Use Parent Font</source>
      <translation>Use Parent Font</translation>
    </message>
  </context>
  <context>
    <name>SMLineStyle</name>
    <message>
      <source>Properties</source>
      <translation>Properties</translation>
    </message>
    <message>
      <source>Line Styles</source>
      <translation>Line Styles</translation>
    </message>
    <message>
      <source>Line Style</source>
      <translation>Line Style</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>New Style</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>Clone of %1</translation>
    </message>
    <message>
      <source>%1 (%2)</source>
      <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
      <translation>%1 (%2)</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
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
      <source> pt </source>
      <translation> pt </translation>
    </message>
  </context>
  <context>
    <name>SMLineStyleWidget</name>
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
      <source>Add a new line</source>
      <translation>Add a new line</translation>
    </message>
    <message>
      <source>Remove a line</source>
      <translation>Remove a line</translation>
    </message>
    <message>
      <source>Line style</source>
      <translation>Line style</translation>
    </message>
    <message>
      <source>Line width</source>
      <translation>Line width</translation>
    </message>
    <message>
      <source>End style</source>
      <translation>End style</translation>
    </message>
    <message>
      <source>Join style</source>
      <translation>Join style</translation>
    </message>
    <message>
      <source>Line color</source>
      <translation>Line colour</translation>
    </message>
    <message>
      <source>Line shade</source>
      <translation>Line shade</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>%</source>
      <translation>%</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>Line Width:</translation>
    </message>
  </context>
  <context>
    <name>SMPStyleWidget</name>
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
      <source>Distances and Alignment</source>
      <translation>Distances and Alignment</translation>
    </message>
    <message>
      <source>Drop Caps</source>
      <translation>Drop Caps</translation>
    </message>
    <message>
      <source>Tabulators and Indentation</source>
      <translation>Tabulators and Indentation</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>Properties</translation>
    </message>
    <message>
      <source>Character Style</source>
      <translation>Character Style</translation>
    </message>
    <message>
      <source>&amp;Lines:</source>
      <translation>&amp;Lines:</translation>
    </message>
    <message>
      <source>Distance from Text:</source>
      <translation>Distance from Text:</translation>
    </message>
    <message>
      <source>Alignment</source>
      <translation>Alignment</translation>
    </message>
    <message>
      <source>Parent Style</source>
      <translation>Parent Style</translation>
    </message>
    <message>
      <source>Line Spacing Mode</source>
      <translation>Line Spacing Mode</translation>
    </message>
    <message>
      <source>Line Spacing</source>
      <translation>Line Spacing</translation>
    </message>
    <message>
      <source>Space Above</source>
      <translation>Space Above</translation>
    </message>
    <message>
      <source>Space Below</source>
      <translation>Space Below</translation>
    </message>
    <message>
      <source>Drop Cap Lines</source>
      <translation>Drop Cap Lines</translation>
    </message>
    <message>
      <source>Drop Cap Offset</source>
      <translation>Drop Cap Offset</translation>
    </message>
    <message>
      <source>First Line Indent</source>
      <translation>First Line Indent</translation>
    </message>
    <message>
      <source>Left Indent</source>
      <translation>Left Indent</translation>
    </message>
    <message>
      <source>Right Indent</source>
      <translation>Right Indent</translation>
    </message>
    <message>
      <source>Based On:</source>
      <translation>Based On:</translation>
    </message>
    <message>
      <source>TextLabel</source>
      <translation>TextLabel</translation>
    </message>
    <message>
      <source>Parent's Drop Cap Status</source>
      <translation>Parent's Drop Cap Status</translation>
    </message>
    <message>
      <source>Ch&amp;aracter Style</source>
      <translation>Ch&amp;aracter Style</translation>
    </message>
  </context>
  <context>
    <name>SMParagraphStyle</name>
    <message>
      <source>Paragraph Styles</source>
      <translation>Paragraph Styles</translation>
    </message>
    <message>
      <source>Paragraph Style</source>
      <translation>Paragraph Style</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>New Style</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>Clone of %1</translation>
    </message>
    <message>
      <source>%1 (%2)</source>
      <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
      <translation>%1 (%2)</translation>
    </message>
  </context>
  <context>
    <name>SMReplaceDia</name>
    <message>
      <source>Remove</source>
      <translation>Remove</translation>
    </message>
    <message>
      <source>Replace with</source>
      <translation>Replace with</translation>
    </message>
    <message>
      <source>Delete Styles</source>
      <translation>Delete Styles</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Ca&amp;ncel</source>
      <translation>Ca&amp;ncel</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
  </context>
  <context>
    <name>SMRowWidget</name>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
  </context>
  <context>
    <name>SMScComboBox</name>
    <message>
      <source>Use Parent Value</source>
      <translation>Use Parent Value</translation>
    </message>
  </context>
  <context>
    <name>SMShadeButton</name>
    <message>
      <source>Use Parent Value</source>
      <translation>Use Parent Value</translation>
    </message>
  </context>
  <context>
    <name>SMStyleImport</name>
    <message>
      <source>Character Styles</source>
      <translation>Character Styles</translation>
    </message>
    <message>
      <source>Paragraph Styles</source>
      <translation>Paragraph Styles</translation>
    </message>
    <message>
      <source>Line Styles</source>
      <translation>Line Styles</translation>
    </message>
    <message>
      <source>Choose Styles</source>
      <translation>Choose Styles</translation>
    </message>
    <message>
      <source>Available Styles</source>
      <translation>Available Styles</translation>
    </message>
    <message>
      <source>In case of the name clash</source>
      <translation>In case of the name clash</translation>
    </message>
    <message>
      <source>&amp;Rename Imported Style</source>
      <translation>&amp;Rename Imported Style</translation>
    </message>
    <message>
      <source>R&amp;eplace Existing Style</source>
      <translation>R&amp;eplace Existing Style</translation>
    </message>
  </context>
  <context>
    <name>SMStyleSelect</name>
    <message>
      <source>P</source>
      <comment>P as in Parent</comment>
      <translation>P</translation>
    </message>
    <message>
      <source>Use parent style's effects instead of overriding them</source>
      <translation>Use parent style's effects instead of overriding them</translation>
    </message>
  </context>
  <context>
    <name>SMTabruler</name>
    <message>
      <source> Parent Tabs </source>
      <translation> Parent Tabs </translation>
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
    <name>SVGExportPlugin</name>
    <message>
      <source>Exports SVG Files</source>
      <translation>Exports SVG Files</translation>
    </message>
    <message>
      <source>Exports the current page into an SVG file.</source>
      <translation>Exports the current page into an SVG file.</translation>
    </message>
    <message>
      <source>Save as &amp;SVG...</source>
      <translation>Save as &amp;SVG...</translation>
    </message>
  </context>
  <context>
    <name>SVGImportPlugin</name>
    <message>
      <source>Import &amp;SVG...</source>
      <translation>Import &amp;SVG...</translation>
    </message>
    <message>
      <source>Imports SVG Files</source>
      <translation>Imports SVG Files</translation>
    </message>
    <message>
      <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</translation>
    </message>
    <message>
      <source>SVG file contains some unsupported features</source>
      <translation>SVG file contains some unsupported features</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>The file could not be imported</translation>
    </message>
  </context>
  <context>
    <name>SVGPlug</name>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
  </context>
  <context>
    <name>SWDialog</name>
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
  </context>
  <context>
    <name>SWPrefsGui</name>
    <message>
      <source>User settings</source>
      <translation>User settings</translation>
    </message>
    <message>
      <source>System wide configuration</source>
      <translation>System wide configuration</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Save</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Reset</translation>
    </message>
    <message>
      <source>Save user configuration</source>
      <translation>Save user configuration</translation>
    </message>
    <message>
      <source>Reload system wide configuration and remove user defined one</source>
      <translation>Reload system wide configuration and remove user defined one</translation>
    </message>
    <message>
      <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
      <translation>Edit custom configuration. If you save it, it will be used over system wide configuration</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>Cannot write file %1.</source>
      <translation>Cannot write file %1.</translation>
    </message>
    <message>
      <source>User settings saved</source>
      <translation>User settings saved</translation>
    </message>
    <message>
      <source>System wide configuration reloaded</source>
      <translation>System wide configuration reloaded</translation>
    </message>
    <message>
      <source>Cannot open file %1</source>
      <translation>Cannot open file %1</translation>
    </message>
    <message>
      <source>User configuration exists already. Do you really want to overwrite it?</source>
      <translation>User configuration exists already. Do you really want to overwrite it?</translation>
    </message>
  </context>
  <context>
    <name>SaveAsTemplatePlugin</name>
    <message>
      <source>Save as &amp;Template...</source>
      <translation>Save as &amp;Template...</translation>
    </message>
    <message>
      <source>Save a document as a template</source>
      <translation>Save a document as a template</translation>
    </message>
    <message>
      <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
      <translation>Save a document as a template. Good way to ease the initial work for documents with a constant look</translation>
    </message>
  </context>
  <context>
    <name>ScGTFileDialog</name>
    <message>
      <source>Select a file to import</source>
      <translation>Select a file to import</translation>
    </message>
    <message>
      <source>Append</source>
      <translation>Append</translation>
    </message>
    <message>
      <source>Show options</source>
      <translation>Show options</translation>
    </message>
  </context>
  <context>
    <name>ScInputDialog</name>
    <message>
      <source>Input Dialog</source>
      <translation>Input Dialog</translation>
    </message>
    <message>
      <source>InputDialog</source>
      <translation>InputDialog</translation>
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
    <name>ScPlugin</name>
    <message>
      <source>Persistent</source>
      <comment>plugin manager plugin type</comment>
      <translation>Persistent</translation>
    </message>
    <message>
      <source>Action</source>
      <comment>plugin manager plugin type</comment>
      <translation>Action</translation>
    </message>
    <message>
      <source>Load/Save/Import/Export</source>
      <translation>Load/Save/Import/Export</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Unknown</translation>
    </message>
  </context>
  <context>
    <name>ScProgressBar</name>
    <message>
      <source>%1 of %2</source>
      <translation>%1 of %2</translation>
    </message>
  </context>
  <context>
    <name>ScWinPrint</name>
    <message>
      <source>Printing...</source>
      <translation>Printing...</translation>
    </message>
  </context>
  <context>
    <name>Scribus12Format</name>
    <message>
      <source>Scribus 1.2.x Document</source>
      <translation>Scribus 1.2.x Document</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Copy #%1 of </translation>
    </message>
  </context>
  <context>
    <name>Scribus134Format</name>
    <message>
      <source>Scribus 1.3.4 Document</source>
      <translation>Scribus 1.3.4 Document</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Copy #%1 of </translation>
    </message>
  </context>
  <context>
    <name>Scribus13Format</name>
    <message>
      <source>Copy #%1 of </source>
      <translation>Copy #%1 of </translation>
    </message>
    <message>
      <source>Scribus 1.3.0->1.3.3.7 Document</source>
      <translation>Scribus 1.3.0->1.3.3.7 Document</translation>
    </message>
  </context>
  <context>
    <name>ScribusCore</name>
    <message>
      <source>Initializing Plugins</source>
      <translation>Initialising Plugins</translation>
    </message>
    <message>
      <source>Initializing Keyboard Shortcuts</source>
      <translation>Initialising Keyboard Shortcuts</translation>
    </message>
    <message>
      <source>Reading Preferences</source>
      <translation>Reading Preferences</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>Searching for Fonts</translation>
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
      <source>Fatal Error</source>
      <translation>Fatal Error</translation>
    </message>
    <message>
      <source>Font System Initialized</source>
      <translation>Font System Initialised</translation>
    </message>
    <message>
      <source>Reading Color Profiles</source>
      <translation>Reading Colour Profiles</translation>
    </message>
  </context>
  <context>
    <name>ScribusDoc</name>
    <message>
      <source>Document</source>
      <translation>Document</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Background</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>Do you really want to clear all your text?</translation>
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
      <source>Some objects are locked.</source>
      <translation>Some objects are locked.</translation>
    </message>
    <message>
      <source>&amp;Unlock All</source>
      <translation>&amp;Unlock All</translation>
    </message>
    <message>
      <source>&amp;Skip locked objects</source>
      <translation>&amp;Skip locked objects</translation>
    </message>
    <message>
      <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
      <translation>An error occurred while opening ICC profiles, colour management is not enabled.</translation>
    </message>
    <message>
      <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
      <translation>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>Adjusting Colours</translation>
    </message>
    <message>
      <source>Default Paragraph Style</source>
      <translation>Default Paragraph Style</translation>
    </message>
    <message>
      <source>Default Character Style</source>
      <translation>Default Character Style</translation>
    </message>
    <message>
      <source>remove direct paragraph formatting</source>
      <translation>remove direct paragraph formatting</translation>
    </message>
    <message>
      <source>remove direct char formatting</source>
      <translation>remove direct char formatting</translation>
    </message>
    <message>
      <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3
Rotation: %4</source>
      <translation>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3
Rotation: %4</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
  </context>
  <context>
    <name>ScribusMainWindow</name>
    <message>
      <source>Initializing Story Editor</source>
      <translation>Initialising Story Editor</translation>
    </message>
    <message>
      <source>Initializing Hyphenator</source>
      <translation>Initialising Hyphenator</translation>
    </message>
    <message>
      <source>Reading Scrapbook</source>
      <translation>Reading Scrapbook</translation>
    </message>
    <message>
      <source>Setting up Shortcuts</source>
      <translation>Setting up Shortcuts</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>Edit</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>Fatal Error</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;File</translation>
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
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>St&amp;yle</source>
      <translation>St&amp;yle</translation>
    </message>
    <message>
      <source>&amp;Color</source>
      <translation>&amp;Colour</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>&amp;Size</translation>
    </message>
    <message>
      <source>&amp;Shade</source>
      <translation>&amp;Shade</translation>
    </message>
    <message>
      <source>&amp;Font</source>
      <translation>&amp;Font</translation>
    </message>
    <message>
      <source>&amp;Effects</source>
      <translation>&amp;Effects</translation>
    </message>
    <message>
      <source>&amp;Item</source>
      <translation>&amp;Item</translation>
    </message>
    <message>
      <source>Preview Settings</source>
      <translation>Preview Settings</translation>
    </message>
    <message>
      <source>Level</source>
      <translation>Level</translation>
    </message>
    <message>
      <source>Send to La&amp;yer</source>
      <translation>Send to La&amp;yer</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>&amp;PDF Options</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Shape</translation>
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
      <source>&amp;Page</source>
      <translation>&amp;Page</translation>
    </message>
    <message>
      <source>&amp;View</source>
      <translation>&amp;View</translation>
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
      <source>&amp;Alignment</source>
      <translation>&amp;Alignment</translation>
    </message>
    <message>
      <source>Ready</source>
      <translation>Ready</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
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
      <source>Import done</source>
      <translation>Import done</translation>
    </message>
    <message>
      <source>Found nothing to import</source>
      <translation>Found nothing to import</translation>
    </message>
    <message>
      <source>File %1 is not in an acceptable format</source>
      <translation>File %1 is not in an acceptable format</translation>
    </message>
    <message>
      <source>Loading...</source>
      <translation>Loading...</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation> was replaced by: </translation>
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
      <source>Saving...</source>
      <translation>Saving...</translation>
    </message>
    <message>
      <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
      <translation>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</translation>
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
      <source>Cannot Cut In-Use Item</source>
      <translation>Cannot Cut In-Use Item</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
      <translation>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</translation>
    </message>
    <message>
      <source>About Qt</source>
      <translation>About Qt</translation>
    </message>
    <message>
      <source>Scribus Manual</source>
      <translation>Scribus Manual</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Save as</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>Text Files (*.txt);;All Files(*)</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Size</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>&amp;Shade:</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Shade</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>No Style</translation>
    </message>
    <message>
      <source>The following programs are missing:</source>
      <translation>The following programs are missing:</translation>
    </message>
    <message>
      <source>Ghostscript : You cannot use EPS images or Print Preview</source>
      <translation>Ghostscript : You cannot use EPS images or Print Preview</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
      <translation>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</translation>
    </message>
    <message>
      <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
      <translation>Detected some errors.
Consider using the Preflight Verifier to correct them</translation>
    </message>
    <message>
      <source>-Page%1</source>
      <translation>-Page%1</translation>
    </message>
    <message>
      <source>Some objects are locked.</source>
      <translation>Some objects are locked.</translation>
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
      <source>Information</source>
      <translation>Information</translation>
    </message>
    <message>
      <source>The program %1 is already running!</source>
      <translation>The program %1 is already running!</translation>
    </message>
    <message>
      <source>The program %1 is missing!</source>
      <translation>The program %1 is missing!</translation>
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
      <source>&amp;Level</source>
      <translation>&amp;Level</translation>
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
      <source>&amp;Tools</source>
      <translation>&amp;Tools</translation>
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
      <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
      <translation>Ghostscript : You cannot use EPS images or PostScript Print Preview</translation>
    </message>
    <message>
      <source>Ghostscript is missing : Postscript Print Preview is not available</source>
      <translation>Ghostscript is missing : Postscript Print Preview is not available</translation>
    </message>
    <message>
      <source>Do you really want to replace your existing image?</source>
      <translation>Do you really want to replace your existing image?</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>Contents</translation>
    </message>
    <message>
      <source>&amp;Character</source>
      <translation>&amp;Character</translation>
    </message>
    <message>
      <source>&amp;Quote</source>
      <translation>&amp;Quote</translation>
    </message>
    <message>
      <source>S&amp;paces &amp;&amp; Breaks</source>
      <translation>S&amp;paces &amp;&amp; Breaks</translation>
    </message>
    <message>
      <source>Liga&amp;ture</source>
      <translation>Liga&amp;ture</translation>
    </message>
    <message>
      <source>Paste Recent</source>
      <translation>Paste Recent</translation>
    </message>
    <message>
      <source>Updating Pictures</source>
      <translation>Updating Pictures</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz);;All Files (*)</source>
      <translation>Documents (*.sla *.sla.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>Do you really want to clear all your text?</translation>
    </message>
    <message>
      <source>Scribus </source>
      <translation>Scribus </translation>
    </message>
    <message>
      <source>Online &amp;Tutorials</source>
      <translation>Online &amp;Tutorials</translation>
    </message>
    <message>
      <source>Some color profiles used by this document are not installed:</source>
      <translation>Some colour profiles used by this document are not installed:</translation>
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation>%1;;All Files (*)</translation>
    </message>
  </context>
  <context>
    <name>ScribusQApp</name>
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
      <source>Uses xx as shortcut for a language, eg `en' or `de'</source>
      <translation>Uses xx as shortcut for a language, eg `en' or `de'</translation>
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
      <source>Do not show the splashscreen on startup</source>
      <translation>Do not show the splashscreen on startup</translation>
    </message>
    <message>
      <source>Output version information and exit</source>
      <translation>Output version information and exit</translation>
    </message>
    <message>
      <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
      <translation>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</translation>
    </message>
    <message>
      <source>filename</source>
      <translation>filename</translation>
    </message>
    <message>
      <source>Use filename as path for user given preferences</source>
      <translation>Use filename as path for user given preferences</translation>
    </message>
    <message>
      <source>Installed interface languages for Scribus are as follows:</source>
      <translation>Installed interface languages for Scribus are as follows:</translation>
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
      <source>Scribus Version</source>
      <translation>Scribus Version</translation>
    </message>
    <message>
      <source>Scribus, Open Source Desktop Publishing</source>
      <translation>Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
      <source>Homepage</source>
      <translation>Homepage</translation>
    </message>
    <message>
      <source>Documentation</source>
      <translation>Documentation</translation>
    </message>
    <message>
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>Issues</source>
      <translation>Issues</translation>
    </message>
    <message>
      <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
      <translation>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</translation>
    </message>
    <message>
      <source>Download a file from the Scribus website and show the latest available version.</source>
      <translation>Download a file from the Scribus website and show the latest available version.</translation>
    </message>
    <message>
      <source>Display a console window</source>
      <translation>Display a console window</translation>
    </message>
    <message>
      <source>Show location ICC profile information on console while starting</source>
      <translation>Show location ICC profile information on console while starting</translation>
    </message>
    <message>
      <source>Invalid argument: %1</source>
      <translation>Invalid argument: %1</translation>
    </message>
  </context>
  <context>
    <name>ScribusView</name>
    <message>
      <source> %</source>
      <translation> %</translation>
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
      <source>Linking Text Frames</source>
      <translation>Linking Text Frames</translation>
    </message>
    <message>
      <source>Cannot Convert In-Use Item</source>
      <translation>Cannot Convert In-Use Item</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
      <translation>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</translation>
    </message>
    <message>
      <source>Page %1 to %2</source>
      <translation>Page %1 to %2</translation>
    </message>
    <message>
      <source>Colorspace: </source>
      <translation>Colourspace: </translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>Contents</translation>
    </message>
    <message>
      <source>Paste Recent</source>
      <translation>Paste Recent</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
    <message>
      <source>Enables the Preview Mode</source>
      <translation>Enables the Preview Mode</translation>
    </message>
    <message>
      <source>Here you can select the visual appearance of the display
You can choose between normal and several color blindness forms</source>
      <translation>Here you can select the visual appearance of the display
You can choose between normal and several colour blindness forms</translation>
    </message>
    <message>
      <source>Switches Color Management on or off</source>
      <translation>Switches Colour Management on or off</translation>
    </message>
    <message>
      <source>Preview Mode</source>
      <translation>Preview Mode</translation>
    </message>
    <message>
      <source>CMS is active. Therefore the color display may not match the perception by visually impaired</source>
      <translation>CMS is active. Therefore the color display may not match the perception by visually impaired</translation>
    </message>
    <message>
      <source>Enter Object Size</source>
      <translation>Enter Object Size</translation>
    </message>
    <message>
      <source>No Image Loaded</source>
      <translation>No Image Loaded</translation>
    </message>
    <message>
      <source>You are trying to link a frame to itself.</source>
      <translation>You are trying to link a frame to itself.</translation>
    </message>
    <message>
      <source>You are trying to link a frame which is already linked.</source>
      <translation>You are trying to link a frame which is already linked.</translation>
    </message>
  </context>
  <context>
    <name>ScribusWin</name>
    <message>
      <source>Document:</source>
      <translation>Document:</translation>
    </message>
    <message>
      <source>has been changed since the last save.</source>
      <translation>has been changed since the last save.</translation>
    </message>
  </context>
  <context>
    <name>ScriptPlugin</name>
    <message>
      <source>Embedded Python scripting support.</source>
      <translation>Embedded Python scripting support.</translation>
    </message>
    <message>
      <source>Scripter</source>
      <translation>Scripter</translation>
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
      <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
      <translation>There was an internal error while trying the command you entered. Details were printed to stderr. </translation>
    </message>
    <message>
      <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
      <translation>Setting up the Python plugin failed. Error details were printed to stderr. </translation>
    </message>
    <message>
      <source>Documentation for:</source>
      <translation>Documentation for:</translation>
    </message>
    <message>
      <source>Script</source>
      <translation>Script</translation>
    </message>
    <message>
      <source> doesn't contain any docstring!</source>
      <translation> doesn't contain any docstring!</translation>
    </message>
    <message>
      <source>Python Scripts (*.py *.PY);;All Files (*)</source>
      <translation>Python Scripts (*.py *.PY);;All Files (*)</translation>
    </message>
  </context>
  <context>
    <name>ScripterPrefsGui</name>
    <message>
      <source>Scripter Preferences</source>
      <translation>Scripter Preferences</translation>
    </message>
    <message>
      <source>Enable Extension Scripts</source>
      <translation>Enable Extension Scripts</translation>
    </message>
    <message>
      <source>Extensions</source>
      <translation>Extensions</translation>
    </message>
    <message>
      <source>Console</source>
      <translation>Console</translation>
    </message>
    <message>
      <source>Startup Script:</source>
      <translation>Startup Script:</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>Change...</translation>
    </message>
    <message>
      <source>Locate Startup Script</source>
      <translation>Locate Startup Script</translation>
    </message>
    <message>
      <source>Form</source>
      <translation>Form</translation>
    </message>
    <message>
      <source>Comments:</source>
      <translation>Comments:</translation>
    </message>
    <message>
      <source>Keywords:</source>
      <translation>Keywords:</translation>
    </message>
    <message>
      <source>Signs:</source>
      <translation>Signs:</translation>
    </message>
    <message>
      <source>Strings:</source>
      <translation>Strings:</translation>
    </message>
    <message>
      <source>Numbers:</source>
      <translation>Numbers:</translation>
    </message>
    <message>
      <source>Errors:</source>
      <translation>Errors:</translation>
    </message>
    <message>
      <source>Base Texts:</source>
      <translation>Base Texts:</translation>
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
      <source>Search finished, found %1 matches</source>
      <translation>Search finished, found %1 matches</translation>
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
    <name>ShortWordsPlugin</name>
    <message>
      <source>Short &amp;Words...</source>
      <comment>short words plugin</comment>
      <translation>Short &amp;Words...</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
      <translation>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </translation>
    </message>
  </context>
  <context>
    <name>ShortcutWidget</name>
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
      <source>Meta</source>
      <translation>Meta</translation>
    </message>
    <message>
      <source>Meta+</source>
      <translation>Meta+</translation>
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
      <source>No shortcut for the style</source>
      <translation>No shortcut for the style</translation>
    </message>
    <message>
      <source>Style has user defined shortcut</source>
      <translation>Style has user defined shortcut</translation>
    </message>
    <message>
      <source>Assign a shortcut for the style</source>
      <translation>Assign a shortcut for the style</translation>
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
    <message>
      <source>&amp;Insert</source>
      <translation>&amp;Insert</translation>
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
      <source>Spaces &amp;&amp; Breaks</source>
      <translation>Spaces &amp;&amp; Breaks</translation>
    </message>
    <message>
      <source>Ligature</source>
      <translation>Ligature</translation>
    </message>
    <message>
      <source>Space</source>
      <translation>Space</translation>
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
    <name>StyleManager</name>
    <message>
      <source>Name:</source>
      <translation>Name:</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Reset</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>&amp;Apply</translation>
    </message>
    <message>
      <source>&lt;&lt; &amp;Done</source>
      <translation>&lt;&lt; &amp;Done</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;New</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>&amp;Import</translation>
    </message>
    <message>
      <source>&amp;Clone</source>
      <translation>&amp;Clone</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Reset all changes</source>
      <translation>Reset all changes</translation>
    </message>
    <message>
      <source>Apply all changes</source>
      <translation>Apply all changes</translation>
    </message>
    <message>
      <source>Apply all changes and exit edit mode</source>
      <translation>Apply all changes and exit edit mode</translation>
    </message>
    <message>
      <source>Create a new style</source>
      <translation>Create a new style</translation>
    </message>
    <message>
      <source>Import styles from another document</source>
      <translation>Import styles from another document</translation>
    </message>
    <message>
      <source>Clone selected style</source>
      <translation>Clone selected style</translation>
    </message>
    <message>
      <source>Delete selected styles</source>
      <translation>Delete selected styles</translation>
    </message>
    <message>
      <source>New</source>
      <translation>New</translation>
    </message>
    <message>
      <source>Import</source>
      <translation>Import</translation>
    </message>
    <message>
      <source>Clone</source>
      <translation>Clone</translation>
    </message>
    <message>
      <source>Send to Scrapbook</source>
      <translation>Send to Scrapbook</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>Delete</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Edit</translation>
    </message>
    <message>
      <source>&amp;Done</source>
      <translation>&amp;Done</translation>
    </message>
    <message>
      <source>Shortcut</source>
      <translation>Shortcut</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Name</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>Edit</translation>
    </message>
    <message>
      <source>New %1</source>
      <translation>New %1</translation>
    </message>
    <message>
      <source>This key sequence is already in use</source>
      <translation>This key sequence is already in use</translation>
    </message>
    <message>
      <source>Edit styles</source>
      <translation>Edit styles</translation>
    </message>
    <message>
      <source>Name of the selected style</source>
      <translation>Name of the selected style</translation>
    </message>
    <message>
      <source>More than one style selected</source>
      <translation>More than one style selected</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Clone copies the style to make similar styles easily.</source>
      <translation>Clone copies the style to make similar styles easily.</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation>Alt+I</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Please select a unique name for the style</source>
      <translation>Please select a unique name for the style</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
  </context>
  <context>
    <name>StyleSelect</name>
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
      <source>All Caps</source>
      <translation>All Caps</translation>
    </message>
    <message>
      <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>Underline Text. Hold down the button momentarily to set line width and displacement options.</translation>
    </message>
    <message>
      <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</translation>
    </message>
    <message>
      <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>Strike Out. Hold down the button momentarily to set line width and displacement options.</translation>
    </message>
    <message>
      <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
      <comment>Text Style Selector</comment>
      <translation>Outline. Hold down the button momentarily to change the outline stroke width.</translation>
    </message>
    <message>
      <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
      <translation>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</translation>
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
    <message>
      <source>Cancel</source>
      <translation>Cancel</translation>
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
      <source>Table of Contents %1</source>
      <translation>Table of Contents %1</translation>
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
      <source>Check for overflow in text frames</source>
      <translation>Check for overflow in text frames</translation>
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
    <message>
      <source>Highest allowed resolution</source>
      <translation>Highest allowed resolution</translation>
    </message>
    <message>
      <source>Check for GIF images</source>
      <translation>Check for GIF images</translation>
    </message>
    <message>
      <source>Ignore non-printable Layers</source>
      <translation>Ignore non-printable Layers</translation>
    </message>
    <message>
      <source>Check for items not on a page</source>
      <translation>Check for items not on a page</translation>
    </message>
    <message>
      <source>Check for used transparencies</source>
      <translation>Check for used transparencies</translation>
    </message>
  </context>
  <context>
    <name>TabDisplay</name>
    <message>
      <source>Color for paper</source>
      <translation>Colour for paper</translation>
    </message>
    <message>
      <source>Mask the area outside the margins in the margin color</source>
      <translation>Mask the area outside the margins in the margin colour</translation>
    </message>
    <message>
      <source>Enable or disable  the display of linked frames.</source>
      <translation>Enable or disable  the display of linked frames.</translation>
    </message>
    <message>
      <source>Display non-printing characters such as paragraph markers in text frames</source>
      <translation>Display non-printing characters such as paragraph markers in text frames</translation>
    </message>
    <message>
      <source>Turns the display of frames on or off</source>
      <translation>Turns the display of frames on or off</translation>
    </message>
    <message>
      <source>Turns the display of layer indicators on or off</source>
      <translation>Turns the display of layer indicators on or off</translation>
    </message>
    <message>
      <source>Turns the display of pictures on or off</source>
      <translation>Turns the display of pictures on or off</translation>
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
      <source>Set the default zoom level</source>
      <translation>Set the default zoom level</translation>
    </message>
    <message>
      <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
      <translation>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</translation>
    </message>
    <message>
      <source>TabDisplayBase</source>
      <translation>TabDisplayBase</translation>
    </message>
    <message>
      <source>General</source>
      <translation>General</translation>
    </message>
    <message>
      <source>Adjust Display Size</source>
      <translation>Adjust Display Size</translation>
    </message>
    <message>
      <source>Scale%</source>
      <translation>Scale%</translation>
    </message>
    <message>
      <source>To adjust the display drag the ruler below with the slider.</source>
      <translation>To adjust the display drag the ruler below with the slider.</translation>
    </message>
    <message>
      <source>Gaps Between Pages</source>
      <translation>Gaps Between Pages</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation>Vertical:</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation>Horizontal:</translation>
    </message>
    <message>
      <source>Scratch Space</source>
      <translation>Scratch Space</translation>
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
      <source>Page Display</source>
      <translation>Page Display</translation>
    </message>
    <message>
      <source>Show Bleed Area</source>
      <translation>Show Bleed Area</translation>
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
      <source>Show Layer Indicators</source>
      <translation>Show Layer Indicators</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation>Show Frames</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation>Show Text Chains</translation>
    </message>
    <message>
      <source>Rulers Relative to Page</source>
      <translation>Rulers Relative to Page</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation>Show Text Control Characters</translation>
    </message>
    <message>
      <source>Show Pictures</source>
      <translation>Show Pictures</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Colours</translation>
    </message>
    <message>
      <source>Pages:</source>
      <translation>Pages:</translation>
    </message>
    <message>
      <source>Selected Page Border:</source>
      <translation>Selected Page Border:</translation>
    </message>
    <message>
      <source>Fill Color:</source>
      <translation>Fill Colour:</translation>
    </message>
    <message>
      <source>Frames</source>
      <translation>Frames</translation>
    </message>
    <message>
      <source>Grouped:</source>
      <translation>Grouped:</translation>
    </message>
    <message>
      <source>Annotation:</source>
      <translation>Annotation:</translation>
    </message>
    <message>
      <source>Selected:</source>
      <translation>Selected:</translation>
    </message>
    <message>
      <source>Linked:</source>
      <translation>Linked:</translation>
    </message>
    <message>
      <source>Locked:</source>
      <translation>Locked:</translation>
    </message>
    <message>
      <source>Normal:</source>
      <translation>Normal:</translation>
    </message>
    <message>
      <source>Text:</source>
      <translation>Text:</translation>
    </message>
    <message>
      <source>Control Characters:</source>
      <translation>Control Characters:</translation>
    </message>
  </context>
  <context>
    <name>TabDocument</name>
    <message>
      <source>Page Size</source>
      <translation>Page Size</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Size:</translation>
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
      <source>Orie&amp;ntation:</source>
      <translation>Orie&amp;ntation:</translation>
    </message>
    <message>
      <source>Units:</source>
      <translation>Units:</translation>
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
      <source>&amp;Interval:</source>
      <translation>&amp;Interval:</translation>
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
      <source>Width of document pages, editable if you have chosen a custom page size</source>
      <translation>Width of document pages, editable if you have chosen a custom page size</translation>
    </message>
    <message>
      <source>Height of document pages, editable if you have chosen a custom page size</source>
      <translation>Height of document pages, editable if you have chosen a custom page size</translation>
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
      <source>Default unit of measurement for document editing</source>
      <translation>Default unit of measurement for document editing</translation>
    </message>
    <message>
      <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
      <translation>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</translation>
    </message>
    <message>
      <source>Time period between saving automatically</source>
      <translation>Time period between saving automatically</translation>
    </message>
    <message>
      <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
      <translation>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing pages in the document</source>
      <translation>Apply the page size changes to all existing pages in the document</translation>
    </message>
    <message>
      <source>Apply settings to:</source>
      <translation>Apply settings to:</translation>
    </message>
    <message>
      <source>All Document Pages</source>
      <translation>All Document Pages</translation>
    </message>
    <message>
      <source>All Master Pages</source>
      <translation>All Master Pages</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing master pages in the document</source>
      <translation>Apply the page size changes to all existing master pages in the document</translation>
    </message>
  </context>
  <context>
    <name>TabExternalToolsWidget</name>
    <message>
      <source>Locate Ghostscript</source>
      <translation>Locate Ghostscript</translation>
    </message>
    <message>
      <source>Locate your image editor</source>
      <translation>Locate your image editor</translation>
    </message>
    <message>
      <source>Locate your web browser</source>
      <translation>Locate your web browser</translation>
    </message>
    <message>
      <source>External Tools</source>
      <translation>External Tools</translation>
    </message>
    <message>
      <source>Web Browser to launch with links from the Help system</source>
      <translation>Web Browser to launch with links from the Help system</translation>
    </message>
    <message>
      <source>Web Browser</source>
      <translation>Web Browser</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation>&amp;Change...</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&lt;qt>File system location for your web browser. This is used for external links from the Help system.&lt;/qt></source>
      <translation>&lt;qt>File system location for your web browser. This is used for external links from the Help system.&lt;/qt></translation>
    </message>
    <message>
      <source>Name of &amp;Executable:</source>
      <translation>Name of &amp;Executable:</translation>
    </message>
    <message>
      <source>Image Processing Tool</source>
      <translation>Image Processing Tool</translation>
    </message>
    <message>
      <source>&lt;qt>File system location for graphics editor. If you use gimp and your distribution includes it, we recommend 'gimp-remote', as it allows you to edit the image in an already running instance of gimp.&lt;/qt></source>
      <translation>&lt;qt>File system location for graphics editor. If you use gimp and your distribution includes it, we recommend 'gimp-remote', as it allows you to edit the image in an already running instance of gimp.&lt;/qt></translation>
    </message>
    <message>
      <source>PostScript Interpreter</source>
      <translation>PostScript Interpreter</translation>
    </message>
    <message>
      <source>Antialias text for EPS and PDF onscreen rendering</source>
      <translation>Antialias text for EPS and PDF onscreen rendering</translation>
    </message>
    <message>
      <source>Antialias &amp;Text</source>
      <translation>Antialias &amp;Text</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Antialias graphics for EPS and PDF onscreen rendering</source>
      <translation>Antialias graphics for EPS and PDF onscreen rendering</translation>
    </message>
    <message>
      <source>Antialias &amp;Graphics</source>
      <translation>Antialias &amp;Graphics</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>Resolution:</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>&amp;Name of Executable:</source>
      <translation>&amp;Name of Executable:</translation>
    </message>
    <message>
      <source>&lt;qt>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt></source>
      <translation>&lt;qt>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt></translation>
    </message>
    <message>
      <source>Rescan for the external tools if they do not exist in the already specified location</source>
      <translation>Rescan for the external tools if they do not exist in the already specified location</translation>
    </message>
    <message>
      <source>&amp;Rescan</source>
      <translation>&amp;Rescan</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Locate your LaTeX executable</source>
      <translation>Locate your LaTeX executable</translation>
    </message>
    <message>
      <source>Locate your LaTeX editor</source>
      <translation>Locate your LaTeX editor</translation>
    </message>
    <message>
      <source>LaTeX</source>
      <translation>LaTeX</translation>
    </message>
    <message>
      <source>&lt;qt>Path to LaTeX executable. Must be a program that produces one of the formats supported by image frames!&lt;/qt></source>
      <translation>&lt;qt>Path to LaTeX executable. Must be a program that produces one of the formats supported by image frames!&lt;/qt></translation>
    </message>
    <message>
      <source>Output file extension:</source>
      <translation>Output file extension:</translation>
    </message>
    <message>
      <source>Start with empty frame</source>
      <translation>Start with empty frame</translation>
    </message>
    <message>
      <source>Always use the configured DPI setting for calculating the size, even if the image file reports something different.</source>
      <translation>Always use the configured DPI setting for calculating the size, even if the image file reports something different.</translation>
    </message>
    <message>
      <source>Force DPI</source>
      <translation>Force DPI</translation>
    </message>
    <message>
      <source>Latex Editor:</source>
      <translation>Latex Editor:</translation>
    </message>
    <message>
      <source>&lt;qt>Path to the editor executable.&lt;/qt></source>
      <translation>&lt;qt>Path to the editor executable.&lt;/qt></translation>
    </message>
    <message>
      <source>Use Embedded Editor</source>
      <translation>Use Embedded Editor</translation>
    </message>
    <message>
      <source>Preamble:</source>
      <translation>Preamble:</translation>
    </message>
    <message>
      <source>End:</source>
      <translation>End:</translation>
    </message>
  </context>
  <context>
    <name>TabGeneral</name>
    <message>
      <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
      <translation>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</translation>
    </message>
    <message>
      <source>Number of recently edited documents to show in the File menu</source>
      <translation>Number of recently edited documents to show in the File menu</translation>
    </message>
    <message>
      <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
      <translation>Number of lines Scribus will scroll for each move of the mouse wheel</translation>
    </message>
    <message>
      <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
      <translation>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</translation>
    </message>
    <message>
      <source>Default font size for the menus and windows</source>
      <translation>Default font size for the menus and windows</translation>
    </message>
    <message>
      <source>Default font size for the tool windows</source>
      <translation>Default font size for the tool windows</translation>
    </message>
    <message>
      <source>Default documents directory</source>
      <translation>Default documents directory</translation>
    </message>
    <message>
      <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
      <translation>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </translation>
    </message>
    <message>
      <source>Default Scripter scripts directory</source>
      <translation>Default Scripter scripts directory</translation>
    </message>
    <message>
      <source>Additional directory for document templates</source>
      <translation>Additional directory for document templates</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Choose a Directory</translation>
    </message>
    <message>
      <source>TabGeneralBase</source>
      <translation>TabGeneralBase</translation>
    </message>
    <message>
      <source>User Interface</source>
      <translation>User Interface</translation>
    </message>
    <message>
      <source>&amp;Recent Documents:</source>
      <translation>&amp;Recent Documents:</translation>
    </message>
    <message>
      <source>&amp;Wheel Jump:</source>
      <translation>&amp;Wheel Jump:</translation>
    </message>
    <message>
      <source>Show Splashscreen on Startup</source>
      <translation>Show Splashscreen on Startup</translation>
    </message>
    <message>
      <source>Show Startup Dialog</source>
      <translation>Show Startup Dialog</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>&amp;Font Size (Menus):</source>
      <translation>&amp;Font Size (Menus):</translation>
    </message>
    <message>
      <source>Font Size (&amp;Palettes):</source>
      <translation>Font Size (&amp;Palettes):</translation>
    </message>
    <message>
      <source>Time before a Move or Resize starts:</source>
      <translation>Time before a Move or Resize starts:</translation>
    </message>
    <message>
      <source> ms</source>
      <translation> ms</translation>
    </message>
    <message>
      <source>&amp;Theme:</source>
      <translation>&amp;Theme:</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Language:</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>Paths</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation>&amp;Change...</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>C&amp;hange...</translation>
    </message>
    <message>
      <source>Alt+H</source>
      <translation>Alt+H</translation>
    </message>
    <message>
      <source>&amp;Scripts:</source>
      <translation>&amp;Scripts:</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>Cha&amp;nge...</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>&amp;ICC Profiles:</source>
      <translation>&amp;ICC Profiles:</translation>
    </message>
    <message>
      <source>&amp;Documents:</source>
      <translation>&amp;Documents:</translation>
    </message>
    <message>
      <source>Document &amp;Templates:</source>
      <translation>Document &amp;Templates:</translation>
    </message>
    <message>
      <source>Ch&amp;ange...</source>
      <translation>Ch&amp;ange...</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Palette windows will use smaller (space savy) widgets. Requires application restart</source>
      <translation>Palette windows will use smaller (space savy) widgets. Requires application restart</translation>
    </message>
    <message>
      <source>Use Small Widgets in Palettes</source>
      <translation>Use Small Widgets in Palettes</translation>
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
    <message>
      <source>px</source>
      <translation>px</translation>
    </message>
  </context>
  <context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
      <source>Key Set XML Files (*.ksxml)</source>
      <translation>Key Set XML Files (*.ksxml)</translation>
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
      <source>Meta</source>
      <translation>Meta</translation>
    </message>
    <message>
      <source>Meta+</source>
      <translation>Meta+</translation>
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
      <source>This key sequence is already in use</source>
      <translation>This key sequence is already in use</translation>
    </message>
    <message>
      <source>Keyboard Shortcuts</source>
      <translation>Keyboard Shortcuts</translation>
    </message>
    <message>
      <source>Action</source>
      <translation>Action</translation>
    </message>
    <message>
      <source>Shortcut</source>
      <translation>Shortcut</translation>
    </message>
    <message>
      <source>Search:</source>
      <translation>Search:</translation>
    </message>
    <message>
      <source>Shortcut for Selected Action</source>
      <translation>Shortcut for Selected Action</translation>
    </message>
    <message>
      <source>CTRL+ALT+SHIFT+W</source>
      <translation>CTRL+ALT+SHIFT+W</translation>
    </message>
    <message>
      <source>Set &amp;Key</source>
      <translation>Set &amp;Key</translation>
    </message>
    <message>
      <source>Alt+K</source>
      <translation>Alt+K</translation>
    </message>
    <message>
      <source>&amp;User Defined Key</source>
      <translation>&amp;User Defined Key</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>&amp;No Key</source>
      <translation>&amp;No Key</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Loadable Shortcut Sets</source>
      <translation>Loadable Shortcut Sets</translation>
    </message>
    <message>
      <source>Reload the default Scribus shortcuts</source>
      <translation>Reload the default Scribus shortcuts</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Reset</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Export the current shortcuts into an importable file</source>
      <translation>Export the current shortcuts into an importable file</translation>
    </message>
    <message>
      <source>&amp;Export...</source>
      <translation>&amp;Export...</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>Import a shortcut set into the current configuration</source>
      <translation>Import a shortcut set into the current configuration</translation>
    </message>
    <message>
      <source>&amp;Import...</source>
      <translation>&amp;Import...</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation>Alt+I</translation>
    </message>
    <message>
      <source>Load the selected shortcut set</source>
      <translation>Load the selected shortcut set</translation>
    </message>
    <message>
      <source>&amp;Load</source>
      <translation>&amp;Load</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Keyboard shortcut sets available to load</source>
      <translation>Keyboard shortcut sets available to load</translation>
    </message>
    <message>
      <source>Choose a file to read</source>
      <translation>Choose a file to read</translation>
    </message>
    <message>
      <source>Choose a file to save</source>
      <translation>Choose a file to save</translation>
    </message>
    <message>
      <source>Export Keyboard Shortcuts to File</source>
      <translation>Export Keyboard Shortcuts to File</translation>
    </message>
    <message>
      <source>Enter the name of the shortcut set:</source>
      <translation>Enter the name of the shortcut set:</translation>
    </message>
  </context>
  <context>
    <name>TabManager</name>
    <message>
      <source>Manage Tabulators</source>
      <translation>Manage Tabulators</translation>
    </message>
  </context>
  <context>
    <name>TabMiscellaneous</name>
    <message>
      <source>Lorem Ipsum</source>
      <translation>Lorem Ipsum</translation>
    </message>
    <message>
      <source>Count of the Paragraphs:</source>
      <translation>Count of the Paragraphs:</translation>
    </message>
    <message>
      <source>Always use standard Lorem Ipsum</source>
      <translation>Always use standard Lorem Ipsum</translation>
    </message>
    <message>
      <source>Preview of current Paragraph Style visible when editing Styles</source>
      <translation>Preview of current Paragraph Style visible when editing Styles</translation>
    </message>
    <message>
      <source>Always ask before fonts are replaced when loading a document</source>
      <translation>Always ask before fonts are replaced when loading a document</translation>
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
      <source>Automatic</source>
      <translation>Automatic</translation>
    </message>
    <message>
      <source>None</source>
      <translation>None</translation>
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
      <source>&amp;General</source>
      <translation>&amp;General</translation>
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
      <source>Fonts to embed:</source>
      <translation>Fonts to embed:</translation>
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
      <source>Show page previews of each page listed above.</source>
      <translation>Show page previews of each page listed above.</translation>
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
      <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
      <translation>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</translation>
    </message>
    <message>
      <source>DPI (Dots Per Inch) for image export.</source>
      <translation>DPI (Dots Per Inch) for image export.</translation>
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
      <source>Convert Spot Colors to Process Colors</source>
      <translation>Convert Spot Colours to Process Colours</translation>
    </message>
    <message>
      <source>Compression &amp;Quality:</source>
      <translation>Compression &amp;Quality:</translation>
    </message>
    <message>
      <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
      <translation>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</translation>
    </message>
    <message>
      <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
      <translation>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Enables Spot Colours to be converted to composite colours. Unless you are planning to print spot colours at a commercial printer, this is probably best left enabled.</translation>
    </message>
    <message>
      <source>Include La&amp;yers</source>
      <translation>Include La&amp;yers</translation>
    </message>
    <message>
      <source>Compression Metho&amp;d:</source>
      <translation>Compression Metho&amp;d:</translation>
    </message>
    <message>
      <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
      <translation>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</translation>
    </message>
    <message>
      <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
      <translation>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
      <translation>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</translation>
    </message>
    <message>
      <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
      <translation>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</translation>
    </message>
    <message>
      <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
      <translation>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</translation>
    </message>
    <message>
      <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
      <translation>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</translation>
    </message>
    <message>
      <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
      <translation>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</translation>
    </message>
    <message>
      <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
      <translation>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</translation>
    </message>
    <message>
      <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
      <translation>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</translation>
    </message>
    <message>
      <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
      <translation>Choose a master password which enables or disables all the security features in your exported PDF</translation>
    </message>
    <message>
      <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
      <translation>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</translation>
    </message>
    <message>
      <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
      <translation>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</translation>
    </message>
    <message>
      <source>Display Settings</source>
      <translation>Display Settings</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>Single Page</translation>
    </message>
    <message>
      <source>Continuous</source>
      <translation>Continuous</translation>
    </message>
    <message>
      <source>Double Page Left</source>
      <translation>Double Page Left</translation>
    </message>
    <message>
      <source>Double Page Right</source>
      <translation>Double Page Right</translation>
    </message>
    <message>
      <source>Visual Appearance</source>
      <translation>Visual Appearance</translation>
    </message>
    <message>
      <source>Use Viewers Defaults</source>
      <translation>Use Viewers Defaults</translation>
    </message>
    <message>
      <source>Use Full Screen Mode</source>
      <translation>Use Full Screen Mode</translation>
    </message>
    <message>
      <source>Display Bookmarks Tab</source>
      <translation>Display Bookmarks Tab</translation>
    </message>
    <message>
      <source>Display Thumbnails</source>
      <translation>Display Thumbnails</translation>
    </message>
    <message>
      <source>Display Layers Tab</source>
      <translation>Display Layers Tab</translation>
    </message>
    <message>
      <source>Hide Viewers Toolbar</source>
      <translation>Hide Viewers Toolbar</translation>
    </message>
    <message>
      <source>Hide Viewers Menubar</source>
      <translation>Hide Viewers Menubar</translation>
    </message>
    <message>
      <source>Zoom Pages to fit Viewer Window</source>
      <translation>Zoom Pages to fit Viewer Window</translation>
    </message>
    <message>
      <source>Special Actions</source>
      <translation>Special Actions</translation>
    </message>
    <message>
      <source>No Script</source>
      <translation>No Script</translation>
    </message>
    <message>
      <source>Viewer</source>
      <translation>Viewer</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>Clip to Page Margins</translation>
    </message>
    <message>
      <source>Lossy - JPEG</source>
      <translation>Lossy - JPEG</translation>
    </message>
    <message>
      <source>Lossless - Zip</source>
      <translation>Lossless - Zip</translation>
    </message>
    <message>
      <source>Image Compression Method</source>
      <translation>Image Compression Method</translation>
    </message>
    <message>
      <source>Javascript to be executed
when PDF document is opened:</source>
      <translation>Javascript to be executed
when PDF document is opened:</translation>
    </message>
    <message>
      <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
      <translation>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</translation>
    </message>
    <message>
      <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
      <translation>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</translation>
    </message>
    <message>
      <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
      <translation>Colour model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 colour CMYK printer. Choose Greyscale when you want a grey scale PDF.</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins in the exported file</source>
      <translation>Do not show objects outside the margins in the exported file</translation>
    </message>
    <message>
      <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
      <translation>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</translation>
    </message>
    <message>
      <source>Force Overprint Mode</source>
      <translation>Force Overprint Mode</translation>
    </message>
    <message>
      <source>Enables global Overprint Mode for this document, overrides object settings</source>
      <translation>Enables global Overprint Mode for this document, overrides object settings</translation>
    </message>
    <message>
      <source>Determines the PDF compatibility.&lt;br/>The default is &lt;b>PDF 1.3&lt;/b> which gives the widest compatibility.&lt;br/>Choose &lt;b>PDF 1.4&lt;/b> if your file uses features such as transparency or you require 128 bit encryption.&lt;br/>&lt;b>PDF 1.5&lt;/b> is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/>&lt;b>PDF/X-3&lt;/b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
      <translation>Determines the PDF compatibility.&lt;br/>The default is &lt;b>PDF 1.3&lt;/b> which gives the widest compatibility.&lt;br/>Choose &lt;b>PDF 1.4&lt;/b> if your file uses features such as transparency or you require 128 bit encryption.&lt;br/>&lt;b>PDF 1.5&lt;/b> is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/>&lt;b>PDF/X-3&lt;/b> is for exporting the PDF when you want colour managed RGB for commercial printing and is selectable when you have activated colour management. Use only when advised by your printer or in some cases printing to a 4 colour digital colour laser printer.</translation>
    </message>
    <message>
      <source>Printer Marks</source>
      <translation>Printer Marks</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>Crop Marks</translation>
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>Bleed Marks</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>Registration Marks</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>Colour Bars</translation>
    </message>
    <message>
      <source>Page Information</source>
      <translation>Page Information</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>Offset:</translation>
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation>Bleed Settings</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>Top:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>Bottom:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>Left:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>Right:</translation>
    </message>
    <message>
      <source>Use Document Bleeds</source>
      <translation>Use Document Bleeds</translation>
    </message>
    <message>
      <source>Pre-Press</source>
      <translation>Pre-Press</translation>
    </message>
    <message>
      <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
      <translation>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colours. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</translation>
    </message>
    <message>
      <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
      <translation>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation>Inside:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>Outside:</translation>
    </message>
    <message>
      <source>&amp;Embed all</source>
      <translation>&amp;Embed all</translation>
    </message>
    <message>
      <source>Fonts to outline:</source>
      <translation>Fonts to outline:</translation>
    </message>
    <message>
      <source>&amp;Outline all</source>
      <translation>&amp;Outline all</translation>
    </message>
    <message>
      <source>Convert all glyphs in the document to outlines.</source>
      <translation>Convert all glyphs in the document to outlines.</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation>Document Layout</translation>
    </message>
    <message>
      <source>Maximum Image Resolution:</source>
      <translation>Maximum Image Resolution:</translation>
    </message>
    <message>
      <source>Show the document in single page mode</source>
      <translation>Show the document in single page mode</translation>
    </message>
    <message>
      <source>Show the document in single page mode with the pages displayed continuously end to end like a scroll</source>
      <translation>Show the document in single page mode with the pages displayed continuously end to end like a scroll</translation>
    </message>
    <message>
      <source>Show the document with facing pages, starting with the first page displayed on the left</source>
      <translation>Show the document with facing pages, starting with the first page displayed on the left</translation>
    </message>
    <message>
      <source>Show the document with facing pages, starting with the first page displayed on the right</source>
      <translation>Show the document with facing pages, starting with the first page displayed on the right</translation>
    </message>
    <message>
      <source>Use the viewer's defaults or the user's preferences if set differently from the viewer defaults</source>
      <translation>Use the viewer's defaults or the user's preferences if set differently from the viewer defaults</translation>
    </message>
    <message>
      <source>Enables viewing the document in full screen</source>
      <translation>Enables viewing the document in full screen</translation>
    </message>
    <message>
      <source>Display the bookmarks upon opening</source>
      <translation>Display the bookmarks upon opening</translation>
    </message>
    <message>
      <source>Display the page thumbnails upon opening</source>
      <translation>Display the page thumbnails upon opening</translation>
    </message>
    <message>
      <source>Forces the displaying of layers. Useful only for PDF 1.5+.</source>
      <translation>Forces the displaying of layers. Useful only for PDF 1.5+.</translation>
    </message>
    <message>
      <source>Hides the Tool Bar which has selection and other editing capabilities</source>
      <translation>Hides the Tool Bar which has selection and other editing capabilities</translation>
    </message>
    <message>
      <source>Hides the Menu Bar for the viewer, the PDF will display in a plain window. </source>
      <translation>Hides the Menu Bar for the viewer, the PDF will display in a plain window. </translation>
    </message>
    <message>
      <source>Fit the document page or pages to the available space in the viewer window.</source>
      <translation>Fit the document page or pages to the available space in the viewer window.</translation>
    </message>
    <message>
      <source>Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
      <translation>Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</translation>
    </message>
    <message>
      <source>Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation>Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</translation>
    </message>
    <message>
      <source>Add registration marks to each separation</source>
      <translation>Add registration marks to each separation</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation>Add colour calibration bars</translation>
    </message>
    <message>
      <source>Add document information which includes the document title and page numbers</source>
      <translation>Add document information which includes the document title and page numbers</translation>
    </message>
    <message>
      <source>Indicate the distance offset for the registration marks</source>
      <translation>Indicate the distance offset for the registration marks</translation>
    </message>
    <message>
      <source>Use the existing bleed settings from the document preferences</source>
      <translation>Use the existing bleed settings from the document preferences</translation>
    </message>
    <message>
      <source>Use Color Profile</source>
      <translation>Use Colour Profile</translation>
    </message>
    <message>
      <source>Do not use embedded color profiles</source>
      <translation>Do not use embedded colour profiles</translation>
    </message>
  </context>
  <context>
    <name>TabPrinter</name>
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
      <source>Do not show objects outside the margins on the printed page</source>
      <translation>Do not show objects outside the margins on the printed page</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
      <translation>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</translation>
    </message>
    <message>
      <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
      <translation>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>A way of switching off some of the grey shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the grey. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Enables Spot Colours to be converted to composite colours. Unless you are planning to print spot colours at a commercial printer, this is probably best left enabled.</translation>
    </message>
    <message>
      <source>Enables global Overprint Mode for this document, overrides object settings</source>
      <translation>Enables global Overprint Mode for this document, overrides object settings</translation>
    </message>
    <message>
      <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
      <translation>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</translation>
    </message>
    <message>
      <source>File</source>
      <translation>File</translation>
    </message>
    <message>
      <source>All</source>
      <translation>All</translation>
    </message>
    <message>
      <source>TabPrinterBase</source>
      <translation>TabPrinterBase</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Options</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Page</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>Mirror Page(s) Horizontal</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>Mirror Page(s) Vertical</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation>Set Media Size</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>Clip to Page Margins</translation>
    </message>
    <message>
      <source>Postscript Options</source>
      <translation>Postscript Options</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>Print in Greyscale</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation>Print in Colour if Available</translation>
    </message>
    <message>
      <source>Level 1</source>
      <translation>Level 1</translation>
    </message>
    <message>
      <source>Level 2</source>
      <translation>Level 2</translation>
    </message>
    <message>
      <source>Level 3</source>
      <translation>Level 3</translation>
    </message>
    <message>
      <source>General</source>
      <translation>General</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation>Print Separations</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>Print Normal</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Colour</translation>
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation>Apply Under Colour Removal</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>Convert Spot Colours to Process Colours</translation>
    </message>
    <message>
      <source>Force Overprint Mode</source>
      <translation>Force Overprint Mode</translation>
    </message>
    <message>
      <source>Apply ICC Profiles</source>
      <translation>Apply ICC Profiles</translation>
    </message>
    <message>
      <source>Marks &amp;&amp; Bleeds</source>
      <translation>Marks &amp;&amp; Bleeds</translation>
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation>Bleed Settings</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>Top:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>Bottom:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>Left:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>Right:</translation>
    </message>
    <message>
      <source>Printer Marks</source>
      <translation>Printer Marks</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation>Add colour calibration bars</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>Colour Bars</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>Offset:</translation>
    </message>
    <message>
      <source>Add registration marks which are added to each separation</source>
      <translation>Add registration marks which are added to each separation</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>Registration Marks</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</translation>
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>Bleed Marks</translation>
    </message>
    <message>
      <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>Crop Marks</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation>Print Destination</translation>
    </message>
    <message>
      <source>Alternative Printer Command</source>
      <translation>Alternative Printer Command</translation>
    </message>
    <message>
      <source>Command:</source>
      <translation>Command:</translation>
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation>Allows you to embed colour profiles in the print stream when colour management is enabled</translation>
    </message>
  </context>
  <context>
    <name>TabScrapbook</name>
    <message>
      <source>This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically</source>
      <translation>This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically</translation>
    </message>
    <message>
      <source>Send Copied Items Automatically to Scrapbook</source>
      <translation>Send Copied Items Automatically to Scrapbook</translation>
    </message>
    <message>
      <source>This enables copied items to be kept permanently in the scrapbook.</source>
      <translation>This enables copied items to be kept permanently in the scrapbook.</translation>
    </message>
    <message>
      <source>Keep Copied Items Permanently Across Sessions</source>
      <translation>Keep Copied Items Permanently Across Sessions</translation>
    </message>
    <message>
      <source>The minimum number is 1; the maximum us 100.</source>
      <translation>The minimum number is 1; the maximum us 100.</translation>
    </message>
    <message>
      <source>Number of Copied Items to Keep in Scrapbook:</source>
      <translation>Number of Copied Items to Keep in Scrapbook:</translation>
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
      <source>Text Color:</source>
      <translation>Text Colour:</translation>
    </message>
    <message>
      <source>Shading:</source>
      <translation>Shading:</translation>
    </message>
    <message>
      <source>Text Stroke:</source>
      <translation>Text Stroke:</translation>
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
    <message>
      <source>None</source>
      <comment>tab fill</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Text</translation>
    </message>
    <message>
      <source>Shapes</source>
      <translation>Shapes</translation>
    </message>
    <message>
      <source>Lines</source>
      <translation>Lines</translation>
    </message>
    <message>
      <source>Images</source>
      <translation>Images</translation>
    </message>
    <message>
      <source>Regular Polygons</source>
      <translation>Regular Polygons</translation>
    </message>
    <message>
      <source>Zoom</source>
      <translation>Zoom</translation>
    </message>
    <message>
      <source>Rotation Tool</source>
      <translation>Rotation Tool</translation>
    </message>
    <message>
      <source>Constrain to:</source>
      <translation>Constrain to:</translation>
    </message>
    <message>
      <source>Other Properties</source>
      <translation>Other Properties</translation>
    </message>
    <message>
      <source>Miscellaneous Settings</source>
      <translation>Miscellaneous Settings</translation>
    </message>
    <message>
      <source>Item Duplicate</source>
      <translation>Item Duplicate</translation>
    </message>
    <message>
      <source>X Displacement</source>
      <translation>X Displacement</translation>
    </message>
    <message>
      <source>Y Displacement</source>
      <translation>Y Displacement</translation>
    </message>
    <message>
      <source>Horizontal displacement of page items</source>
      <translation>Horizontal displacement of page items</translation>
    </message>
    <message>
      <source>Vertical displacement of page items</source>
      <translation>Vertical displacement of page items</translation>
    </message>
    <message>
      <source>Constrain value for the rotation tool when the Control key is pressed</source>
      <translation>Constrain value for the rotation tool when the Control key is pressed</translation>
    </message>
    <message>
      <source>Degrees</source>
      <translation>Degrees</translation>
    </message>
    <message>
      <source>Use the embedded clipping paths in images when importing them. JPEG, PSD and TIFF are the image formats which can embedded clipping paths.</source>
      <translation>Use the embedded clipping paths in images when importing them. JPEG, PSD and TIFF are the image formats which can embedded clipping paths.</translation>
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
      <source>Fill Char:</source>
      <translation>Fill Char:</translation>
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
    <message>
      <source>None</source>
      <comment>tab fill</comment>
      <translation>None</translation>
    </message>
    <message>
      <source>Indentation from the right for the whole paragraph</source>
      <translation>Indentation from the right for the whole paragraph</translation>
    </message>
    <message>
      <source>Fill Character of Tab</source>
      <translation>Fill Character of Tab</translation>
    </message>
    <message>
      <source>Type/Orientation of Tab</source>
      <translation>Type/Orientation of Tab</translation>
    </message>
    <message>
      <source>Position of Tab</source>
      <translation>Position of Tab</translation>
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
      <source>Colorspace: </source>
      <translation>Colourspace: </translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Unknown</translation>
    </message>
    <message>
      <source>No Image Loaded</source>
      <translation>No Image Loaded</translation>
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
      <source>Lines: </source>
      <translation>Lines: </translation>
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
      <source>Rename</source>
      <translation>Rename</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Delete</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>Contents</translation>
    </message>
    <message>
      <source>Export: </source>
      <translation>Export: </translation>
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
    <message>
      <source>Text on a Path</source>
      <translation>Text on a Path</translation>
    </message>
    <message>
      <source>Enable Item Printing</source>
      <translation>Enable Item Printing</translation>
    </message>
    <message>
      <source>Disable Item Printing</source>
      <translation>Disable Item Printing</translation>
    </message>
    <message>
      <source>Multiple duplicate</source>
      <translation>Multiple duplicate</translation>
    </message>
    <message>
      <source>Change Image Offset</source>
      <translation>Change Image Offset</translation>
    </message>
    <message>
      <source>Change Image Scale</source>
      <translation>Change Image Scale</translation>
    </message>
    <message>
      <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
      <translation>X1: %1, Y1: %2
X2: %4, Y2: %5</translation>
    </message>
    <message>
      <source>X: %1, Y: %2
X: %4, Y: %5</source>
      <translation>X: %1, Y: %2
X: %4, Y: %5</translation>
    </message>
    <message>
      <source>Apply text style</source>
      <translation>Apply text style</translation>
    </message>
    <message>
      <source>&amp;Undo: %1</source>
      <comment>f.e. Undo: Move</comment>
      <translation>&amp;Undo: %1</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Undo</translation>
    </message>
    <message>
      <source>&amp;Redo: %1</source>
      <comment>f.e. Redo: Move</comment>
      <translation>&amp;Redo: %1</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Redo</translation>
    </message>
    <message>
      <source>No object frame</source>
      <translation>No object frame</translation>
    </message>
    <message>
      <source>Reset control point</source>
      <translation>Reset control point</translation>
    </message>
    <message>
      <source>Reset control points</source>
      <translation>Reset control points</translation>
    </message>
    <message>
      <source>Apply image effects</source>
      <translation>Apply image effects</translation>
    </message>
    <message>
      <source>Insert frame</source>
      <translation>Insert frame</translation>
    </message>
    <message>
      <source>Adjust frame to the image size</source>
      <translation>Adjust frame to the image size</translation>
    </message>
    <message>
      <source>Set start and end arrows</source>
      <translation>Set start and end arrows</translation>
    </message>
    <message>
      <source>Remove all guides</source>
      <translation>Remove all guides</translation>
    </message>
    <message>
      <source>Copy</source>
      <translation>Copy</translation>
    </message>
    <message>
      <source>Copy page</source>
      <translation>Copy page</translation>
    </message>
    <message>
      <source>Remove vertical auto guide</source>
      <translation>Remove vertical auto guide</translation>
    </message>
    <message>
      <source>Remove horizontal auto guide</source>
      <translation>Remove horizontal auto guide</translation>
    </message>
    <message>
      <source>Text flows around image clipping path</source>
      <translation>Text flows around image clipping path</translation>
    </message>
    <message>
      <source>Remove page guides</source>
      <translation>Remove page guides</translation>
    </message>
    <message>
      <source>Convert to outlines</source>
      <translation>Convert to outlines</translation>
    </message>
    <message>
      <source>Latex frame</source>
      <translation>Latex frame</translation>
    </message>
    <message>
      <source>Change formula</source>
      <translation>Change formula</translation>
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
    <name>UnicodeChooseButton</name>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Search</translation>
    </message>
  </context>
  <context>
    <name>UnicodeSearch</name>
    <message>
      <source>Unicode Search</source>
      <translation>Unicode Search</translation>
    </message>
    <message>
      <source>&amp;Search:</source>
      <translation>&amp;Search:</translation>
    </message>
  </context>
  <context>
    <name>UpgradeChecker</name>
    <message>
      <source>Attempting to get the Scribus version update file</source>
      <translation>Attempting to get the Scribus version update file</translation>
    </message>
    <message>
      <source>(No data on your computer will be sent to an external location)</source>
      <translation>(No data on your computer will be sent to an external location)</translation>
    </message>
    <message>
      <source>Timed out when attempting to get update file.</source>
      <translation>Timed out when attempting to get update file.</translation>
    </message>
    <message>
      <source>Error when attempting to get update file: %1</source>
      <translation>Error when attempting to get update file: %1</translation>
    </message>
    <message>
      <source>File not found on server</source>
      <translation>File not found on server</translation>
    </message>
    <message>
      <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
      <translation>Could not open version file: %1
Error:%2 at line: %3, row: %4</translation>
    </message>
    <message>
      <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
      <translation>An error occurred while looking for updates for Scribus, please check your internet connection.</translation>
    </message>
    <message>
      <source>No updates are available for your version of Scribus %1</source>
      <translation>No updates are available for your version of Scribus %1</translation>
    </message>
    <message>
      <source>One or more updates for your version of Scribus (%1) are available:</source>
      <translation>One or more updates for your version of Scribus (%1) are available:</translation>
    </message>
    <message>
      <source>Please visit www.scribus.net for details.</source>
      <translation>Please visit www.scribus.net for details.</translation>
    </message>
    <message>
      <source>This list may contain development versions.</source>
      <translation>This list may contain development versions.</translation>
    </message>
  </context>
  <context>
    <name>UrlLauncher</name>
    <message>
      <source>Locate your web browser</source>
      <translation>Locate your web browser</translation>
    </message>
    <message>
      <source>External Web Browser Failed to Start</source>
      <translation>External Web Browser Failed to Start</translation>
    </message>
    <message>
      <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences.
Would you like to start the system's default browser instead?</source>
      <translation>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences.
Would you like to start the system's default browser instead?</translation>
    </message>
  </context>
  <context>
    <name>UsePrinterMarginsDialog</name>
    <message>
      <source>Minimum Margins for Page Size %1</source>
      <translation>Minimum Margins for Page Size %1</translation>
    </message>
    <message>
      <source>Use Printer Margins</source>
      <translation>Use Printer Margins</translation>
    </message>
    <message>
      <source>Select &amp;Printer:</source>
      <translation>Select &amp;Printer:</translation>
    </message>
    <message>
      <source>Margins</source>
      <translation>Margins</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>Right:</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Top:</translation>
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
    <name>WMFImport</name>
    <message>
      <source>Group%1</source>
      <translation>Group%1</translation>
    </message>
  </context>
  <context>
    <name>WMFImportPlugin</name>
    <message>
      <source>Import &amp;WMF...</source>
      <translation>Import &amp;WMF...</translation>
    </message>
    <message>
      <source>Imports WMF Files</source>
      <translation>Imports WMF Files</translation>
    </message>
    <message>
      <source>Imports most WMF files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>Imports most WMF files into the current document,
converting their vector data into Scribus objects.</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>The file could not be imported</translation>
    </message>
    <message>
      <source>WMF file contains some unsupported features</source>
      <translation>WMF file contains some unsupported features</translation>
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
      <source>Open</source>
      <translation>Open</translation>
    </message>
    <message>
      <source>&amp;Importer:</source>
      <translation>&amp;Importer:</translation>
    </message>
    <message>
      <source>Import &amp;Text Only</source>
      <translation>Import &amp;Text Only</translation>
    </message>
    <message>
      <source>&amp;Encoding:</source>
      <translation>&amp;Encoding:</translation>
    </message>
  </context>
  <context>
    <name>gtImporterDialog</name>
    <message>
      <source>Choose the importer to use</source>
      <translation>Choose the importer to use</translation>
    </message>
    <message>
      <source>Remember association</source>
      <translation>Remember association</translation>
    </message>
    <message>
      <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
      <translation>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</translation>
    </message>
  </context>
  <context>
    <name>hysettingsBase</name>
    <message>
      <source>Form</source>
      <translation>Form</translation>
    </message>
    <message>
      <source>General Options</source>
      <translation>General Options</translation>
    </message>
    <message>
      <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
      <translation>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</translation>
    </message>
    <message>
      <source>&amp;Hyphenation Suggestions</source>
      <translation>&amp;Hyphenation Suggestions</translation>
    </message>
    <message>
      <source>Enables automatic hyphenation of your text while typing.</source>
      <translation>Enables automatic hyphenation of your text while typing.</translation>
    </message>
    <message>
      <source>Hyphenate Text Automatically &amp;During Typing</source>
      <translation>Hyphenate Text Automatically &amp;During Typing</translation>
    </message>
    <message>
      <source>Behaviour</source>
      <translation>Behaviour</translation>
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
      <source>Length of the smallest word to be hyphenated.</source>
      <translation>Length of the smallest word to be hyphenated.</translation>
    </message>
    <message>
      <source>Chars</source>
      <translation>Chars</translation>
    </message>
    <message>
      <source>Consecutive Hyphenations &amp;Allowed:</source>
      <translation>Consecutive Hyphenations &amp;Allowed:</translation>
    </message>
    <message>
      <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
      <translation>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</translation>
    </message>
    <message>
      <source>Exceptions</source>
      <translation>Exceptions</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>Edit</translation>
    </message>
    <message>
      <source>Ignore List</source>
      <translation>Ignore List</translation>
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
    <message>
      <source>&amp;About</source>
      <translation>&amp;About</translation>
    </message>
    <message>
      <source>&amp;Image</source>
      <translation>&amp;Image</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation>&amp;Help</translation>
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
    <message>
      <source>&amp;More Details</source>
      <translation>&amp;More Details</translation>
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
