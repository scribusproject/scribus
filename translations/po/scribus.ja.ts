<!DOCTYPE TS>
<TS>
  <context>
    <name></name>
    <message>
      <source>getColorNames() -> list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>newDocDialog() -> bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFillColor([&quot;name&quot;]) -> string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type>).
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>createParagraphStyle(...)

Creates a paragraph style. This function takes the following keyword parameters:

&quot;name&quot; [required] -> specifies the name of the paragraphstyle to create

linespacingmode [optional] -> specifies the linespacing mode; possible modes are:

fixed linespacing:          0

automatic linespacing:      1

baseline grid linespacing:  2

linespacing [optional] -> specifies the linespacing if using fixed linespacing

alignment [optional] -> specifies the alignment of the paragraph

-> left:     0

-> center:   1

-> right:    2

-> justify:  3

-> extend:   4

leftmargin [optional], rightmargin [optional] -> specify the margin

gapbefore [optional], gapafter [optional] -> specify the gaps to the heading and following paragraphs

firstindent [optional] -> the indent of the first line

hasdropcap [optional] -> specifies if there are caps (1 = yes, 0 = no)

dropcaplines [optional] -> height (in lines) of the caps if used

dropcapoffset [optional] -> offset of the caps if used

&quot;charstyle&quot; [optional] -> char style to use

</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFontSize([&quot;name&quot;]) -> float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or 'unicode' string(recommended).
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeSVG(&quot;filename&quot;, x, y)

Places the SVG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SVG placed on the page

If loading was successful, the selection contains the imported SVG
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>valueDialog(caption, message [,defaultvalue]) -> string

Shows the common 'Ask for string' dialog and returns its value as a string
Parameters: window title, text in the window and optional 'default' value.

Example: valueDialog('title', 'text in the window', 'optional')
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>newStyleDialog() -> string

Shows 'Create new paragraph style' dialog. Function returns real
style name or None when user cancels the dialog.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>haveDoc() -> bool

Returns true if there is a document open.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type>.

May raise ValueError if an invalid unit is passed.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getUnit() -> integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineColor([&quot;name&quot;]) -> string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineWidth([&quot;name&quot;]) -> integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineShade([&quot;name&quot;]) -> integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineEnd([&quot;name&quot;]) -> integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineStyle([&quot;name&quot;]) -> integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFillShade([&quot;name&quot;]) -> integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getImageScale([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getImageName([&quot;name&quot;]) -> string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getSize([&quot;name&quot;]) -> (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type>
for reference.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getRotation([&quot;name&quot;]) -> integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getAllObjects() -> list

Returns a list containing the names of all objects on the current page.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property' of `object'. See getProperty()
for details of arguments.

If `includesuper' is true, search inherited properties too.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object'.
If `includesuper' is true, return properties supported
by parent classes as well.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>setProperty(object, property, value)

Set `property' of `object' to `value'. If `value' cannot be converted to a type
compatible with the type of `property', an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getSelectedObject([nr]) -> string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>selectionCount() -> integer

Returns the number of selected objects.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>deselectAll()

Deselects all objects in the whole document.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>lockObject([&quot;name&quot;]) -> bool

Locks the object &quot;name&quot; if it's unlocked or unlock it if it's locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLocked([&quot;name&quot;]) -> bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection>)

Sets the scale to frame on the selected or specified image frame to `scaletoframe'.
If `proportional' is specified, set fixed aspect ratio scaling to `proportional'.
Both `scaletoframe' and `proportional' are boolean.

May raise WrongFrameTypeError.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFontNames() -> list

Returns a list with the names of all available fonts.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getXFontNames() -> list of tuples

Returns a larger font info. It's a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLayers() -> list

Returns a list with the names of all defined layers.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getActiveLayer() -> string

Returns the name of the current active layer.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn't
exists or if it's the only layer in the document.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getGuiLanguage() -> string

Returns a string with the -lang value.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>objectExists([&quot;name&quot;]) -> bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it's applied on the selected object.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getAllStyles() -> list

Return a list of the names of all paragraph styles in the current document.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>currentPage() -> integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>redrawAll()

Redraws all pages.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>pageCount() -> integer

Returns the number of pages in the document.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getHGuides() -> list

Returns a list containing positions of the horizontal guides. Values are in the
document's current units - see UNIT_&lt;type> constants.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type> constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getVGuides()

See getHGuides.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setVGuides()

See setHGuides.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPageSize() -> tuple

Returns a tuple with page dimensions measured in the document's current units.
See UNIT_&lt;type> constants and getPageMargins()
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPageItems() -> list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]
means that object named 'Text1' is a text frame (type 4) and is the first at
the page...
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type> constants and getPageSize().
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineShade(shade, [&quot;name&quot;])

Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full color intensity). If &quot;name&quot; is not given the currently selected item
is used.

May raise ValueError if the line shade is out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type>.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type>.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style>.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFillShade(shade, [&quot;name&quot;])

Sets the shading of the fill color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full Color intensity). If &quot;name&quot; is not given the currently selected
Item is used.

May raise ValueError if the fill shade is out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn't exist.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFont([&quot;name&quot;]) -> string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getTextLength([&quot;name&quot;]) -> integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getAllText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineSpacing([&quot;name&quot;]) -> float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getColumnGap([&quot;name&quot;]) -> float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getColumns([&quot;name&quot;]) -> integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2'). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFontSize(size, [&quot;name&quot;])

Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated
as a value in points. If there is some text selected only the selected text is
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError for a font size that's out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared.  If &quot;name&quot; is not given the currently
selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setTextShade(shade, [&quot;name&quot;])

Sets the shading of the text color of the object &quot;name&quot; to &quot;shade&quot;. If
there is some text selected only the selected text is changed. &quot;shade&quot; must
be an integer value in the range from 0 (lightest) to 100 (full color
intensity). If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg 'a->b->c' becomes 'a->c' when you unlinkTextFrames(b)'

May throw ScribusException if linking rules are violated.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>textOverflows([&quot;name&quot;, nolinks]) -> integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn't
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isPDFBookmark([&quot;name&quot;]) -> bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>progressTotal(max)

Sets the progress bar's maximum steps value to the specified number.
See progressSet.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It's
useful to call this procedure when you're changing the document, because Scribus
won't automatically notice when you change the document using a script.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFillTransparency([&quot;name&quot;]) -> float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getFillBlendmode([&quot;name&quot;]) -> integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineTransparency([&quot;name&quot;]) -> float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineBlendmode([&quot;name&quot;]) -> integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLayerLocked(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLayerOutlined(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLayerFlow(&quot;layer&quot;) -> bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLayerBlendmode(&quot;layer&quot;) -> int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLayerTransparency(&quot;layer&quot;) -> float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>duplicateObject([&quot;name&quot;]) -> string

creates a Duplicate of the selected Object (or Selection Group).
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPageType() -> integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getTextLines([&quot;name&quot;]) -> integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the
printable set to false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLayerVisible(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting text at position -1 appends it to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Qt::DockLeft(lr), Qt::DockRight(rr), Qt::DockTop(tr) and Qt::DockBottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerOutlined(&quot;layer&quot;, outline)

Sets the layer &quot;layer&quot; to be locked or not. If outline is set to
true the layer will be displayed outlined.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerFlow(&quot;layer&quot;, flow)

Sets the layers &quot;layer&quot;  flowcontrol to flow. If flow is set to
true text in layers above this one will flow around objects on this layer.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerBlendmode(&quot;layer&quot;, blend)

Sets the layers &quot;layer&quot;  blendmode to blend.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerTransparency(&quot;layer&quot;, trans)

Sets the layers &quot;layer&quot;  transparency to trans.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFillBlendmode(blendmode, [&quot;name&quot;])

Sets the fill blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineBlendmode(blendmode, [&quot;name&quot;])

Sets the line blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeEPS(&quot;filename&quot;, x, y)

Places the EPS &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the EPS placed on the page

If loading was successful, the selection contains the imported EPS
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeSXD(&quot;filename&quot;, x, y)

Places the SXD &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SXD placed on the page

If loading was successful, the selection contains the imported SXD
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeODG(&quot;filename&quot;, x, y)

Places the ODG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the ODG placed on the page

If loading was successful, the selection contains the imported ODG
</source>
      <translation type="unfinished" />
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineJoin([&quot;name&quot;]) -> integer (see constants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertex at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (currently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets whether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>moveSelectionToFront()

Moves current selection to front.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>moveSelectionToFront()

Moves current selection to back.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>importPage(&quot;fromDoc&quot;, (pageList), [create, imortwhere, importwherePage])

Imports a set of pages (given as a tuple) from an existing document (the file name must be given). This functions maps the &quot;Page->Import&quot; dropdown menu function.
fromDoc: string; the filename of the document to import pages from
pageList: tuple with page numbers of pages to import
create: number; 0 to replace existing pages, 1 (default) to insert new pages
importWhere: number; the page number (of the current document) at which import the pages
importWherePage: number; used if create==1; 0 to create pages before selected page; 1 to create pages after selected page; 2 (default) to create pages at the end of the document
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>createCharStyle(...)

Creates a character style. This function takes the following keyword parameters:

&quot;name&quot; [required] -> name of the char style to create

&quot;font&quot; [optional] -> name of the font to use

fontsize [optional] -> font size to set (double)

&quot;features&quot; [optional] -> nearer typographic details can be defined by a string that might contain the following phrases comma-seperated (without spaces!):

-> inherit

-> bold

-> italic

-> underline

-> underlinewords

-> strike

-> superscript

-> subscript

-> outline

-> shadowed

-> allcaps

-> smallcaps

&quot;fillcolor&quot; [optional], &quot;fillshade&quot; [optional] -> specify fill options

&quot;strokecolor&quot; [optional], &quot;strokeshade&quot; [optional] -> specify stroke options

baselineoffset [optional] -> offset of the baseline

shadowxoffset [optional], shadowyoffset [optional] -> offset of the shadow if used

outlinewidth [optional] -> width of the outline if used

underlineoffset [optional], underlinewidth [optional] -> underline options if used

strikethruoffset [optional], strikethruwidth [optional] -> strikethru options if used

scaleh [optional], scalev [optional] -> scale of the chars

tracking [optional] -> tracking of the text

&quot;language&quot; [optional] -> language code

</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>hyphenateText([&quot;name&quot;]) -> bool

Does hyphenation on text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>dehyphenateText([&quot;name&quot;]) -> bool

Does dehyphenation on text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>scrollDocument(x,y)

Scroll the document in main GUI window by x and y.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getTextDistances([&quot;name&quot;]) -> tuple

Returns the text distances of the text frame &quot;name&quot; expressed in points. The
distances are returned as a tuple like (left, right, top, bottom). If &quot;name&quot;
is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setTextDistances(left, right, top, bottom, [&quot;name&quot;])

Sets the text distances of the text frame &quot;name&quot; to the values &quot;left&quot;
&quot;right&quot;, &quot;top&quot; and &quot;bottom&quot;. If &quot;name&quot; is not given the currently
selected item is used.

May throw ValueError if any of the distances are out of bounds (must be positive).
</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>AIPlug</name>
    <message>
      <source>Importing: %1</source>
      <translation>インポート中: %1</translation>
    </message>
    <message>
      <source>Analyzing File:</source>
      <translation>ファイルを解析:</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>アイテムを生成</translation>
    </message>
  </context>
  <context>
    <name>About</name>
    <message>
      <source>About Scribus %1</source>
      <translation>Scribusについて %1</translation>
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
      <translation>Ghostscriptバージョン %1 を使用</translation>
    </message>
    <message>
      <source>No Ghostscript version available</source>
      <translation>利用可能なGhostscriptのバージョンはありません</translation>
    </message>
    <message>
      <source>Build ID:</source>
      <translation>ビルドID:</translation>
    </message>
    <message>
      <source>&amp;About</source>
      <translation>バージョン情報(&amp;A)</translation>
    </message>
    <message>
      <source>Development Team:</source>
      <translation>開発チーム:</translation>
    </message>
    <message>
      <source>Contributions from:</source>
      <translation>貢献者:</translation>
    </message>
    <message>
      <source>Mac OS&amp;#174; X Aqua Port:</source>
      <translation>Mac OS&amp;#174; X Aqua 移植:</translation>
    </message>
    <message>
      <source>Windows&amp;#174; Port:</source>
      <translation>Windows&amp;#174; 移植:</translation>
    </message>
    <message>
      <source>Official Documentation:</source>
      <translation>オフィシャルなドキュメント担当者:</translation>
    </message>
    <message>
      <source>Other Documentation:</source>
      <translation>その他のドキュメント担当者:</translation>
    </message>
    <message>
      <source>A&amp;uthors</source>
      <translation>作者(&amp;U)</translation>
    </message>
    <message>
      <source>Official Translations and Translators:</source>
      <translation>オフィシャルな翻訳並びに翻訳者:</translation>
    </message>
    <message>
      <source>Previous Translation Contributors:</source>
      <translation>以前の翻訳の貢献者:</translation>
    </message>
    <message>
      <source>&amp;Translations</source>
      <translation>翻訳(&amp;T)</translation>
    </message>
    <message>
      <source>Homepage</source>
      <translation>ホームページ</translation>
    </message>
    <message>
      <source>Online Reference</source>
      <translation>オンラインリファレンス</translation>
    </message>
    <message>
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>Bugs and Feature Requests</source>
      <translation>バグや機能の要望</translation>
    </message>
    <message>
      <source>Mailing List</source>
      <translation>メーリングリスト</translation>
    </message>
    <message>
      <source>&amp;Online</source>
      <translation>オンライン(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
      <source>Tango Project Icons:</source>
      <translation>Tangoプロジェクトアイコン:</translation>
    </message>
    <message>
      <source>&amp;Updates</source>
      <translation>アップデート(&amp;U)</translation>
    </message>
    <message>
      <source>Check for updates to Scribus. No data from your machine will be transferred off it.</source>
      <translation>Scribusのアップデートをチェックします。あなたのコンピュータのデータを外部に送信することはありません。</translation>
    </message>
    <message>
      <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
      <translation>OS/2&amp;#174;/eComStation&amp;#8482; 移植:</translation>
    </message>
    <message>
      <source>Splash Screen:</source>
      <translation>スプラッシュスクリーン</translation>
    </message>
    <message>
      <source>This panel shows the version, build date and compiled in library support in Scribus.</source>
      <translation>このパネルではバージョン、ビルド日時、Scribusでサポートされているコンパイル済みライブラリを表示します。</translation>
    </message>
    <message>
      <source>The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.Last Letter is the renderer C=cairo or Q=Qt</source>
      <translation>C-C-T-Fは、C=littlecms C=CUPS T=TIFFサポート F=Fontconfigサポートを意味します。最後の文字はレンダラを意味し、C=cairo or Q=Qtです。</translation>
    </message>
    <message>
      <source>Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
      <translation>ライブラリのサポートがない場合は * で示されます。これはScribusが検知したGhostscriptのバージョンも示しています。</translation>
    </message>
    <message>
      <source>The Windows version does not use fontconfig or CUPS libraries.</source>
      <translation>WindowsバージョンではfontconfigやCUPSライブラリは使用されません。</translation>
    </message>
    <message>
      <source>&lt;p align=&quot;center&quot;>&lt;b>%1 %2&lt;/b>&lt;/p>&lt;p align=&quot;center&quot;>%3&lt;br>%4 %5&lt;br>%6&lt;/p></source>
      <translation>&lt;p align=&quot;center&quot;>&lt;b>%1 %2&lt;/b>&lt;/p>&lt;p align=&quot;center&quot;>%3&lt;br>%4 %5&lt;br>%6&lt;/p></translation>
    </message>
    <message>
      <source>Scribus Version</source>
      <translation>Scribusバージョン</translation>
    </message>
    <message>
      <source>Check for Updates</source>
      <translation>アップデートをチェック</translation>
    </message>
    <message>
      <source>Abort Update Check</source>
      <translation>アップデートのチェックを中断</translation>
    </message>
    <message>
      <source>Developer Blog</source>
      <translation>開発者Blog</translation>
    </message>
    <message>
      <source>&amp;Licence</source>
      <translation>ライセンス(&amp;L)</translation>
    </message>
    <message>
      <source>Unable to open licence file. Please check your install directory or the Scribus website for licencing information.</source>
      <translation>ライセンスファイルを開けません。インストールしたディレクトリかScribusのウェブサイトでライセンス情報をチェックしてください。</translation>
    </message>
  </context>
  <context>
    <name>AboutPlugins</name>
    <message>
      <source>Scribus: About Plug-ins</source>
      <translation>Scribus: プラグインについて</translation>
    </message>
  </context>
  <context>
    <name>ActionManager</name>
    <message>
      <source>&amp;New</source>
      <translation>新規(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Open...</source>
      <translation>開く(&amp;O)...</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>保存(&amp;S)</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>名前を付けて保存(&amp;A)...</translation>
    </message>
    <message>
      <source>Re&amp;vert to Saved</source>
      <translation>保存した状態に戻す(&amp;V)</translation>
    </message>
    <message>
      <source>Collect for O&amp;utput...</source>
      <translation>まとめて出力(&amp;U)...</translation>
    </message>
    <message>
      <source>Get Text...</source>
      <translation>テキストを取得...</translation>
    </message>
    <message>
      <source>Append &amp;Text...</source>
      <translation>テキストを追加(&amp;T)...</translation>
    </message>
    <message>
      <source>Get Image...</source>
      <translation>画像を取得...</translation>
    </message>
    <message>
      <source>Save &amp;Text...</source>
      <translation>テキストを保存(&amp;T)...</translation>
    </message>
    <message>
      <source>Save as P&amp;DF...</source>
      <translation>PDF形式で保存(&amp;D)...</translation>
    </message>
    <message>
      <source>Document &amp;Setup...</source>
      <translation>ドキュメント設定(&amp;S)...</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>印刷(&amp;P)...</translation>
    </message>
    <message>
      <source>Print Previe&amp;w</source>
      <translation>印刷プレビュー(&amp;W)</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation>終了(&amp;Q)</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>元に戻す(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>やり直し(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Item Action Mode</source>
      <translation>アイテムアクションモード(&amp;I)</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>切り取り(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>貼り付け(&amp;P)</translation>
    </message>
    <message>
      <source>Select &amp;All</source>
      <translation>全て選択(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Deselect All</source>
      <translation>全てを選択解除(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Search/Replace...</source>
      <translation>検索/置換(&amp;S)...</translation>
    </message>
    <message>
      <source>Edit Image...</source>
      <translation>画像を編集...</translation>
    </message>
    <message>
      <source>C&amp;olors...</source>
      <translation>色(&amp;O)...</translation>
    </message>
    <message>
      <source>&amp;Master Pages...</source>
      <translation>マスターページ(&amp;M)...</translation>
    </message>
    <message>
      <source>&amp;JavaScripts...</source>
      <translation>Javascript(&amp;J)...</translation>
    </message>
    <message>
      <source>P&amp;references...</source>
      <translation>設定(&amp;R)...</translation>
    </message>
    <message>
      <source>%1 pt</source>
      <translation>%1 pt</translation>
    </message>
    <message>
      <source>&amp;Other...</source>
      <translation>その他(&amp;O)...</translation>
    </message>
    <message>
      <source>&amp;Left</source>
      <translation>左(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Center</source>
      <translation>中央(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Right</source>
      <translation>右(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Block</source>
      <translation>ブロック(&amp;B)</translation>
    </message>
    <message>
      <source>&amp;Forced</source>
      <translation>強制(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;%1 %</source>
      <translation>&amp;%1 %</translation>
    </message>
    <message>
      <source>&amp;Normal</source>
      <translation>標準(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Underline</source>
      <translation>下線(&amp;U)</translation>
    </message>
    <message>
      <source>Underline &amp;Words</source>
      <translation>単語に下線を引く</translation>
    </message>
    <message>
      <source>&amp;Strike Through</source>
      <translation>取り消し線(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;All Caps</source>
      <translation>全て大文字(&amp;A)</translation>
    </message>
    <message>
      <source>Small &amp;Caps</source>
      <translation>スモールキャピタル(&amp;C)</translation>
    </message>
    <message>
      <source>Su&amp;perscript</source>
      <translation>上付き文字(&amp;P)</translation>
    </message>
    <message>
      <source>Su&amp;bscript</source>
      <translation>下付き文字(&amp;B)</translation>
    </message>
    <message>
      <source>S&amp;hadow</source>
      <translation>影(&amp;H)</translation>
    </message>
    <message>
      <source>&amp;Image Effects</source>
      <translation>画像のエフェクト(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Tabulators...</source>
      <translation>タブ(&amp;T)...</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>複製(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Multiple Duplicate</source>
      <translation>複数コピー(&amp;M)</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Group</source>
      <translation>グループ化(&amp;G)</translation>
    </message>
    <message>
      <source>&amp;Ungroup</source>
      <translation>グループ解除(&amp;U)</translation>
    </message>
    <message>
      <source>Is &amp;Locked</source>
      <translation>ロック(&amp;L)</translation>
    </message>
    <message>
      <source>Si&amp;ze is Locked</source>
      <translation>サイズをロック(&amp;Z)</translation>
    </message>
    <message>
      <source>Lower to &amp;Bottom</source>
      <translation>最背面に移動(&amp;B)</translation>
    </message>
    <message>
      <source>Raise to &amp;Top</source>
      <translation>最前面に移動(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Lower</source>
      <translation>背面へ(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Raise</source>
      <translation>前面へ(&amp;R)</translation>
    </message>
    <message>
      <source>Send to S&amp;crapbook</source>
      <translation>スクラップブックに送る(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Attributes...</source>
      <translation>属性(&amp;A)...</translation>
    </message>
    <message>
      <source>More Info...</source>
      <translation>詳細な情報...</translation>
    </message>
    <message>
      <source>I&amp;mage Visible</source>
      <translation>画像表示が可能(&amp;M)</translation>
    </message>
    <message>
      <source>&amp;Update Image</source>
      <translation>画像を更新(&amp;U)</translation>
    </message>
    <message>
      <source>Adjust Frame to Image</source>
      <translation>フレームを画像に合わせる</translation>
    </message>
    <message>
      <source>Extended Image Properties</source>
      <translation>拡張画像のプロパティ</translation>
    </message>
    <message>
      <source>&amp;Low Resolution</source>
      <translation>低解像度(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Normal Resolution</source>
      <translation>標準解像度(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Full Resolution</source>
      <translation>最大解像度(&amp;F)</translation>
    </message>
    <message>
      <source>Is PDF &amp;Bookmark</source>
      <translation>PDFブックマーク(&amp;B)</translation>
    </message>
    <message>
      <source>Is PDF A&amp;nnotation</source>
      <translation>PDF注釈(&amp;N)</translation>
    </message>
    <message>
      <source>Annotation P&amp;roperties</source>
      <translation>注釈のプロパティ(&amp;R)</translation>
    </message>
    <message>
      <source>Field P&amp;roperties</source>
      <translation>フィールドのプロパティ(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Edit Shape...</source>
      <translation>形状を編集(&amp;E)...</translation>
    </message>
    <message>
      <source>&amp;Attach Text to Path</source>
      <translation>テキストをパスに合わせる(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Detach Text from Path</source>
      <translation>テキストをパスから離す(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Combine Polygons</source>
      <translation>多角形を結合(&amp;C)</translation>
    </message>
    <message>
      <source>Split &amp;Polygons</source>
      <translation>多角形を分割(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Bezier Curve</source>
      <translation>ベジエ曲線(&amp;B)</translation>
    </message>
    <message>
      <source>&amp;Image Frame</source>
      <translation>画像フレーム(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Polygon</source>
      <translation>多角形(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation>テキストフレーム(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Glyph...</source>
      <translation>グリフ(&amp;G)...</translation>
    </message>
    <message>
      <source>Sample Text</source>
      <translation>サンプルテキスト</translation>
    </message>
    <message>
      <source>&amp;Insert...</source>
      <translation>挿入(&amp;I)...</translation>
    </message>
    <message>
      <source>Im&amp;port...</source>
      <translation>インポート(&amp;P)...</translation>
    </message>
    <message>
      <source>&amp;Delete...</source>
      <translation>削除(&amp;D)...</translation>
    </message>
    <message>
      <source>&amp;Copy...</source>
      <translation>コピー(&amp;C)...</translation>
    </message>
    <message>
      <source>&amp;Move...</source>
      <translation>移動(&amp;M)...</translation>
    </message>
    <message>
      <source>&amp;Apply Master Page...</source>
      <translation>マスターページに適用(&amp;A)...</translation>
    </message>
    <message>
      <source>Convert to Master Page...</source>
      <translation>マスターページに変換...</translation>
    </message>
    <message>
      <source>Manage &amp;Guides...</source>
      <translation>ガイドを管理(&amp;G)...</translation>
    </message>
    <message>
      <source>Manage Page Properties...</source>
      <translation>ページプロパティを管理...</translation>
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
      <translation>マージンを表示(&amp;M)</translation>
    </message>
    <message>
      <source>Show &amp;Frames</source>
      <translation>フレームを表示(&amp;F)</translation>
    </message>
    <message>
      <source>Show &amp;Images</source>
      <translation>画像を表示(&amp;I)</translation>
    </message>
    <message>
      <source>Show &amp;Grid</source>
      <translation>グリッドを表示(&amp;G)</translation>
    </message>
    <message>
      <source>Show G&amp;uides</source>
      <translation>ガイドを表示(&amp;U)</translation>
    </message>
    <message>
      <source>Show &amp;Baseline Grid</source>
      <translation>ベースライングリッドを表示(&amp;B)</translation>
    </message>
    <message>
      <source>Show &amp;Text Chain</source>
      <translation>テキストチェーンを表示(&amp;T)</translation>
    </message>
    <message>
      <source>Show Control Characters</source>
      <translation>制御文字を表示</translation>
    </message>
    <message>
      <source>Sn&amp;ap to Grid</source>
      <translation>グリッドに合わせる(&amp;A)</translation>
    </message>
    <message>
      <source>Sna&amp;p to Guides</source>
      <translation>ガイドに合わせる(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Properties</source>
      <translation>プロパティ(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Scrapbook</source>
      <translation>スクラップブック(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Layers</source>
      <translation>レイヤー(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Arrange Pages</source>
      <translation>ページの配置(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Bookmarks</source>
      <translation>ブックマーク(&amp;B)</translation>
    </message>
    <message>
      <source>&amp;Measurements</source>
      <translation>計測(&amp;M)</translation>
    </message>
    <message>
      <source>Action &amp;History</source>
      <translation>アクション履歴(&amp;H)</translation>
    </message>
    <message>
      <source>Preflight &amp;Verifier</source>
      <translation>プリフライト検証(&amp;V)</translation>
    </message>
    <message>
      <source>&amp;Align and Distribute</source>
      <translation>整列と配置(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Tools</source>
      <translation>ツール(&amp;T)</translation>
    </message>
    <message>
      <source>P&amp;DF Tools</source>
      <translation>PDFツール(&amp;D)</translation>
    </message>
    <message>
      <source>Select Item</source>
      <translation>アイテムを選択</translation>
    </message>
    <message>
      <source>Rotate Item</source>
      <translation>アイテムを回転</translation>
    </message>
    <message>
      <source>Zoom in or out</source>
      <translation>ズームイン/アウト</translation>
    </message>
    <message>
      <source>Zoom in</source>
      <translation>ズームイン</translation>
    </message>
    <message>
      <source>Zoom out</source>
      <translation>ズームアウト</translation>
    </message>
    <message>
      <source>Edit Contents of Frame</source>
      <translation>フレームの内容を編集</translation>
    </message>
    <message>
      <source>Edit Text...</source>
      <translation>テキストを編集...</translation>
    </message>
    <message>
      <source>Link Text Frames</source>
      <translation>テキストフレームを繋ぐ</translation>
    </message>
    <message>
      <source>Unlink Text Frames</source>
      <translation>テキストフレームを離す</translation>
    </message>
    <message>
      <source>&amp;Eye Dropper</source>
      <translation>スポイト(&amp;E)</translation>
    </message>
    <message>
      <source>Copy Item Properties</source>
      <translation>アイテムのプロパティをコピー</translation>
    </message>
    <message>
      <source>&amp;Hyphenate Text</source>
      <translation>テキストをハイフンでつなぐ(&amp;H)</translation>
    </message>
    <message>
      <source>Dehyphenate Text</source>
      <translation>テキストのハイフンを取る</translation>
    </message>
    <message>
      <source>&amp;Generate Table Of Contents</source>
      <translation>目次を生成(&amp;G)</translation>
    </message>
    <message>
      <source>&amp;Cascade</source>
      <translation>カスケード表示(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Tile</source>
      <translation>タイル表示(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;About Scribus</source>
      <translation>Scribusについて(&amp;A)</translation>
    </message>
    <message>
      <source>About &amp;Qt</source>
      <translation>Qtについて(&amp;Q)</translation>
    </message>
    <message>
      <source>Toolti&amp;ps</source>
      <translation>ツールチップ(&amp;P)</translation>
    </message>
    <message>
      <source>Scribus &amp;Manual...</source>
      <translation>Scribusマニュアル(&amp;M)...</translation>
    </message>
    <message>
      <source>Smart &amp;Hyphen</source>
      <translation>スマートハイフン(&amp;H)</translation>
    </message>
    <message>
      <source>Non Breaking Dash</source>
      <translation>改行なしダッシュ</translation>
    </message>
    <message>
      <source>Non Breaking &amp;Space</source>
      <translation>改行なしスペース(&amp;S)</translation>
    </message>
    <message>
      <source>Page &amp;Number</source>
      <translation>ページ番号(&amp;N)</translation>
    </message>
    <message>
      <source>Frame Break</source>
      <translation>改フレーム</translation>
    </message>
    <message>
      <source>Column Break</source>
      <translation>改コラム</translation>
    </message>
    <message>
      <source>Copyright</source>
      <translation>著作権</translation>
    </message>
    <message>
      <source>Registered Trademark</source>
      <translation>登録商標</translation>
    </message>
    <message>
      <source>Trademark</source>
      <translation>商標</translation>
    </message>
    <message>
      <source>Bullet</source>
      <translation>黒点</translation>
    </message>
    <message>
      <source>Em Dash</source>
      <translation>emダッシュ</translation>
    </message>
    <message>
      <source>En Dash</source>
      <translation>enダッシュ</translation>
    </message>
    <message>
      <source>Figure Dash</source>
      <translation>フィギュアダッシュ</translation>
    </message>
    <message>
      <source>Quotation Dash</source>
      <translation>クォーテーションダッシュ</translation>
    </message>
    <message>
      <source>Toggle Palettes</source>
      <translation>パレットを切替え</translation>
    </message>
    <message>
      <source>Toggle Guides</source>
      <translation>ガイドを切替え</translation>
    </message>
    <message>
      <source>Paste (&amp;Absolute)</source>
      <translation>貼り付け (絶対)(&amp;A)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>S&amp;tyles...</source>
      <translation>スタイル(&amp;T)...</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <comment>type effect</comment>
      <translation>アウトライン(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Printing Enabled</source>
      <translation>印刷可能(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Flip Horizontally</source>
      <translation>水平方向に反転(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;Flip Vertically</source>
      <translation>垂直方向に反転(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;Outlines</source>
      <comment>Convert to oulines</comment>
      <translation>アウトライン(&amp;O)</translation>
    </message>
    <message>
      <source>Show Rulers</source>
      <translation>定規を表示</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <comment>Document Outline Palette</comment>
      <translation>アウトライン(&amp;O)</translation>
    </message>
    <message>
      <source>Insert PDF Push Button</source>
      <translation>PDFプッシュボタンを挿入</translation>
    </message>
    <message>
      <source>Insert PDF Text Field</source>
      <translation>PDFテキストフィールドを挿入</translation>
    </message>
    <message>
      <source>Insert PDF Check Box</source>
      <translation>PDFチェックボックスを挿入</translation>
    </message>
    <message>
      <source>Insert PDF Combo Box</source>
      <translation>PDFコンボボックスを挿入</translation>
    </message>
    <message>
      <source>Insert PDF List Box</source>
      <translation>PDFリストボックスを挿入</translation>
    </message>
    <message>
      <source>Insert Text Annotation</source>
      <translation>テキストの注釈を挿入</translation>
    </message>
    <message>
      <source>Insert Link Annotation</source>
      <translation>リンクの注釈を挿入</translation>
    </message>
    <message>
      <source>New Line</source>
      <translation>復帰改行</translation>
    </message>
    <message>
      <source>Solidus</source>
      <translation>固相線</translation>
    </message>
    <message>
      <source>Middle Dot</source>
      <translation>中黒</translation>
    </message>
    <message>
      <source>En Space</source>
      <translation>Enスペース</translation>
    </message>
    <message>
      <source>Em Space</source>
      <translation>Emスペース</translation>
    </message>
    <message>
      <source>Thin Space</source>
      <translation>Thinスペース</translation>
    </message>
    <message>
      <source>Thick Space</source>
      <translation>Thickスペース</translation>
    </message>
    <message>
      <source>Mid Space</source>
      <translation>Midスペース</translation>
    </message>
    <message>
      <source>Hair Space</source>
      <translation>ヘアスペース</translation>
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
      <source>Save as &amp;EPS...</source>
      <translation>EPS形式で保存(&amp;E)...</translation>
    </message>
    <message>
      <source>Show Text Frame Columns</source>
      <translation>テキストフレームの行を表示</translation>
    </message>
    <message>
      <source>&amp;Frame...</source>
      <translation>フレーム(&amp;F)...</translation>
    </message>
    <message>
      <source>Preview Mode</source>
      <translation>プレビューモード</translation>
    </message>
    <message>
      <source>Show Layer Indicators</source>
      <translation>レイヤー目印を表示</translation>
    </message>
    <message>
      <source>Patterns...</source>
      <translation>パターン...</translation>
    </message>
    <message>
      <source>Send to Patterns</source>
      <translation>パターンに送る</translation>
    </message>
    <message>
      <source>Sticky Tools</source>
      <translation>スティッキーツール</translation>
    </message>
    <message>
      <source>&amp;Fit to Height</source>
      <translation>高さに合わせる(&amp;F)</translation>
    </message>
    <message>
      <source>Fit to Width</source>
      <translation>幅に合わせる</translation>
    </message>
    <message>
      <source>Show Bleeds</source>
      <translation>断ち切りを表示</translation>
    </message>
    <message>
      <source>&amp;Zero Width Space</source>
      <translation>ゼロ幅スペース(&amp;Z)</translation>
    </message>
    <message>
      <source>Zero Width NB Space</source>
      <translation>ゼロ幅NBスペース</translation>
    </message>
    <message>
      <source>Apostrophe</source>
      <comment>Unicode 0x0027</comment>
      <translation>アポストロフィー</translation>
    </message>
    <message>
      <source>Straight Double</source>
      <comment>Unicode 0x0022</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Single Left</source>
      <comment>Unicode 0x2018</comment>
      <translation>左シングルクォート</translation>
    </message>
    <message>
      <source>Single Right</source>
      <comment>Unicode 0x2019</comment>
      <translation>右シングルクォート</translation>
    </message>
    <message>
      <source>Double Left</source>
      <comment>Unicode 0x201C</comment>
      <translation>左ダブルクォート</translation>
    </message>
    <message>
      <source>Double Right</source>
      <comment>Unicode 0x201D</comment>
      <translation>右ダブルクォート</translation>
    </message>
    <message>
      <source>Single Reversed</source>
      <comment>Unicode 0x201B</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Double Reversed</source>
      <comment>Unicode 0x201F</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Single Left Guillemet</source>
      <comment>Unicode 0x2039</comment>
      <translation>左一重ギュメ</translation>
    </message>
    <message>
      <source>Single Right Guillemet</source>
      <comment>Unicode 0x203A</comment>
      <translation>右一重ギュメ</translation>
    </message>
    <message>
      <source>Double Left Guillemet</source>
      <comment>Unicode 0x00AB</comment>
      <translation>左二重ギュメ</translation>
    </message>
    <message>
      <source>Double Right Guillemet</source>
      <comment>Unicode 0x00BB</comment>
      <translation>右二重ギュメ</translation>
    </message>
    <message>
      <source>Low Single Comma</source>
      <comment>Unicode 0x201A</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Low Double Comma</source>
      <comment>Unicode 0x201E</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>CJK Single Left</source>
      <comment>Unicode 0x300C</comment>
      <translation>始めかぎ括弧</translation>
    </message>
    <message>
      <source>CJK Single Right</source>
      <comment>Unicode 0x300D</comment>
      <translation>終わりかぎ括弧</translation>
    </message>
    <message>
      <source>CJK Double Left</source>
      <comment>Unicode 0x300E</comment>
      <translation>始め二重かぎ括弧</translation>
    </message>
    <message>
      <source>CJK Double Right</source>
      <comment>Unicode 0x300F</comment>
      <translation>終わり二重かぎ括弧</translation>
    </message>
    <message>
      <source>&amp;400%</source>
      <translation>&amp;400%</translation>
    </message>
    <message>
      <source>Insert &amp;Text Frame</source>
      <translation>テキストフレームを挿入(&amp;T)</translation>
    </message>
    <message>
      <source>Insert &amp;Image Frame</source>
      <translation>画像フレームを挿入(&amp;I)</translation>
    </message>
    <message>
      <source>Insert T&amp;able</source>
      <translation>テーブルを挿入(&amp;A)</translation>
    </message>
    <message>
      <source>Insert &amp;Shape</source>
      <translation>形状を挿入(&amp;S)</translation>
    </message>
    <message>
      <source>Insert &amp;Polygon</source>
      <translation>多角形を挿入(&amp;P)</translation>
    </message>
    <message>
      <source>Insert &amp;Line</source>
      <translation>線を挿入(&amp;L)</translation>
    </message>
    <message>
      <source>Insert &amp;Bezier Curve</source>
      <translation>ベジエ曲線を挿入(&amp;B)</translation>
    </message>
    <message>
      <source>Insert &amp;Freehand Line</source>
      <translation>フリーハンド線を挿入(&amp;F)</translation>
    </message>
    <message>
      <source>Scribus Homepage</source>
      <translation>Scribusホームページ</translation>
    </message>
    <message>
      <source>Scribus Online Documentation</source>
      <translation>Scribus オンラインドキュメント</translation>
    </message>
    <message>
      <source>Scribus Wiki</source>
      <translation>Scribus Wiki</translation>
    </message>
    <message>
      <source>Getting Started with Scribus</source>
      <translation>Scribusをはじめる</translation>
    </message>
    <message>
      <source>Show Context Menu</source>
      <translation>コンテキストメニューを表示</translation>
    </message>
    <message>
      <source>&amp;Manage Images</source>
      <translation>画像を管理(&amp;M)</translation>
    </message>
    <message>
      <source>&amp;About Plugins</source>
      <translation>プラグインについて(&amp;A)</translation>
    </message>
    <message>
      <source>Insert Unicode Character Begin Sequence</source>
      <translation>Unicode文字の開始シーケンスを挿入</translation>
    </message>
    <message>
      <source>Get Vector File...</source>
      <translation>ベクトルファイルを取得...</translation>
    </message>
    <message>
      <source>Advanced Select All...</source>
      <translation>高度な全て選択...</translation>
    </message>
    <message>
      <source>Edit Source...</source>
      <translation>ソースを編集...</translation>
    </message>
    <message>
      <source>Replace Colors...</source>
      <translation>色を置換...</translation>
    </message>
    <message>
      <source>Rulers Relative to Page</source>
      <translation>ページに相対する定規</translation>
    </message>
    <message>
      <source>Insert &amp;Render Frame</source>
      <translation>描画フレームを挿入(&amp;R)</translation>
    </message>
    <message>
      <source>Check for Updates</source>
      <translation>アップデートをチェック</translation>
    </message>
    <message>
      <source>Number of Pages</source>
      <translation>ページ数</translation>
    </message>
  </context>
  <context>
    <name>AdjustCmsDialog</name>
    <message>
      <source>CMS Settings</source>
      <translation>CMS設定</translation>
    </message>
  </context>
  <context>
    <name>AlignDistribute</name>
    <message>
      <source>Align</source>
      <translation>整列</translation>
    </message>
    <message>
      <source>&amp;Selected Guide:</source>
      <translation>選択されたガイド(&amp;S):</translation>
    </message>
    <message>
      <source>&amp;Relative To:</source>
      <translation>相対(&amp;R):</translation>
    </message>
    <message>
      <source>...</source>
      <translation>...</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>配置</translation>
    </message>
    <message>
      <source>&amp;Distance:</source>
      <translation>距離(&amp;D):</translation>
    </message>
  </context>
  <context>
    <name>AlignSelect</name>
    <message>
      <source>Align Text Left</source>
      <translation>テキストを左揃え</translation>
    </message>
    <message>
      <source>Align Text Right</source>
      <translation>テキストを右揃え</translation>
    </message>
    <message>
      <source>Align Text Center</source>
      <translation>テキストを中央揃え</translation>
    </message>
    <message>
      <source>Align Text Justified</source>
      <translation>テキストを両端揃え</translation>
    </message>
    <message>
      <source>Align Text Forced Justified</source>
      <translation>テキストを強制的に両端揃え</translation>
    </message>
  </context>
  <context>
    <name>Annot</name>
    <message>
      <source>Field Properties</source>
      <translation>フィールドのプロパティ</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>タイプ:</translation>
    </message>
    <message>
      <source>Button</source>
      <translation>ボタン</translation>
    </message>
    <message>
      <source>Text Field</source>
      <translation>テキストフィールド</translation>
    </message>
    <message>
      <source>Check Box</source>
      <translation>チェックボックス</translation>
    </message>
    <message>
      <source>Combo Box</source>
      <translation>コンボボックス</translation>
    </message>
    <message>
      <source>List Box</source>
      <translation>リストボックス</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>プロパティ</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>名前:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>テキスト</translation>
    </message>
    <message>
      <source>Font for use with PDF 1.3:</source>
      <translation>PDF 1.3で利用するフォント:</translation>
    </message>
    <message>
      <source>Border</source>
      <translation>境界</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>色:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>なし</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>幅:</translation>
    </message>
    <message>
      <source>Thin</source>
      <translation>狭い</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>標準</translation>
    </message>
    <message>
      <source>Wide</source>
      <translation>広い</translation>
    </message>
    <message>
      <source>Style:</source>
      <translation>スタイル:</translation>
    </message>
    <message>
      <source>Solid</source>
      <translation>実線</translation>
    </message>
    <message>
      <source>Dashed</source>
      <translation>破線</translation>
    </message>
    <message>
      <source>Underline</source>
      <translation>下線</translation>
    </message>
    <message>
      <source>Beveled</source>
      <translation>斜線</translation>
    </message>
    <message>
      <source>Inset</source>
      <translation>挿入</translation>
    </message>
    <message>
      <source>Other</source>
      <translation>その他</translation>
    </message>
    <message>
      <source>Read Only</source>
      <translation>読み込み専用</translation>
    </message>
    <message>
      <source>Required</source>
      <translation>必須</translation>
    </message>
    <message>
      <source>Visibility:</source>
      <translation>可視性:</translation>
    </message>
    <message>
      <source>Visible</source>
      <translation>可視</translation>
    </message>
    <message>
      <source>Hidden</source>
      <translation>隠し</translation>
    </message>
    <message>
      <source>No Print</source>
      <translation>印刷なし</translation>
    </message>
    <message>
      <source>No View</source>
      <translation>ビューなし</translation>
    </message>
    <message>
      <source>Appearance</source>
      <translation>外観</translation>
    </message>
    <message>
      <source>Text for Button Down</source>
      <translation>Button Downのテキスト</translation>
    </message>
    <message>
      <source>Text for Roll Over</source>
      <translation>Roll Overのテキスト</translation>
    </message>
    <message>
      <source>Icons</source>
      <translation>アイコン</translation>
    </message>
    <message>
      <source>Use Icons</source>
      <translation>アイコンを使用</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>削除</translation>
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
      <translation>アイコンの位置ずれ...</translation>
    </message>
    <message>
      <source>Highlight</source>
      <translation>強調</translation>
    </message>
    <message>
      <source>Invert</source>
      <translation>反転</translation>
    </message>
    <message>
      <source>Outlined</source>
      <translation>アウトライン化</translation>
    </message>
    <message>
      <source>Push</source>
      <translation>押す</translation>
    </message>
    <message>
      <source>Multi-Line</source>
      <translation>複数行</translation>
    </message>
    <message>
      <source>Password</source>
      <translation>パスワード</translation>
    </message>
    <message>
      <source>Characters</source>
      <translation>文字</translation>
    </message>
    <message>
      <source>Do Not Scroll</source>
      <translation>スクロールしない</translation>
    </message>
    <message>
      <source>Do Not Spell Check</source>
      <translation>スペルチェックしない</translation>
    </message>
    <message>
      <source>Check Style:</source>
      <translation>スタイルをチェック:</translation>
    </message>
    <message>
      <source>Check</source>
      <translation>チェック</translation>
    </message>
    <message>
      <source>Cross</source>
      <translation>十字</translation>
    </message>
    <message>
      <source>Diamond</source>
      <translation>ダイアモンド</translation>
    </message>
    <message>
      <source>Circle</source>
      <translation>円</translation>
    </message>
    <message>
      <source>Star</source>
      <translation>星</translation>
    </message>
    <message>
      <source>Square</source>
      <translation>四角</translation>
    </message>
    <message>
      <source>Editable</source>
      <translation>編集可能</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>JavaScript</source>
      <translation>JavaScript</translation>
    </message>
    <message>
      <source>Go To</source>
      <translation>移動</translation>
    </message>
    <message>
      <source>Submit Form</source>
      <translation>送信フォーム</translation>
    </message>
    <message>
      <source>Reset Form</source>
      <translation>リセットフォーム</translation>
    </message>
    <message>
      <source>Import Data</source>
      <translation>データをインポート</translation>
    </message>
    <message>
      <source>Event:</source>
      <translation>イベント:</translation>
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
      <translation>スクリプト:</translation>
    </message>
    <message>
      <source>Edit...</source>
      <translation>編集...</translation>
    </message>
    <message>
      <source>Submit to URL:</source>
      <translation>以下のURLに送信:</translation>
    </message>
    <message>
      <source>Submit Data as HTML</source>
      <translation>データをHTMLとして送信</translation>
    </message>
    <message>
      <source>Import Data from:</source>
      <translation>以下からデータをインポート:</translation>
    </message>
    <message>
      <source>Destination</source>
      <translation>出力先</translation>
    </message>
    <message>
      <source>To File:</source>
      <translation>ファイルへ:</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>変更...</translation>
    </message>
    <message>
      <source>Page:</source>
      <translation>ページ:</translation>
    </message>
    <message>
      <source>X-Pos:</source>
      <translation>位置X:</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Y-Pos:</source>
      <translation>位置Y:</translation>
    </message>
    <message>
      <source>Action</source>
      <translation>アクション</translation>
    </message>
    <message>
      <source>Field is formatted as:</source>
      <translation>フィールドをフォーマット:</translation>
    </message>
    <message>
      <source>Plain</source>
      <translation>プレーン</translation>
    </message>
    <message>
      <source>Number</source>
      <translation>数字</translation>
    </message>
    <message>
      <source>Percentage</source>
      <translation>パーセント</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>日付</translation>
    </message>
    <message>
      <source>Time</source>
      <translation>時刻</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>カスタム</translation>
    </message>
    <message>
      <source>Number Format</source>
      <translation>数字のフォーマット</translation>
    </message>
    <message>
      <source>Decimals:</source>
      <translation>小数:</translation>
    </message>
    <message>
      <source>Use Currency Symbol</source>
      <translation>通貨シンボルを使用</translation>
    </message>
    <message>
      <source>Prepend Currency Symbol</source>
      <translation>通貨シンボルを前に追加</translation>
    </message>
    <message>
      <source>Formatting</source>
      <translation>フォーマット中</translation>
    </message>
    <message>
      <source>Percent Format</source>
      <translation>パーセントのフォーマット</translation>
    </message>
    <message>
      <source>Date Format</source>
      <translation>日付のフォーマット</translation>
    </message>
    <message>
      <source>Time Format</source>
      <translation>時刻のフォーマット</translation>
    </message>
    <message>
      <source>Custom Scripts</source>
      <translation>カスタムスクリプト</translation>
    </message>
    <message>
      <source>Format:</source>
      <translation>フォーマット:</translation>
    </message>
    <message>
      <source>Keystroke:</source>
      <translation>キー入力:</translation>
    </message>
    <message>
      <source>Format</source>
      <translation>フォーマット</translation>
    </message>
    <message>
      <source>Value is not validated</source>
      <translation>値は認証されていません</translation>
    </message>
    <message>
      <source>Value must be greater than or equal to:</source>
      <translation>値は等しいか大きいものでなければなりません:</translation>
    </message>
    <message>
      <source>and less or equal to:</source>
      <translation>等しいか少ない:</translation>
    </message>
    <message>
      <source>Custom validate script:</source>
      <translation>カスタム認証スクリプト:</translation>
    </message>
    <message>
      <source>Validate</source>
      <translation>認証</translation>
    </message>
    <message>
      <source>Value is not calculated</source>
      <translation>値は計算されていません</translation>
    </message>
    <message>
      <source>Value is the</source>
      <translation>値は</translation>
    </message>
    <message>
      <source>sum</source>
      <translation>合計</translation>
    </message>
    <message>
      <source>product</source>
      <translation>積</translation>
    </message>
    <message>
      <source>average</source>
      <translation>平均</translation>
    </message>
    <message>
      <source>minimum</source>
      <translation>最小</translation>
    </message>
    <message>
      <source>maximum</source>
      <translation>最大</translation>
    </message>
    <message>
      <source>of the following fields:</source>
      <translation>以下のフィールド:</translation>
    </message>
    <message>
      <source>Pick...</source>
      <translation>抽出...</translation>
    </message>
    <message>
      <source>Custom calculation script:</source>
      <translation>カスタム計算スクリプト:</translation>
    </message>
    <message>
      <source>Calculate</source>
      <translation>計算</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Flag is ignored for PDF 1.3</source>
      <translation>フラグはPDF 1.3では無視されます</translation>
    </message>
    <message>
      <source>Enter a comma separated list of fields here</source>
      <translation>ここにカンマ区切りのフィールドのリストを入力してください</translation>
    </message>
    <message>
      <source>You need at least the Icon for Normal to use Icons for Buttons</source>
      <translation>ボタンにアイコンを使うには正常なアイコンが少なくとも必要です</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Example:</source>
      <translation>例:</translation>
    </message>
    <message>
      <source>Selection Change</source>
      <translation>選択を変更</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDFファイル (*.pdf);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>None</source>
      <comment>highlight</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>Limit of</source>
      <translation>制限</translation>
    </message>
    <message>
      <source>Default is Checked</source>
      <translation>標準でチェック</translation>
    </message>
    <message>
      <source>None</source>
      <comment>action</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>Tooltip:</source>
      <translation>ツールチップ:</translation>
    </message>
    <message>
      <source>Do Not Export Value</source>
      <translation>値を出力しない</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)</source>
      <translation>画像 (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;%1;;All Files (*)</source>
      <translation>画像 (*.tif *.png *.jpg *.xpm);;%1;;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Export absolute Filename</source>
      <translation>完全なファイル名でエクスポート</translation>
    </message>
  </context>
  <context>
    <name>Annota</name>
    <message>
      <source>Annotation Properties</source>
      <translation>注釈のプロパティ</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>テキスト</translation>
    </message>
    <message>
      <source>Link</source>
      <translation>リンク</translation>
    </message>
    <message>
      <source>External Link</source>
      <translation>外部リンク</translation>
    </message>
    <message>
      <source>External Web-Link</source>
      <translation>外部ウェブリンク</translation>
    </message>
    <message>
      <source>&amp;Type:</source>
      <translation>タイプ(&amp;T):</translation>
    </message>
    <message>
      <source>Destination</source>
      <translation>出力先</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>変更(&amp;H)...</translation>
    </message>
    <message>
      <source>&amp;Page:</source>
      <translation>ページ(&amp;P):</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>&amp;X-Pos</source>
      <translation>位置&amp;X</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>位置&amp;Y:</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation>%1;;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Export absolute Filename</source>
      <translation>完全なファイル名でエクスポート</translation>
    </message>
  </context>
  <context>
    <name>ApplyMasterPageDialog</name>
    <message>
      <source>Apply Master Page</source>
      <translation>マスターページに適用</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>マスターページ(&amp;M):</translation>
    </message>
    <message>
      <source>Apply To</source>
      <translation>適用先</translation>
    </message>
    <message>
      <source>Current &amp;page</source>
      <translation>現在のページ(&amp;P)</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>&amp;Even pages</source>
      <translation>偶数ページ(&amp;E)</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>O&amp;dd pages</source>
      <translation>奇数ページ(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>&amp;All pages</source>
      <translation>全てのページ(&amp;A)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Within range</source>
      <translation>範囲内(&amp;W)</translation>
    </message>
    <message>
      <source>Alt+W</source>
      <translation>Alt+W</translation>
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
      <translation>選択されたマスターページを、以下の範囲内の偶数、奇数、全てのページに適用します</translation>
    </message>
    <message>
      <source>to</source>
      <translation>から</translation>
    </message>
    <message>
      <source>Possible Hyphenation</source>
      <translation>利用可能なハイフネーション</translation>
    </message>
  </context>
  <context>
    <name>AspellPlugin</name>
    <message>
      <source>Spell-checking support</source>
      <translation>スペルチェックのサポート</translation>
    </message>
    <message>
      <source>Adds support for spell-checking via aspell. Languages can be chosen from among the installed aspell dictionaries, and spell-checking can be done on the fly, or on selected text.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>0.1</source>
      <translation>0.1</translation>
    </message>
    <message>
      <source>Spell Checker</source>
      <translation>スペルチェッカー</translation>
    </message>
  </context>
  <context>
    <name>AspellPluginBase</name>
    <message>
      <source>Spell Check</source>
      <translation>スペルチェック</translation>
    </message>
    <message>
      <source>Mis-spelling:</source>
      <translation>ミススペル</translation>
    </message>
    <message>
      <source>Replacement:</source>
      <translation>置換:</translation>
    </message>
    <message>
      <source>Active dictionary: </source>
      <translation>Active dictionary: </translation>
    </message>
    <message>
      <source>Personal
Dictionary</source>
      <translation>個人ディレクトリ</translation>
    </message>
    <message>
      <source>&amp;Add Word</source>
      <translation>単語を追加(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Change</source>
      <translation>変更(&amp;C)</translation>
    </message>
    <message>
      <source>C&amp;hange All</source>
      <translation>全て変更(&amp;H)</translation>
    </message>
    <message>
      <source>&amp;Skip</source>
      <translation>スキップ(&amp;S)</translation>
    </message>
    <message>
      <source>S&amp;kip All</source>
      <translation>全てスキップ(&amp;K)</translation>
    </message>
    <message>
      <source>&amp;Exit</source>
      <translation>終了(&amp;E)</translation>
    </message>
  </context>
  <context>
    <name>AspellPluginImpl</name>
    <message>
      <source>Loaded </source>
      <translation>読み込み </translation>
    </message>
    <message>
      <source>default </source>
      <translation>デフォルト </translation>
    </message>
    <message>
      <source> aspell dictionary.</source>
      <translation> aspell辞書.</translation>
    </message>
    <message>
      <source>aspellplugin (AspellPluginImpl::AspellPluginImpl): Error in aspell speller configuration.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>aspellplugin (AspellPluginImpl::AspellPluginImpl): Error in creating aspell speller.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Spell-Checker</source>
      <translation>スペルチェッカ</translation>
    </message>
    <message>
      <source>Spell-checking completed.</source>
      <translation>スペルチェック完了</translation>
    </message>
    <message>
      <source>Spell-checking done.</source>
      <translation>スペルチェック終了</translation>
    </message>
    <message>
      <source>AspellPluginImpl::on_fskipAllBtn_clicked(): Unable to skip all instances of &quot;</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source> by adding it to the session list.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>AspellPluginImpl::on_faddWordBtn_clicked(): Unable to add word to personal list.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>AutoformButtonGroup</name>
    <message>
      <source>Arrows</source>
      <translation>矢印</translation>
    </message>
    <message>
      <source>Flow Chart</source>
      <translation>フローチャート</translation>
    </message>
    <message>
      <source>Jigsaw</source>
      <translation>ジグソー</translation>
    </message>
    <message>
      <source>Specials</source>
      <translation>特別</translation>
    </message>
    <message>
      <source>Default Shapes</source>
      <translation>デフォルトの形状</translation>
    </message>
  </context>
  <context>
    <name>Barcode</name>
    <message>
      <source>Scribus frontend for Pure Postscript Barcode Writer</source>
      <translation>純粋なPostscriptバーコードライタのScribusフロントエンド</translation>
    </message>
    <message>
      <source>&amp;Barcode...</source>
      <translation>バーコード(&amp;B)...</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGeneratorBase</name>
    <message>
      <source>&amp;Type:</source>
      <translation>タイプ(&amp;T):</translation>
    </message>
    <message>
      <source>Co&amp;de:</source>
      <translation>コード(&amp;D):</translation>
    </message>
    <message>
      <source>Select one of the available barcode type here</source>
      <translation>利用可能なバーコードの種類を選択してください</translation>
    </message>
    <message>
      <source>The numeric representation of the code itself. See the help message below</source>
      <translation>コードそのものを表す数字です。以下のヘルプメッセージを参照してください。</translation>
    </message>
    <message>
      <source>Reset the barcode samples</source>
      <translation>バーコードのサンプルをリセット</translation>
    </message>
    <message>
      <source>&amp;Include text in barcode</source>
      <translation>バーコード中にテキストを含める(&amp;I)</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation>Alt+I</translation>
    </message>
    <message>
      <source>If checked, there will be numbers in the barcode too</source>
      <translation>チェックすると、バーコードに数字も追加します</translation>
    </message>
    <message>
      <source>&amp;Guard whitespace</source>
      <translation>余白を保護(&amp;G)</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Draw arrows to be sure of space next the code</source>
      <translation>次のコードとのスペースを保つために矢印を表示します</translation>
    </message>
    <message>
      <source>I&amp;nclude checksum</source>
      <translation>チェックサムを含める(&amp;N)</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Generate and include a checksum in barcode</source>
      <translation>バーコードにチェックサムを含めて生成する</translation>
    </message>
    <message>
      <source>Incl&amp;ude checksum digit</source>
      <translation>チェックデジットを含める(&amp;U)</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Include the checksum digit in the barcode text</source>
      <translation>バーコードテキストにチェックデジットを含める</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>色</translation>
    </message>
    <message>
      <source>&amp;Background</source>
      <translation>背景(&amp;B)</translation>
    </message>
    <message>
      <source>Alt+B</source>
      <translation>Alt+B</translation>
    </message>
    <message>
      <source>Background color - under the code lines</source>
      <translation>背景色 - コードの線の背景</translation>
    </message>
    <message>
      <source>&amp;Lines</source>
      <translation>線(&amp;L)</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Color of the lines in barcode</source>
      <translation>バーコードの線の色</translation>
    </message>
    <message>
      <source>&amp;Text</source>
      <translation>テキスト(&amp;T)</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Color of the text and numbers</source>
      <translation>テキストと数字の色</translation>
    </message>
    <message>
      <source>Hints and help is shown here</source>
      <translation>ヒントとヘルプがここに表示されます</translation>
    </message>
    <message>
      <source>Preview of the result. 72dpi sample.</source>
      <translation>結果のプレビュー. 72dpiのサンプル</translation>
    </message>
    <message>
      <source>Insert Barcode</source>
      <translation>バーコードを挿入</translation>
    </message>
    <message>
      <source>Format</source>
      <translation>フォーマット</translation>
    </message>
  </context>
  <context>
    <name>Biblio</name>
    <message>
      <source>Rename</source>
      <translation>名前を変更</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>スクラップブック</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
      <translation>名前 &quot;%1&quot; はすでに存在します。
他のものを選択してください。</translation>
    </message>
    <message>
      <source>Object</source>
      <translation>オブジェクト</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>名前(&amp;N):</translation>
    </message>
    <message>
      <source>New Entry</source>
      <translation>新規エントリ</translation>
    </message>
    <message>
      <source>Choose a Scrapbook Directory</source>
      <translation>スクラップブックのディレクトリを選択</translation>
    </message>
    <message>
      <source>Choose a scrapbook file to import</source>
      <translation>インポートするスクラップブックファイルを選択</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>ディレクトリを選択</translation>
    </message>
    <message>
      <source>Create a new scrapbook page</source>
      <translation>新規スクラップブックページを作成</translation>
    </message>
    <message>
      <source>Load an existing scrapbook</source>
      <translation>既存のスクラップブックを読み込む</translation>
    </message>
    <message>
      <source>Save the selected scrapbook</source>
      <translation>選択されたスクラップブックを保存</translation>
    </message>
    <message>
      <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
      <translation>Scribus 1.3.2以下からスクラップブックファイルをインポート</translation>
    </message>
    <message>
      <source>Close the selected scrapbook</source>
      <translation>選択されたスクラップブックを閉じる</translation>
    </message>
    <message>
      <source>Copy To:</source>
      <translation>コピー:</translation>
    </message>
    <message>
      <source>Move To:</source>
      <translation>移動:</translation>
    </message>
    <message>
      <source>Main</source>
      <translation>メイン</translation>
    </message>
    <message>
      <source>Copied Items</source>
      <translation>コピーされたアイテム</translation>
    </message>
    <message>
      <source>New Name</source>
      <translation>新しい名前:</translation>
    </message>
    <message>
      <source>Scrapbook (*.scs *.SCS)</source>
      <translation>スクラップブック (*.scs *.SCS)</translation>
    </message>
    <message>
      <source>Paste to Page</source>
      <translation>ページに貼り付け</translation>
    </message>
    <message>
      <source>Save as...</source>
      <translation>名前を付けて保存...</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>閉じる</translation>
    </message>
    <message>
      <source>Delete Contents</source>
      <translation>内容を削除</translation>
    </message>
    <message>
      <source>Do you really want to delete all entries?</source>
      <translation>本当に全てのエントリを削除しますか?</translation>
    </message>
  </context>
  <context>
    <name>BookPalette</name>
    <message>
      <source>Bookmarks</source>
      <translation>ブックマーク</translation>
    </message>
  </context>
  <context>
    <name>ButtonIcon</name>
    <message>
      <source>Icon Placement</source>
      <translation>アイコンの位置ずれ</translation>
    </message>
    <message>
      <source>Layout:</source>
      <translation>レイアウト:</translation>
    </message>
    <message>
      <source>Caption only</source>
      <translation>キャプションのみ</translation>
    </message>
    <message>
      <source>Icon only</source>
      <translation>アイコンのみ</translation>
    </message>
    <message>
      <source>Caption below Icon</source>
      <translation>アイコンの下にキャプション</translation>
    </message>
    <message>
      <source>Caption above Icon</source>
      <translation>アイコンの上にキャプション</translation>
    </message>
    <message>
      <source>Caption right to Icon</source>
      <translation>アイコンの右にキャプション</translation>
    </message>
    <message>
      <source>Caption left to Icon</source>
      <translation>アイコンの左にキャプション</translation>
    </message>
    <message>
      <source>Scale:</source>
      <translation>倍率:</translation>
    </message>
    <message>
      <source>Always</source>
      <translation>常に</translation>
    </message>
    <message>
      <source>When Icon is too small</source>
      <translation>アイコンが小さすぎるとき</translation>
    </message>
    <message>
      <source>When Icon is too big</source>
      <translation>アイコンが大きすぎるとき</translation>
    </message>
    <message>
      <source>Never</source>
      <translation>しない</translation>
    </message>
    <message>
      <source>Scale How:</source>
      <translation>倍率の大きさ:</translation>
    </message>
    <message>
      <source>Proportional</source>
      <translation>プロポーショナル</translation>
    </message>
    <message>
      <source>Non Proportional</source>
      <translation>非プロポーショナル</translation>
    </message>
    <message>
      <source>Icon</source>
      <translation>アイコン</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Reset</source>
      <translation>リセット</translation>
    </message>
    <message>
      <source>Caption overlays Icon</source>
      <translation>アイコンにオーバーレイでキャプション</translation>
    </message>
  </context>
  <context>
    <name>CMSPrefs</name>
    <message>
      <source>Perceptual</source>
      <translation>知覚</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>相対的な色彩</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>彩度</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>絶対的な色彩</translation>
    </message>
  </context>
  <context>
    <name>CMSPrefsBase</name>
    <message>
      <source>Form</source>
      <translation>フォーム</translation>
    </message>
    <message>
      <source>&amp;Activate Color Management</source>
      <translation>カラーマネージメントを有効に(&amp;A)</translation>
    </message>
    <message>
      <source>System Profiles</source>
      <translation>システムプロファイル</translation>
    </message>
    <message>
      <source>&amp;RGB Images:</source>
      <translation>RGB画像(&amp;R):</translation>
    </message>
    <message>
      <source>Default color profile for imported RGB images</source>
      <translation>インポートしたRGB画像の標準カラープロファイル</translation>
    </message>
    <message>
      <source>&amp;CMYK Images:</source>
      <translation>CMYK画像(&amp;C):</translation>
    </message>
    <message>
      <source>Default color profile for imported CMYK images</source>
      <translation>インポートしたCMYK画像の標準カラープロファイル</translation>
    </message>
    <message>
      <source>&amp;RGB Solid Colors:</source>
      <translation>RGB単色(&amp;R):</translation>
    </message>
    <message>
      <source>Default color profile for solid RGB colors on the page</source>
      <translation>ページのRGB単色の標準カラープロファイル</translation>
    </message>
    <message>
      <source>&amp;CMYK Solid Colors:</source>
      <translation>CMYK単色(&amp;C):</translation>
    </message>
    <message>
      <source>Default color profile for solid CMYK colors on the page</source>
      <translation>ページのCMYK単色の標準カラープロファイル</translation>
    </message>
    <message>
      <source>&amp;Monitor:</source>
      <translation>モニタ(&amp;M):</translation>
    </message>
    <message>
      <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
      <translation>自分で作成したか、メーカから受け取ったカラープロファイルです。
このプロファイルはモニタ特有のものであり、一般的なプロファイル(例 sRGB)ではありません。</translation>
    </message>
    <message>
      <source>P&amp;rinter:</source>
      <translation>プリンタ(&amp;R):</translation>
    </message>
    <message>
      <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
      <translation>メーカから出されている、プリンタモデル向けのカラープロファイルです。
このプロファイルはプリンタ特有のものであり、一般的なプロファイル(例 sRGB)ではありません。</translation>
    </message>
    <message>
      <source>Rendering Intents</source>
      <translation>レンダリングインテント</translation>
    </message>
    <message>
      <source>Images:</source>
      <translation>画像:</translation>
    </message>
    <message>
      <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation>画像のレンダリングインテントです。変更する理由が分からなければ、
相対的な色彩もしくは知覚を選択してください。</translation>
    </message>
    <message>
      <source>Sol&amp;id Colors:</source>
      <translation>単色(&amp;I):</translation>
    </message>
    <message>
      <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation>単色の標準レンダリングインテントです。変更する理由が分からなければ、
相対的な色彩もしくは知覚を選択してください。</translation>
    </message>
    <message>
      <source>Enable 'soft proofing' of how your document colors will print,
based on the chosen printer profile.</source>
      <translation>選択されたプリンタプロファイルに基づいて、ドキュメントカラーがどのように印刷されるかという'soft-proofing'を有効にします。</translation>
    </message>
    <message>
      <source>Sim&amp;ulate Printer on the Screen</source>
      <translation>画面上でプリンタをシミュレートする(&amp;U)</translation>
    </message>
    <message>
      <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
      <translation>フルカラー管理環境のシミュレート:
rgbもしくはcmykの全ての色は、プリンタ色空間に変換されます。</translation>
    </message>
    <message>
      <source>Convert all colors to printer space</source>
      <translation>全ての色をプリンタ空間に変換する</translation>
    </message>
    <message>
      <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Mark Colors out of &amp;Gamut</source>
      <translation>範囲外の色をマークする</translation>
    </message>
    <message>
      <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
      <translation>黒点補正は写真のコントラストを改善する手法です。
ドキュメント中に写真がある場合は、これを有効にすることを推奨します。</translation>
    </message>
    <message>
      <source>Use &amp;Blackpoint Compensation</source>
      <translation>黒点補正を使用(&amp;B)</translation>
    </message>
  </context>
  <context>
    <name>CMYKChoose</name>
    <message>
      <source>Edit Color</source>
      <translation>色を編集</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>名前(&amp;N):</translation>
    </message>
    <message>
      <source>Color &amp;Model</source>
      <translation>カラーモデル(&amp;M)</translation>
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
      <translation>Webセーフ RGB</translation>
    </message>
    <message>
      <source>Is Spot Color</source>
      <translation>スポットカラー</translation>
    </message>
    <message>
      <source>New</source>
      <translation>新規</translation>
    </message>
    <message>
      <source>Old</source>
      <translation>前の色</translation>
    </message>
    <message>
      <source>HSV-Colormap</source>
      <translation>HSVカラーマップ</translation>
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
      <translation>ダイナミックカラーバー</translation>
    </message>
    <message>
      <source>Static Color Bars</source>
      <translation>スタティックカラーバー</translation>
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
      <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
      <translation>&quot;%1&quot;という名前の色を作成できません。 
透明色の名前として予約されています。</translation>
    </message>
    <message>
      <source>The name of the color already exists,
please choose another one.</source>
      <translation>その色の名前は既に存在します、
他のものを選択してください。</translation>
    </message>
    <message>
      <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You cannot create a color without a name
Please give it a name</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>CWDialog</name>
    <message>
      <source>Color Wheel</source>
      <translation>カラーホイール</translation>
    </message>
    <message>
      <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
      <translation>べースカラーを取得するためにホイールをクリックしてください。カラーモデルは選択されたタブに依存します。</translation>
    </message>
    <message>
      <source>Result Colors</source>
      <translation>出力色</translation>
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
      <translation>選択したカラースキームの色</translation>
    </message>
    <message>
      <source>Color Scheme Method</source>
      <translation>カラースキームメソッド</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation>角度:</translation>
    </message>
    <message>
      <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
      <translation>選択された値と他のものの間の差です。詳細はドキュメントを参照してください。</translation>
    </message>
    <message>
      <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
      <translation>カラースキームを作成するメソッドを選択してください。詳細な情報はドキュメントを参照してください。</translation>
    </message>
    <message>
      <source>Merge created colors into the document colors</source>
      <translation>作成された色をドキュメントカラーにマージする</translation>
    </message>
    <message>
      <source>&amp;Merge</source>
      <translation>マージ(&amp;M)</translation>
    </message>
    <message>
      <source>Alt+M</source>
      <translation>Alt+M</translation>
    </message>
    <message>
      <source>Replace created colors in the document colors</source>
      <translation>ドキュメントカラー中で作成された色を置換する</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>置換(&amp;R)</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Leave colors untouched</source>
      <translation>色を触らないでおく</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Preview:</source>
      <translation>プレビュー:</translation>
    </message>
    <message>
      <source>Sample color scheme.</source>
      <translation>サンプルカラースキーム</translation>
    </message>
    <message>
      <source>Simulate common vision defects here. Select type of the defect.</source>
      <translation>ここで一般的な色覚異常をシミュレートします。色覚異常の型を選択してください。</translation>
    </message>
    <message>
      <source>Vision Defect Type:</source>
      <translation>視覚異常のタイプ:</translation>
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
      <translation>ドキュメント</translation>
    </message>
  </context>
  <context>
    <name>CharSelect</name>
    <message>
      <source>Font:</source>
      <translation>フォント:</translation>
    </message>
    <message>
      <source>Character Class:</source>
      <translation>文字クラス:</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>挿入(&amp;I)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>Insert the characters at the cursor in the text</source>
      <translation>テキスト中のカーソルの文字を挿入</translation>
    </message>
    <message>
      <source>Delete the current selection(s).</source>
      <translation>現在選択しているものを削除</translation>
    </message>
    <message>
      <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enhanced Palette</source>
      <translation>拡張パレット</translation>
    </message>
    <message>
      <source>Quick Palette</source>
      <translation>クイックパレット</translation>
    </message>
    <message>
      <source>Character Palette</source>
      <translation>文字パレット</translation>
    </message>
    <message>
      <source>Hide/Show Enhanced Palette</source>
      <translation>拡張パレットを隠す/表示</translation>
    </message>
  </context>
  <context>
    <name>CharStyleComboBox</name>
    <message>
      <source>No Style</source>
      <translation>スタイルなし</translation>
    </message>
  </context>
  <context>
    <name>CharTableView</name>
    <message>
      <source>Delete</source>
      <translation>削除</translation>
    </message>
  </context>
  <context>
    <name>CheckDocument</name>
    <message>
      <source>Glyphs missing</source>
      <translation>グリフが見つかりません</translation>
    </message>
    <message>
      <source>Text overflow</source>
      <translation>テキストがあふれています</translation>
    </message>
    <message>
      <source>Object is not on a Page</source>
      <translation>オブジェクトはページ上にありません</translation>
    </message>
    <message>
      <source>Missing Image</source>
      <translation>画像が見つかりません</translation>
    </message>
    <message>
      <source>Object has transparency</source>
      <translation>オブジェクトは透明です</translation>
    </message>
    <message>
      <source>Object is a PDF Annotation or Field</source>
      <translation>オブジェクトはPDF注釈もしくはフィールドです</translation>
    </message>
    <message>
      <source>Object is a placed PDF</source>
      <translation>オブジェクトは配置されたPDFです</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>ドキュメント</translation>
    </message>
    <message>
      <source>No Problems found</source>
      <translation>問題は見つかりませんでした</translation>
    </message>
    <message>
      <source>Page </source>
      <translation>ページ </translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation>フリーオブジェクト</translation>
    </message>
    <message>
      <source>Problems found</source>
      <translation>問題が見つかりました</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>プリフライト検証</translation>
    </message>
    <message>
      <source>Items</source>
      <translation>アイテム</translation>
    </message>
    <message>
      <source>Problems</source>
      <translation>問題</translation>
    </message>
    <message>
      <source>Current Profile:</source>
      <translation>現在のプロファイル:</translation>
    </message>
    <message>
      <source>&amp;Ignore Errors</source>
      <translation>エラーを無視(&amp;I)</translation>
    </message>
    <message>
      <source>Check again</source>
      <translation>再度チェック</translation>
    </message>
    <message>
      <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
      <translation>画像の解像度が %1 DPI以下です。現在 %2 x %3 DPIです。</translation>
    </message>
    <message>
      <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
      <translation>画像の解像度が %1 DPI以上です。現在 %2 x %3 DPIです。</translation>
    </message>
    <message>
      <source>Image is GIF</source>
      <translation>GIF画像です</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Transparency used</source>
      <translation>透明度が使用されています</translation>
    </message>
    <message>
      <source>Blendmode used</source>
      <translation>合成モードが使用されています</translation>
    </message>
    <message>
      <source>Layer &quot;%1&quot;</source>
      <translation>レイヤー &quot;%1&quot;</translation>
    </message>
    <message>
      <source>Annotation uses a non TrueType font</source>
      <translation>注釈でTrueTypeフォントでないものが使用されています</translation>
    </message>
    <message>
      <source>Preflight profile to base the report generation on. Options can be set in Document Setup or Preferences</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ignore found errors and continue to export or print. Be sure to understand the errors you are ignoring before continuing.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rerun the document scan to check corrections you may have made</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Empty Image Frame</source>
      <translation>画像フレームを空に</translation>
    </message>
  </context>
  <context>
    <name>CollectForOutput</name>
    <message>
      <source>Choose a Directory</source>
      <translation>ディレクトリを選択</translation>
    </message>
    <message>
      <source>Collecting...</source>
      <translation>まとめ中...</translation>
    </message>
    <message>
      <source>Cannot collect the file: 
%1</source>
      <translation>ファイルをまとめることができません:
%1</translation>
    </message>
    <message>
      <source>Cannot collect all files for output for file:
%1</source>
      <translation>全てのファイルを出力ファイルにまとめることができません:
%1</translation>
    </message>
  </context>
  <context>
    <name>ColorManager</name>
    <message>
      <source>Colors</source>
      <translation>色</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>インポート(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>新規(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>編集(&amp;E)</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>複製(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Remove Unused</source>
      <translation>使用しないものを削除(&amp;R)</translation>
    </message>
    <message>
      <source>Color Sets</source>
      <translation>カラーセット</translation>
    </message>
    <message>
      <source>Current Color Set:</source>
      <translation>現在のカラーセット:</translation>
    </message>
    <message>
      <source>&amp;Save Color Set</source>
      <translation>カラーセットを保存(&amp;S)</translation>
    </message>
    <message>
      <source>Choose a color set to load</source>
      <translation>読み込むカラーセットを選択</translation>
    </message>
    <message>
      <source>Save the current color set</source>
      <translation>現在のカラーセットを保存</translation>
    </message>
    <message>
      <source>Remove unused colors from current document's color set</source>
      <translation>現在のドキュメントのカラーセットから使用されていない色を削除します</translation>
    </message>
    <message>
      <source>Import colors to the current set from an existing document</source>
      <translation>既存のドキュメントから現在のセットに色をインポート</translation>
    </message>
    <message>
      <source>Create a new color within the current set</source>
      <translation>現在のセット中に新しい色を作成します</translation>
    </message>
    <message>
      <source>Edit the currently selected color</source>
      <translation>現在選択されている色を編集</translation>
    </message>
    <message>
      <source>Make a copy of the currently selected color</source>
      <translation>現在選択されている色のコピーを作成</translation>
    </message>
    <message>
      <source>Delete the currently selected color</source>
      <translation>現在選択されている色を削除</translation>
    </message>
    <message>
      <source>Make the current colorset the default color set</source>
      <translation>現在の色をデフォルトのカラーセットに設定する</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>名前(&amp;N):</translation>
    </message>
    <message>
      <source>Choose a Name</source>
      <translation>名前を選択</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>%1のコピー</translation>
    </message>
    <message>
      <source>New Color</source>
      <translation>新しい色</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>情報</translation>
    </message>
    <message>
      <source>Import</source>
      <translation>インポート</translation>
    </message>
    <message>
      <source>The file %1 does not contain colors which can be imported.
If the file was a PostScript-based, try to import it with File -&amp;gt; Import. 
Not all files have DSC conformant comments where the color descriptions are located.
 This prevents importing colors from some files.
See the Edit Colors section of the documentation for more details.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. More hints about gamut warnings are in the online help under Color Management. Registration colors will have a registration mark next to the color. Use Registration only for printers marks and crop marks.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>All Supported Formats (%1);;Documents (%2);;Other Files (%3);;All Files (*)</source>
      <translation>全てのサポートしているファイル (%1);;ドキュメント (%2);;他のファイル (%3);;全てのファイル (*)</translation>
    </message>
  </context>
  <context>
    <name>ColorWheel</name>
    <message>
      <source>Monochromatic</source>
      <translation>単色</translation>
    </message>
    <message>
      <source>Analogous</source>
      <translation>類似色</translation>
    </message>
    <message>
      <source>Complementary</source>
      <translation>補色</translation>
    </message>
    <message>
      <source>Split Complementary</source>
      <translation>分裂補色</translation>
    </message>
    <message>
      <source>Triadic</source>
      <translation>トライアド配色</translation>
    </message>
    <message>
      <source>Tetradic (Double Complementary)</source>
      <translation>テトラード配色 (二重補色)</translation>
    </message>
    <message>
      <source>Base Color</source>
      <translation>基調色</translation>
    </message>
    <message>
      <source>Monochromatic Light</source>
      <translation>明るい単色</translation>
    </message>
    <message>
      <source>Monochromatic Dark</source>
      <translation>暗い単色</translation>
    </message>
    <message>
      <source>1st. Analogous</source>
      <translation>第1類似色</translation>
    </message>
    <message>
      <source>2nd. Analogous</source>
      <translation>第2類似色</translation>
    </message>
    <message>
      <source>1st. Split</source>
      <translation>第1分裂色</translation>
    </message>
    <message>
      <source>2nd. Split</source>
      <translation>第2分裂色</translation>
    </message>
    <message>
      <source>3rd. Split</source>
      <translation>第3分裂色</translation>
    </message>
    <message>
      <source>4th. Split</source>
      <translation>第4分裂色</translation>
    </message>
    <message>
      <source>1st. Triadic</source>
      <translation>第1トライアド</translation>
    </message>
    <message>
      <source>2nd. Triadic</source>
      <translation>第2トライアド</translation>
    </message>
    <message>
      <source>1st. Tetradic (base opposite)</source>
      <translation>第1テトラード(反対基調)</translation>
    </message>
    <message>
      <source>2nd. Tetradic (angle)</source>
      <translation>第2テトラード(角度)</translation>
    </message>
    <message>
      <source>3rd. Tetradic (angle opposite)</source>
      <translation>第3テトラード(反対角度)</translation>
    </message>
  </context>
  <context>
    <name>ColorWheelPlugin</name>
    <message>
      <source>&amp;Color Wheel...</source>
      <translation>カラーホイール(&amp;C)...</translation>
    </message>
    <message>
      <source>Color setting helper</source>
      <translation>色設定ヘルパー</translation>
    </message>
    <message>
      <source>Color selector with color theory included.</source>
      <translation>カラー理論を含んだカラーセレクタ</translation>
    </message>
  </context>
  <context>
    <name>CommonStrings</name>
    <message>
      <source>&amp;Apply</source>
      <translation>適用(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
    </message>
    <message>
      <source>None</source>
      <translation>なし</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>保存(&amp;S)</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>警告</translation>
    </message>
    <message>
      <source>None</source>
      <comment>color name</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>Custom</source>
      <comment>CommonStrings, custom page size</comment>
      <translation>カスタム</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>単一ページ</translation>
    </message>
    <message>
      <source>Double Sided</source>
      <translation>両面</translation>
    </message>
    <message>
      <source>3-Fold</source>
      <translation>三つ折り</translation>
    </message>
    <message>
      <source>4-Fold</source>
      <translation>四つ折り</translation>
    </message>
    <message>
      <source>Monday</source>
      <translation>月曜日</translation>
    </message>
    <message>
      <source>Tuesday</source>
      <translation>火曜日</translation>
    </message>
    <message>
      <source>Wednesday</source>
      <translation>水曜日</translation>
    </message>
    <message>
      <source>Thursday</source>
      <translation>木曜日</translation>
    </message>
    <message>
      <source>Friday</source>
      <translation>金曜日</translation>
    </message>
    <message>
      <source>Saturday</source>
      <translation>土曜日</translation>
    </message>
    <message>
      <source>Sunday</source>
      <translation>日曜日</translation>
    </message>
    <message>
      <source>January</source>
      <translation>1月</translation>
    </message>
    <message>
      <source>February</source>
      <translation>2月</translation>
    </message>
    <message>
      <source>March</source>
      <translation>3月</translation>
    </message>
    <message>
      <source>April</source>
      <translation>4月</translation>
    </message>
    <message>
      <source>May</source>
      <translation>5月</translation>
    </message>
    <message>
      <source>June</source>
      <translation>6月</translation>
    </message>
    <message>
      <source>July</source>
      <translation>6月</translation>
    </message>
    <message>
      <source>August</source>
      <translation>8月</translation>
    </message>
    <message>
      <source>September</source>
      <translation>9月</translation>
    </message>
    <message>
      <source>October</source>
      <translation>10月</translation>
    </message>
    <message>
      <source>November</source>
      <translation>11月</translation>
    </message>
    <message>
      <source>December</source>
      <translation>12月</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>はい</translation>
    </message>
    <message>
      <source>No</source>
      <translation>いいえ</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>はい(&amp;Y)</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>いいえ(&amp;N)</translation>
    </message>
    <message>
      <source>Left Page</source>
      <comment>Left page location</comment>
      <translation>左ページ</translation>
    </message>
    <message>
      <source>Middle</source>
      <comment>Middle page location</comment>
      <translation>中央ページ</translation>
    </message>
    <message>
      <source>Middle Left</source>
      <comment>Middle Left page location</comment>
      <translation>中央左ページ</translation>
    </message>
    <message>
      <source>Middle Right</source>
      <comment>Middle Right page location</comment>
      <translation>中央右ページ</translation>
    </message>
    <message>
      <source>Right Page</source>
      <comment>Right page location</comment>
      <translation>右ページ</translation>
    </message>
    <message>
      <source>Normal</source>
      <comment>Default single master page</comment>
      <translation>標準</translation>
    </message>
    <message>
      <source>Normal Left</source>
      <comment>Default left master page</comment>
      <translation>標準 左</translation>
    </message>
    <message>
      <source>Normal Middle</source>
      <comment>Default middle master page</comment>
      <translation>標準 中央ページ</translation>
    </message>
    <message>
      <source>Normal Right</source>
      <comment>Default right master page</comment>
      <translation>標準 右</translation>
    </message>
    <message>
      <source>Normal Vision</source>
      <comment>Color Blindness - Normal Vision</comment>
      <translation>正常視力</translation>
    </message>
    <message>
      <source>Protanopia (Red)</source>
      <comment>Color Blindness - Red Color Blind</comment>
      <translation>赤色覚異常</translation>
    </message>
    <message>
      <source>Deuteranopia (Green)</source>
      <comment>Color Blindness - Greed Color Blind</comment>
      <translation>緑色覚異常</translation>
    </message>
    <message>
      <source>Tritanopia (Blue)</source>
      <comment>Color Blindness - Blue Color Blind</comment>
      <translation>青黄色覚異常</translation>
    </message>
    <message>
      <source>Full Color Blindness</source>
      <comment>Color Blindness - Full Color Blindness</comment>
      <translation>全色覚異常</translation>
    </message>
    <message>
      <source>Custom: </source>
      <comment>Custom Tab Fill Option</comment>
      <translation>カスタム: </translation>
    </message>
    <message>
      <source>Solid Line</source>
      <translation>実線</translation>
    </message>
    <message>
      <source>Dashed Line</source>
      <translation>破線</translation>
    </message>
    <message>
      <source>Dotted Line</source>
      <translation>点線</translation>
    </message>
    <message>
      <source>Dash Dot Line</source>
      <translation>破点線</translation>
    </message>
    <message>
      <source>Dash Dot Dot Line</source>
      <translation>破点点線</translation>
    </message>
    <message>
      <source>None</source>
      <comment>Optical Margin Setting</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>Left Protruding</source>
      <comment>Optical Margin Setting</comment>
      <translation>左に突出</translation>
    </message>
    <message>
      <source>Right Protruding</source>
      <comment>Optical Margin Setting</comment>
      <translation>右に突出</translation>
    </message>
    <message>
      <source>Left Hanging Punctuation</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Right Hanging Punctuation</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Default</source>
      <comment>Optical Margin Setting</comment>
      <translation>デフォルト</translation>
    </message>
    <message>
      <source>Min. Word Tracking</source>
      <translation>最小のワードトラッキング</translation>
    </message>
    <message>
      <source>Max. Word Tracking</source>
      <translation>最大のワードトラッキング</translation>
    </message>
    <message>
      <source>Min. Glyph Extension</source>
      <translation>最小のグリフ拡張</translation>
    </message>
    <message>
      <source>Max. Glyph Extension</source>
      <translation>最大のグリフ拡張</translation>
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
      <translation>グレースケール</translation>
    </message>
    <message>
      <source>Duotone</source>
      <comment>Colorspace</comment>
      <translation>2色網版</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>Colorspace (Unknown)</comment>
      <translation>不明</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation>テキストフレーム</translation>
    </message>
    <message>
      <source>Image Frame</source>
      <translation>画像フレーム</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>線</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation>多角形</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>ポリライン</translation>
    </message>
    <message>
      <source>Text on a Path</source>
      <translation>パス上のテキスト</translation>
    </message>
    <message>
      <source>Multiple</source>
      <comment>Multiple frame types</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Push Button</source>
      <translation>PDFプッシュボタン</translation>
    </message>
    <message>
      <source>PDF Text Field</source>
      <translation>PDFテキストフィールド</translation>
    </message>
    <message>
      <source>PDF Check Box</source>
      <translation>PDFチェックボックス</translation>
    </message>
    <message>
      <source>PDF Combo Box</source>
      <translation>PDFコンボボックス</translation>
    </message>
    <message>
      <source>PDF List Box</source>
      <translation>PDFリストボックス</translation>
    </message>
    <message>
      <source>PDF Text Annotation</source>
      <translation>PDFテキスト注釈</translation>
    </message>
    <message>
      <source>PDF Link Annotation</source>
      <translation>PDFリンク注釈</translation>
    </message>
    <message>
      <source>PostScript Level 1</source>
      <translation>PostScript レベル1</translation>
    </message>
    <message>
      <source>PostScript Level 2</source>
      <translation>PostScript レベル2</translation>
    </message>
    <message>
      <source>PostScript Level 3</source>
      <translation>PostScript レベル3</translation>
    </message>
    <message>
      <source>Windows GDI</source>
      <translation>Windows GDI</translation>
    </message>
    <message>
      <source>Render Frame</source>
      <translation>フレームを描画</translation>
    </message>
    <message>
      <source>Default Paragraph Style</source>
      <translation>標準段落スタイル</translation>
    </message>
    <message>
      <source>Default Character Style</source>
      <translation>標準文字スタイル</translation>
    </message>
    <message>
      <source>Default Line Style</source>
      <translation>標準線スタイル</translation>
    </message>
  </context>
  <context>
    <name>ContextMenu</name>
    <message>
      <source>Preview Settings</source>
      <translation>プレビュー設定</translation>
    </message>
    <message>
      <source>Paste File...</source>
      <translation>ファイルを貼り付け...</translation>
    </message>
    <message>
      <source>Delete Page</source>
      <translation>ページを削除</translation>
    </message>
  </context>
  <context>
    <name>CopyPageToMasterPageBase</name>
    <message>
      <source>Convert Page to Master Page</source>
      <translation>ページをマスターページに変換</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>名前:</translation>
    </message>
    <message>
      <source>Copy Applied Master Page Items</source>
      <translation>マスタページに適用されたアイテムをコピー</translation>
    </message>
  </context>
  <context>
    <name>CopyPageToMasterPageDialog</name>
    <message>
      <source>New Master Page %1</source>
      <translation>新規マスターページ %1</translation>
    </message>
  </context>
  <context>
    <name>Cpalette</name>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Shade:</source>
      <translation>色の濃さ:</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>不透明度:</translation>
    </message>
    <message>
      <source>Move Vector</source>
      <translation>ベクトルを移動</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>標準</translation>
    </message>
    <message>
      <source>Horizontal Gradient</source>
      <translation>水平グラディエント</translation>
    </message>
    <message>
      <source>Vertical Gradient</source>
      <translation>垂直グラディエント</translation>
    </message>
    <message>
      <source>Diagonal Gradient</source>
      <translation>対角グラディエント</translation>
    </message>
    <message>
      <source>Cross Diagonal Gradient</source>
      <translation>クロス対角グラディエント</translation>
    </message>
    <message>
      <source>Radial Gradient</source>
      <translation>放射グラディエント</translation>
    </message>
    <message>
      <source>Free linear Gradient</source>
      <translation>自由直線グラディエント</translation>
    </message>
    <message>
      <source>Free radial Gradient</source>
      <translation>自由放射グラディエント</translation>
    </message>
    <message>
      <source>Edit Line Color Properties</source>
      <translation>線の色のプロパティを編集</translation>
    </message>
    <message>
      <source>Edit Fill Color Properties</source>
      <translation>塗りつぶし色のプロパティを編集</translation>
    </message>
    <message>
      <source>Saturation of color</source>
      <translation>色の彩度</translation>
    </message>
    <message>
      <source>Normal or gradient fill method</source>
      <translation>標準もしくはグラディエントの塗りつぶし方</translation>
    </message>
    <message>
      <source>Set the transparency for the color selected</source>
      <translation>選択した色の透明度を設定してください</translation>
    </message>
    <message>
      <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Transparency Settings</source>
      <translation>透明度の設定</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>合成モード:</translation>
    </message>
    <message>
      <source>Darken</source>
      <translation>暗くする</translation>
    </message>
    <message>
      <source>Lighten</source>
      <translation>明るくする</translation>
    </message>
    <message>
      <source>Multiply</source>
      <translation>乗算</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>スクリーン</translation>
    </message>
    <message>
      <source>Overlay</source>
      <translation>オーバーレイ</translation>
    </message>
    <message>
      <source>Hard Light</source>
      <translation>ハードライト</translation>
    </message>
    <message>
      <source>Soft Light</source>
      <translation>ソフトライト</translation>
    </message>
    <message>
      <source>Difference</source>
      <translation>差の絶対値</translation>
    </message>
    <message>
      <source>Color Dodge</source>
      <translation>覆い焼きカラー</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>焼き込み</translation>
    </message>
    <message>
      <source>Hue</source>
      <translation>色合い</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>彩度</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Luminosity</source>
      <translation>輝度</translation>
    </message>
    <message>
      <source>Offsets</source>
      <translation>オフセット</translation>
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
      <translation>倍率</translation>
    </message>
    <message>
      <source>Rotation</source>
      <translation>回転</translation>
    </message>
    <message>
      <source>Angle</source>
      <translation>角度</translation>
    </message>
    <message>
      <source>Pattern</source>
      <translation>パターン</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>除外</translation>
    </message>
    <message>
      <source>X-Scale:</source>
      <translation>倍率X:</translation>
    </message>
    <message>
      <source>Y-Scale:</source>
      <translation>倍率Y:</translation>
    </message>
    <message>
      <source>Display only used Colors</source>
      <translation>使用中の色のみ表示</translation>
    </message>
    <message>
      <source>Display all colors from the document color list, or only the already used colors</source>
      <translation>ドキュメントカラーリストの全ての色か、すでに使用されている色のみ表示</translation>
    </message>
  </context>
  <context>
    <name>CreateRange</name>
    <message>
      <source>Create Range</source>
      <translation>範囲を作成</translation>
    </message>
    <message>
      <source>Number of Pages in Document:</source>
      <translation>ドキュメント中のページ数:</translation>
    </message>
    <message>
      <source>Doc Page Range</source>
      <translation>ドキュメントページの範囲</translation>
    </message>
    <message>
      <source>Basic Range Selection</source>
      <translation>基本的な範囲選択</translation>
    </message>
    <message>
      <source>Range of Pages</source>
      <translation>ページ範囲</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation>削除(&amp;L)</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Move &amp;Down</source>
      <translation>下に移動(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Move &amp;Up</source>
      <translation>上に移動(&amp;U)</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Add a Range of Pages</source>
      <translation>指定範囲を追加</translation>
    </message>
    <message>
      <source>Consecutive Pages</source>
      <translation>連続ページ</translation>
    </message>
    <message>
      <source>Even Pages</source>
      <translation>偶数ページ</translation>
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
      <translation>範囲に追加(&amp;A)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Odd Pages</source>
      <translation>奇数ページ</translation>
    </message>
    <message>
      <source>Comma Separated List</source>
      <translation>カンマ区切りのリスト</translation>
    </message>
    <message>
      <source>Advanced Reordering</source>
      <translation>詳細な並べ替え</translation>
    </message>
    <message>
      <source>Page Order</source>
      <translation>ページオーダー</translation>
    </message>
    <message>
      <source>Sample Page Order:</source>
      <translation>サンプルページ順:</translation>
    </message>
    <message>
      <source>Page Group Size:</source>
      <translation>ページグループサイズ:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
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
      <translation>CSVインポータオプション</translation>
    </message>
    <message>
      <source>Field delimiter:</source>
      <translation>フィールド区切り:</translation>
    </message>
    <message>
      <source>(TAB)</source>
      <translation>(TAB)</translation>
    </message>
    <message>
      <source>Value delimiter:</source>
      <translation>値の区切り:</translation>
    </message>
    <message>
      <source>First row is a header</source>
      <translation>最初の行をヘッダにする</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>None</source>
      <comment>delimiter</comment>
      <translation>なし</translation>
    </message>
  </context>
  <context>
    <name>CupsOptions</name>
    <message>
      <source>Printer Options</source>
      <translation>プリンタオプション</translation>
    </message>
    <message>
      <source>Page Set</source>
      <translation>ページセット</translation>
    </message>
    <message>
      <source>All Pages</source>
      <translation>全てのページ</translation>
    </message>
    <message>
      <source>Even Pages only</source>
      <translation>偶数ページのみ</translation>
    </message>
    <message>
      <source>Odd Pages only</source>
      <translation>奇数ページのみ</translation>
    </message>
    <message>
      <source>Orientation</source>
      <translation>方向</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>縦方向</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>横方向</translation>
    </message>
    <message>
      <source>N-Up Printing</source>
      <translation>集約印刷</translation>
    </message>
    <message>
      <source>Page per Sheet</source>
      <translation>シートあたりのページ数</translation>
    </message>
    <message>
      <source>Pages per Sheet</source>
      <translation>シートあたりのページ数</translation>
    </message>
    <message>
      <source>Option</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>値</translation>
    </message>
    <message>
      <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help > About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
      <translation>このパネルでは印刷時の各種CUPSオプションを表示します。利用可能な正確なパラメータは、プリンタドライバに依存します。CUPSがサポートされているか ヘルプ > Scribusについて を選択して確認してください。リストを参照してください: C-C-T はC=CUPS C=littlecms T=TIFF サポートと同等です。ライブラリのサポートがない場合は * で表示されます。</translation>
    </message>
    <message>
      <source>Mirror</source>
      <translation>ミラー</translation>
    </message>
  </context>
  <context>
    <name>CurveWidget</name>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Curve Files (*.scu);;All Files (*)</source>
      <translation>曲線ファイル (*.scu);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>ファイルを書き込めません: 
%1</translation>
    </message>
    <message>
      <source>Inverts the curve</source>
      <translation>曲線を反転</translation>
    </message>
    <message>
      <source>Resets the curve</source>
      <translation>曲線をリセット</translation>
    </message>
    <message>
      <source>Switches between linear and cubic interpolation of the curve</source>
      <translation>曲線の直線補間と3次補間を切り替えます</translation>
    </message>
    <message>
      <source>Loads a curve</source>
      <translation>曲線を読み込み</translation>
    </message>
    <message>
      <source>Saves this curve</source>
      <translation>この曲線を保存</translation>
    </message>
  </context>
  <context>
    <name>CustomFDialog</name>
    <message>
      <source>&amp;Compress File</source>
      <translation>ファイルを圧縮(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Include Fonts</source>
      <translation>フォントを含める(&amp;I)</translation>
    </message>
    <message>
      <source>Encoding:</source>
      <translation>エンコーディング:</translation>
    </message>
    <message>
      <source>Show Preview</source>
      <translation>プレビューを表示</translation>
    </message>
    <message>
      <source>&amp;Include Color Profiles</source>
      <translation>カラープロファイルを含む(&amp;I)</translation>
    </message>
    <message>
      <source>Show a preview and information for the selected file</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Compress the Scribus document on save</source>
      <translation>保存時にScribusドキュメントを圧縮</translation>
    </message>
    <message>
      <source>Include fonts when collecting files for the document. Be sure to know and understand licensing information for any fonts you collect and possibly redistribute.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Include color profiles when collecting files for the document</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>DashEditor</name>
    <message>
      <source>Value:</source>
      <translation>値:</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>オフセット:</translation>
    </message>
  </context>
  <context>
    <name>DeferredTask</name>
    <message>
      <source>Cancelled by user</source>
      <translation>ユーザによってキャンセルされました</translation>
    </message>
  </context>
  <context>
    <name>DelColor</name>
    <message>
      <source>Delete Color</source>
      <translation>色を削除</translation>
    </message>
    <message>
      <source>Delete Color:</source>
      <translation>色を削除:</translation>
    </message>
    <message>
      <source>Replace With:</source>
      <translation>以下で置換:</translation>
    </message>
  </context>
  <context>
    <name>DelPages</name>
    <message>
      <source>Delete Pages</source>
      <translation>ページを削除</translation>
    </message>
    <message>
      <source>Delete From:</source>
      <translation>削除:</translation>
    </message>
    <message>
      <source>to:</source>
      <translation>to:</translation>
    </message>
  </context>
  <context>
    <name>DocIm</name>
    <message>
      <source>Importing failed</source>
      <translation>インポートに失敗しました</translation>
    </message>
    <message>
      <source>Importing Word document failed 
%1</source>
      <translation>Wordドキュメントのインポートに失敗しました
%1</translation>
    </message>
  </context>
  <context>
    <name>DocInfos</name>
    <message>
      <source>Document Information</source>
      <translation>ドキュメント情報</translation>
    </message>
    <message>
      <source>&amp;Title:</source>
      <translation>タイトル(&amp;T):</translation>
    </message>
    <message>
      <source>&amp;Author:</source>
      <translation>作者(&amp;A):</translation>
    </message>
    <message>
      <source>&amp;Keywords:</source>
      <translation>キーワード(&amp;K):</translation>
    </message>
    <message>
      <source>Descri&amp;ption:</source>
      <translation>詳細(&amp;P):</translation>
    </message>
    <message>
      <source>P&amp;ublisher:</source>
      <translation>発行者(&amp;U):</translation>
    </message>
    <message>
      <source>&amp;Contributors:</source>
      <translation>寄稿者(&amp;C):</translation>
    </message>
    <message>
      <source>Dat&amp;e:</source>
      <translation>日付(&amp;E):</translation>
    </message>
    <message>
      <source>T&amp;ype:</source>
      <translation>タイプ(&amp;Y):</translation>
    </message>
    <message>
      <source>F&amp;ormat:</source>
      <translation>フォーマット(&amp;O):</translation>
    </message>
    <message>
      <source>Identi&amp;fier:</source>
      <translation>識別子(&amp;F):</translation>
    </message>
    <message>
      <source>&amp;Source:</source>
      <translation>ソース(&amp;S):</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>言語(&amp;L):</translation>
    </message>
    <message>
      <source>&amp;Relation:</source>
      <translation>関連(&amp;R):</translation>
    </message>
    <message>
      <source>Co&amp;verage:</source>
      <translation>範囲(&amp;V):</translation>
    </message>
    <message>
      <source>Ri&amp;ghts:</source>
      <translation>権利(&amp;G):</translation>
    </message>
    <message>
      <source>Documen&amp;t</source>
      <translation>ドキュメント(&amp;T)</translation>
    </message>
    <message>
      <source>Further &amp;Information</source>
      <translation>詳細情報(&amp;I)</translation>
    </message>
    <message>
      <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A person or organisation responsible for making the document available</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A person or organisation responsible for making contributions to the content of the document</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>DocSections</name>
    <message>
      <source>Document Sections</source>
      <translation>ドキュメントセクション</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>追加(&amp;A)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Shown</source>
      <translation>表示</translation>
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
      <translation>スタイル</translation>
    </message>
    <message>
      <source>Start</source>
      <translation>開始</translation>
    </message>
  </context>
  <context>
    <name>DocumentItemAttributes</name>
    <message>
      <source>Document Item Attributes</source>
      <translation>ドキュメントアイテム属性</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>追加(&amp;A)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>タイプ</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>値</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation>パラメータ</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation>関連</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation>関連先</translation>
    </message>
    <message>
      <source>Auto Add To</source>
      <translation>自動追加先</translation>
    </message>
  </context>
  <context>
    <name>EPSPlug</name>
    <message>
      <source>Importing File:
%1
failed!</source>
      <translation>ファイルのインポート:
%1
に失敗しました!</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>致命的なエラー</translation>
    </message>
    <message>
      <source>Analyzing PostScript:</source>
      <translation>PostScriptを解析:</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>アイテムを生成</translation>
    </message>
    <message>
      <source>Converting of %1 images failed!</source>
      <translation>%1 画像の変換に失敗しました!</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>エラー</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
    <message>
      <source>Importing: %1</source>
      <translation>インポート中: %1</translation>
    </message>
  </context>
  <context>
    <name>EditToolBar</name>
    <message>
      <source>Edit</source>
      <translation>編集</translation>
    </message>
  </context>
  <context>
    <name>Editor</name>
    <message>
      <source>Editor</source>
      <translation>エディタ</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>新規(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Open...</source>
      <translation>開く(&amp;O)...</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>名前を付けて保存(&amp;A)...</translation>
    </message>
    <message>
      <source>&amp;Save and Exit</source>
      <translation>保存して終了(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Exit without Saving</source>
      <translation>保存せずに終了(&amp;E)</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>元に戻す(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>やり直し(&amp;R)</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>切り取り(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>貼り付け(&amp;P)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Get Field Names</source>
      <translation>フィールド名を取得(&amp;G)</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>編集(&amp;E)</translation>
    </message>
    <message>
      <source>JavaScripts (*.js);;All Files (*)</source>
      <translation>JavaScript (*.js);;全てのファイル (*)</translation>
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
      <translation>画像のエフェクト</translation>
    </message>
    <message>
      <source>Options:</source>
      <translation>オプション:</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>色:</translation>
    </message>
    <message>
      <source>Shade:</source>
      <translation>色の濃さ:</translation>
    </message>
    <message>
      <source>Brightness:</source>
      <translation>輝度:</translation>
    </message>
    <message>
      <source>Contrast:</source>
      <translation>コントラスト:</translation>
    </message>
    <message>
      <source>Radius:</source>
      <translation>半径:</translation>
    </message>
    <message>
      <source>Value:</source>
      <translation>値:</translation>
    </message>
    <message>
      <source>Posterize:</source>
      <translation>ポスタライズ:</translation>
    </message>
    <message>
      <source>Available Effects</source>
      <translation>利用可能なエフェクト</translation>
    </message>
    <message>
      <source>Blur</source>
      <translation>ブラー</translation>
    </message>
    <message>
      <source>Brightness</source>
      <translation>輝度</translation>
    </message>
    <message>
      <source>Colorize</source>
      <translation>色を付ける</translation>
    </message>
    <message>
      <source>Contrast</source>
      <translation>コントラスト</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>グレースケール</translation>
    </message>
    <message>
      <source>Invert</source>
      <translation>反転</translation>
    </message>
    <message>
      <source>Posterize</source>
      <translation>ポスタライズ</translation>
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
      <translation>エフェクトは使用中です</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Sharpen</source>
      <translation>強調</translation>
    </message>
    <message>
      <source>Color 1:</source>
      <translation>色 1:</translation>
    </message>
    <message>
      <source>Color 2:</source>
      <translation>色 2:</translation>
    </message>
    <message>
      <source>Color 3:</source>
      <translation>色3:</translation>
    </message>
    <message>
      <source>Color 4:</source>
      <translation>色4:</translation>
    </message>
    <message>
      <source>Duotone</source>
      <translation>2色網版</translation>
    </message>
    <message>
      <source>Tritone</source>
      <translation>3色網版</translation>
    </message>
    <message>
      <source>Quadtone</source>
      <translation>4トーン</translation>
    </message>
    <message>
      <source>Curves</source>
      <translation>曲線</translation>
    </message>
  </context>
  <context>
    <name>ExportBitmap</name>
    <message>
      <source>File exists. Overwrite?</source>
      <translation>ファイルは存在します。上書きしますか?</translation>
    </message>
    <message>
      <source>exists already. Overwrite?</source>
      <translation>すでに存在します。上書きしますか?</translation>
    </message>
    <message>
      <source>Save as Image</source>
      <translation>画像として保存</translation>
    </message>
    <message>
      <source>Insufficient memory for this image size.</source>
      <translation>この画像サイズを扱うにはメモリが不足しています。</translation>
    </message>
    <message>
      <source>Error writing the output file(s).</source>
      <translation>出力ファイルの書き込み中にエラー</translation>
    </message>
  </context>
  <context>
    <name>ExportForm</name>
    <message>
      <source>Choose a Export Directory</source>
      <translation>エクスポートするディレクトリを選択</translation>
    </message>
    <message>
      <source>Export as Image(s)</source>
      <translation>画像としてエクスポート</translation>
    </message>
    <message>
      <source>&amp;Export to Directory:</source>
      <translation>エクスポートするディレクトリ(&amp;E):</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>変更(&amp;H)...</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>Image &amp;Type:</source>
      <translation>画像タイプ(&amp;T):</translation>
    </message>
    <message>
      <source>&amp;Quality:</source>
      <translation>品質(&amp;Q):</translation>
    </message>
    <message>
      <source>&amp;Resolution:</source>
      <translation>解像度(&amp;R):</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>サイズ(&amp;S):</translation>
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
      <translation>範囲</translation>
    </message>
    <message>
      <source>&amp;Current page</source>
      <translation>現在のページ(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;All pages</source>
      <translation>全てのページ(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Range</source>
      <translation>範囲(&amp;R)</translation>
    </message>
    <message>
      <source>Export a range of pages</source>
      <translation>指定範囲をエクスポート</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>カンマで区切られたトークンのリストを入力してください。
トークンは*で全てのページを、1-5のようにしてページ範囲を、
もしくは単一のページ番号が利用できます。</translation>
    </message>
    <message>
      <source>Export all pages</source>
      <translation>全てのページをエクスポート</translation>
    </message>
    <message>
      <source>Export only the current page</source>
      <translation>現在のページのみエクスポート</translation>
    </message>
    <message>
      <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
      <translation>画像の解像度 
画面向けの画像には72 dpiを使用してください</translation>
    </message>
    <message>
      <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
      <translation>画像のサイズ。100%では変化せず、200%では2倍大きくなります。</translation>
    </message>
    <message>
      <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
      <translation>画像の品質 - 100%が最高で、1%は最低の品質です</translation>
    </message>
    <message>
      <source>Available export formats</source>
      <translation>エクスポート可能なフォーマット</translation>
    </message>
    <message>
      <source>The output directory - the place to store your images.
Name of the export file will be 'documentname-pagenumber.filetype'</source>
      <translation>出力ディレクトリ - 画像を保存する場所です。
出力ファイル名は 'ドキュメント名-ページ番号.ファイルタイプ' になります。</translation>
    </message>
    <message>
      <source>Change the output directory</source>
      <translation>出力ディレクトリを変更</translation>
    </message>
    <message>
      <source>Image size in Pixels</source>
      <translation>ピクセルでの画像サイズ</translation>
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
      <translation>拡張画像のプロパティ</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>標準</translation>
    </message>
    <message>
      <source>Darken</source>
      <translation>暗くする</translation>
    </message>
    <message>
      <source>Lighten</source>
      <translation>明るくする</translation>
    </message>
    <message>
      <source>Hue</source>
      <translation>色合い</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>彩度</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Luminosity</source>
      <translation>輝度</translation>
    </message>
    <message>
      <source>Multiply</source>
      <translation>乗算</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>スクリーン</translation>
    </message>
    <message>
      <source>Dissolve</source>
      <translation>ディソルブ</translation>
    </message>
    <message>
      <source>Overlay</source>
      <translation>オーバーレイ</translation>
    </message>
    <message>
      <source>Hard Light</source>
      <translation>ハードライト</translation>
    </message>
    <message>
      <source>Soft Light</source>
      <translation>ソフトライト</translation>
    </message>
    <message>
      <source>Difference</source>
      <translation>差の絶対値</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>除外</translation>
    </message>
    <message>
      <source>Color Dodge</source>
      <translation>覆い焼きカラー</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>焼き込み</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>合成モード:</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>不透明度:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Layers</source>
      <translation>レイヤー</translation>
    </message>
    <message>
      <source>Don't use any Path</source>
      <translation>どのパスも使用しない</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>パス</translation>
    </message>
    <message>
      <source>Live Preview</source>
      <translation>ライブプレビュー</translation>
    </message>
  </context>
  <context>
    <name>FDialogPreview</name>
    <message>
      <source>Size:</source>
      <translation>サイズ:</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>解像度:</translation>
    </message>
    <message>
      <source>DPI</source>
      <translation>DPI</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>不明</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation>色空間:</translation>
    </message>
    <message>
      <source>Title:</source>
      <translation>タイトル:</translation>
    </message>
    <message>
      <source>No Title</source>
      <translation>タイトルなし</translation>
    </message>
    <message>
      <source>Author:</source>
      <translation>作者:</translation>
    </message>
    <message>
      <source>Scribus Document</source>
      <translation>Scribusドキュメント</translation>
    </message>
    <message>
      <source>File Format:</source>
      <translation>ファイルフォーマット:</translation>
    </message>
  </context>
  <context>
    <name>FileLoader</name>
    <message>
      <source>Some fonts used by this document have been substituted:</source>
      <translation>このドキュメントで使用されているいくつかのフォントは置換されました:</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation> は以下で置換されます: </translation>
    </message>
  </context>
  <context>
    <name>FileToolBar</name>
    <message>
      <source>File</source>
      <translation>ファイル</translation>
    </message>
  </context>
  <context>
    <name>FontComboH</name>
    <message>
      <source>Face:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Style:</source>
      <translation>スタイル:</translation>
    </message>
  </context>
  <context>
    <name>FontListModel</name>
    <message>
      <source>Font Name</source>
      <translation>フォント名</translation>
    </message>
    <message>
      <source>Use Font</source>
      <translation>フォントの使用</translation>
    </message>
    <message>
      <source>Family</source>
      <translation>ファミリー</translation>
    </message>
    <message>
      <source>Style</source>
      <translation>スタイル</translation>
    </message>
    <message>
      <source>Variant</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Type</source>
      <translation>タイプ</translation>
    </message>
    <message>
      <source>Format</source>
      <translation>フォーマット</translation>
    </message>
    <message>
      <source>Embed in PostScript</source>
      <translation>PostScriptに埋め込む</translation>
    </message>
    <message>
      <source>Subset</source>
      <translation>サブセット</translation>
    </message>
    <message>
      <source>Access</source>
      <translation>アクセス</translation>
    </message>
    <message>
      <source>Used in Doc</source>
      <translation>ドキュメントで使用</translation>
    </message>
    <message>
      <source>Path to Font File</source>
      <translation>フォントファイルのパス</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>font type</comment>
      <translation>不明</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>font format</comment>
      <translation>不明</translation>
    </message>
    <message>
      <source>User</source>
      <comment>font preview</comment>
      <translation>ユーザ</translation>
    </message>
    <message>
      <source>System</source>
      <comment>font preview</comment>
      <translation>システム</translation>
    </message>
    <message>
      <source>Click to change the value</source>
      <translation>クリックして値を変更してください</translation>
    </message>
  </context>
  <context>
    <name>FontPrefs</name>
    <message>
      <source>Available Fonts</source>
      <translation>利用可能なフォント</translation>
    </message>
    <message>
      <source>Font Substitutions</source>
      <translation>フォント置換</translation>
    </message>
    <message>
      <source>Additional Paths</source>
      <translation>追加パス</translation>
    </message>
    <message>
      <source>&amp;Available Fonts</source>
      <translation>利用可能なフォント(&amp;A)</translation>
    </message>
    <message>
      <source>Font Name</source>
      <translation>フォント名</translation>
    </message>
    <message>
      <source>Replacement</source>
      <translation>代替フォント</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Font &amp;Substitutions</source>
      <translation>フォント置換(&amp;S)</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>変更(&amp;H)...</translation>
    </message>
    <message>
      <source>A&amp;dd...</source>
      <translation>追加(&amp;D)...</translation>
    </message>
    <message>
      <source>&amp;Remove</source>
      <translation>削除(&amp;R)</translation>
    </message>
    <message>
      <source>Additional &amp;Paths</source>
      <translation>追加パス(&amp;P)</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>ディレクトリを選択</translation>
    </message>
    <message>
      <source>Font search paths can only be set in File > Preferences, and only when there is no document currently open. Close any open documents, then use File > Preferences > Fonts to change the font search path.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>FontPreview</name>
    <message>
      <source>&amp;Search</source>
      <translation>検索(&amp;A)</translation>
    </message>
    <message>
      <source>Fonts Preview</source>
      <translation>フォントプレビュー</translation>
    </message>
    <message>
      <source>&amp;Quick Search:</source>
      <translation>クイックサーチ(&amp;Q):</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>&amp;Font Size:</source>
      <translation>フォントサイズ(&amp;F):</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>テキスト</translation>
    </message>
    <message>
      <source>Sample text to display</source>
      <translation>表示するサンプルテキスト</translation>
    </message>
    <message>
      <source>Se&amp;t</source>
      <translation>設定(&amp;T)</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Reset the text</source>
      <translation>テキストをリセット</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Show Extended Font Informations</source>
      <translation>詳細なフォント情報を表示</translation>
    </message>
    <message>
      <source>&amp;Default</source>
      <translation>デフォルト(&amp;D)</translation>
    </message>
  </context>
  <context>
    <name>FontPreviewPlugin</name>
    <message>
      <source>&amp;Font Preview...</source>
      <translation>フォントプレビュー(&amp;F)...</translation>
    </message>
    <message>
      <source>Font Preview dialog</source>
      <translation>フォントプレビューダイアログ</translation>
    </message>
    <message>
      <source>Sorting, searching and browsing available fonts.</source>
      <translation>利用可能なフォントをソート、検索、ブラウズしています</translation>
    </message>
  </context>
  <context>
    <name>FontReplaceDialog</name>
    <message>
      <source>Font Substitution</source>
      <translation>フォント置換</translation>
    </message>
    <message>
      <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
      <translation>このドキュメントはシステムにインストールされていないフォントを含んでいるので、適切な代替フォントを選択してください。キャンセルすると、ドキュメントの読み込みを中止します。</translation>
    </message>
    <message>
      <source>Original Font</source>
      <translation>元のフォント</translation>
    </message>
    <message>
      <source>Substitution Font</source>
      <translation>代替フォント</translation>
    </message>
    <message>
      <source>Make these substitutions permanent</source>
      <translation>これらの置換を常に行う</translation>
    </message>
    <message>
      <source>Cancels these font substitutions and stops loading the document.</source>
      <translation>これらのフォント代替をキャンセルしてドキュメントの読み込みを止めます。</translation>
    </message>
    <message>
      <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit > Preferences > Fonts.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
      <translation>OKを選択すると保存され、これらの置換はドキュメントで今後ずっと使用されます。</translation>
    </message>
  </context>
  <context>
    <name>GradientEditor</name>
    <message>
      <source>Position:</source>
      <translation>位置:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Add, change or remove color stops here</source>
      <translation>ここにカラーストップを追加/変更/削除します</translation>
    </message>
  </context>
  <context>
    <name>GradientVectorDialog</name>
    <message>
      <source>Gradient Vector</source>
      <translation>グラディエントベクトル</translation>
    </message>
  </context>
  <context>
    <name>GuideManager</name>
    <message>
      <source>&amp;Add</source>
      <translation>追加(&amp;A)</translation>
    </message>
    <message>
      <source>D&amp;elete</source>
      <translation>削除(&amp;E)</translation>
    </message>
    <message>
      <source>A&amp;dd</source>
      <translation>追加(&amp;D)</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation>削除(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>ページ(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Lock Guides</source>
      <translation>ガイドをロック(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Single</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontals</source>
      <translation>水平線</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>Verticals</source>
      <translation>垂直線</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Appl&amp;y to All Pages</source>
      <translation>全てのページに適用(&amp;Y)</translation>
    </message>
    <message>
      <source>Alt+Y</source>
      <translation>Alt+Y</translation>
    </message>
    <message>
      <source>&amp;Column/Row</source>
      <translation>行/列(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Number:</source>
      <translation>番号(&amp;N):</translation>
    </message>
    <message>
      <source>U&amp;se Gap:</source>
      <translation>ギャップを使用(&amp;S):</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>Refer To</source>
      <translation>参照</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>M&amp;argins</source>
      <translation>マージン(&amp;A)</translation>
    </message>
    <message>
      <source>S&amp;election</source>
      <translation>選択(&amp;E):</translation>
    </message>
    <message>
      <source>Nu&amp;mber:</source>
      <translation>番号(&amp;M):</translation>
    </message>
    <message>
      <source>Use &amp;Gap:</source>
      <translation>ギャップを使用(&amp;G):</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>&amp;Misc</source>
      <translation>その他(&amp;M)</translation>
    </message>
    <message>
      <source>Delete all guides from the current page</source>
      <translation>現在のページから全てのガイドを削除</translation>
    </message>
    <message>
      <source>Delete Guides from Current &amp;Page</source>
      <translation>現在のページからガイドを削除(&amp;P)</translation>
    </message>
    <message>
      <source>Delete all guides from the current document</source>
      <translation>現在のドキュメントから全てのガイドを削除</translation>
    </message>
    <message>
      <source>Delete Guides from &amp;All Pages</source>
      <translation>全てのページからガイドを削除(&amp;A)</translation>
    </message>
    <message>
      <source>Guide Manager</source>
      <translation>ガイドマネージャ</translation>
    </message>
  </context>
  <context>
    <name>HelpBrowser</name>
    <message>
      <source>&amp;Contents</source>
      <translation>目次(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>検索(&amp;S)</translation>
    </message>
    <message>
      <source>Se&amp;arch</source>
      <translation>検索(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>新規(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Book&amp;marks</source>
      <translation>ブックマーク(&amp;M)</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>印刷(&amp;P)...</translation>
    </message>
    <message>
      <source>&amp;Find...</source>
      <translation>検索(&amp;F)...</translation>
    </message>
    <message>
      <source>D&amp;elete All</source>
      <translation>全て削除(&amp;E)</translation>
    </message>
    <message>
      <source>Scribus Help</source>
      <translation>Scribusヘルプ</translation>
    </message>
    <message>
      <source>Searching is case insensitive</source>
      <translation>大文字小文字を区別して検索</translation>
    </message>
    <message>
      <source>1</source>
      <translation>1</translation>
    </message>
    <message>
      <source>&amp;Exit</source>
      <translation>終了(&amp;E)</translation>
    </message>
    <message>
      <source>Find &amp;Next...</source>
      <translation>次を検索(&amp;N)...</translation>
    </message>
    <message>
      <source>Find &amp;Previous...</source>
      <translation>前を検索(&amp;P)...</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>追加(&amp;A)</translation>
    </message>
  </context>
  <context>
    <name>HyAsk</name>
    <message>
      <source>Possible Hyphenation</source>
      <translation>利用可能なハイフネーション</translation>
    </message>
    <message>
      <source>Accept</source>
      <translation>許可</translation>
    </message>
    <message>
      <source>Skip</source>
      <translation>スキップ</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Add to the
Exception List</source>
      <translation>例外リストに追加</translation>
    </message>
    <message>
      <source>Add to the
Ignore List</source>
      <translation>無視リストに追加</translation>
    </message>
  </context>
  <context>
    <name>HySettings</name>
    <message>
      <source>Ignore List</source>
      <translation>無視リスト</translation>
    </message>
    <message>
      <source>Add a new Entry</source>
      <translation>新規エントリを追加</translation>
    </message>
    <message>
      <source>Edit Entry</source>
      <translation>エントリを編集</translation>
    </message>
    <message>
      <source>Exception List</source>
      <translation>例外リスト</translation>
    </message>
  </context>
  <context>
    <name>ImageInfoDialog</name>
    <message>
      <source>Image Info</source>
      <translation>画像情報</translation>
    </message>
    <message>
      <source>General Info</source>
      <translation>全般情報</translation>
    </message>
    <message>
      <source>Date / Time:</source>
      <translation>日付 / 時刻:</translation>
    </message>
    <message>
      <source>Has Embedded Profile:</source>
      <translation>埋め込みプロファイルあり:</translation>
    </message>
    <message>
      <source>Profile Name:</source>
      <translation>プロファイル名:</translation>
    </message>
    <message>
      <source>Has Embedded Paths:</source>
      <translation>埋め込みパスあり:</translation>
    </message>
    <message>
      <source>Has Layers:</source>
      <translation>レイヤーあり:</translation>
    </message>
    <message>
      <source>EXIF Info</source>
      <translation>EXIF情報</translation>
    </message>
    <message>
      <source>Artist:</source>
      <translation>アーティスト:</translation>
    </message>
    <message>
      <source>Comment:</source>
      <translation>コメント:</translation>
    </message>
    <message>
      <source>User Comment:</source>
      <translation>ユーザコメント:</translation>
    </message>
    <message>
      <source>Camera Model:</source>
      <translation>カメラモデル:</translation>
    </message>
    <message>
      <source>Camera Manufacturer:</source>
      <translation>カメラ製造者:</translation>
    </message>
    <message>
      <source>Description:</source>
      <translation>詳細:</translation>
    </message>
    <message>
      <source>Copyright:</source>
      <translation>著作権:</translation>
    </message>
    <message>
      <source>Scanner Model:</source>
      <translation>スキャナモデル:</translation>
    </message>
    <message>
      <source>Scanner Manufacturer:</source>
      <translation>スキャナ製造者:</translation>
    </message>
    <message>
      <source>Exposure time</source>
      <translation>露光時間</translation>
    </message>
    <message>
      <source>Aperture:</source>
      <translation>鮮明に見える範囲:</translation>
    </message>
    <message>
      <source>ISO equiv.:</source>
      <translation>ISO感度:</translation>
    </message>
  </context>
  <context>
    <name>ImportAIPlugin</name>
    <message>
      <source>Import AI...</source>
      <translation>AIのインポート...</translation>
    </message>
    <message>
      <source>Imports Illustrator Files</source>
      <translation>Illustratorファイルのインポート</translation>
    </message>
    <message>
      <source>Imports most Illustrator files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>ファイルがインポートできません</translation>
    </message>
  </context>
  <context>
    <name>ImportPSPlugin</name>
    <message>
      <source>Import PostScript...</source>
      <translation>PostScriptのインポート...</translation>
    </message>
    <message>
      <source>Imports PostScript Files</source>
      <translation>PostScriptファイルのインポート</translation>
    </message>
    <message>
      <source>Imports most PostScript files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ImportXfigPlugin</name>
    <message>
      <source>Import Xfig...</source>
      <translation>Xfigのインポート...</translation>
    </message>
    <message>
      <source>Imports Xfig Files</source>
      <translation>Xfigファイルのインポート</translation>
    </message>
    <message>
      <source>Imports most Xfig files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>サポートされている全てのフォーマット</translation>
    </message>
  </context>
  <context>
    <name>Imposition</name>
    <message>
      <source>Portrait</source>
      <translation>縦方向</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>横方向</translation>
    </message>
  </context>
  <context>
    <name>ImpositionBase</name>
    <message>
      <source>Imposition</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Gri&amp;d</source>
      <translation>グリッド(&amp;D)</translation>
    </message>
    <message>
      <source>Copies</source>
      <translation>部</translation>
    </message>
    <message>
      <source>Do&amp;uble sided</source>
      <translation>両面(&amp;U)</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Front side</source>
      <translation>正面</translation>
    </message>
    <message>
      <source>Back side</source>
      <translation>裏面</translation>
    </message>
    <message>
      <source>&amp;Booklet</source>
      <translation>ブックレット(&amp;B)</translation>
    </message>
    <message>
      <source>Pages per sheet</source>
      <translation>シートあたりのページ数</translation>
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
      <translation>ページ</translation>
    </message>
    <message>
      <source>&lt;html>&lt;head>&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; />&lt;style type=&quot;text/css&quot;>
p, li { white-space: pre-wrap; }
&lt;/style>&lt;/head>&lt;body style=&quot; font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;&quot;>
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;>Separate pages with a comma, ranges with a hyphen, e.g. 1,4,9-11 to get pages 1,4,9,10,11.&lt;/p>&lt;/body>&lt;/html></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fold</source>
      <translation>折りたたみ</translation>
    </message>
    <message>
      <source>Front page from</source>
      <translation>正面ページの開始</translation>
    </message>
    <message>
      <source>Double sided</source>
      <translation>両面</translation>
    </message>
    <message>
      <source>Back page from</source>
      <translation>背面ページの開始</translation>
    </message>
    <message>
      <source>Destination page</source>
      <translation>目的ページ</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>サイズ</translation>
    </message>
    <message>
      <source>Orientation</source>
      <translation>方向</translation>
    </message>
    <message>
      <source>Width</source>
      <translation>幅</translation>
    </message>
    <message>
      <source>Height</source>
      <translation>高さ</translation>
    </message>
    <message>
      <source>Preview</source>
      <translation>プレビュー</translation>
    </message>
    <message>
      <source>Refresh preview</source>
      <translation>プレビューを更新</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imposition dialog</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imposition on grids, booklets and folds</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>InsPage</name>
    <message>
      <source>Insert Page</source>
      <translation>ページを挿入</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>挿入(&amp;I)</translation>
    </message>
    <message>
      <source>Page(s)</source>
      <translation>ページ</translation>
    </message>
    <message>
      <source>before Page</source>
      <translation>ページの前に</translation>
    </message>
    <message>
      <source>after Page</source>
      <translation>ページの後に</translation>
    </message>
    <message>
      <source>at End</source>
      <translation>最後に</translation>
    </message>
    <message>
      <source>Master Pages</source>
      <translation>マスターページ</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>マスターページ(&amp;M):</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>ページサイズ</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>サイズ(&amp;S):</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>方向(&amp;N):</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>縦方向</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>横方向</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>幅(&amp;W):</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>高さ(&amp;H):</translation>
    </message>
    <message>
      <source>Move Objects with their Page</source>
      <translation>ページとともにオブジェクトを移動</translation>
    </message>
  </context>
  <context>
    <name>InsertAFrame</name>
    <message>
      <source>Insert A Frame</source>
      <translation>フレームを挿入</translation>
    </message>
    <message>
      <source>T&amp;ype</source>
      <translation>型(&amp;Y)</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation>テキストフレーム(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Image Frame</source>
      <translation>画像フレーム(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Location</source>
      <translation>場所(&amp;L)</translation>
    </message>
    <message>
      <source>Page Placement</source>
      <translation>ページ配置</translation>
    </message>
    <message>
      <source>Current Page</source>
      <translation>現在のページ</translation>
    </message>
    <message>
      <source>All Pages</source>
      <translation>全てのページ</translation>
    </message>
    <message>
      <source>...</source>
      <translation>...</translation>
    </message>
    <message>
      <source>Position of Frame</source>
      <translation>フレームの位置</translation>
    </message>
    <message>
      <source>Top Left of Margins</source>
      <translation>マージンの左上</translation>
    </message>
    <message>
      <source>Top Left of Page</source>
      <translation>ページの左上</translation>
    </message>
    <message>
      <source>Top Left of Bleed</source>
      <translation>断ち切りの左上</translation>
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
      <translation>サイズ(&amp;S)</translation>
    </message>
    <message>
      <source>Same as the Page Margins</source>
      <translation>ページマージンと同じ</translation>
    </message>
    <message>
      <source>Same as the Page</source>
      <translation>ページと同じ</translation>
    </message>
    <message>
      <source>Same as the Bleed</source>
      <translation>断ち切りと同じ</translation>
    </message>
    <message>
      <source>Same as the Imported Image</source>
      <translation>インポートされた画像と同じ</translation>
    </message>
    <message>
      <source>Height:</source>
      <translation>高さ:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>幅:</translation>
    </message>
    <message>
      <source>&amp;Options</source>
      <translation>オプション(&amp;O)</translation>
    </message>
    <message>
      <source>Source Image:</source>
      <translation>ソースイメージ:</translation>
    </message>
    <message>
      <source>There are no options for this type of frame</source>
      <translation>この種類のフレームにはオプションがありません</translation>
    </message>
    <message>
      <source>Columns:</source>
      <translation>列:</translation>
    </message>
    <message>
      <source>Gap:</source>
      <translation>ギャップ:</translation>
    </message>
    <message>
      <source>Link Created Frames</source>
      <translation>作成したフレームを繋ぐ</translation>
    </message>
    <message>
      <source>Source Document:</source>
      <translation>ソースドキュメント:</translation>
    </message>
    <message>
      <source>Range of Pages</source>
      <translation>ページ範囲</translation>
    </message>
    <message>
      <source>Custom Position</source>
      <translation>カスタム位置</translation>
    </message>
    <message>
      <source>Custom Size</source>
      <translation>カスタムサイズ</translation>
    </message>
    <message>
      <source>Link to Existing Frame</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>InsertTable</name>
    <message>
      <source>Insert Table</source>
      <translation>テーブルを挿入</translation>
    </message>
    <message>
      <source>Number of rows:</source>
      <translation>行数:</translation>
    </message>
    <message>
      <source>Number of columns:</source>
      <translation>列数:</translation>
    </message>
  </context>
  <context>
    <name>JavaDocs</name>
    <message>
      <source>Edit JavaScripts</source>
      <translation>JavaScriptを編集</translation>
    </message>
    <message>
      <source>&amp;Edit...</source>
      <translation>編集(&amp;E)...</translation>
    </message>
    <message>
      <source>&amp;Add...</source>
      <translation>追加(&amp;A)...</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
      <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;New Script:</source>
      <translation>新規スクリプト(&amp;N):</translation>
    </message>
    <message>
      <source>New Script</source>
      <translation>新規スクリプト</translation>
    </message>
    <message>
      <source>Do you really want to delete this script?</source>
      <translation>本当にこのスクリプトを削除しますか?</translation>
    </message>
  </context>
  <context>
    <name>LatexEditor</name>
    <message>
      <source>Enter Code:</source>
      <translation>コード入力:</translation>
    </message>
    <message>
      <source>Update</source>
      <translation>更新</translation>
    </message>
    <message>
      <source>Revert</source>
      <translation>戻る</translation>
    </message>
    <message>
      <source>Program Messages:</source>
      <translation>プログラムメッセージ:</translation>
    </message>
    <message>
      <source>Status: Unknown</source>
      <translation>ステータス: 不明</translation>
    </message>
    <message>
      <source>Kill Program</source>
      <translation>プログラムを強制停止する</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>解像度:</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>自動</translation>
    </message>
    <message>
      <source> DPI</source>
      <translation> DPI</translation>
    </message>
    <message>
      <source>Program:</source>
      <translation>プログラム: </translation>
    </message>
    <message>
      <source>Use Preamble</source>
      <translation>プリアンブルを使用</translation>
    </message>
    <message>
      <source>Editor</source>
      <translation>エディタ</translation>
    </message>
  </context>
  <context>
    <name>LayerPalette</name>
    <message>
      <source>Delete Layer</source>
      <translation>レイヤーを削除</translation>
    </message>
    <message>
      <source>Do you want to delete all objects on this layer too?</source>
      <translation>このレイヤー上の全てのオブジェクトも削除しますか?</translation>
    </message>
    <message>
      <source>Layers</source>
      <translation>レイヤー</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Add a new layer</source>
      <translation>新規レイヤーを追加</translation>
    </message>
    <message>
      <source>Delete layer</source>
      <translation>レイヤーを削除</translation>
    </message>
    <message>
      <source>Raise layer</source>
      <translation>レイヤーを上に</translation>
    </message>
    <message>
      <source>Lower layer</source>
      <translation>レイヤーを下に</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>不透明度:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>合成モード:</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>標準</translation>
    </message>
    <message>
      <source>Darken</source>
      <translation>暗くする</translation>
    </message>
    <message>
      <source>Lighten</source>
      <translation>明るくする</translation>
    </message>
    <message>
      <source>Multiply</source>
      <translation>乗算</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>スクリーン</translation>
    </message>
    <message>
      <source>Overlay</source>
      <translation>オーバーレイ</translation>
    </message>
    <message>
      <source>Hard Light</source>
      <translation>ハードライト</translation>
    </message>
    <message>
      <source>Soft Light</source>
      <translation>ソフトライト</translation>
    </message>
    <message>
      <source>Difference</source>
      <translation>差の絶対値</translation>
    </message>
    <message>
      <source>Color Dodge</source>
      <translation>覆い焼きカラー</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>焼き込み</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>除外</translation>
    </message>
    <message>
      <source>Hue</source>
      <translation>色合い</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>彩度</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Luminosity</source>
      <translation>輝度</translation>
    </message>
    <message>
      <source>Duplicates the current layer</source>
      <translation>現在のレイヤーを複製</translation>
    </message>
    <message>
      <source>Color of the Layer Indicator - Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color. </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make Layer Visible - Uncheck to hide the layer from the display </source>
      <translation>レイヤーを常に表示 - チェック解除するとレイヤーの表示を隠します</translation>
    </message>
    <message>
      <source>Print Layer - Uncheck to disable printing. </source>
      <translation>レイヤーを印刷 - チェック解除すると印刷を無効にします</translation>
    </message>
    <message>
      <source>Lock or Unlock Layer - Unchecked is unlocked </source>
      <translation>レイヤーをロック/ロック解除- チェック解除するとロック解除します</translation>
    </message>
    <message>
      <source>Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Outline Mode - Toggles the 'wireframe' display of objects to speed the display of very complex objects.</source>
      <translation>アウトラインモード - 非常に複雑なオブジェクトの表示を速くするために'ワイヤフレーム'表示を切替えます。</translation>
    </message>
    <message>
      <source>Name of the Layer - Double clicking on the name of a layer enabled editing</source>
      <translation>レイヤー名 - レイヤー名の上でダブルクリックすると編集できます</translation>
    </message>
  </context>
  <context>
    <name>LegacyMode</name>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>サポートされている全てのフォーマット</translation>
    </message>
  </context>
  <context>
    <name>LensDialogBase</name>
    <message>
      <source>Optical Lens</source>
      <translation>光学レンズ</translation>
    </message>
    <message>
      <source>Add Lens</source>
      <translation>レンズを追加</translation>
    </message>
    <message>
      <source>Remove Lens</source>
      <translation>レンズを削除</translation>
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
      <translation>レンズパラメータ</translation>
    </message>
    <message>
      <source>X Pos:</source>
      <translation>位置X:</translation>
    </message>
    <message>
      <source>Y Pos:</source>
      <translation>位置Y:</translation>
    </message>
    <message>
      <source>Radius:</source>
      <translation>半径:</translation>
    </message>
    <message>
      <source>Strength:</source>
      <translation>強さ:</translation>
    </message>
    <message>
      <source>Magnification Lens</source>
      <translation>拡大レンズ</translation>
    </message>
    <message>
      <source>Fish Eye Lens</source>
      <translation>魚眼レンズ</translation>
    </message>
  </context>
  <context>
    <name>LensEffectsPlugin</name>
    <message>
      <source>Lens Effects...</source>
      <translation>レンズ効果...</translation>
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Lens Effects</source>
      <translation>レンズ効果</translation>
    </message>
    <message>
      <source>Apply fancy lens effects</source>
      <translation>面白いレンズ効果を適用</translation>
    </message>
  </context>
  <context>
    <name>LoadSavePlugin</name>
    <message>
      <source>All Files (*)</source>
      <translation>全てのファイル (*)</translation>
    </message>
    <message>
      <source>No File Loader Plugins Found</source>
      <translation>ファイル読み込みプラグインが見つかりませんでした</translation>
    </message>
  </context>
  <context>
    <name>LoremManager</name>
    <message>
      <source>Select Lorem Ipsum</source>
      <translation>Lorem Ipsumを選択</translation>
    </message>
    <message>
      <source>Author:</source>
      <translation>作者:</translation>
    </message>
    <message>
      <source>Get More:</source>
      <translation>さらに取得:</translation>
    </message>
    <message>
      <source>XML File:</source>
      <translation>XMLファイル:</translation>
    </message>
    <message>
      <source>Lorem Ipsum</source>
      <translation>Lorem Ipsum</translation>
    </message>
    <message>
      <source>Paragraphs:</source>
      <translation>段落:</translation>
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
      <translation>標準のLorem Ipsum</translation>
    </message>
    <message>
      <source>Random Paragraphs</source>
      <translation>ランダムな段落</translation>
    </message>
    <message>
      <source>Number of paragraphs of selected sample text to insert</source>
      <translation>選択したサンプルテキストに挿入する段落数</translation>
    </message>
    <message>
      <source>List of languages available to insert sample text in</source>
      <translation>サンプルテキストに挿入可能な言語のリスト</translation>
    </message>
  </context>
  <context>
    <name>MarginDialog</name>
    <message>
      <source>Manage Page Properties</source>
      <translation>ページプロパティを管理</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>ページサイズ</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>サイズ(&amp;S):</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>方向(&amp;N):</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>縦方向</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>横方向</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>幅(&amp;W):</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>高さ(&amp;H):</translation>
    </message>
    <message>
      <source>Move Objects with their Page</source>
      <translation>ページとともにオブジェクトを移動</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>タイプ:</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>マージンガイド</translation>
    </message>
    <message>
      <source>Other Settings</source>
      <translation>他の設定</translation>
    </message>
    <message>
      <source>Master Page:</source>
      <translation>マスターページ:</translation>
    </message>
    <message>
      <source>Size of the inserted pages, either a standard or custom size.</source>
      <translation>挿入されたページサイズ:標準またはカスタムサイズ。</translation>
    </message>
    <message>
      <source>Orientation of the page(s) to be inserted</source>
      <translation>挿入するページ方向</translation>
    </message>
    <message>
      <source>Width of the page(s) to be inserted</source>
      <translation>挿入するページ幅</translation>
    </message>
    <message>
      <source>Height of the page(s) to be inserted</source>
      <translation>挿入するページ高さ</translation>
    </message>
    <message>
      <source>When inserting a new page between others, move objects with their current pages. This is the default action.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>MarginWidget</name>
    <message>
      <source>Preset Layouts:</source>
      <translation>プリセットレイアウト:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>下(&amp;B):</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>上(&amp;T):</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>右(&amp;R):</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>左(&amp;L):</translation>
    </message>
    <message>
      <source>Apply the margin changes to all existing pages in the document</source>
      <translation>ドキュメント中の全てのマスタページにマージンの変更を適用する</translation>
    </message>
    <message>
      <source>Distance between the top margin guide and the edge of the page</source>
      <translation>上のマージンガイドとページの端の間隔</translation>
    </message>
    <message>
      <source>Distance between the bottom margin guide and the edge of the page</source>
      <translation>下のマージンガイドとページの端の間隔</translation>
    </message>
    <message>
      <source>&amp;Inside:</source>
      <translation>内側(&amp;I):</translation>
    </message>
    <message>
      <source>O&amp;utside:</source>
      <translation>外側(&amp;U):</translation>
    </message>
    <message>
      <source>Printer Margins...</source>
      <translation>プリンタマージン...</translation>
    </message>
    <message>
      <source>Import the margins for the selected page size from the available printers.</source>
      <translation>利用可能なプリンタから、選択されたページサイズに合ったマージンを読み込みます</translation>
    </message>
    <message>
      <source>Apply settings to:</source>
      <translation>設定を適用:</translation>
    </message>
    <message>
      <source>All Document Pages</source>
      <translation>全てのドキュメントページ</translation>
    </message>
    <message>
      <source>All Master Pages</source>
      <translation>全てのマスターページ</translation>
    </message>
    <message>
      <source>Apply the margin changes to all existing master pages in the document</source>
      <translation>ドキュメント中の全てのマスタページにマージンの変更を適用します</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>マージンガイド</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>上:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>下:</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation>物理ページの上から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation>物理ページの下から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation>物理ページの左から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation>物理ページの右から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Bleeds</source>
      <translation>断ち切り</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation>内側:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>外側:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>左:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>右:</translation>
    </message>
    <message>
      <source>Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>左のマージンガイドとページの端の間隔。もし、両面、三つ折り、四つ折りを選択した場合、このマージンスペースは、結合の際に正しいマージンを実現するのに利用できます。</translation>
    </message>
    <message>
      <source>Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>右のマージンガイドとページの端の間隔。もし、両面、三つ折り、四つ折りを選択した場合、このマージンスペースは、結合の際に正しいマージンを実現するのに利用できます。</translation>
    </message>
  </context>
  <context>
    <name>MasterPagesPalette</name>
    <message>
      <source>Edit Master Pages</source>
      <translation>マスターページを編集</translation>
    </message>
    <message>
      <source>Duplicate the selected master page</source>
      <translation>選択されたマスターページを複製する</translation>
    </message>
    <message>
      <source>Delete the selected master page</source>
      <translation>選択されたマスターページを削除する</translation>
    </message>
    <message>
      <source>Add a new master page</source>
      <translation>新規マスターページを追加</translation>
    </message>
    <message>
      <source>Import master pages from another document</source>
      <translation>他のドキュメントからマスターページをインポート</translation>
    </message>
    <message>
      <source>Do you really want to delete this master page?</source>
      <translation>本当にこのマスターページを削除しますか?</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>名前(&amp;N):</translation>
    </message>
    <message>
      <source>New Master Page</source>
      <translation>新規マスターページ</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>名前:</translation>
    </message>
    <message>
      <source>New MasterPage</source>
      <translation>新規マスターページ</translation>
    </message>
    <message>
      <source>New Master Page %1</source>
      <translation>新規マスターページ %1</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>#%1のコピー</translation>
    </message>
    <message>
      <source>Unable to Rename Master Page</source>
      <translation>マスターページの名前を変更できません</translation>
    </message>
    <message>
      <source>The Normal page is not allowed to be renamed.</source>
      <translation>通常のページは名前を変更することができません。</translation>
    </message>
    <message>
      <source>Rename Master Page</source>
      <translation>マスターページの名前を変更</translation>
    </message>
    <message>
      <source>New Name:</source>
      <translation>新しい名前:</translation>
    </message>
    <message>
      <source>Copy #%1 of %2</source>
      <translation>%1を%2にコピー</translation>
    </message>
  </context>
  <context>
    <name>Measurements</name>
    <message>
      <source>Distances</source>
      <translation>距離</translation>
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
      <translation>距離X:</translation>
    </message>
    <message>
      <source>DY:</source>
      <translation>距離Y:</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation>角度:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation>長さ:</translation>
    </message>
    <message>
      <source> °</source>
      <translation> °</translation>
    </message>
  </context>
  <context>
    <name>MergeDoc</name>
    <message>
      <source>Import Master Page</source>
      <translation>マスターページをインポート</translation>
    </message>
    <message>
      <source>Import Page(s)</source>
      <translation>ページをインポート</translation>
    </message>
    <message>
      <source>&amp;From Document:</source>
      <translation>ドキュメントから(&amp;F):</translation>
    </message>
    <message>
      <source>Chan&amp;ge...</source>
      <translation>変更(&amp;G)...</translation>
    </message>
    <message>
      <source>&amp;Import Page(s):</source>
      <translation>ページをインポート(&amp;I):</translation>
    </message>
    <message>
      <source>&amp;Import Master Page</source>
      <translation>マスターページをインポート(&amp;I)</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
      <translation>カンマで区切られたトークンのリストを入力してください。
トークンは*で全てのページを、1-5のようにしてページ範囲を、
もしくは単一のページ番号が利用できます。</translation>
    </message>
    <message>
      <source> from 0</source>
      <translation>0 から</translation>
    </message>
    <message>
      <source>Create Page(s)</source>
      <translation>ページを作成</translation>
    </message>
    <message>
      <source>Before Page</source>
      <translation>ページの前に</translation>
    </message>
    <message>
      <source>After Page</source>
      <translation>ページの後に</translation>
    </message>
    <message>
      <source>At End</source>
      <translation>最後に</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>インポート(&amp;I)</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>ドキュメント (*.sla *.sla.gz *.scd *.scd.gz);;全てのファイル (*)</translation>
    </message>
    <message>
      <source> from %1</source>
      <translation>%1 から</translation>
    </message>
  </context>
  <context>
    <name>MeshDistortionDialog</name>
    <message>
      <source>Dialog</source>
      <translation>ダイアログ</translation>
    </message>
    <message>
      <source>+</source>
      <translation>+</translation>
    </message>
    <message>
      <source>-</source>
      <translation>-</translation>
    </message>
  </context>
  <context>
    <name>MeshDistortionPlugin</name>
    <message>
      <source>Mesh Distortion...</source>
      <translation>メッシュ歪み...</translation>
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Mesh Distortion of Polygons</source>
      <translation>ポリゴンのメッシュ歪み</translation>
    </message>
  </context>
  <context>
    <name>MissingFont</name>
    <message>
      <source>Missing Font</source>
      <translation>不明なフォント</translation>
    </message>
    <message>
      <source>The Font %1 is not installed.</source>
      <translation>フォント %1 はインストールされていません</translation>
    </message>
    <message>
      <source>Use</source>
      <translation>使用</translation>
    </message>
    <message>
      <source>instead</source>
      <translation>代替として</translation>
    </message>
  </context>
  <context>
    <name>ModeToolBar</name>
    <message>
      <source>Tools</source>
      <translation>ツール</translation>
    </message>
    <message>
      <source>Properties...</source>
      <translation>プロパティ...</translation>
    </message>
  </context>
  <context>
    <name>MovePages</name>
    <message>
      <source>Move Pages</source>
      <translation>ページを移動</translation>
    </message>
    <message>
      <source>Copy Page</source>
      <translation>ページをコピー</translation>
    </message>
    <message>
      <source>Move Page(s)</source>
      <translation>ページを移動</translation>
    </message>
    <message>
      <source>To:</source>
      <translation>To:</translation>
    </message>
    <message>
      <source>Number of copies:</source>
      <translation>部数:</translation>
    </message>
    <message>
      <source>Before Page</source>
      <translation>ページの前に</translation>
    </message>
    <message>
      <source>After Page</source>
      <translation>ページの後に</translation>
    </message>
    <message>
      <source>At End</source>
      <translation>最後に</translation>
    </message>
    <message>
      <source>Move Page(s):</source>
      <translation>ページを移動:</translation>
    </message>
  </context>
  <context>
    <name>MultiProgressDialog</name>
    <message>
      <source>Progress</source>
      <translation>進捗</translation>
    </message>
    <message>
      <source>Overall Progress:</source>
      <translation>全体の進捗:</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
    </message>
  </context>
  <context>
    <name>MultipleDuplicate</name>
    <message>
      <source>&amp;Horizontal Shift:</source>
      <translation>水平移動(&amp;H):</translation>
    </message>
    <message>
      <source>&amp;Vertical Shift:</source>
      <translation>垂直移動(&amp;V):</translation>
    </message>
    <message>
      <source>Multiple Duplicate</source>
      <translation>複数コピー</translation>
    </message>
    <message>
      <source>&amp;By Number of Copies</source>
      <translation>部数で(&amp;B)</translation>
    </message>
    <message>
      <source>&amp;Number of Copies:</source>
      <translation>部数(&amp;N):</translation>
    </message>
    <message>
      <source>&amp;Shift Created Items By</source>
      <translation>作成されたアイテムを移動(&amp;S)</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
    <message>
      <source>Create &amp;Gap Between Items Of</source>
      <translation>アイテム間のギャップを作成(&amp;G)</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Rotation:</source>
      <translation>回転:</translation>
    </message>
    <message>
      <source>By &amp;Rows &amp;&amp; Columns</source>
      <translation>行列で(&amp;R)</translation>
    </message>
    <message>
      <source>Vertical Gap:</source>
      <translation>垂直間隔:</translation>
    </message>
    <message>
      <source>Horizontal Gap:</source>
      <translation>水平間隔:</translation>
    </message>
    <message>
      <source>Number of Rows:</source>
      <translation>行数:</translation>
    </message>
    <message>
      <source>Number of Columns:</source>
      <translation>列数:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
    </message>
  </context>
  <context>
    <name>MyPlugin</name>
    <message>
      <source>My &amp;Plugin</source>
      <translation>自作プラグイン(&amp;P)</translation>
    </message>
  </context>
  <context>
    <name>MyPluginImpl</name>
    <message>
      <source>Scribus - My Plugin</source>
      <translation>Scribus - 自作プラグイン</translation>
    </message>
    <message>
      <source>The plugin worked!</source>
      <translation>プラグインを実行しました!</translation>
    </message>
  </context>
  <context>
    <name>NewDoc</name>
    <message>
      <source>New Document</source>
      <translation>新規ドキュメント</translation>
    </message>
    <message>
      <source>&amp;New Document</source>
      <translation>新規ドキュメント(&amp;N)</translation>
    </message>
    <message>
      <source>Open &amp;Existing Document</source>
      <translation>既存のドキュメントを開く(&amp;E)</translation>
    </message>
    <message>
      <source>Open Recent &amp;Document</source>
      <translation>最近のドキュメントを開く(&amp;D)</translation>
    </message>
    <message>
      <source>Do not show this dialog again</source>
      <translation>次回このダイアログを表示しない</translation>
    </message>
    <message>
      <source>Document page size, either a standard size or a custom size</source>
      <translation>ドキュメントのページサイズ。標準サイズとカスタムサイズの両方</translation>
    </message>
    <message>
      <source>Orientation of the document's pages</source>
      <translation>デフォルトのドキュメントページの方向</translation>
    </message>
    <message>
      <source>Width of the document's pages, editable if you have chosen a custom page size</source>
      <translation>ドキュメントのページ幅。カスタムページサイズを選べば編集が可能です</translation>
    </message>
    <message>
      <source>Height of the document's pages, editable if you have chosen a custom page size</source>
      <translation>ドキュメントのページの高さ。カスタムページサイズを選べば編集が可能です</translation>
    </message>
    <message>
      <source>Initial number of pages of the document</source>
      <translation>ドキュメントの初期のページ数</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>ドキュメントを編集する際のデフォルトの計測単位</translation>
    </message>
    <message>
      <source>Create text frames automatically when new pages are added</source>
      <translation>新規ページが追加された際にテキストフレームを自動的に作成します</translation>
    </message>
    <message>
      <source>Number of columns to create in automatically created text frames</source>
      <translation>自動的に生成されたテキストフレーム中に作成する列の数</translation>
    </message>
    <message>
      <source>Distance between automatically created columns</source>
      <translation>自動的に生成された列の間隔</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>サイズ(&amp;S):</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>方向(&amp;N):</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>縦方向</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>横方向</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>幅(&amp;W):</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>高さ(&amp;H):</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>マージンガイド</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>N&amp;umber of Pages:</source>
      <translation>総ページ数(&amp;U):</translation>
    </message>
    <message>
      <source>&amp;Default Unit:</source>
      <translation>デフォルトの単位(&amp;D):</translation>
    </message>
    <message>
      <source>&amp;Automatic Text Frames</source>
      <translation>自動テキストフレーム(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>ギャップ(&amp;G):</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>列数(&amp;M):</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>First Page is:</source>
      <translation>最初のページ:</translation>
    </message>
    <message>
      <source>Show Document Settings After Creation</source>
      <translation>作成後にドキュメント設定を表示</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation>ドキュメントレイアウト</translation>
    </message>
  </context>
  <context>
    <name>NewFromTemplatePlugin</name>
    <message>
      <source>New &amp;from Template...</source>
      <translation>テンプレートから新規作成(&amp;F)...</translation>
    </message>
    <message>
      <source>Load documents with predefined layout</source>
      <translation>あらかじめ定義されたレイアウトでドキュメントを読み込む</translation>
    </message>
    <message>
      <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>NodePalette</name>
    <message>
      <source>Nodes</source>
      <translation>ノード</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Absolute Coordinates</source>
      <translation>絶対座標(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>位置&amp;X:</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>位置&amp;Y:</translation>
    </message>
    <message>
      <source>Edit &amp;Contour Line</source>
      <translation>等高線を編集(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Reset Contour Line</source>
      <translation>等高線をリセット(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;End Editing</source>
      <translation>編集を終了(&amp;E)</translation>
    </message>
    <message>
      <source>Move Nodes</source>
      <translation>ノードを移動</translation>
    </message>
    <message>
      <source>Move Control Points</source>
      <translation>コントロールポイントを移動</translation>
    </message>
    <message>
      <source>Add Nodes</source>
      <translation>ノードを追加</translation>
    </message>
    <message>
      <source>Delete Nodes</source>
      <translation>ノードを削除</translation>
    </message>
    <message>
      <source>Move Control Points Independently</source>
      <translation>コントロールポイントを無関係に移動</translation>
    </message>
    <message>
      <source>Move Control Points Symmetrical</source>
      <translation>コントロールポイントを対称的に移動</translation>
    </message>
    <message>
      <source>Reset Control Points</source>
      <translation>コントロールポイントをリセット</translation>
    </message>
    <message>
      <source>Reset this Control Point</source>
      <translation>このコントロールポイントをリセット</translation>
    </message>
    <message>
      <source>Open a Polygon or Cuts a Bezier Curve</source>
      <translation>多角形を開くかベジエ曲線を切る</translation>
    </message>
    <message>
      <source>Close this Bezier Curve</source>
      <translation>このベジエ曲線を閉じる</translation>
    </message>
    <message>
      <source>Mirror the Path Horizontally</source>
      <translation>パスを水平方向にコピーする</translation>
    </message>
    <message>
      <source>Mirror the Path Vertically</source>
      <translation>パスを垂直方向にコピーする</translation>
    </message>
    <message>
      <source>Shear the Path Horizontally to the Right</source>
      <translation>パスを水平に右にずらす</translation>
    </message>
    <message>
      <source>Shear the Path Horizontally to the Left</source>
      <translation>パスを水平に左にずらす</translation>
    </message>
    <message>
      <source>Shear the Path Vertically Up</source>
      <translation>パスを垂直に上にずらす</translation>
    </message>
    <message>
      <source>Shear the Path Vertically Down</source>
      <translation>パスを垂直に下にずらす</translation>
    </message>
    <message>
      <source>Rotate the Path Counter-Clockwise</source>
      <translation>パスを反時計回りに回転</translation>
    </message>
    <message>
      <source>Rotate the Path Clockwise</source>
      <translation>パスを時計回りに回転</translation>
    </message>
    <message>
      <source>Enlarge the Size of the Path by shown %</source>
      <translation>表示された%だけパスのサイズを大きくする</translation>
    </message>
    <message>
      <source>Angle of Rotation</source>
      <translation>回転角</translation>
    </message>
    <message>
      <source>Activate Contour Line Editing Mode</source>
      <translation>等高線編集モードを有効に</translation>
    </message>
    <message>
      <source>Reset the Contour Line to the Original Shape of the Frame</source>
      <translation>フレームの本来の形状に等高線をリセット</translation>
    </message>
    <message>
      <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
      <translation>チェックするとページに相対する座標を使用します。
そうでなければ、オブジェクトに相対する座標を使用します。</translation>
    </message>
    <message>
      <source>Shrink the Size of the Path by shown %</source>
      <translation>表示された%だけパスのサイズを小さくする</translation>
    </message>
    <message>
      <source>Reduce the Size of the Path by the shown value</source>
      <translation>表示された値だけパスのサイズを小さくする</translation>
    </message>
    <message>
      <source>Enlarge the Size of the Path by the shown value</source>
      <translation>表示された値だけパスのサイズを大きくする</translation>
    </message>
    <message>
      <source>% to Enlarge or Shrink By</source>
      <translation>拡大または縮小する%</translation>
    </message>
    <message>
      <source>Value to Enlarge or Shrink By</source>
      <translation>拡大または縮小する値</translation>
    </message>
    <message>
      <source>Set Contour to Image Clip</source>
      <translation>画像クリップに等高線を設定</translation>
    </message>
    <message>
      <source>Reset the Contour Line to the Clipping Path of the Image</source>
      <translation>画像のクリッピングパスの等高線をリセット</translation>
    </message>
    <message>
      <source>to Canvas</source>
      <translation>キャンバスへ</translation>
    </message>
    <message>
      <source>to Page</source>
      <translation>ページへ</translation>
    </message>
  </context>
  <context>
    <name>OODPlug</name>
    <message>
      <source>This document does not seem to be an OpenOffice Draw file.</source>
      <translation>このドキュメントはOpenOffice Drawファイルではないようです。</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
  </context>
  <context>
    <name>OODrawImportPlugin</name>
    <message>
      <source>Import &amp;OpenOffice.org Draw...</source>
      <translation>OpenOffice.org Drawをインポート(&amp;O)...</translation>
    </message>
    <message>
      <source>Imports OpenOffice.org Draw Files</source>
      <translation>OpenOffice.org Drawファイルをインポート</translation>
    </message>
    <message>
      <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
      <translation>現在のドキュメントに、ベクトルデータをScribusオブジェクトに変換して、ほとんどのOpenOffice.org Drawファイルをインポートします。</translation>
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
      <translation>このファイルはサポートされていない機能を含んでいます</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>ファイルがインポートできません</translation>
    </message>
  </context>
  <context>
    <name>OdtDialog</name>
    <message>
      <source>OpenDocument Importer Options</source>
      <translation>OpenDocument インポータオプション</translation>
    </message>
    <message>
      <source>Overwrite Paragraph Styles</source>
      <translation>段落スタイルを上書き</translation>
    </message>
    <message>
      <source>Enabling this will overwrite existing styles in the current Scribus document</source>
      <translation>これを有効にすると、現在のScribusドキュメント中の既存のスタイルを上書きします</translation>
    </message>
    <message>
      <source>Merge Paragraph Styles</source>
      <translation>段落スタイルをマージ</translation>
    </message>
    <message>
      <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use document name as a prefix for paragraph styles</source>
      <translation>段落スタイルをドキュメント名のプレフィックスとして使用する</translation>
    </message>
    <message>
      <source>Prepend the document name to the paragraph style name in Scribus.</source>
      <translation>段落スタイル名の先頭にドキュメント名を追加します。</translation>
    </message>
    <message>
      <source>Do not ask again</source>
      <translation>次回は尋ねない</translation>
    </message>
    <message>
      <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
      <translation>これらの設定をデフォルトにして、OASISオープンドキュメントをインポートする際に再度尋ねません。</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
  </context>
  <context>
    <name>OldScribusFormat</name>
    <message>
      <source>Scribus Document</source>
      <translation>Scribusドキュメント</translation>
    </message>
    <message>
      <source>Scribus 1.2.x Document</source>
      <translation>Scribus 1.2.x ドキュメント</translation>
    </message>
  </context>
  <context>
    <name>OneClick</name>
    <message>
      <source>Origin</source>
      <translation>始点</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>サイズ</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>幅:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation>長さ:</translation>
    </message>
    <message>
      <source>Height:</source>
      <translation>高さ:</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation>角度:</translation>
    </message>
    <message>
      <source>Remember Values</source>
      <translation>値を記憶する</translation>
    </message>
  </context>
  <context>
    <name>OutlinePalette</name>
    <message>
      <source>Element</source>
      <translation>要素</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</source>
      <translation>名前 &quot;%1&quot; はすでに存在します。&lt;br/>他の名前を選んでください。</translation>
    </message>
    <message>
      <source>Group </source>
      <translation>グループ </translation>
    </message>
    <message>
      <source>Page </source>
      <translation>ページ </translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation>フリーオブジェクト</translation>
    </message>
    <message>
      <source>Outline</source>
      <translation>アウトライン</translation>
    </message>
    <message>
      <source>Enter a keyword or regular expression to filter the outline.</source>
      <translation>アウトラインをフィルタするためのキーワードまたは正規表現を入力します。</translation>
    </message>
    <message>
      <source>Ctrl+f</source>
      <comment>Filter the Outline using a keyword</comment>
      <translation>Ctrl+f</translation>
    </message>
    <message>
      <source>Filter:</source>
      <translation>フィルタ:</translation>
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
      <translation>線幅</translation>
    </message>
  </context>
  <context>
    <name>PDFExportDialog</name>
    <message>
      <source>Save as PDF</source>
      <translation>PDF形式で保存</translation>
    </message>
    <message>
      <source>O&amp;utput to File:</source>
      <translation>ファイルに出力(&amp;U):</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>変更(&amp;N)...</translation>
    </message>
    <message>
      <source>Output one file for eac&amp;h page</source>
      <translation>複数ページを一つのファイルに出力(&amp;H)</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>保存(&amp;S)</translation>
    </message>
    <message>
      <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
      <translation>これを有効にすると、ドキュメント中のページごとにそれぞれ名前のついたPDFファイルを出力します。ページ番号が自動的に追加されます。これは、商業印刷でPDFを印象的にするのに最も有効です。</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDFファイル (*.pdf);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
      <translation>PDF/X-3タブから、PDF/X-3をエクスポートする際に情報が見つからない場合、保存ボタンが無効になります。</translation>
    </message>
    <message>
      <source>%1 does not exists and will be created, continue?</source>
      <translation>%1 は存在しないので作成されます。続けますか?</translation>
    </message>
    <message>
      <source>Cannot create directory: 
%1</source>
      <translation>ディレクトリを作成できません: 
%1</translation>
    </message>
  </context>
  <context>
    <name>PDFLibCore</name>
    <message>
      <source>Saving PDF</source>
      <translation>PDFを保存中</translation>
    </message>
    <message>
      <source>Exporting Master Page:</source>
      <translation>マスターページをエクスポート中:</translation>
    </message>
    <message>
      <source>Exporting Page:</source>
      <translation>ページをエクスポート中:</translation>
    </message>
    <message>
      <source>Exporting Items on Current Page:</source>
      <translation>現在のページのアイテムをエクスポート中:</translation>
    </message>
    <message>
      <source>Page:</source>
      <translation>ページ:</translation>
    </message>
    <message>
      <source>Date:</source>
      <translation>日付:</translation>
    </message>
    <message>
      <source>Failed to load an image : %1</source>
      <translation>画像の読み込みに失敗しました : %1</translation>
    </message>
    <message>
      <source>Failed to write an image : %1</source>
      <translation>画像の書き込みに失敗しました : %1</translation>
    </message>
    <message>
      <source>Failed to load an image mask : %1</source>
      <translation>画像マスクの読み込みに失敗しました : %1</translation>
    </message>
    <message>
      <source>Insufficient memory for processing an image</source>
      <translation>画像の処理に必要なメモリが足りません</translation>
    </message>
    <message>
      <source>A write error occured, please check available disk space</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PDFToolBar</name>
    <message>
      <source>PDF Tools</source>
      <translation>PDFツール</translation>
    </message>
  </context>
  <context>
    <name>PPreview</name>
    <message>
      <source>Print Preview</source>
      <translation>印刷プレビュー</translation>
    </message>
    <message>
      <source>Display Trans&amp;parency</source>
      <translation>透明度を表示(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;Under Color Removal</source>
      <translation>下色除去(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Display CMYK</source>
      <translation>CMYKを表示(&amp;D)</translation>
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
      <source>Separation Name</source>
      <translation>分解名</translation>
    </message>
    <message>
      <source>Cyan</source>
      <translation>シアン</translation>
    </message>
    <message>
      <source>Magenta</source>
      <translation>マゼンダ</translation>
    </message>
    <message>
      <source>Yellow</source>
      <translation>イエロー</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>ブラック</translation>
    </message>
    <message>
      <source>Scaling:</source>
      <translation>倍率:</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>閉じる</translation>
    </message>
    <message>
      <source>Print...</source>
      <translation>印刷...</translation>
    </message>
    <message>
      <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
      <translation>ドキュメント中で透明度や透明アイテムを表示します。Ghostscript 7.07もしくはそれ以降が必要です</translation>
    </message>
    <message>
      <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
      <translation>RGBカラーの代わりにCMYKインクのシミュレーションで印刷プレビューする</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Resize the scale of the page.</source>
      <translation>ページの倍率をサイズ変更する</translation>
    </message>
    <message>
      <source>Enable/disable the C (Cyan) ink plate</source>
      <translation>C(シアン)インクの有効/無効</translation>
    </message>
    <message>
      <source>Enable/disable the M (Magenta) ink plate</source>
      <translation>M(マゼンダ)インクの有効/無効</translation>
    </message>
    <message>
      <source>Enable/disable the Y (Yellow) ink plate</source>
      <translation>Y(イエロー)インクの有効/無効</translation>
    </message>
    <message>
      <source>Enable/disable the K (Black) ink plate</source>
      <translation>K(ブラック)インクの有効/無効</translation>
    </message>
    <message>
      <source>All</source>
      <translation>全て</translation>
    </message>
    <message>
      <source>File</source>
      <translation>ファイル</translation>
    </message>
    <message>
      <source>Enable &amp;Antialiasing</source>
      <translation>アンチエイリアスを有効に(&amp;A)</translation>
    </message>
    <message>
      <source>Fit to Width</source>
      <translation>幅に合わせる</translation>
    </message>
    <message>
      <source>Fit to Height</source>
      <translation>高さに合わせる</translation>
    </message>
    <message>
      <source>Fit to Page</source>
      <translation>ページに合わせる</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
      <translation>プレビューが少し遅くなりますが、プレビュー中でType1フォント、TrueTypeフォント、OpenTypeフォント、EPS、PDF、ベクトルグラフィックを使った、より見やすいビューになります。</translation>
    </message>
    <message>
      <source>Display Settings</source>
      <translation>表示設定</translation>
    </message>
    <message>
      <source>Print Settings</source>
      <translation>印刷の設定</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>ページを水平方向に反転</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>ページを垂直方向に反転</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>ページマージンで切り取る</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>グレースケールで印刷</translation>
    </message>
    <message>
      <source>Convert Spot Colors</source>
      <translation>スポットカラーを変換</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>スポットカラーが合成色に変換されるのを有効にします。商業プリンタでスポットカラーを印刷するつもりでなければ、おそらく有効のままにしておくのが最適でしょう。</translation>
    </message>
    <message>
      <source>Apply Color Profiles</source>
      <translation>カラープロファイルを適用</translation>
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation>カラーマネージメントが有効の場合は、印刷ストリームにICCプロファイルを埋め込むようにする</translation>
    </message>
  </context>
  <context>
    <name>PSLib</name>
    <message>
      <source>Processing Master Page:</source>
      <translation>マスタページを処理中:</translation>
    </message>
    <message>
      <source>Exporting Page:</source>
      <translation>ページをエクスポート中:</translation>
    </message>
    <message>
      <source>Failed to write data for an image</source>
      <translation>画像のデータ書き込みに失敗しました</translation>
    </message>
    <message>
      <source>Failed to load an image : %1</source>
      <translation>画像の読み込みに失敗しました : %1</translation>
    </message>
    <message>
      <source>Failed to load an image mask : %1</source>
      <translation>画像マスクの読み込みに失敗しました : %1</translation>
    </message>
    <message>
      <source>Insufficient memory for processing an image</source>
      <translation>画像の処理に必要なメモリが足りません</translation>
    </message>
  </context>
  <context>
    <name>PageItem</name>
    <message>
      <source>Image</source>
      <translation>画像</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>テキスト</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>線</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation>多角形</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>ポリライン</translation>
    </message>
    <message>
      <source>PathText</source>
      <translation>パステキスト</translation>
    </message>
    <message>
      <source>Copy of</source>
      <translation>コピー</translation>
    </message>
  </context>
  <context>
    <name>PageItemAttributes</name>
    <message>
      <source>&amp;Add</source>
      <translation>追加(&amp;A)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>タイプ</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>値</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation>パラメータ</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation>関連</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation>関連先</translation>
    </message>
    <message>
      <source>Attributes</source>
      <translation>属性</translation>
    </message>
  </context>
  <context>
    <name>PageItem_ImageFrame</name>
    <message>
      <source>Preview Settings</source>
      <translation>プレビュー設定</translation>
    </message>
    <message>
      <source>Image</source>
      <translation>画像</translation>
    </message>
  </context>
  <context>
    <name>PageItem_LatexFrame</name>
    <message>
      <source>Error</source>
      <translation>エラー</translation>
    </message>
    <message>
      <source>Running the external application failed!</source>
      <translation>外部アプリケーションの実行に失敗しました!</translation>
    </message>
    <message>
      <source>Could not create a temporary file to run the application!</source>
      <translation>アプリケーションを実行するのに必要な一時ファイルが、作成できませんでした。</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>情報</translation>
    </message>
    <message>
      <source>An editor for this frame is already running!</source>
      <translation>このフレームのエディタは既に起動しています!</translation>
    </message>
    <message>
      <source>Please specify an editor in the preferences!</source>
      <translation>設定でエディタを指定してください!</translation>
    </message>
    <message>
      <source>Could not create a temporary file to run the external editor!</source>
      <translation>外部エディタ実行のための一時ファイルを作成できませんでした!</translation>
    </message>
    <message>
      <source>Running the editor failed with exitcode %d!</source>
      <translation>エディタは終了コード %d で実行に失敗しました!</translation>
    </message>
    <message>
      <source>Running the editor &quot;%1&quot; failed!</source>
      <translation>エディタ &quot;\%1&quot; の実行に失敗しました!</translation>
    </message>
    <message>
      <source>Running the application &quot;%1&quot; failed!</source>
      <translation>アプリケーション &quot;%1&quot; の実行に失敗しました!</translation>
    </message>
    <message>
      <source>Running</source>
      <translation>実行中</translation>
    </message>
    <message>
      <source>The application &quot;%1&quot; failed to start!</source>
      <translation>アプリケーション &quot;%1&quot; は起動に失敗しました!</translation>
    </message>
    <message>
      <source>The application &quot;%1&quot; crashed!</source>
      <translation>アプリケーション &quot;%1&quot; はクラッシュしました!</translation>
    </message>
    <message>
      <source>Application</source>
      <translation>アプリケーション</translation>
    </message>
    <message>
      <source>DPI</source>
      <translation>DPI</translation>
    </message>
    <message>
      <source>State</source>
      <translation>状態</translation>
    </message>
    <message>
      <source>Finished</source>
      <translation>終了</translation>
    </message>
    <message>
      <source>Render</source>
      <translation>描画</translation>
    </message>
    <message>
      <source>The config file didn't specify a executable path!</source>
      <translation>設定ファイルが実行パスで特定できません!</translation>
    </message>
    <message>
      <source>Render Frame</source>
      <translation>フレームを描画</translation>
    </message>
  </context>
  <context>
    <name>PageItem_PathText</name>
    <message>
      <source>Paragraphs: </source>
      <translation>段落数: </translation>
    </message>
    <message>
      <source>Lines: </source>
      <translation>行数: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>単語数: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>文字数: </translation>
    </message>
  </context>
  <context>
    <name>PageItem_TextFrame</name>
    <message>
      <source>Linked Text</source>
      <translation>リンクされたテキスト</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation>テキストフレーム</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>段落数: </translation>
    </message>
    <message>
      <source>Lines: </source>
      <translation>行数: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>単語数: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>文字数: </translation>
    </message>
  </context>
  <context>
    <name>PageLayouts</name>
    <message>
      <source>First Page is:</source>
      <translation>最初のページ:</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation>ドキュメントレイアウト</translation>
    </message>
  </context>
  <context>
    <name>PagePalette</name>
    <message>
      <source>Arrange Pages</source>
      <translation>ページの配置</translation>
    </message>
    <message>
      <source>Available Master Pages:</source>
      <translation>利用できるマスターページ:</translation>
    </message>
    <message>
      <source>Document Pages:</source>
      <translation>ドキュメントページ:</translation>
    </message>
    <message>
      <source>List of normal pages in the document, shown with the document layout. Pages may be dragged to rearrange or delete them.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Drag pages or master pages onto the trash to delete them</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>List of master pages in the document. Master page names may be dragged onto the page view below to apply master pages, or onto the empty space between pages to create new pages.</source>
      <translation type="unfinished" />
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
      <translation>スタイルなし</translation>
    </message>
  </context>
  <context>
    <name>PathAlongPathPlugin</name>
    <message>
      <source>Path Along Path...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Bends a Polygon along a Polyline</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This plugin bends a Polygon with the help of a Polyline.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathCutPlugin</name>
    <message>
      <source>Cut Polygon</source>
      <translation>多角形をカット</translation>
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Cuts a Polygon by a Polyline</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Qt Version too old</source>
      <translation>Qtバージョンが古すぎます</translation>
    </message>
    <message>
      <source>This plugin requires at least version 4.3.3 of the Qt library</source>
      <translation>このプラグインはQtライブラリのバージョン4.3.3以上が必要です</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>エラー</translation>
    </message>
    <message>
      <source>The cutting line must cross the polygon and
both end points must lie outside of the polygon</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathDialogBase</name>
    <message>
      <source>Path along Path</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Effect Type</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Single</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Single, stretched</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Repeated</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Repeated, stretched</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal Offset</source>
      <translation>水平オフセット</translation>
    </message>
    <message>
      <source>Vertical Offset</source>
      <translation>垂直オフセット</translation>
    </message>
    <message>
      <source>Gap between Objects</source>
      <translation>オブジェクト間のギャップ</translation>
    </message>
    <message>
      <source>Preview on Canvas</source>
      <translation>キャンバスでプレビュー</translation>
    </message>
    <message>
      <source>Rotate Object by 90°</source>
      <translation>オブジェクトを90度回転</translation>
    </message>
  </context>
  <context>
    <name>PathFinderBase</name>
    <message>
      <source>Boolean Path Operations</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Keep a copy of the original Item after applying the operation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>keep</source>
      <translation>保持</translation>
    </message>
    <message>
      <source>+</source>
      <translation>+</translation>
    </message>
    <message>
      <source>=</source>
      <translation>=</translation>
    </message>
    <message>
      <source>Operation</source>
      <translation>操作</translation>
    </message>
    <message>
      <source>...</source>
      <translation>...</translation>
    </message>
    <message>
      <source>Swap Shapes</source>
      <translation>形状を交換</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>Result gets Color of:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>first Shape</source>
      <translation>第一形状</translation>
    </message>
    <message>
      <source>second Shape</source>
      <translation>第二形状</translation>
    </message>
  </context>
  <context>
    <name>PathFinderDialog</name>
    <message>
      <source>Result gets Color of:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Intersection gets Color of:</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathFinderPlugin</name>
    <message>
      <source>Path Operations...</source>
      <translation>パス操作...</translation>
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Path Operations</source>
      <translation>パス操作</translation>
    </message>
    <message>
      <source>Apply fancy boolean operations to paths.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Qt Version too old</source>
      <translation>Qtバージョンが古すぎます</translation>
    </message>
    <message>
      <source>This plugin requires at least version 4.3.3 of the Qt library</source>
      <translation>このプラグインはQtライブラリのバージョン4.3.3以上が必要です</translation>
    </message>
  </context>
  <context>
    <name>PathStrokerPlugin</name>
    <message>
      <source>Create Path from Stroke</source>
      <translation>ストロークからパスを生成</translation>
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Converts the stroke of a Path to a filled Path.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PatternDialog</name>
    <message>
      <source>Patterns</source>
      <translation>パターン</translation>
    </message>
    <message>
      <source>Load</source>
      <translation>読み込み</translation>
    </message>
    <message>
      <source>Load Set</source>
      <translation>セットの読み込み</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>Remove All</source>
      <translation>全て削除</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation>名前を変更</translation>
    </message>
  </context>
  <context>
    <name>PicSearch</name>
    <message>
      <source>Result</source>
      <translation>結果</translation>
    </message>
    <message>
      <source>Search Results for: </source>
      <translation>検索結果: </translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>&amp;Preview</source>
      <translation>プレビュー(&amp;P)</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>&amp;Select</source>
      <translation>選択(&amp;S)</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation>Alt+S</translation>
    </message>
  </context>
  <context>
    <name>PicSearchOptions</name>
    <message>
      <source>Start Search</source>
      <translation>検索を開始</translation>
    </message>
    <message>
      <source>Search Images</source>
      <translation>画像検索</translation>
    </message>
    <message>
      <source>Search for:</source>
      <translation>以下で検索:</translation>
    </message>
    <message>
      <source>Start at:</source>
      <translation>以下で始まる:</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>変更...</translation>
    </message>
    <message>
      <source>Searching</source>
      <translation>検索中</translation>
    </message>
    <message>
      <source>Case insensitive search</source>
      <translation>大文字小文字を区別しないで検索</translation>
    </message>
    <message>
      <source>Search recursively</source>
      <translation>再帰的に検索</translation>
    </message>
  </context>
  <context>
    <name>PicStatus</name>
    <message>
      <source>Close</source>
      <translation>閉じる</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>情報</translation>
    </message>
    <message>
      <source>Path:</source>
      <translation>パス:</translation>
    </message>
    <message>
      <source>Search...</source>
      <translation>検索...</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>名前:</translation>
    </message>
    <message>
      <source>Image</source>
      <translation>画像</translation>
    </message>
    <message>
      <source>DPI:</source>
      <translation>DPI:</translation>
    </message>
    <message>
      <source>Format:</source>
      <translation>フォーマット:</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation>色空間:</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>サイズ</translation>
    </message>
    <message>
      <source>Pixels:</source>
      <translation>ピクセル:</translation>
    </message>
    <message>
      <source>Scale:</source>
      <translation>倍率:</translation>
    </message>
    <message>
      <source>Printed:</source>
      <translation>印刷:</translation>
    </message>
    <message>
      <source>Layout</source>
      <translation>レイアウト:</translation>
    </message>
    <message>
      <source>On Page:</source>
      <translation>ページ上:</translation>
    </message>
    <message>
      <source>Object:</source>
      <translation>オブジェクト:</translation>
    </message>
    <message>
      <source>Select</source>
      <translation>選択</translation>
    </message>
    <message>
      <source>Image Tools</source>
      <translation>画像ツール</translation>
    </message>
    <message>
      <source>Image Visible</source>
      <translation>画像を表示</translation>
    </message>
    <message>
      <source>Image Effects...</source>
      <translation>画像のエフェクト...</translation>
    </message>
    <message>
      <source>Edit Image...</source>
      <translation>画像を編集...</translation>
    </message>
    <message>
      <source>Print Image</source>
      <translation>画像を印刷</translation>
    </message>
    <message>
      <source>Extended Image Properties...</source>
      <translation>拡張画像のプロパティ...</translation>
    </message>
    <message>
      <source>Manage Images</source>
      <translation>画像を管理</translation>
    </message>
    <message>
      <source>Eff. DPI:</source>
      <translation>有効なDPI:</translation>
    </message>
    <message>
      <source>Go to</source>
      <translation>移動</translation>
    </message>
  </context>
  <context>
    <name>PixmapExportPlugin</name>
    <message>
      <source>Save as &amp;Image...</source>
      <translation>画像として保存(&amp;I)...</translation>
    </message>
    <message>
      <source>Export As Image</source>
      <translation>画像としてエクスポート</translation>
    </message>
    <message>
      <source>Exports selected pages as bitmap images.</source>
      <translation>選択されたページをビットマップ画像としてエクスポート</translation>
    </message>
    <message>
      <source>Export successful</source>
      <translation>エクスポートに成功</translation>
    </message>
  </context>
  <context>
    <name>PluginManager</name>
    <message>
      <source>Cannot find plugin</source>
      <comment>plugin manager</comment>
      <translation>プラグインが見つかりません</translation>
    </message>
    <message>
      <source>unknown error</source>
      <comment>plugin manager</comment>
      <translation>未知のエラー</translation>
    </message>
    <message>
      <source>Cannot find symbol (%1)</source>
      <comment>plugin manager</comment>
      <translation>シンボル (%1) が見つかりません</translation>
    </message>
    <message>
      <source>Plugin: loading %1</source>
      <comment>plugin manager</comment>
      <translation>プラグイン: %1 を読み込み中</translation>
    </message>
    <message>
      <source>init failed</source>
      <comment>plugin load error</comment>
      <translation>初期化に失敗しました</translation>
    </message>
    <message>
      <source>unknown plugin type</source>
      <comment>plugin load error</comment>
      <translation>未知のプラグインタイプ</translation>
    </message>
    <message>
      <source>Plugin: %1 loaded</source>
      <comment>plugin manager</comment>
      <translation>プラグイン: %1 読み込み完了</translation>
    </message>
    <message>
      <source>Plugin: %1 failed to load: %2</source>
      <comment>plugin manager</comment>
      <translation>プラグイン: %1 読み込みに失敗: %2</translation>
    </message>
    <message>
      <source>Plugin: %1 initialized ok </source>
      <comment>plugin manager</comment>
      <translation>プラグイン: %1 初期化OK </translation>
    </message>
    <message>
      <source>Plugin: %1 failed post initialization</source>
      <comment>plugin manager</comment>
      <translation>プラグイン: %1 初期化後の処理に失敗</translation>
    </message>
    <message>
      <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PluginManagerPrefsGui</name>
    <message>
      <source>Plugin Manager</source>
      <translation>プラグインマネージャ</translation>
    </message>
    <message>
      <source>Plugin</source>
      <translation>プラグイン</translation>
    </message>
    <message>
      <source>How to run</source>
      <translation>実行方法</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>タイプ</translation>
    </message>
    <message>
      <source>Load it?</source>
      <translation>読み込むか</translation>
    </message>
    <message>
      <source>Plugin ID</source>
      <translation>プラグイン ID</translation>
    </message>
    <message>
      <source>File</source>
      <translation>ファイル</translation>
    </message>
    <message>
      <source>You need to restart the application to apply the changes.</source>
      <translation>変更を適用するにはアプリケーションの再起動が必要です</translation>
    </message>
    <message>
      <source>Form</source>
      <translation>フォーム</translation>
    </message>
  </context>
  <context>
    <name>PolygonProps</name>
    <message>
      <source>Polygon Properties</source>
      <translation>多角形のプロパティ</translation>
    </message>
  </context>
  <context>
    <name>PolygonWidget</name>
    <message>
      <source>Corn&amp;ers:</source>
      <translation>角(&amp;E):</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>回転(&amp;R):</translation>
    </message>
    <message>
      <source>Apply &amp;Factor</source>
      <translation>因数を適用(&amp;F)</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Factor:</source>
      <translation>因数(&amp;F):</translation>
    </message>
    <message>
      <source>Number of corners for polygons</source>
      <translation>多角形の角の数</translation>
    </message>
    <message>
      <source>Degrees of rotation for polygons</source>
      <translation>多角形の回転の度数</translation>
    </message>
    <message>
      <source>Apply Convex/Concave Factor to change shape of Polygons</source>
      <translation>多角形の形状を変更するのに凹凸の因数を適用する</translation>
    </message>
    <message>
      <source>Sample Polygon</source>
      <translation>サンプルの多角形</translation>
    </message>
    <message>
      <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
      <translation>負の値だと多角形は凸になり(もしくは星の形)、
正の値だと凹になります</translation>
    </message>
  </context>
  <context>
    <name>Preferences</name>
    <message>
      <source>Preferences</source>
      <translation>設定</translation>
    </message>
    <message>
      <source>General</source>
      <translation>全般</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>ドキュメント</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>ガイド</translation>
    </message>
    <message>
      <source>Typography</source>
      <translation>タイポグラフィ</translation>
    </message>
    <message>
      <source>Tools</source>
      <translation>ツール</translation>
    </message>
    <message>
      <source>Hyphenator</source>
      <translation>ハイフン</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>フォント</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>プリフライト検証</translation>
    </message>
    <message>
      <source>Color Management</source>
      <translation>カラーマネージメント</translation>
    </message>
    <message>
      <source>PDF Export</source>
      <translation>PDF出力</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>ドキュメントアイテム属性</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>目次とインデックス</translation>
    </message>
    <message>
      <source>Keyboard Shortcuts</source>
      <translation>キーボードショートカット</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>スクラップブック</translation>
    </message>
    <message>
      <source>Display</source>
      <translation>表示</translation>
    </message>
    <message>
      <source>External Tools</source>
      <translation>外部ツール</translation>
    </message>
    <message>
      <source>Miscellaneous</source>
      <translation>その他</translation>
    </message>
    <message>
      <source>Plugins</source>
      <translation>プラグイン</translation>
    </message>
    <message>
      <source>Printer</source>
      <translation>プリンタ</translation>
    </message>
  </context>
  <context>
    <name>PrefsDialogBase</name>
    <message>
      <source>&amp;Defaults</source>
      <translation>標準設定(&amp;D)</translation>
    </message>
    <message>
      <source>Save Preferences</source>
      <translation>設定を保存</translation>
    </message>
    <message>
      <source>Export...</source>
      <translation>エクスポート...</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>適用(&amp;A)</translation>
    </message>
    <message>
      <source>All preferences can be reset here</source>
      <translation>全ての設定をリセットできます</translation>
    </message>
    <message>
      <source>Apply all changes without closing the dialog</source>
      <translation>ダイアログを閉じずに全ての変更を適用する</translation>
    </message>
    <message>
      <source>Export current preferences into file</source>
      <translation>現在の設定をファイルに出力</translation>
    </message>
  </context>
  <context>
    <name>PrefsManager</name>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Migrate Old Scribus Settings?</source>
      <translation>古いScribusの設定を移行しますか?</translation>
    </message>
    <message>
      <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
      <translation>Scribusは既存のScribus 1.2設定ファイルを発見しました。
新しいScribusのバージョンにこの設定を移行しますか?</translation>
    </message>
    <message>
      <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
      <translation>書き込みの際に設定ファイル&quot;%1&quot;を開けませんでした: %2</translation>
    </message>
    <message>
      <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
      <translation>設定ファイル&quot;%1&quot;の書き込みに失敗しました: QIOデバイスステータスコード %2</translation>
    </message>
    <message>
      <source>Failed to open prefs file &quot;%1&quot;: %2</source>
      <translation>設定ファイル&quot;%1&quot;を開くのに失敗しました: %2</translation>
    </message>
    <message>
      <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
      <translation>&quot;%1&quot;からの設定XMLの読み込みに失敗しました: %2 行 %3 列 %4</translation>
    </message>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
    </message>
    <message>
      <source>Error Writing Preferences</source>
      <translation>設定の書き込みエラー</translation>
    </message>
    <message>
      <source>Scribus was not able to save its preferences:&lt;br>%1&lt;br>Please check file and directory permissions and available disk space.</source>
      <comment>scribus app error</comment>
      <translation>Scribusは設定を保存できませんでした:&lt;br>%1&lt;br>ファイルとディレクトリのパーミッションならびに空きディスク容量をチェックしてください。</translation>
    </message>
    <message>
      <source>Error Loading Preferences</source>
      <translation>設定の読み込みエラー</translation>
    </message>
    <message>
      <source>Scribus was not able to load its preferences:&lt;br>%1&lt;br>Default settings will be loaded.</source>
      <translation>Scribusは設定を読み込めませんでした:&lt;br>%1&lt;br>デフォルト設定を読み込みます。</translation>
    </message>
  </context>
  <context>
    <name>PresetLayout</name>
    <message>
      <source>Gutenberg</source>
      <translation>グーテンベルク</translation>
    </message>
    <message>
      <source>Magazine</source>
      <translation>雑誌</translation>
    </message>
    <message>
      <source>Fibonacci</source>
      <translation>フィボナッチ</translation>
    </message>
    <message>
      <source>Golden Mean</source>
      <translation>黄金分割</translation>
    </message>
    <message>
      <source>Nine Parts</source>
      <translation>九分割</translation>
    </message>
    <message>
      <source>None</source>
      <comment>layout type</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>You can select a predefined page layout here. 'None' leaves margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins to the same value. Leading is Left/Inside value.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PrintDialog</name>
    <message>
      <source>File</source>
      <translation>ファイル</translation>
    </message>
    <message>
      <source>Print Current Pa&amp;ge</source>
      <translation>現在のページを印刷(&amp;G)</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>標準で印刷</translation>
    </message>
    <message>
      <source>All</source>
      <translation>全て</translation>
    </message>
    <message>
      <source>Cyan</source>
      <translation>シアン</translation>
    </message>
    <message>
      <source>Magenta</source>
      <translation>マゼンダ</translation>
    </message>
    <message>
      <source>Yellow</source>
      <translation>イエロー</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>ブラック</translation>
    </message>
    <message>
      <source>Failed to retrieve printer settings</source>
      <translation>プリンタ設定の取得に失敗しました</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>PostScript Files (*.ps);;All Files (*)</source>
      <translation>PostScriptファイル (*.ps);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation>内側:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>外側:</translation>
    </message>
  </context>
  <context>
    <name>PrintDialogBase</name>
    <message>
      <source>Setup Printer</source>
      <translation>プリンタ設定</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation>印刷先</translation>
    </message>
    <message>
      <source>&amp;Options...</source>
      <translation>オプション(&amp;O)...</translation>
    </message>
    <message>
      <source>&amp;File:</source>
      <translation>ファイル(&amp;F):</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>変更(&amp;H)...</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
      <translation>追加印刷オプションを利用するために、kprinterやgtklpといった代わりのプリンタマネージャを使用します。</translation>
    </message>
    <message>
      <source>A&amp;lternative Printer Command</source>
      <translation>代わりのプリンタコマンド(&amp;L)</translation>
    </message>
    <message>
      <source>Co&amp;mmand:</source>
      <translation>コマンド(&amp;M):</translation>
    </message>
    <message>
      <source>Range</source>
      <translation>範囲</translation>
    </message>
    <message>
      <source>Print &amp;All</source>
      <translation>全て印刷(&amp;A)</translation>
    </message>
    <message>
      <source>N&amp;umber of Copies:</source>
      <translation>部数(&amp;U):</translation>
    </message>
    <message>
      <source>Print Current Pa&amp;ge</source>
      <translation>現在のページを印刷(&amp;G)</translation>
    </message>
    <message>
      <source>Print &amp;Range</source>
      <translation>指定された範囲を印刷(&amp;R)</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>カンマで区切られたトークンのリストを入力してください。
トークンは*で全てのページを、1-5のようにしてページ範囲を、
もしくは単ページ番号が利用できます。</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>標準で印刷</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation>色分解して印刷</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation>可能ならカラーで印刷</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>グレースケールで印刷</translation>
    </message>
    <message>
      <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
      <translation>PostScriptレベルを設定します。
レベル1もしくは2に設定すると、巨大なファイルが作成される可能性があります。</translation>
    </message>
    <message>
      <source>Advanced Options</source>
      <translation>詳細オプション</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>ページ</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>ページを水平方向に反転</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>ページを垂直方向に反転</translation>
    </message>
    <message>
      <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
      <translation>これを有効にすると、PostScriptファイルのサイズを明確に設定できます。プリンタにとって必要なければ推奨しません。</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation>メディアサイズを設定</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>ページマージンで切り取る</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>色</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation>下色除去を適用</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>スポットカラーが合成色に変換されるのを有効にします。商業プリンタでスポットカラーを印刷するつもりでなければ、おそらく有効のままにしておくのが最適でしょう。</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>スポットカラーをプロセスカラーに変換する</translation>
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation>カラーマネージメントが有効の場合は、印刷ストリームにICCプロファイルを埋め込むようにする</translation>
    </message>
    <message>
      <source>Apply Color Profiles</source>
      <translation>カラープロファイルを適用</translation>
    </message>
    <message>
      <source>Marks</source>
      <translation>マーク</translation>
    </message>
    <message>
      <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation>印刷後に紙を切断したり切り取ったりする目安となる切り取りマークをPDF中に作成します。</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>切り取りマーク</translation>
    </message>
    <message>
      <source>Add registration marks which are added to each separation</source>
      <translation>それぞれの版に追加される登録マークを追加します。</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>登録マーク</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>断ち切りマーク</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation>カラーキャリブレーションバーを追加</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>カラーバー</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>オフセット:</translation>
    </message>
    <message>
      <source>Indicate the distance offset for the registration marks</source>
      <translation>登録マークへのオフセット距離を表示します</translation>
    </message>
    <message>
      <source>Bleeds</source>
      <translation>断ち切り</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>上:</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation>物理ページの上から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>左:</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation>物理ページの右から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>下:</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation>物理ページの下から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>右:</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation>物理ページの左から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Use the existing bleed settings from the document preferences</source>
      <translation>ドキュメントの設定にある既存の断ち切り設定を使用します</translation>
    </message>
    <message>
      <source>Use Document Bleeds</source>
      <translation>ドキュメントの断ち切りを使う</translation>
    </message>
    <message>
      <source>Preview...</source>
      <translation>プレビュー...</translation>
    </message>
    <message>
      <source>&amp;Print</source>
      <translation>印刷(&amp;P)</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
  </context>
  <context>
    <name>PropertiesPalette</name>
    <message>
      <source>Fixed Linespacing</source>
      <translation>固定した行間隔</translation>
    </message>
    <message>
      <source>Automatic Linespacing</source>
      <translation>自動行間隔</translation>
    </message>
    <message>
      <source>Align to Baseline Grid</source>
      <translation>ベースライングリッドに合わせる</translation>
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
      <source>&amp;X-Pos:</source>
      <translation>位置&amp;X:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>幅(&amp;W):</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>位置&amp;Y:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>高さ(&amp;H):</translation>
    </message>
    <message>
      <source>Distance between columns</source>
      <translation>列の間隔</translation>
    </message>
    <message>
      <source>Column width</source>
      <translation>列幅</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>スタイルなし</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</source>
      <translation>名前 &quot;%1&quot; はすでに存在します。&lt;br/>他の名前を選んでください。</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>プロパティ</translation>
    </message>
    <message>
      <source>X, Y, &amp;Z</source>
      <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
      <source>&amp;Text</source>
      <translation>テキスト(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Image</source>
      <translation>画像(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>形状(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Line</source>
      <translation>線(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Colors</source>
      <translation>色(&amp;C)</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Geometry</source>
      <translation>ジオメトリ</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>回転(&amp;R):</translation>
    </message>
    <message>
      <source>Basepoint:</source>
      <translation>ベースポイント:</translation>
    </message>
    <message>
      <source>Level</source>
      <translation>レベル</translation>
    </message>
    <message>
      <source>Shape:</source>
      <translation>形状:</translation>
    </message>
    <message>
      <source>R&amp;ound
Corners:</source>
      <translation>角を丸める(&amp;O):</translation>
    </message>
    <message>
      <source>Distance of Text</source>
      <translation>テキストの間隔</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>列数(&amp;M):</translation>
    </message>
    <message>
      <source>To&amp;p:</source>
      <translation>上(&amp;P):</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>下(&amp;B):</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>左(&amp;L):</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>右(&amp;R):</translation>
    </message>
    <message>
      <source>T&amp;abulators...</source>
      <translation>タブ(&amp;A)...</translation>
    </message>
    <message>
      <source>Path Text Properties</source>
      <translation>パステキストのプロパティ</translation>
    </message>
    <message>
      <source>Show Curve</source>
      <translation>曲線を表示</translation>
    </message>
    <message>
      <source>Start Offset:</source>
      <translation>オフセットを開始:</translation>
    </message>
    <message>
      <source>Distance from Curve:</source>
      <translation>曲線からの距離:</translation>
    </message>
    <message>
      <source>Use &amp;Bounding Box</source>
      <translation>バウンディングボックスを使用(&amp;B)</translation>
    </message>
    <message>
      <source>&amp;Use Contour Line</source>
      <translation>等高線を使用(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Free Scaling</source>
      <translation>自由倍率(&amp;F)</translation>
    </message>
    <message>
      <source>Actual X-DPI:</source>
      <translation>実X-DPI:</translation>
    </message>
    <message>
      <source>Actual Y-DPI:</source>
      <translation>実Y-DPI:</translation>
    </message>
    <message>
      <source>X-Sc&amp;ale:</source>
      <translation>比率X (&amp;A):</translation>
    </message>
    <message>
      <source>Y-Scal&amp;e:</source>
      <translation>比率Y (&amp;E):</translation>
    </message>
    <message>
      <source>Scale &amp;To Frame Size</source>
      <translation>フレームサイズに合わせる(&amp;T)</translation>
    </message>
    <message>
      <source>P&amp;roportional</source>
      <translation>プロポーショナル(&amp;R)</translation>
    </message>
    <message>
      <source>Input Profile:</source>
      <translation>プロファイルを入力:</translation>
    </message>
    <message>
      <source>Rendering Intent:</source>
      <translation>レンダリングインテント:</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>知覚</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>相対的な色彩</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>彩度</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>絶対的な色彩</translation>
    </message>
    <message>
      <source>Left Point</source>
      <translation>左点</translation>
    </message>
    <message>
      <source>End Points</source>
      <translation>終点</translation>
    </message>
    <message>
      <source>&amp;Basepoint:</source>
      <translation>ベースポイント(&amp;B):</translation>
    </message>
    <message>
      <source>T&amp;ype of Line:</source>
      <translation>線の種類(&amp;Y):</translation>
    </message>
    <message>
      <source>Start Arrow:</source>
      <translation>始点矢印:</translation>
    </message>
    <message>
      <source>End Arrow:</source>
      <translation>終端矢印:</translation>
    </message>
    <message>
      <source>Line &amp;Width:</source>
      <translation>線幅(&amp;W):</translation>
    </message>
    <message>
      <source>Ed&amp;ges:</source>
      <translation>先端(&amp;G):</translation>
    </message>
    <message>
      <source>Miter Join</source>
      <translation>マイタージョイン</translation>
    </message>
    <message>
      <source>Bevel Join</source>
      <translation>ベベルジョイン</translation>
    </message>
    <message>
      <source>Round Join</source>
      <translation>ラウンドジョイン</translation>
    </message>
    <message>
      <source>Flat Cap</source>
      <translation>フラットキャップ</translation>
    </message>
    <message>
      <source>Square Cap</source>
      <translation>スクェアキャップ</translation>
    </message>
    <message>
      <source>Round Cap</source>
      <translation>ラウンドキャップ</translation>
    </message>
    <message>
      <source>&amp;Endings:</source>
      <translation>終端(&amp;E):</translation>
    </message>
    <message>
      <source>Cell Lines</source>
      <translation>セルの線</translation>
    </message>
    <message>
      <source>Line at Top</source>
      <translation>上の線</translation>
    </message>
    <message>
      <source>Line at the Left</source>
      <translation>左の線</translation>
    </message>
    <message>
      <source>Line at the Right </source>
      <translation>右の線</translation>
    </message>
    <message>
      <source>Line at Bottom</source>
      <translation>下の線</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Name of selected object</source>
      <translation>選択されたオブジェクトの名前</translation>
    </message>
    <message>
      <source>Horizontal position of current basepoint</source>
      <translation>現在のベースポイントの水平位置</translation>
    </message>
    <message>
      <source>Vertical position of current basepoint</source>
      <translation>現在のベースポイントの垂直位置</translation>
    </message>
    <message>
      <source>Width</source>
      <translation>幅</translation>
    </message>
    <message>
      <source>Height</source>
      <translation>高さ</translation>
    </message>
    <message>
      <source>Rotation of object at current basepoint</source>
      <translation>現在のベースポイントのオブジェクトの回転</translation>
    </message>
    <message>
      <source>Point from which measurements or rotation angles are referenced</source>
      <translation>計測や回転角で参照されるポイント</translation>
    </message>
    <message>
      <source>Flip Horizontal</source>
      <translation>水平方向に反転</translation>
    </message>
    <message>
      <source>Flip Vertical</source>
      <translation>垂直方向に反転</translation>
    </message>
    <message>
      <source>Move one level up</source>
      <translation>1つ上に移動</translation>
    </message>
    <message>
      <source>Move one level down</source>
      <translation>1つ下に移動</translation>
    </message>
    <message>
      <source>Move to front</source>
      <translation>前面に移動</translation>
    </message>
    <message>
      <source>Move to back</source>
      <translation>背面に移動</translation>
    </message>
    <message>
      <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
      <translation>オブジェクトのあるレベルを示します。0はオブジェクトが一番下にあることを意味します。</translation>
    </message>
    <message>
      <source>Lock or unlock the object</source>
      <translation>オブジェクトをロック/ロック解除</translation>
    </message>
    <message>
      <source>Lock or unlock the size of the object</source>
      <translation>オブジェクトのサイズをロック/ロック解除</translation>
    </message>
    <message>
      <source>Font of selected text or object</source>
      <translation>選択されたテキストまたはオブジェクトのフォント</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>フォントサイズ</translation>
    </message>
    <message>
      <source>Offset to baseline of characters</source>
      <translation>文字のベースラインに対するオフセット</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>文字幅の倍率</translation>
    </message>
    <message>
      <source>Scaling height of characters</source>
      <translation>文字高さの倍率</translation>
    </message>
    <message>
      <source>Saturation of color of text stroke</source>
      <translation>テキストの輪郭色の彩度</translation>
    </message>
    <message>
      <source>Saturation of color of text fill</source>
      <translation>テキストの塗りつぶし色の彩度</translation>
    </message>
    <message>
      <source>Right to Left Writing</source>
      <translation>右から左の書式</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>手動トラッキング</translation>
    </message>
    <message>
      <source>Line Spacing</source>
      <translation>行間隔</translation>
    </message>
    <message>
      <source>Change settings for left or end points</source>
      <translation>左点もしくは終点の設定を変更</translation>
    </message>
    <message>
      <source>Pattern of line</source>
      <translation>線のパターン</translation>
    </message>
    <message>
      <source>Thickness of line</source>
      <translation>線の太さ</translation>
    </message>
    <message>
      <source>Type of line joins</source>
      <translation>線の結合の種類</translation>
    </message>
    <message>
      <source>Type of line end</source>
      <translation>線の端の種類</translation>
    </message>
    <message>
      <source>Line style of current object</source>
      <translation>現在のオブジェクトの線スタイル</translation>
    </message>
    <message>
      <source>Choose the shape of frame...</source>
      <translation>フレームの形状を選択...</translation>
    </message>
    <message>
      <source>Edit shape of the frame...</source>
      <translation>フレームの形状を編集...</translation>
    </message>
    <message>
      <source>Set radius of corner rounding</source>
      <translation>角の丸みの半径を設定</translation>
    </message>
    <message>
      <source>Number of columns in text frame</source>
      <translation>テキストフレーム中の列数</translation>
    </message>
    <message>
      <source>Switches between Gap or Column width</source>
      <translation>ギャップもしくは列幅に切替えます</translation>
    </message>
    <message>
      <source>Distance of text from top of frame</source>
      <translation>フレームの上部からのテキストの距離</translation>
    </message>
    <message>
      <source>Distance of text from bottom of frame</source>
      <translation>フレームの下部からのテキストの距離</translation>
    </message>
    <message>
      <source>Distance of text from left of frame</source>
      <translation>フレームの左からのテキストの距離</translation>
    </message>
    <message>
      <source>Distance of text from right of frame</source>
      <translation>フレームの右からのテキストの距離</translation>
    </message>
    <message>
      <source>Edit tab settings of text frame...</source>
      <translation>テキストフレームのタブ設定を編集...</translation>
    </message>
    <message>
      <source>Allow the image to be a different size to the frame</source>
      <translation>画像がフレームのサイズと異なるのを許可</translation>
    </message>
    <message>
      <source>Horizontal offset of image within frame</source>
      <translation>フレーム内の画像の水平オフセット</translation>
    </message>
    <message>
      <source>Vertical offset of image within frame</source>
      <translation>フレーム内の画像の垂直オフセット</translation>
    </message>
    <message>
      <source>Resize the image horizontally</source>
      <translation>画像を水平方向にサイズ変更する</translation>
    </message>
    <message>
      <source>Resize the image vertically</source>
      <translation>画像を垂直方向にサイズ変更する</translation>
    </message>
    <message>
      <source>Keep the X and Y scaling the same</source>
      <translation>XY方向の倍率が同じになるようにする</translation>
    </message>
    <message>
      <source>Keep the aspect ratio</source>
      <translation>アスペクト比を保持</translation>
    </message>
    <message>
      <source>Make the image fit within the size of the frame</source>
      <translation>画像をフレームのサイズ内に合わせる</translation>
    </message>
    <message>
      <source>Use image proportions rather than those of the frame</source>
      <translation>フレームの比率より画像の比率を使用する</translation>
    </message>
    <message>
      <source>Source profile of the image</source>
      <translation>画像のソースプロファイル</translation>
    </message>
    <message>
      <source>Rendering intent for the image</source>
      <translation>画像のレンダリングインテント</translation>
    </message>
    <message>
      <source>Fill Rule</source>
      <translation>塗りつぶしルール</translation>
    </message>
    <message>
      <source>Even-Odd</source>
      <translation>奇偶</translation>
    </message>
    <message>
      <source>Non Zero</source>
      <translation>0以外</translation>
    </message>
    <message>
      <source>Overprinting</source>
      <translation>重ね刷り</translation>
    </message>
    <message>
      <source>Knockout</source>
      <translation>ノックアウト</translation>
    </message>
    <message>
      <source>Overprint</source>
      <translation>重ね刷り</translation>
    </message>
    <message>
      <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
      <translation>テキストの輪郭色 と/または 
両方を選択した場合は、同じ色を共有します。</translation>
    </message>
    <message>
      <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
      <translation>選択されたテキストの色。アウトラインテキストの装飾が有効な場合、この色は塗りつぶし色になります。ドロップシャドウテキストが有効な場合、最も上の色になります。</translation>
    </message>
    <message>
      <source>Gap:</source>
      <translation>ギャップ:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>幅:</translation>
    </message>
    <message>
      <source>Text &amp;Flow Around Frame</source>
      <translation>フレーム周りのテキストの回り込み(&amp;F)</translation>
    </message>
    <message>
      <source>Disabled</source>
      <translation>無効</translation>
    </message>
    <message>
      <source>Use Frame &amp;Shape</source>
      <translation>フレームの形状を使用(&amp;S)</translation>
    </message>
    <message>
      <source>Image Effects</source>
      <translation>画像のエフェクト</translation>
    </message>
    <message>
      <source>Extended Image Properties</source>
      <translation>拡張画像のプロパティ</translation>
    </message>
    <message>
      <source>Disable text flow from lower frames around object</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the frame shape for text flow of text frames below the object.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Transparency Settings</source>
      <translation>透明度の設定</translation>
    </message>
    <message>
      <source>&amp;Group</source>
      <translation>グループ化(&amp;G)</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>不透明度:</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>合成モード:</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>標準</translation>
    </message>
    <message>
      <source>Darken</source>
      <translation>暗くする</translation>
    </message>
    <message>
      <source>Lighten</source>
      <translation>明るくする</translation>
    </message>
    <message>
      <source>Multiply</source>
      <translation>乗算</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>スクリーン</translation>
    </message>
    <message>
      <source>Overlay</source>
      <translation>オーバーレイ</translation>
    </message>
    <message>
      <source>Hard Light</source>
      <translation>ハードライト</translation>
    </message>
    <message>
      <source>Soft Light</source>
      <translation>ソフトライト</translation>
    </message>
    <message>
      <source>Difference</source>
      <translation>差の絶対値</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>除外</translation>
    </message>
    <message>
      <source>Color Dodge</source>
      <translation>覆い焼きカラー</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>焼き込み</translation>
    </message>
    <message>
      <source>Hue</source>
      <translation>色合い</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Group the selected objects</source>
      <translation>選択されたオブジェクトをグループ化</translation>
    </message>
    <message>
      <source>Auto</source>
      <translation>自動</translation>
    </message>
    <message>
      <source>When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object. T</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Click and hold down to select the line spacing mode.</source>
      <translation>クリックして押し下げて、行間隔モードを選択してください。</translation>
    </message>
    <message>
      <source>Default</source>
      <translation>デフォルト</translation>
    </message>
    <message>
      <source>Stair Step</source>
      <translation>階段</translation>
    </message>
    <message>
      <source>Skew</source>
      <translation>歪曲</translation>
    </message>
    <message>
      <source>Flip Text</source>
      <translation>テキストを反転</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>タイプ:</translation>
    </message>
    <message>
      <source>Use Image Clip Path</source>
      <translation>画像クリップパスを使用</translation>
    </message>
    <message>
      <source>Paragraph St&amp;yle:</source>
      <translation>段落スタイル(&amp;Y):</translation>
    </message>
    <message>
      <source>Character St&amp;yle:</source>
      <translation>文字スタイル(&amp;Y):</translation>
    </message>
    <message>
      <source>Optical Margins:</source>
      <translation>光学マージン:</translation>
    </message>
    <message>
      <source>Word Tracking</source>
      <translation>ワードトラッキング</translation>
    </message>
    <message>
      <source>Min:</source>
      <translation>最小:</translation>
    </message>
    <message>
      <source>Norm:</source>
      <translation>普通:</translation>
    </message>
    <message>
      <source>Glyph Extension</source>
      <translation>グリフ拡張</translation>
    </message>
    <message>
      <source>Max:</source>
      <translation>最大:</translation>
    </message>
    <message>
      <source>Use the clipping path of the image</source>
      <translation>画像のクリッピングパスを使用します。</translation>
    </message>
    <message>
      <source>Paragraph style of currently selected text or paragraph</source>
      <translation>現在選択されているテキストまたは段落の段落スタイル</translation>
    </message>
    <message>
      <source>Character style of currently selected text or paragraph</source>
      <translation>現在選択されているテキストまたは段落の文字スタイル</translation>
    </message>
    <message>
      <source>Remove Direct Paragraph Formatting</source>
      <translation>段落フォーマットを削除</translation>
    </message>
    <message>
      <source>Remove Direct Character Formatting</source>
      <translation>文字フォーマットを削除</translation>
    </message>
    <message>
      <source>Minimal width of spaces between words</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Normal width of spaces between words</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Minimal shrinkage of glyphs for justification</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Maximal extension of glyphs for justification</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Uses hanging punctuation and margin kerning to achieve nicer looking columns</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enable or disable exporting of the object</source>
      <translation>オブジェクトのエクスポートの有効/無効</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>カスタム</translation>
    </message>
    <message>
      <source>&amp;Edit...</source>
      <translation>編集(&amp;E)...</translation>
    </message>
    <message>
      <source>First Line Offset</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Maximum Ascent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Font Ascent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Color &amp; Effects</source>
      <translation>色と効果</translation>
    </message>
    <message>
      <source>Advanced Settings</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Style Settings</source>
      <translation>透明度の設定</translation>
    </message>
    <message>
      <source>Ungroup the selected group</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PythonConsole</name>
    <message>
      <source>&amp;Open...</source>
      <translation>開く(&amp;O)...</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>保存(&amp;S)</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>名前を付けて保存(&amp;A)...</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;Run</source>
      <translation>実行(&amp;R)</translation>
    </message>
    <message>
      <source>Run As &amp;Console</source>
      <translation>コンソールで実行(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Save Output...</source>
      <translation>出力を保存(&amp;S)...</translation>
    </message>
    <message>
      <source>&amp;Script</source>
      <translation>スクリプト(&amp;S)</translation>
    </message>
    <message>
      <source>Script Console</source>
      <translation>スクリプトコンソール</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation>終了(&amp;Q)</translation>
    </message>
  </context>
  <context>
    <name>QColorDialog</name>
    <message>
      <source>Hu&amp;e:</source>
      <translation>色相(&amp;E):</translation>
    </message>
    <message>
      <source>&amp;Sat:</source>
      <translation>彩度(&amp;S):</translation>
    </message>
    <message>
      <source>&amp;Val:</source>
      <translation>明度(&amp;V):</translation>
    </message>
    <message>
      <source>&amp;Red:</source>
      <translation>赤(&amp;R):</translation>
    </message>
    <message>
      <source>&amp;Green:</source>
      <translation>緑(&amp;G):</translation>
    </message>
    <message>
      <source>Bl&amp;ue:</source>
      <translation>青(&amp;U):</translation>
    </message>
    <message>
      <source>A&amp;lpha channel:</source>
      <translation>アルファチャンネル(&amp;L):</translation>
    </message>
    <message>
      <source>&amp;Basic colors</source>
      <translation>基本色(&amp;B)</translation>
    </message>
    <message>
      <source>&amp;Custom colors</source>
      <translation>カスタムカラー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Define Custom Colors >></source>
      <translation>カスタムカラーを定義(&amp;D) >></translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>&amp;Add to Custom Colors</source>
      <translation>カスタムカラーに追加(&amp;A)</translation>
    </message>
    <message>
      <source>Select color</source>
      <translation>色を選択</translation>
    </message>
  </context>
  <context>
    <name>QFileDialog</name>
    <message>
      <source>Copy or Move a File</source>
      <translation>ファイルをコピーもしくは移動</translation>
    </message>
    <message>
      <source>Read: %1</source>
      <translation>読み込み: %1</translation>
    </message>
    <message>
      <source>Write: %1</source>
      <translation>書き込み: %1</translation>
    </message>
    <message>
      <source>File &amp;name:</source>
      <translation>ファイル名(&amp;N):</translation>
    </message>
    <message>
      <source>File &amp;type:</source>
      <translation>ファイルタイプ(&amp;T):</translation>
    </message>
    <message>
      <source>One directory up</source>
      <translation>一つ上のディレクトリへ</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>全てのファイル (*)</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>サイズ</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>タイプ</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>日付</translation>
    </message>
    <message>
      <source>Attributes</source>
      <translation>属性</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Back</source>
      <translation>戻る</translation>
    </message>
    <message>
      <source>Create New Folder</source>
      <translation>新規フォルダを作成</translation>
    </message>
    <message>
      <source>List View</source>
      <translation>リストビュー</translation>
    </message>
    <message>
      <source>Detail View</source>
      <translation>詳細ビュー</translation>
    </message>
    <message>
      <source>Preview File Info</source>
      <translation>ファイル情報をプレビュー</translation>
    </message>
    <message>
      <source>Preview File Contents</source>
      <translation>ファイル内容をプレビュー</translation>
    </message>
    <message>
      <source>Read-write</source>
      <translation>読み書き</translation>
    </message>
    <message>
      <source>Read-only</source>
      <translation>読み込み専用</translation>
    </message>
    <message>
      <source>Write-only</source>
      <translation>書き込み専用</translation>
    </message>
    <message>
      <source>Inaccessible</source>
      <translation>アクセス不能</translation>
    </message>
    <message>
      <source>Symlink to File</source>
      <translation>ファイルへのシンボリックリンク</translation>
    </message>
    <message>
      <source>Symlink to Directory</source>
      <translation>ディレクトリへのシンボリックリンク</translation>
    </message>
    <message>
      <source>Symlink to Special</source>
      <translation>特殊ファイルへのシンボリックリンク</translation>
    </message>
    <message>
      <source>File</source>
      <translation>ファイル</translation>
    </message>
    <message>
      <source>Dir</source>
      <translation>ディレクトリ</translation>
    </message>
    <message>
      <source>Special</source>
      <translation>スペシャル</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Save As</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>&amp;Open</source>
      <translation>開く(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>保存(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Rename</source>
      <translation>名前変更(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>R&amp;eload</source>
      <translation>リロード(&amp;E)</translation>
    </message>
    <message>
      <source>Sort by &amp;Name</source>
      <translation>名前でソート(&amp;N)</translation>
    </message>
    <message>
      <source>Sort by &amp;Size</source>
      <translation>サイズでソート(&amp;S)</translation>
    </message>
    <message>
      <source>Sort by &amp;Date</source>
      <translation>日付でソート(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Unsorted</source>
      <translation>ソートしない(&amp;U)</translation>
    </message>
    <message>
      <source>Sort</source>
      <translation>ソート</translation>
    </message>
    <message>
      <source>Show &amp;hidden files</source>
      <translation>隠しファイルを表示(&amp;H)</translation>
    </message>
    <message>
      <source>the file</source>
      <translation>ファイル</translation>
    </message>
    <message>
      <source>the directory</source>
      <translation>ディレクトリ</translation>
    </message>
    <message>
      <source>the symlink</source>
      <translation>シンボリックリンク</translation>
    </message>
    <message>
      <source>Delete %1</source>
      <translation>%1 を削除</translation>
    </message>
    <message>
      <source>&lt;qt>Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt></source>
      <translation>&lt;qt>本当に%1 &quot;%2&quot;を削除しますか ?&lt;/qt></translation>
    </message>
    <message>
      <source>New Folder 1</source>
      <translation>新規フォルダ 1</translation>
    </message>
    <message>
      <source>New Folder</source>
      <translation>新規フォルダ</translation>
    </message>
    <message>
      <source>New Folder %1</source>
      <translation>新規フォルダ %1</translation>
    </message>
    <message>
      <source>Find Directory</source>
      <translation>ディレクトリを検索</translation>
    </message>
    <message>
      <source>Directories</source>
      <translation>ディレクトリ</translation>
    </message>
    <message>
      <source>Save</source>
      <translation>保存</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>エラー</translation>
    </message>
    <message>
      <source>%1
File not found.
Check path and filename.</source>
      <translation>%1
ファイルが見つかりません。
パスとファイル名を確認してください。</translation>
    </message>
    <message>
      <source>All Files (*.*)</source>
      <translation>全てのファイル (*.*)</translation>
    </message>
    <message>
      <source>Select a Directory</source>
      <translation>ディレクトリを選択</translation>
    </message>
    <message>
      <source>Directory:</source>
      <translation>ディレクトリ:</translation>
    </message>
    <message>
      <source>Look &amp;in:</source>
      <translation>参照(&amp;I):</translation>
    </message>
  </context>
  <context>
    <name>QFontDialog</name>
    <message>
      <source>&amp;Font</source>
      <translation>フォント(&amp;F)</translation>
    </message>
    <message>
      <source>Font st&amp;yle</source>
      <translation>フォントスタイル(&amp;Y)</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>サイズ(&amp;S)</translation>
    </message>
    <message>
      <source>Effects</source>
      <translation>エフェクト</translation>
    </message>
    <message>
      <source>Stri&amp;keout</source>
      <translation>取り消し線(&amp;K)</translation>
    </message>
    <message>
      <source>&amp;Underline</source>
      <translation>下線(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Color</source>
      <translation>色(&amp;C)</translation>
    </message>
    <message>
      <source>Sample</source>
      <translation>サンプル</translation>
    </message>
    <message>
      <source>Scr&amp;ipt</source>
      <translation>スクリプト(&amp;I)</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Apply</source>
      <translation>適用</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>閉じる</translation>
    </message>
    <message>
      <source>Select Font</source>
      <translation>フォントを選択</translation>
    </message>
  </context>
  <context>
    <name>QLineEdit</name>
    <message>
      <source>Clear</source>
      <translation>クリア</translation>
    </message>
    <message>
      <source>Select All</source>
      <translation>全て選択</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>元に戻す(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>やり直し(&amp;R)</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>切り取り(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>貼り付け(&amp;P)</translation>
    </message>
  </context>
  <context>
    <name>QMainWindow</name>
    <message>
      <source>Line up</source>
      <translation>整列</translation>
    </message>
    <message>
      <source>Customize...</source>
      <translation>カスタマイズ...</translation>
    </message>
  </context>
  <context>
    <name>QMessageBox</name>
    <message>
      <source>&lt;h3>About Qt&lt;/h3>&lt;p>This program uses Qt version %1.&lt;/p>&lt;p>Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p>&lt;p>Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br>Qt is also available for embedded devices.&lt;/p>&lt;p>Qt is a Trolltech product. See &lt;tt>http://www.trolltech.com/qt/&lt;/tt> for more information.&lt;/p></source>
      <translation>&lt;h3>Qt について&lt;/h3>&lt;p>このプログラムは Qt バージョン %1 を使用しています。&lt;/p>&lt;p>
QtはクロスプラットフォームのC++ GUI &amp;amp; アプリケーション開発ツールキットです。&lt;/p>&lt;p>
Qtにより、MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, 商用のUnix派生版でソースの移植性があります。&lt;br>
Qtは組込みデバイスでも利用可能です。&lt;/p>&lt;p>QtはTrolltechの製品です。詳細は&lt;tt>http://www.trolltech.com/qt/&lt;
/tt>を参照してください。&lt;/p></translation>
    </message>
  </context>
  <context>
    <name>QObject</name>
    <message>
      <source>Copy #%1 of </source>
      <translation>#%1のコピー</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>背景</translation>
    </message>
    <message>
      <source>Importing text</source>
      <translation>テキストをインポート中</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>サポートされている全てのフォーマット</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>全てのファイル (*)</translation>
    </message>
    <message>
      <source>Afrikaans</source>
      <translation>アフリカーンス語</translation>
    </message>
    <message>
      <source>Albanian</source>
      <translation>アルバニア語</translation>
    </message>
    <message>
      <source>Basque</source>
      <translation>バスク語</translation>
    </message>
    <message>
      <source>Bulgarian</source>
      <translation>ブルガリア語</translation>
    </message>
    <message>
      <source>Catalan</source>
      <translation>カタロニア語</translation>
    </message>
    <message>
      <source>Chinese</source>
      <translation>中国語</translation>
    </message>
    <message>
      <source>Croatian</source>
      <translation>クロアチア語</translation>
    </message>
    <message>
      <source>Czech</source>
      <translation>チェコ語</translation>
    </message>
    <message>
      <source>Danish</source>
      <translation>デンマーク語</translation>
    </message>
    <message>
      <source>Dutch</source>
      <translation>オランダ語</translation>
    </message>
    <message>
      <source>English</source>
      <translation>英語</translation>
    </message>
    <message>
      <source>English (British)</source>
      <translation>英語 (イギリス)</translation>
    </message>
    <message>
      <source>Esperanto</source>
      <translation>エスペラント語</translation>
    </message>
    <message>
      <source>German</source>
      <translation>ドイツ語</translation>
    </message>
    <message>
      <source>German (Trad.)</source>
      <translation>ドイツ語 (Trad.)</translation>
    </message>
    <message>
      <source>Finnish</source>
      <translation>フィンランド語</translation>
    </message>
    <message>
      <source>French</source>
      <translation>フランス語</translation>
    </message>
    <message>
      <source>Galician</source>
      <translation>ガリシア語</translation>
    </message>
    <message>
      <source>Greek</source>
      <translation>ギリシャ語</translation>
    </message>
    <message>
      <source>Hungarian</source>
      <translation>ハンガリー語</translation>
    </message>
    <message>
      <source>Indonesian</source>
      <translation>インドネシア語</translation>
    </message>
    <message>
      <source>Italian</source>
      <translation>イタリア語</translation>
    </message>
    <message>
      <source>Korean</source>
      <translation>韓国語</translation>
    </message>
    <message>
      <source>Lithuanian</source>
      <translation>リトアニア語</translation>
    </message>
    <message>
      <source>Norwegian (Nnyorsk)</source>
      <translation>ノルウェー語 (ニーノシュク)</translation>
    </message>
    <message>
      <source>Norwegian</source>
      <translation>ノルウェー語</translation>
    </message>
    <message>
      <source>Polish</source>
      <translation>ポーランド語</translation>
    </message>
    <message>
      <source>Portuguese</source>
      <translation>ポルトガル語</translation>
    </message>
    <message>
      <source>Portuguese (BR)</source>
      <translation>ポルトガル語(BR)</translation>
    </message>
    <message>
      <source>Russian</source>
      <translation>ロシア語</translation>
    </message>
    <message>
      <source>Swedish</source>
      <translation>スウェーデン語</translation>
    </message>
    <message>
      <source>Spanish</source>
      <translation>スペイン語</translation>
    </message>
    <message>
      <source>Spanish (Latin)</source>
      <translation>スペイン語(ラテン)</translation>
    </message>
    <message>
      <source>Slovak</source>
      <translation>スロバキア語</translation>
    </message>
    <message>
      <source>Slovenian</source>
      <translation>スロベニア語</translation>
    </message>
    <message>
      <source>Serbian</source>
      <translation>セルビア語</translation>
    </message>
    <message>
      <source>Thai</source>
      <translation>タイ語</translation>
    </message>
    <message>
      <source>Turkish</source>
      <translation>トルコ語</translation>
    </message>
    <message>
      <source>Ukranian</source>
      <translation>ウクライナ語</translation>
    </message>
    <message>
      <source>Welsh</source>
      <translation>ウェールズ語</translation>
    </message>
    <message>
      <source>Scribus Crash</source>
      <translation>Scribusはクラッシュしました</translation>
    </message>
    <message>
      <source>Scribus crashes due to Signal #%1</source>
      <translation>Scribusはシグナル #%1 でクラッシュしました</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>Scribus crashes due to the following exception : %1</source>
      <translation>Scribusは以下の例外によりクラッシュしました : %1</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>ページ</translation>
    </message>
    <message>
      <source>Master Page </source>
      <translation>マスターページ </translation>
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
      <source>Could not open output file %1</source>
      <translation>出力ファイル %1 を開けませんでした</translation>
    </message>
    <message>
      <source>Output stream not writeable</source>
      <translation>出力ストリームは書き込み可能ではありません</translation>
    </message>
    <message>
      <source>Verification of settings failed: %1</source>
      <translation>設定の検証に失敗しました: %1</translation>
    </message>
    <message>
      <source>Could not open input file %1</source>
      <translation>入力ファイル %1 を開けませんでした</translation>
    </message>
    <message>
      <source>Unable to read settings XML:</source>
      <translation>設定XMLを読み込めません:</translation>
    </message>
    <message>
      <source>%1 (line %2 col %3)</source>
      <comment>Load PDF settings</comment>
      <translation>%1 (行 %2 列 %3)</translation>
    </message>
    <message>
      <source>Unable to read settings XML: %1</source>
      <translation>設定XMLを読み込めません: %1</translation>
    </message>
    <message>
      <source>null root node</source>
      <comment>Load PDF settings</comment>
      <translation>nullルートノード</translation>
    </message>
    <message>
      <source>&lt;pdfVersion> invalid</source>
      <comment>Load PDF settings</comment>
      <translation>&lt;pdfVersion> 無効</translation>
    </message>
    <message>
      <source>found %1 &lt;%2> nodes, need 1.</source>
      <comment>Load PDF settings</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>unexpected null &lt;%2> node</source>
      <comment>Load PDF settings</comment>
      <translation>予期せぬ null &lt;%2> ノード</translation>
    </message>
    <message>
      <source>node &lt;%1> not an element</source>
      <comment>Load PDF settings</comment>
      <translation>ノード &lt;%1> は要素ではありません</translation>
    </message>
    <message>
      <source>element &lt;%1> lacks `value' attribute</source>
      <comment>Load PDF settings</comment>
      <translation>要素 &lt;%1> に `value' 属性が足りません</translation>
    </message>
    <message>
      <source>element &lt;%1> value must be `true' or `false'</source>
      <comment>Load PDF settings</comment>
      <translation>要素 &lt;%1> の値は `true' もしくは `false' である必要があります</translation>
    </message>
    <message>
      <source>element &lt;lpiSettingsEntry> lacks `name' attribute</source>
      <comment>Load PDF settings</comment>
      <translation>要素 &lt;lpiSettingsEntry> に `name' 属性が足りません</translation>
    </message>
    <message>
      <source>All</source>
      <translation>全て</translation>
    </message>
    <message>
      <source>Exporting PostScript File</source>
      <translation>PostScriptファイルをエクスポート中</translation>
    </message>
    <message>
      <source>Printing File</source>
      <translation>ファイルを印刷中</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>ブラック</translation>
    </message>
    <message>
      <source>Cyan</source>
      <translation>シアン</translation>
    </message>
    <message>
      <source>Magenta</source>
      <translation>マゼンダ</translation>
    </message>
    <message>
      <source>Yellow</source>
      <translation>イエロー</translation>
    </message>
    <message>
      <source>Freetype2 library not available</source>
      <translation>Freetype2ライブラリが利用可能ではありません</translation>
    </message>
    <message>
      <source>Font %1 is broken (read stream), no embedding</source>
      <translation>フォント %1 は破損しているので(読み込みストリーム)、埋め込まれません</translation>
    </message>
    <message>
      <source>Font %1 is broken (no Face), discarding it</source>
      <translation>フォント %1 は破損しているので(フェースなし)、破棄されます</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2 (charcode %3)</source>
      <translation>フォント %1 は破損したグリフです %2 (文字コード %3)</translation>
    </message>
    <message>
      <source>Font %1 cannot be read, no embedding</source>
      <translation>フォント %1 が読み込めないので、埋め込まれません</translation>
    </message>
    <message>
      <source>Font %1 is broken, discarding it</source>
      <translation>フォント %1 は壊れているので破棄されます</translation>
    </message>
    <message>
      <source>Creating Font Cache</source>
      <translation>フォントキャッシュを作成中</translation>
    </message>
    <message>
      <source>Failed to load font %1 - font type unknown</source>
      <translation>フォント %1 の読み込みに失敗しました - フォントタイプ不明</translation>
    </message>
    <message>
      <source>New Font found, checking...</source>
      <translation>新しいフォントが見つかりました。チェック中...</translation>
    </message>
    <message>
      <source>Modified Font found, checking...</source>
      <translation>変更されたフォントを発見、チェック中...</translation>
    </message>
    <message>
      <source>Font %1 loaded from %2(%3)</source>
      <translation>フォント %1 は %2(%3) から読み込まれました</translation>
    </message>
    <message>
      <source>Font %1(%2) is duplicate of %3</source>
      <translation>フォント %1(%2)は %3 のコピーです</translation>
    </message>
    <message>
      <source>Loading font %1 (found using fontconfig)</source>
      <translation>フォントをロード中 %1 (fontconfigを使用して検索)</translation>
    </message>
    <message>
      <source>Failed to load a font - freetype2 couldn't find the font file</source>
      <translation>フォントの読み込みに失敗しました - freetype2はフォントファイルを見つけられませんでした</translation>
    </message>
    <message>
      <source>Reading Font Cache</source>
      <translation>フォントキャッシュを読み込み中</translation>
    </message>
    <message>
      <source>Writing updated Font Cache</source>
      <translation>更新されたフォントキャッシュを書き込み中</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>フォントを検索中</translation>
    </message>
    <message>
      <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
      <translation>フォント %2 からフェース %1 を抽出しています (offset=%3, nTables=%4)</translation>
    </message>
    <message>
      <source>memcpy header: %1 %2 %3</source>
      <translation>memcpyヘッダ: %1 %2 %3</translation>
    </message>
    <message>
      <source>table '%1'</source>
      <translation>テーブル '%1'</translation>
    </message>
    <message>
      <source>memcpy table: %1 %2 %3</source>
      <translation>memcpyテーブル: %1 %2 %3</translation>
    </message>
    <message>
      <source>memcpy offset: %1 %2 %3</source>
      <translation>memcpyオフセット: %1 %2 %3</translation>
    </message>
    <message>
      <source>Initializing...</source>
      <translation>初期化中...</translation>
    </message>
    <message>
      <source>Scribus Development Version</source>
      <translation>Scribus 開発バージョン</translation>
    </message>
    <message>
      <source>&lt;p>You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p>Choose one of the following:&lt;br>&lt;ul>&lt;li>&lt;b>Create&lt;/b> missing pages&lt;/li>&lt;li>&lt;b>Import&lt;/b> pages until the last page&lt;/li>&lt;li>&lt;b>Cancel&lt;/b>&lt;/li>&lt;/ul></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>C&amp;reate</source>
      <translation>作成(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>インポート(&amp;I)</translation>
    </message>
    <message>
      <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
      <translation>ドキュメントの変更点が保存されておらず、取り消しを求められました。続けますか?</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>名前を付けて保存</translation>
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
      <source>c</source>
      <translation>c</translation>
    </message>
    <message>
      <source>Points (pt)</source>
      <translation>ポイント (pt)</translation>
    </message>
    <message>
      <source>Millimeters (mm)</source>
      <translation>ミリメートル(mm)</translation>
    </message>
    <message>
      <source>Inches (in)</source>
      <translation>インチ (in)</translation>
    </message>
    <message>
      <source>Picas (p)</source>
      <translation>パイカ (p)</translation>
    </message>
    <message>
      <source>Centimeters (cm)</source>
      <translation>センチメートル(cm)</translation>
    </message>
    <message>
      <source>Cicero (c)</source>
      <translation>シセロ (c)</translation>
    </message>
    <message>
      <source>File exists</source>
      <translation>ファイルは存在します</translation>
    </message>
    <message>
      <source>A file named '%1' already exists.&lt;br/>Do you want to replace it with the file you are saving?</source>
      <translation>ファイル名 '%1' は既に存在します。&lt;br/>保存しようとしているファイルで置き換えますか?</translation>
    </message>
    <message>
      <source>page</source>
      <comment>page export</comment>
      <translation>ページ</translation>
    </message>
    <message>
      <source>Color Wheel</source>
      <translation>カラーホイール</translation>
    </message>
    <message>
      <source>Font Preview</source>
      <translation>フォントプレビュー</translation>
    </message>
    <message>
      <source>My Plugin</source>
      <translation>自作プラグイン</translation>
    </message>
    <message>
      <source>New From Template</source>
      <translation>テンプレートから新規作成</translation>
    </message>
    <message>
      <source>Document Template: </source>
      <translation>ドキュメントテンプレート:</translation>
    </message>
    <message>
      <source>Newsletters</source>
      <translation>ニュースレター</translation>
    </message>
    <message>
      <source>Brochures</source>
      <translation>パンフレット</translation>
    </message>
    <message>
      <source>Catalogs</source>
      <translation>カタログ</translation>
    </message>
    <message>
      <source>Flyers</source>
      <translation>チラシ</translation>
    </message>
    <message>
      <source>Signs</source>
      <translation>看板</translation>
    </message>
    <message>
      <source>Cards</source>
      <translation>カード</translation>
    </message>
    <message>
      <source>Letterheads</source>
      <translation>レターヘッド</translation>
    </message>
    <message>
      <source>Envelopes</source>
      <translation>封筒</translation>
    </message>
    <message>
      <source>Business Cards</source>
      <translation>名刺</translation>
    </message>
    <message>
      <source>Calendars</source>
      <translation>カレンダー</translation>
    </message>
    <message>
      <source>Advertisements</source>
      <translation>広告</translation>
    </message>
    <message>
      <source>Labels</source>
      <translation>ラベル</translation>
    </message>
    <message>
      <source>Menus</source>
      <translation>メニュー</translation>
    </message>
    <message>
      <source>Programs</source>
      <translation>プログラム</translation>
    </message>
    <message>
      <source>PDF Forms</source>
      <translation>PDFフォーム</translation>
    </message>
    <message>
      <source>PDF Presentations</source>
      <translation>PDFプレゼンテーション</translation>
    </message>
    <message>
      <source>Magazines</source>
      <translation>雑誌</translation>
    </message>
    <message>
      <source>Posters</source>
      <translation>ポスター</translation>
    </message>
    <message>
      <source>Announcements</source>
      <translation>アナウンス</translation>
    </message>
    <message>
      <source>Text Documents</source>
      <translation>テキストドキュメント</translation>
    </message>
    <message>
      <source>Folds</source>
      <translation>折り目</translation>
    </message>
    <message>
      <source>Media Cases</source>
      <translation>メディアケース</translation>
    </message>
    <message>
      <source>Own Templates</source>
      <translation>自作のテンプレート</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Save As Template</source>
      <translation>テンプレートに名前を付けて保存</translation>
    </message>
    <message>
      <source>Cannot get a color with an empty name.</source>
      <comment>python error</comment>
      <translation>名前をつけずに色を取得することはできません</translation>
    </message>
    <message>
      <source>Color not found.</source>
      <comment>python error</comment>
      <translation>色が見つかりません</translation>
    </message>
    <message>
      <source>Cannot change a color with an empty name.</source>
      <comment>python error</comment>
      <translation>名前をつけずに色を変更することはできません</translation>
    </message>
    <message>
      <source>Color not found in document.</source>
      <comment>python error</comment>
      <translation>ドキュメント中に色が見つかりません</translation>
    </message>
    <message>
      <source>Color not found in default colors.</source>
      <comment>python error</comment>
      <translation>デフォルト色の中に色が見つかりません</translation>
    </message>
    <message>
      <source>Cannot create a color with an empty name.</source>
      <comment>python error</comment>
      <translation>名前をつけずに色を作成することはできません</translation>
    </message>
    <message>
      <source>Cannot delete a color with an empty name.</source>
      <comment>python error</comment>
      <translation>名前をつけずに色を削除することはできません</translation>
    </message>
    <message>
      <source>Cannot replace a color with an empty name.</source>
      <comment>python error</comment>
      <translation>名前をつけずに色を置き換えることはできません</translation>
    </message>
    <message>
      <source>firstPageOrder is bigger than allowed.</source>
      <comment>python error</comment>
      <translation>firstPageOrderが限度より大きな値です</translation>
    </message>
    <message>
      <source>Failed to open document.</source>
      <comment>python error</comment>
      <translation>ドキュメントを開くのに失敗しました</translation>
    </message>
    <message>
      <source>Failed to save document.</source>
      <comment>python error</comment>
      <translation>ドキュメントの保存に失敗しました</translation>
    </message>
    <message>
      <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
      <comment>python error</comment>
      <translation>単位が範囲を越えています。scribus.UNIT_*定数のうち一つを使用してください。</translation>
    </message>
    <message>
      <source>Color not found - python error</source>
      <comment>python error</comment>
      <translation>色が見つかりません - pythonエラー</translation>
    </message>
    <message>
      <source>Argument must be page item name, or PyCObject instance</source>
      <translation>引数はページアイテム名かPyCObjectインスタンスである必要があります。</translation>
    </message>
    <message>
      <source>Property not found</source>
      <translation>プロパティが見つかりません</translation>
    </message>
    <message>
      <source>Couldn't convert result type '%1'.</source>
      <translation>結果タイプ '%1' は変換できません</translation>
    </message>
    <message>
      <source>Property type '%1' not supported</source>
      <translation>プロパティタイプ '%1' はサポートされていません</translation>
    </message>
    <message>
      <source>Couldn't convert '%1' to property type '%2'</source>
      <translation>'%1' からプロパティタイプ '%2' に変換できません</translation>
    </message>
    <message>
      <source>Types matched, but setting property failed.</source>
      <translation>タイプは合っていますが、プロパティの設定に失敗しました。</translation>
    </message>
    <message>
      <source>Target is not an image frame.</source>
      <comment>python error</comment>
      <translation>ターゲットは画像フレームではありません</translation>
    </message>
    <message>
      <source>Specified item not an image frame.</source>
      <comment>python error</comment>
      <translation>指定されたアイテムは画像フレームではありません</translation>
    </message>
    <message>
      <source>Cannot group less than two items</source>
      <comment>python error</comment>
      <translation>アイテムが2個以下ではグループ化できません</translation>
    </message>
    <message>
      <source>Can't group less than two items</source>
      <comment>python error</comment>
      <translation>アイテムが2個以下ではグループ化できません</translation>
    </message>
    <message>
      <source>Need selection or argument list of items to group</source>
      <comment>python error</comment>
      <translation>グループ化するにはアイテムの選択か引数リストが必要です</translation>
    </message>
    <message>
      <source>Cannot scale by 0%.</source>
      <comment>python error</comment>
      <translation>スケールは0%にできません</translation>
    </message>
    <message>
      <source>Font not found.</source>
      <comment>python error</comment>
      <translation>フォントが見つかりません</translation>
    </message>
    <message>
      <source>Cannot render an empty sample.</source>
      <comment>python error</comment>
      <translation>サンプルなしでは描画できません</translation>
    </message>
    <message>
      <source>Unable to save pixmap</source>
      <comment>scripter error</comment>
      <translation>ピックスマップを保存できません</translation>
    </message>
    <message>
      <source>Cannot have an empty layer name.</source>
      <comment>python error</comment>
      <translation>レイヤー名なしにすることはできません</translation>
    </message>
    <message>
      <source>Layer not found.</source>
      <comment>python error</comment>
      <translation>レイヤーが見つかりません</translation>
    </message>
    <message>
      <source>Cannot remove the last layer.</source>
      <comment>python error</comment>
      <translation>最後のレイヤーは削除できません</translation>
    </message>
    <message>
      <source>Cannot create layer without a name.</source>
      <comment>python error</comment>
      <translation>名前のないレイヤーを作成することはできません</translation>
    </message>
    <message>
      <source>Point list must contain at least two points (four values).</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Point list must contain an even number of values.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Point list must contain at least three points (six values).</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Point list must contain at least four points (eight values).</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Point list must have a multiple of six values.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Object not found.</source>
      <comment>python error</comment>
      <translation>オブジェクトが見つかりません</translation>
    </message>
    <message>
      <source>Style not found.</source>
      <comment>python error</comment>
      <translation>スタイルが見つかりません</translation>
    </message>
    <message>
      <source>Cannot set style on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのスタイルを設定できません</translation>
    </message>
    <message>
      <source>Failed to save EPS.</source>
      <comment>python error</comment>
      <translation>EPSの保存に失敗しました</translation>
    </message>
    <message>
      <source>Page number out of range.</source>
      <comment>python error</comment>
      <translation>ページ番号が範囲を越えています</translation>
    </message>
    <message>
      <source>argument is not list: must be list of float values.</source>
      <comment>python error</comment>
      <translation>引数がリストではありません: 浮動少数の値のリストでなければなりません</translation>
    </message>
    <message>
      <source>argument contains non-numeric values: must be list of float values.</source>
      <comment>python error</comment>
      <translation>引数に数値以外の値が含まれています: 浮動少数の値のリストでなければなりません</translation>
    </message>
    <message>
      <source>argument contains no-numeric values: must be list of float values.</source>
      <comment>python error</comment>
      <translation>引数に数値以外の値が含まれています: 浮動少数の値のリストでなければなりません</translation>
    </message>
    <message>
      <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
      <comment>python error</comment>
      <translation>線幅が範囲を越えています。0&lt;=線幅&lt;=12でなければなりません。</translation>
    </message>
    <message>
      <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
      <comment>python error</comment>
      <translation>線の濃さが範囲を越えています。0&lt;=濃さ&lt;=100でなければなりません。</translation>
    </message>
    <message>
      <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
      <comment>python error</comment>
      <translation>塗りつぶし色の濃さが範囲を越えています。0&lt;=濃さ&lt;=100でなければなりません。</translation>
    </message>
    <message>
      <source>Corner radius must be a positive number.</source>
      <comment>python error</comment>
      <translation>コーナーの角度は正数でなければなりません</translation>
    </message>
    <message>
      <source>Line style not found.</source>
      <comment>python error</comment>
      <translation>線スタイルが見つかりません</translation>
    </message>
    <message>
      <source>Cannot get font size of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのフォントサイズを取得できません</translation>
    </message>
    <message>
      <source>Cannot get font of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのフォントを取得できません</translation>
    </message>
    <message>
      <source>Cannot get text size of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのテキストサイズを取得できません</translation>
    </message>
    <message>
      <source>Cannot get column count of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームの列数を取得できません</translation>
    </message>
    <message>
      <source>Cannot get line space of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームの行間隔を取得できません</translation>
    </message>
    <message>
      <source>Cannot get column gap of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームの列のギャップを取得できません</translation>
    </message>
    <message>
      <source>Cannot get text of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのテキストを取得できません</translation>
    </message>
    <message>
      <source>Cannot set text of non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのテキストを設定できません</translation>
    </message>
    <message>
      <source>Cannot insert text into non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームにテキストを挿入できません</translation>
    </message>
    <message>
      <source>Insert index out of bounds.</source>
      <comment>python error</comment>
      <translation>挿入されたインデックスが範囲を越えています</translation>
    </message>
    <message>
      <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
      <comment>python error</comment>
      <translation>配置が範囲を越えています。scribus.ALIGN*定数のうち一つを使用してください。</translation>
    </message>
    <message>
      <source>Cannot set text alignment on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームにテキスト配置を設定できません</translation>
    </message>
    <message>
      <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
      <comment>python error</comment>
      <translation>フォントサイズが範囲を越えています。 1&lt;=サイズ&lt;=512でなければなりません。</translation>
    </message>
    <message>
      <source>Cannot set font size on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのフォントサイズを設定できません</translation>
    </message>
    <message>
      <source>Cannot set font on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのフォントを設定できません</translation>
    </message>
    <message>
      <source>Line space out of bounds, must be >= 0.1.</source>
      <comment>python error</comment>
      <translation>行間隔が範囲を越えています。0.1以上にしてください。</translation>
    </message>
    <message>
      <source>Cannot set line spacing on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームに行間隔を設定できません</translation>
    </message>
    <message>
      <source>Column gap out of bounds, must be positive.</source>
      <comment>python error</comment>
      <translation>列のギャップが範囲を越えています。正の値でなければなりません。</translation>
    </message>
    <message>
      <source>Cannot set column gap on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームの列のギャップを設定できません</translation>
    </message>
    <message>
      <source>Column count out of bounds, must be > 1.</source>
      <comment>python error</comment>
      <translation>列数が範囲を越えています。1以上でなければなりません。</translation>
    </message>
    <message>
      <source>Cannot set number of columns on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームに行数を設定できません</translation>
    </message>
    <message>
      <source>Selection index out of bounds</source>
      <comment>python error</comment>
      <translation>選択されたインデックスが範囲を越えています</translation>
    </message>
    <message>
      <source>Cannot select text in a non-text frame</source>
      <comment>python error</comment>
      <translation>非テキストフレームでテキストを選択できません</translation>
    </message>
    <message>
      <source>Cannot delete text from a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームからテキストを削除できません。</translation>
    </message>
    <message>
      <source>Cannot set text fill on a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームにテキストの塗りつぶしを設定できません</translation>
    </message>
    <message>
      <source>Cannot set text stroke on a non-text frame.</source>
      <comment>python error</comment>
      <translation>テキストの輪郭は非テキストフレームには設定できません</translation>
    </message>
    <message>
      <source>Cannot set text shade on a non-text frame.</source>
      <comment>python error</comment>
      <translation>テキストの濃さは非テキストフレームには設定できません</translation>
    </message>
    <message>
      <source>Can only link text frames.</source>
      <comment>python error</comment>
      <translation>テキストフレームのみにリンクできます</translation>
    </message>
    <message>
      <source>Target frame links to another frame.</source>
      <comment>python error</comment>
      <translation>ターゲットフレームは他のフレームにリンクしています</translation>
    </message>
    <message>
      <source>Target frame is linked to by another frame.</source>
      <comment>python error</comment>
      <translation>ターゲットフレームは他のフレームによってリンクされています</translation>
    </message>
    <message>
      <source>Source and target are the same object.</source>
      <comment>python error</comment>
      <translation>ソースとターゲットが同じオブジェクトです</translation>
    </message>
    <message>
      <source>Cannot unlink a non-text frame.</source>
      <comment>python error</comment>
      <translation>非テキストフレームのリンクを解除できません</translation>
    </message>
    <message>
      <source>Object is not a linked text frame, can't unlink.</source>
      <comment>python error</comment>
      <translation>オブジェクトはテキストフレームにリンクされておらず、リンク解除できません</translation>
    </message>
    <message>
      <source>Cannot convert a non-text frame to outlines.</source>
      <comment>python error</comment>
      <translation>非テキストフレームをアウトラインに変換できません</translation>
    </message>
    <message>
      <source>Only text frames can be checked for overflowing</source>
      <comment>python error</comment>
      <translation>テキストフレームのみオーバーフローのチェックを行います</translation>
    </message>
    <message>
      <source>Can't set bookmark on a non-text frame</source>
      <comment>python error</comment>
      <translation>非テキストフレームにブックマークを設定できません</translation>
    </message>
    <message>
      <source>Can't get info from a non-text frame</source>
      <comment>python error</comment>
      <translation>非テキストフレームから情報を取得できません</translation>
    </message>
    <message>
      <source>The filename must be a string.</source>
      <comment>python error</comment>
      <translation>ファイル名は文字列でなければなりません</translation>
    </message>
    <message>
      <source>The filename should not be empty string.</source>
      <comment>python error</comment>
      <translation>ファイル名は空文字列ではいけません</translation>
    </message>
    <message>
      <source>Cannot delete image type settings.</source>
      <comment>python error</comment>
      <translation>画像タイプ設定を削除できません</translation>
    </message>
    <message>
      <source>The image type must be a string.</source>
      <comment>python error</comment>
      <translation>画像タイプは文字列でなければなりません</translation>
    </message>
    <message>
      <source>'allTypes' attribute is READ-ONLY</source>
      <comment>python error</comment>
      <translation>'全てのタイプ'の属性は読み込み専用です</translation>
    </message>
    <message>
      <source>Failed to export image</source>
      <comment>python error</comment>
      <translation>画像のエクスポートに失敗しました</translation>
    </message>
    <message>
      <source>&amp;Execute Script...</source>
      <translation>スクリプトを実行(&amp;E)...</translation>
    </message>
    <message>
      <source>Show &amp;Console</source>
      <translation>コンソールを表示(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;About Script...</source>
      <translation>スクリプトについて(&amp;A)...</translation>
    </message>
    <message>
      <source>&amp;Script</source>
      <translation>スクリプト(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Scribus Scripts</source>
      <translation>Scribusスクリプト(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Recent Scripts</source>
      <translation>最近開いたスクリプト(&amp;R)</translation>
    </message>
    <message>
      <source>About Script</source>
      <translation>スクリプトについて</translation>
    </message>
    <message>
      <source>Scripter</source>
      <translation>スクリプタ</translation>
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
      <translation type="unfinished" />
    </message>
    <message>
      <source>Custom (optional) configuration: </source>
      <comment>short words plugin</comment>
      <translation>カスタム(オプション)設定:</translation>
    </message>
    <message>
      <source>Standard configuration: </source>
      <comment>short words plugin</comment>
      <translation>標準設定:</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>ショートワード</translation>
    </message>
    <message>
      <source>Short Words processing. Wait please...</source>
      <comment>short words plugin</comment>
      <translation>ショートワードを処理しています。お待ちください...</translation>
    </message>
    <message>
      <source>Short Words processing. Done.</source>
      <comment>short words plugin</comment>
      <translation>ショートワードを処理しています。完了。</translation>
    </message>
    <message>
      <source>SVG Export</source>
      <translation>SVGエクスポート</translation>
    </message>
    <message>
      <source>SVG Import</source>
      <translation>SVGインポート</translation>
    </message>
    <message>
      <source>Old .sla format support</source>
      <translation>旧.slaフォーマットのサポート</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw Importer</source>
      <translation>OpenOffice.org Drawインポータ</translation>
    </message>
    <message>
      <source>Comma Separated Value Files</source>
      <translation>カンマ区切りの値のファイル</translation>
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
      <source>HTML Files</source>
      <translation>HTMLファイル</translation>
    </message>
    <message>
      <source>html</source>
      <translation>html</translation>
    </message>
    <message>
      <source>
External Links
</source>
      <translation>外部リンク</translation>
    </message>
    <message>
      <source>OpenDocument Text Documents</source>
      <translation>OpenDocument テキストドキュメント</translation>
    </message>
    <message>
      <source>OpenOffice.org Writer Documents</source>
      <translation>OpenOffice.org Writerドキュメント</translation>
    </message>
    <message>
      <source>Text Filters</source>
      <translation>テキストフィルタ</translation>
    </message>
    <message>
      <source>Text Files</source>
      <translation>テキストファイル</translation>
    </message>
    <message>
      <source>Japanese</source>
      <translation>日本語</translation>
    </message>
    <message>
      <source>Luxembourgish</source>
      <translation>ルクセンブルグ語</translation>
    </message>
    <message>
      <source>Font %1(%2) is broken</source>
      <translation>フォント %1(%2) は壊れています</translation>
    </message>
    <message>
      <source>Barcode Generator</source>
      <translation>バーコード生成</translation>
    </message>
    <message>
      <source>Given master page name does not match any existing.</source>
      <comment>python error</comment>
      <translation>指定されたマスターページは既存のものにマッチしません</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
      <translation>OpenOffice.org Draw (*.sxd *.odg);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Word Documents</source>
      <translation>Wordドキュメント</translation>
    </message>
    <message>
      <source>Palm PDB Documents</source>
      <comment>PDB Importer</comment>
      <translation>Palm PDBドキュメント</translation>
    </message>
    <message>
      <source>PDB_data</source>
      <comment>PDB Importer</comment>
      <translation>PDB_data</translation>
    </message>
    <message>
      <source>PDB Import</source>
      <comment>PDB Importer</comment>
      <translation>PDBインポート</translation>
    </message>
    <message>
      <source>Could not open file %1</source>
      <comment>PDB Importer</comment>
      <translation>ファイル%1を開けません</translation>
    </message>
    <message>
      <source>Arabic</source>
      <translation>アラビア</translation>
    </message>
    <message>
      <source>Dzongkha</source>
      <translation>ゾンカー語</translation>
    </message>
    <message>
      <source>Estonian</source>
      <translation>エストニア語</translation>
    </message>
    <message>
      <source>font %1 </source>
      <translation>フォント %1</translation>
    </message>
    <message>
      <source>size %1 </source>
      <translation>サイズ %1</translation>
    </message>
    <message>
      <source>+style </source>
      <translation>+スタイル　</translation>
    </message>
    <message>
      <source>+color </source>
      <translation>+色 </translation>
    </message>
    <message>
      <source>+underline </source>
      <translation>+下線 </translation>
    </message>
    <message>
      <source>-underline </source>
      <translation>-下線 </translation>
    </message>
    <message>
      <source>+strikeout </source>
      <translation>+打ち消し線 </translation>
    </message>
    <message>
      <source>-strikeout </source>
      <translation>-打ち消し線 </translation>
    </message>
    <message>
      <source>+shadow </source>
      <translation>+シャドゥ </translation>
    </message>
    <message>
      <source>-shadow </source>
      <translation>-シャドゥ </translation>
    </message>
    <message>
      <source>+outline </source>
      <translation>+アウトライン </translation>
    </message>
    <message>
      <source>-outline </source>
      <translation>-アウトライン </translation>
    </message>
    <message>
      <source>-tracking </source>
      <translation>-トラッキング </translation>
    </message>
    <message>
      <source>+stretch </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>parent= %1</source>
      <translation>親 = %1</translation>
    </message>
    <message>
      <source>Latin</source>
      <translation>ラテン語</translation>
    </message>
    <message>
      <source>Icelandic</source>
      <translation>アイスランド語</translation>
    </message>
    <message>
      <source>Romanian</source>
      <translation>ルーマニア語</translation>
    </message>
    <message>
      <source>Quarto</source>
      <translation>クアトロ</translation>
    </message>
    <message>
      <source>Foolscap</source>
      <translation>フールスキャップ</translation>
    </message>
    <message>
      <source>Letter</source>
      <translation>レター</translation>
    </message>
    <message>
      <source>Govt. Letter</source>
      <translation>行政レター</translation>
    </message>
    <message>
      <source>Legal</source>
      <translation>リーガル</translation>
    </message>
    <message>
      <source>Ledger</source>
      <translation>レジャー</translation>
    </message>
    <message>
      <source>Executive</source>
      <translation>エグゼクティブ</translation>
    </message>
    <message>
      <source>Post</source>
      <translation>ポスト</translation>
    </message>
    <message>
      <source>Crown</source>
      <translation>クラウン</translation>
    </message>
    <message>
      <source>Large Post</source>
      <translation>ラージポスト</translation>
    </message>
    <message>
      <source>Demy</source>
      <translation>ドゥミ</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>中</translation>
    </message>
    <message>
      <source>Royal</source>
      <translation>ロイヤル</translation>
    </message>
    <message>
      <source>Elephant</source>
      <translation>エレファント</translation>
    </message>
    <message>
      <source>Double Demy</source>
      <translation>ダブルドゥミ</translation>
    </message>
    <message>
      <source>Quad Demy</source>
      <translation>クワドドゥミ</translation>
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
      <translation>%1 はおそらく破損しています : 不明な解像度タグ</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2</source>
      <translation>フォント %1 に壊れたグリフ %2 が含まれています</translation>
    </message>
    <message>
      <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
      <comment>python error</comment>
      <translation>透明度が範囲を越えています。0 &lt;= 透明度 &lt;= 1である必要があります。</translation>
    </message>
    <message>
      <source>Blendmode out of bounds, must be 0 &lt;= blendmode &lt;= 15.</source>
      <comment>python error</comment>
      <translation>合成モードが範囲を越えています。0 &lt;= 合成モード &lt;= 15である必要があります。</translation>
    </message>
    <message>
      <source>Scribus 1.2.x Support</source>
      <translation>Scribus 1.2.x サポート</translation>
    </message>
    <message>
      <source>Scribus 1.3.4 Support</source>
      <translation>Scribus 1.3.4 サポート</translation>
    </message>
    <message>
      <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
      <comment>PDB Importer</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot get number of lines of non-text frame.</source>
      <comment>python error</comment>
      <translation>テキストフレーム以外では行数を取得できません。</translation>
    </message>
    <message>
      <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+tracking %1 </source>
      <translation>+トラッキング %1 </translation>
    </message>
    <message>
      <source>+baseline %1 </source>
      <translation>+ベースライン %1 </translation>
    </message>
    <message>
      <source>Breton</source>
      <translation>ブリタニー語</translation>
    </message>
    <message>
      <source>English (American)</source>
      <translation>英語 (米国)</translation>
    </message>
    <message>
      <source>%1 may be corrupted : missing or wrong resolution tags</source>
      <translation>%1 はおそらく破損しています : 不明もしくは不正な解像度タグ</translation>
    </message>
    <message>
      <source>The Font(s):
%1 are not available.
They have been replaced by &quot;Courier&quot;
Therefore the image may be not correct</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>English (Australian)</source>
      <translation>英語 (オーストラリア)</translation>
    </message>
    <message>
      <source>German (Swiss)</source>
      <translation>ドイツ語 (スイス)</translation>
    </message>
    <message>
      <source>Hebrew</source>
      <translation>ヘブライ</translation>
    </message>
    <message>
      <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
      <translation>ファイル %2 に含まれるフォント %1 はメトリックスが壊れているので、メトリックスを無視します</translation>
    </message>
    <message>
      <source>Chinese (Trad.)</source>
      <translation>中国語 (繁体字)</translation>
    </message>
    <message>
      <source>Scribus 1.3.0->1.3.3.x Support</source>
      <translation>Scribus 1.3.0->1.3.3.x サポート</translation>
    </message>
    <message>
      <source>Copy of %1 (%2)</source>
      <translation>%1のコピー (%2)</translation>
    </message>
    <message>
      <source>%</source>
      <translation>%</translation>
    </message>
    <message>
      <source>Khmer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lao</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vietnamese</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>An error occurred while initializing icc transforms</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Output profile is not supported</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>WMF Import</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>New Layer</source>
      <translation>新規レイヤー</translation>
    </message>
    <message>
      <source>Adobe Illustrator Importer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imposition</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript Importer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation>%1;;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Do you really want to overwrite the file:
%1 ?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Encapsulated PostScript</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>GIF</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>JPEG</source>
      <translation>JPEG</translation>
    </message>
    <message>
      <source>Pattern Files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Document</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PNG</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Adobe Photoshop</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>TIFF</source>
      <translation>TIFF</translation>
    </message>
    <message>
      <source>XPM</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Windows Meta File</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scalable Vector Graphics</source>
      <translation>スケーラブルベクトルグラフィックス</translation>
    </message>
    <message>
      <source>Adobe Illustrator</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Valid metrics were found for font %1, using metrics in file %2</source>
      <translation>ファイル %2 中で、メトリックスを使用しているフォント %1 に合った有効なメトリックスが見つかりました</translation>
    </message>
    <message>
      <source>No metrics found for font %1, ignoring font</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error</source>
      <translation>エラー</translation>
    </message>
    <message>
      <source>second argument is not tuple: must be tuple of int values.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>second argument contains non-numeric values: must be list of int values.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot have an empty paragraph style name.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>hasdropcap has to be 0 or 1.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot have an empty char style name.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Can only hyphenate text frame</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Can only dehyphenate text frame</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lens Effects</source>
      <translation>レンズ効果</translation>
    </message>
    <message>
      <source>PathCutter</source>
      <translation>PathCutter</translation>
    </message>
    <message>
      <source>PathFinder</source>
      <translation>PathFinder</translation>
    </message>
    <message>
      <source>PathStroker</source>
      <translation>PathStroker</translation>
    </message>
    <message>
      <source>Spell check (aspell)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Subdivide</source>
      <translation>再分割</translation>
    </message>
    <message>
      <source>Xfig Importer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Xfig File</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Parsing the configfile %1 failed! Depending on the type of the error render frames might not work correctly!
%2</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>MeshDistortion</source>
      <translation>MeshDistortion</translation>
    </message>
    <message>
      <source>PathAlongPath</source>
      <translation>PathAlongPath</translation>
    </message>
    <message>
      <source>Transform Effect</source>
      <translation>変形効果</translation>
    </message>
    <message>
      <source>°</source>
      <comment>degrees, unicode 0xB0</comment>
      <translation>°</translation>
    </message>
    <message>
      <source>Norwegian (Bokmål)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot get text distances of non-text frame.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Text distances out of bounds, must be positive.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot set text distances on a non-text frame.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>QTextEdit</name>
    <message>
      <source>Clear</source>
      <translation>クリア</translation>
    </message>
    <message>
      <source>Select All</source>
      <translation>全て選択</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>元に戻す(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>やり直し(&amp;R)</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>切り取り(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>貼り付け(&amp;P)</translation>
    </message>
  </context>
  <context>
    <name>QTitleBar</name>
    <message>
      <source>System Menu</source>
      <translation>システムメニュー</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>シェード</translation>
    </message>
    <message>
      <source>Unshade</source>
      <translation>シェードを解除</translation>
    </message>
    <message>
      <source>Normalize</source>
      <translation>正規化</translation>
    </message>
    <message>
      <source>Minimize</source>
      <translation>最小化</translation>
    </message>
    <message>
      <source>Maximize</source>
      <translation>最大化</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>閉じる</translation>
    </message>
  </context>
  <context>
    <name>QWorkspace</name>
    <message>
      <source>&amp;Restore</source>
      <translation>修復(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Move</source>
      <translation>移動(&amp;M)</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>サイズ(&amp;S)</translation>
    </message>
    <message>
      <source>Mi&amp;nimize</source>
      <translation>最小化(&amp;N)</translation>
    </message>
    <message>
      <source>Ma&amp;ximize</source>
      <translation>最大化(&amp;X)</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
      <source>Stay on &amp;Top</source>
      <translation>最前面に置く(&amp;T)</translation>
    </message>
    <message>
      <source>Minimize</source>
      <translation>最小化</translation>
    </message>
    <message>
      <source>Restore Down</source>
      <translation>元のサイズに戻す</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>閉じる</translation>
    </message>
    <message>
      <source>Sh&amp;ade</source>
      <translation>シェード(&amp;A)</translation>
    </message>
    <message>
      <source>%1 - [%2]</source>
      <translation>%1 - [%2]</translation>
    </message>
    <message>
      <source>&amp;Unshade</source>
      <translation>非シェード(&amp;U)</translation>
    </message>
  </context>
  <context>
    <name>ReformDoc</name>
    <message>
      <source>Document Setup</source>
      <translation>ドキュメントセットアップ</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>ドキュメント</translation>
    </message>
    <message>
      <source>Document Information</source>
      <translation>ドキュメント情報</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>ガイド</translation>
    </message>
    <message>
      <source>Display</source>
      <translation>表示</translation>
    </message>
    <message>
      <source>Typography</source>
      <translation>タイポグラフィ</translation>
    </message>
    <message>
      <source>Tools</source>
      <translation>ツール</translation>
    </message>
    <message>
      <source>Hyphenator</source>
      <translation>ハイフン</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>フォント</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>プリフライト検証</translation>
    </message>
    <message>
      <source>PDF Export</source>
      <translation>PDF出力</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>ドキュメントアイテム属性</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>目次とインデックス</translation>
    </message>
    <message>
      <source>Sections</source>
      <translation>セクション</translation>
    </message>
    <message>
      <source>Color Management</source>
      <translation>カラーマネージメント</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>色を調整中</translation>
    </message>
  </context>
  <context>
    <name>RunScriptDialog</name>
    <message>
      <source>Dialog</source>
      <translation>ダイアログ</translation>
    </message>
    <message>
      <source>Run as Extension Script</source>
      <translation>拡張スクリプトとして実行</translation>
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
      <translation>上書きする代わりに親の整列スタイルを使用</translation>
    </message>
  </context>
  <context>
    <name>SMCStyleWidget</name>
    <message>
      <source>Based On:</source>
      <translation>ベース:</translation>
    </message>
    <message>
      <source>Language:</source>
      <translation>言語:</translation>
    </message>
    <message>
      <source>Basic Formatting</source>
      <translation>基本的なフォーマット</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Advanced Formatting</source>
      <translation>詳細なフォーマット</translation>
    </message>
    <message>
      <source>TextLabel</source>
      <translation>TextLabel</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>色</translation>
    </message>
  </context>
  <context>
    <name>SMCharacterStyle</name>
    <message>
      <source>Properties</source>
      <translation>プロパティ</translation>
    </message>
    <message>
      <source>Character Styles</source>
      <translation>文字スタイル</translation>
    </message>
    <message>
      <source>Character Style</source>
      <translation>文字スタイル</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>新規スタイル</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>%1のコピー</translation>
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
      <translation>親の値を使用</translation>
    </message>
  </context>
  <context>
    <name>SMFontComboH</name>
    <message>
      <source>Use Parent Font</source>
      <translation>親のフォントを使用</translation>
    </message>
  </context>
  <context>
    <name>SMLineStyle</name>
    <message>
      <source>Properties</source>
      <translation>プロパティ</translation>
    </message>
    <message>
      <source>Line Styles</source>
      <translation>線スタイル</translation>
    </message>
    <message>
      <source>Line Style</source>
      <translation>線スタイル</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>新規スタイル</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>%1のコピー</translation>
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
      <translation>実線</translation>
    </message>
    <message>
      <source>Dashed Line</source>
      <translation>破線</translation>
    </message>
    <message>
      <source>Dotted Line</source>
      <translation>点線</translation>
    </message>
    <message>
      <source>Dash Dot Line</source>
      <translation>破点線</translation>
    </message>
    <message>
      <source>Dash Dot Dot Line</source>
      <translation>破点点線</translation>
    </message>
    <message>
      <source> pt </source>
      <translation> pt </translation>
    </message>
  </context>
  <context>
    <name>SMLineStyleWidget</name>
    <message>
      <source>%</source>
      <translation>%</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>線幅:</translation>
    </message>
  </context>
  <context>
    <name>SMPStyleWidget</name>
    <message>
      <source>Distances and Alignment</source>
      <translation>距離と配置</translation>
    </message>
    <message>
      <source>Drop Caps</source>
      <translation>ドロップキャップ</translation>
    </message>
    <message>
      <source>Tabulators and Indentation</source>
      <translation>タブとインデント</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>プロパティ</translation>
    </message>
    <message>
      <source>&amp;Lines:</source>
      <translation>行数(&amp;L):</translation>
    </message>
    <message>
      <source>Distance from Text:</source>
      <translation>テキストからの距離:</translation>
    </message>
    <message>
      <source>Based On:</source>
      <translation>ベース:</translation>
    </message>
    <message>
      <source>TextLabel</source>
      <translation>TextLabel</translation>
    </message>
    <message>
      <source>Parent's Drop Cap Status</source>
      <translation>親のドロップキャップ状態</translation>
    </message>
    <message>
      <source>Ch&amp;aracter Style</source>
      <translation>文字スタイル(&amp;A)</translation>
    </message>
  </context>
  <context>
    <name>SMParagraphStyle</name>
    <message>
      <source>Paragraph Styles</source>
      <translation>段落スタイル</translation>
    </message>
    <message>
      <source>Paragraph Style</source>
      <translation>段落スタイル</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>新規スタイル</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>%1のコピー</translation>
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
      <source>Delete Styles</source>
      <translation>スタイルを削除</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>Ca&amp;ncel</source>
      <translation>キャンセル(&amp;N)</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
  </context>
  <context>
    <name>SMScComboBox</name>
    <message>
      <source>Use Parent Value</source>
      <translation>親の値を使用</translation>
    </message>
  </context>
  <context>
    <name>SMShadeButton</name>
    <message>
      <source>Use Parent Value</source>
      <translation>親の値を使用</translation>
    </message>
  </context>
  <context>
    <name>SMStyleImport</name>
    <message>
      <source>Choose Styles</source>
      <translation>スタイルを選択</translation>
    </message>
    <message>
      <source>Available Styles</source>
      <translation>利用可能なスタイル</translation>
    </message>
    <message>
      <source>In case of the name clash</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Rename Imported Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>R&amp;eplace Existing Style</source>
      <translation type="unfinished" />
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
      <translation>上書きする代わりに親のスタイル効果を使用</translation>
    </message>
  </context>
  <context>
    <name>SMTabruler</name>
    <message>
      <source> Parent Tabs </source>
      <translation> 親タブ </translation>
    </message>
  </context>
  <context>
    <name>SToolBAlign</name>
    <message>
      <source>Style Settings</source>
      <translation>スタイル設定</translation>
    </message>
    <message>
      <source>Style of current paragraph</source>
      <translation>現在の段落のスタイル</translation>
    </message>
  </context>
  <context>
    <name>SToolBColorF</name>
    <message>
      <source>Fill Color Settings</source>
      <translation>塗りつぶし色の設定</translation>
    </message>
    <message>
      <source>Color of text fill</source>
      <translation>テキストの塗りつぶし色</translation>
    </message>
    <message>
      <source>Saturation of color of text fill</source>
      <translation>テキストの塗りつぶし色の彩度</translation>
    </message>
  </context>
  <context>
    <name>SToolBColorS</name>
    <message>
      <source>Stroke Color Settings</source>
      <translation>輪郭色の設定</translation>
    </message>
    <message>
      <source>Color of text stroke</source>
      <translation>テキストの輪郭色</translation>
    </message>
    <message>
      <source>Saturation of color of text stroke</source>
      <translation>テキストの輪郭色の彩度</translation>
    </message>
  </context>
  <context>
    <name>SToolBFont</name>
    <message>
      <source>Font Settings</source>
      <translation>フォント設定</translation>
    </message>
    <message>
      <source>Font of selected text</source>
      <translation>選択されたテキストのフォント</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>フォントサイズ</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>文字幅の倍率</translation>
    </message>
    <message>
      <source>Scaling height of characters</source>
      <translation>文字高さの倍率</translation>
    </message>
  </context>
  <context>
    <name>SToolBStyle</name>
    <message>
      <source>Character Settings</source>
      <translation>文字設定</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>手動トラッキング</translation>
    </message>
  </context>
  <context>
    <name>SVGExportPlugin</name>
    <message>
      <source>Exports SVG Files</source>
      <translation>SVGファイルをエクスポート</translation>
    </message>
    <message>
      <source>Exports the current page into an SVG file.</source>
      <translation>現在のページをSVGファイルにエクスポート</translation>
    </message>
    <message>
      <source>Save as &amp;SVG...</source>
      <translation>SVGとして保存(&amp;S)...</translation>
    </message>
  </context>
  <context>
    <name>SVGImportPlugin</name>
    <message>
      <source>Import &amp;SVG...</source>
      <translation>SVGをインポート(&amp;S)...</translation>
    </message>
    <message>
      <source>Imports SVG Files</source>
      <translation>SVGファイルをインポート</translation>
    </message>
    <message>
      <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>現在のドキュメントに、ベクトルデータをScribusオブジェクトに変換して、ほとんどのSVGファイルをインポートします。</translation>
    </message>
    <message>
      <source>SVG file contains some unsupported features</source>
      <translation>SVGファイルはサポートされていない機能を含んでいます</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>ファイルがインポートできません</translation>
    </message>
  </context>
  <context>
    <name>SVGPlug</name>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
  </context>
  <context>
    <name>SWDialog</name>
    <message>
      <source>Short Words</source>
      <comment>short words plugin</comment>
      <translation>ショートワード</translation>
    </message>
    <message>
      <source>Apply unbreakable space on:</source>
      <comment>short words plugin</comment>
      <translation>改行なしスペースを以下に適用:</translation>
    </message>
    <message>
      <source>&amp;Selected frames</source>
      <comment>short words plugin</comment>
      <translation>選択されたフレーム(&amp;S)</translation>
    </message>
    <message>
      <source>Active &amp;page</source>
      <comment>short words plugin</comment>
      <translation>アクティブなページ(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;All items</source>
      <comment>short words plugin</comment>
      <translation>全てのアイテム(&amp;A)</translation>
    </message>
    <message>
      <source>Only selected frames processed.</source>
      <comment>short words plugin</comment>
      <translation>選択されたフレームのみ処理しました</translation>
    </message>
    <message>
      <source>Only actual page processed.</source>
      <comment>short words plugin</comment>
      <translation>実際のページのみ処理しました</translation>
    </message>
    <message>
      <source>All items in document processed.</source>
      <comment>short words plugin</comment>
      <translation>ドキュメント中の全てのアイテムを処理しました</translation>
    </message>
  </context>
  <context>
    <name>SWPrefsGui</name>
    <message>
      <source>User settings</source>
      <translation>ユーザ設定</translation>
    </message>
    <message>
      <source>System wide configuration</source>
      <translation>システム全般の設定</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>保存(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>リセット(&amp;R)</translation>
    </message>
    <message>
      <source>Save user configuration</source>
      <translation>ユーザ設定を保存</translation>
    </message>
    <message>
      <source>Reload system wide configuration and remove user defined one</source>
      <translation>システム全般の設定を再読み込みしてユーザ定義の設定を削除する</translation>
    </message>
    <message>
      <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
      <translation>カスタム設定を編集します。保存すると、システム全体での設定として使用されます。</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>ショートワード</translation>
    </message>
    <message>
      <source>Cannot write file %1.</source>
      <translation>ファイル %1 の書き込みができません</translation>
    </message>
    <message>
      <source>User settings saved</source>
      <translation>ユーザ設定を保存しました</translation>
    </message>
    <message>
      <source>System wide configuration reloaded</source>
      <translation>システム全般の設定を再読み込みしました</translation>
    </message>
    <message>
      <source>Cannot open file %1</source>
      <translation>ファイル %1 が開けません</translation>
    </message>
    <message>
      <source>User configuration exists already. Do you really want to overwrite it?</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>SaveAsTemplatePlugin</name>
    <message>
      <source>Save as &amp;Template...</source>
      <translation>テンプレートとして保存(&amp;T)...</translation>
    </message>
    <message>
      <source>Save a document as a template</source>
      <translation>ドキュメントをテンプレートとして保存</translation>
    </message>
    <message>
      <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ScGTFileDialog</name>
    <message>
      <source>Select a file to import</source>
      <translation>インポートするファイルを選択</translation>
    </message>
    <message>
      <source>Append</source>
      <translation>追加</translation>
    </message>
    <message>
      <source>Show options</source>
      <translation>オプションを表示</translation>
    </message>
  </context>
  <context>
    <name>ScInputDialog</name>
    <message>
      <source>Input Dialog</source>
      <translation>ダイアログに入力</translation>
    </message>
    <message>
      <source>InputDialog</source>
      <translation>入力ダイアログ</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
    </message>
  </context>
  <context>
    <name>ScPlugin</name>
    <message>
      <source>Load/Save/Import/Export</source>
      <translation>読み込み/保存/インポート/エクスポート</translation>
    </message>
    <message>
      <source>Persistent</source>
      <comment>plugin manager plugin type</comment>
      <translation>保護</translation>
    </message>
    <message>
      <source>Action</source>
      <comment>plugin manager plugin type</comment>
      <translation>アクション</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>不明</translation>
    </message>
  </context>
  <context>
    <name>ScPrintEngine_GDI</name>
    <message>
      <source>Printing...</source>
      <translation>印刷中...</translation>
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
    <name>Scribus12Format</name>
    <message>
      <source>Scribus 1.2.x Document</source>
      <translation>Scribus 1.2.x ドキュメント</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>#%1のコピー</translation>
    </message>
  </context>
  <context>
    <name>Scribus134Format</name>
    <message>
      <source>Scribus 1.3.4 Document</source>
      <translation>Scribus 1.3.4 ドキュメント</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>#%1のコピー</translation>
    </message>
  </context>
  <context>
    <name>Scribus13Format</name>
    <message>
      <source>Copy #%1 of </source>
      <translation>#%1のコピー</translation>
    </message>
    <message>
      <source>Scribus 1.3.0->1.3.3.7 Document</source>
      <translation>Scribus 1.3.0->1.3.3.7 ドキュメント</translation>
    </message>
  </context>
  <context>
    <name>ScribusCore</name>
    <message>
      <source>Initializing Plugins</source>
      <translation>プラグインを初期化中</translation>
    </message>
    <message>
      <source>Initializing Keyboard Shortcuts</source>
      <translation>キーボードショートカットを初期化中</translation>
    </message>
    <message>
      <source>Reading Preferences</source>
      <translation>設定を読み込み中</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>フォントを検索中</translation>
    </message>
    <message>
      <source>There are no fonts found on your system.</source>
      <translation>システム上にフォントがありません</translation>
    </message>
    <message>
      <source>Exiting now.</source>
      <translation>終了しています</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>致命的なエラー</translation>
    </message>
    <message>
      <source>Font System Initialized</source>
      <translation>フォントシステムを初期化</translation>
    </message>
    <message>
      <source>Reading Color Profiles</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ScribusDoc</name>
    <message>
      <source>Document</source>
      <translation>ドキュメント</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>背景</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>本当に全てのテキストを消去しますか?</translation>
    </message>
    <message>
      <source>Cannot Delete In-Use Item</source>
      <translation>使用中のアイテムは削除できません</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
      <translation>アイテム %1 は、現在ストーリエディタで編集されています。削除はキャンセルされました。</translation>
    </message>
    <message>
      <source>Some objects are locked.</source>
      <translation>オブジェクトがロックされています</translation>
    </message>
    <message>
      <source>&amp;Unlock All</source>
      <translation>全てロック解除(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Skip locked objects</source>
      <translation>ロックされたオブジェクトを飛ばす(&amp;S)</translation>
    </message>
    <message>
      <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
      <translation>ICCプロファイルを開く際にエラーが発生しました。カラーマネージメントは有効ではありません。</translation>
    </message>
    <message>
      <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
      <translation>印刷部数: %1
水平ギャップ: %2
垂直ギャップ: %3</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>色を調整中</translation>
    </message>
    <message>
      <source>remove direct paragraph formatting</source>
      <translation>段落フォーマットを削除</translation>
    </message>
    <message>
      <source>remove direct char formatting</source>
      <translation>文字フォーマットを削除</translation>
    </message>
    <message>
      <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3
Rotation: %4</source>
      <translation>部数: %1
水平移動: %2
垂直移動: %3
回転: %4</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
    <message>
      <source>Copy_of_</source>
      <translation>以下のコピー </translation>
    </message>
  </context>
  <context>
    <name>ScribusMainWindow</name>
    <message>
      <source>Initializing Story Editor</source>
      <translation>ストーリエディタを初期化中</translation>
    </message>
    <message>
      <source>Initializing Hyphenator</source>
      <translation>ハイフンを初期化中</translation>
    </message>
    <message>
      <source>Reading Scrapbook</source>
      <translation>スクラップブックを読み込み中</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>致命的なエラー</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
      <source>Open &amp;Recent</source>
      <translation>最近のファイルを開く(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>インポート(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Export</source>
      <translation>エクスポート(&amp;E)</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>編集(&amp;E)</translation>
    </message>
    <message>
      <source>&amp;Item</source>
      <translation>アイテム(&amp;I)</translation>
    </message>
    <message>
      <source>Preview Settings</source>
      <translation>プレビュー設定</translation>
    </message>
    <message>
      <source>Level</source>
      <translation>レベル</translation>
    </message>
    <message>
      <source>Send to La&amp;yer</source>
      <translation>レイヤーに送る(&amp;Y)</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>PDFオプション(&amp;P)</translation>
    </message>
    <message>
      <source>C&amp;onvert To</source>
      <translation>変換(&amp;O)</translation>
    </message>
    <message>
      <source>I&amp;nsert</source>
      <translation>挿入(&amp;N)</translation>
    </message>
    <message>
      <source>Character</source>
      <translation>文字</translation>
    </message>
    <message>
      <source>Quote</source>
      <translation>引用符</translation>
    </message>
    <message>
      <source>Space</source>
      <translation>空白</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>ページ(&amp;P)</translation>
    </message>
    <message>
      <source>&amp;View</source>
      <translation>ビュー(&amp;V)</translation>
    </message>
    <message>
      <source>E&amp;xtras</source>
      <translation>拡張(&amp;X)</translation>
    </message>
    <message>
      <source>&amp;Windows</source>
      <translation>ウィンドウ(&amp;W)</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation>ヘルプ(&amp;H)</translation>
    </message>
    <message>
      <source>&amp;Alignment</source>
      <translation>配置(&amp;A)</translation>
    </message>
    <message>
      <source>Ready</source>
      <translation>準備完了</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Importing Pages...</source>
      <translation>ページをインポート中...</translation>
    </message>
    <message>
      <source>Import Page(s)</source>
      <translation>ページをインポート</translation>
    </message>
    <message>
      <source>Import done</source>
      <translation>インポート完了</translation>
    </message>
    <message>
      <source>Found nothing to import</source>
      <translation>インポートするものが見つかりません</translation>
    </message>
    <message>
      <source>File %1 is not in an acceptable format</source>
      <translation>ファイル %1 は受け入れ可能なフォーマットではありません</translation>
    </message>
    <message>
      <source>Loading...</source>
      <translation>読み込み中...</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation> は以下で置換されます: </translation>
    </message>
    <message>
      <source>(converted)</source>
      <translation>（変換済み）</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>サポートされている全てのフォーマット</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>ファイルを書き込めません: 
%1</translation>
    </message>
    <message>
      <source>Save As</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>Saving...</source>
      <translation>保存中...</translation>
    </message>
    <message>
      <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
      <translation>Scribusはいくつかのエラーを検知しました。修正するにはプリフライト検証の利用を考えてください。</translation>
    </message>
    <message>
      <source>Printing...</source>
      <translation>印刷中...</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>ドキュメント</translation>
    </message>
    <message>
      <source>Printing failed!</source>
      <translation>印刷に失敗しました!</translation>
    </message>
    <message>
      <source>Cannot Cut In-Use Item</source>
      <translation>使用中のアイテムは切り取りできません</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
      <translation>アイテム %1 は、現在ストーリエディタで編集されています。切り取りはキャンセルされました。</translation>
    </message>
    <message>
      <source>About Qt</source>
      <translation>Qtについて</translation>
    </message>
    <message>
      <source>Scribus Manual</source>
      <translation>Scribusマニュアル</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>テキストファイル (*.txt);;全てのファイル(*)</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>サイズ(&amp;S):</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>サイズ</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>色の濃さ(&amp;S):</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>色の濃さ</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>スタイルなし</translation>
    </message>
    <message>
      <source>The following programs are missing:</source>
      <translation>以下のプログラムが見つかりません:</translation>
    </message>
    <message>
      <source>Ghostscript : You cannot use EPS images or Print Preview</source>
      <translation>Ghostscript : EPS画像や印刷プレビューは使用できません</translation>
    </message>
    <message>
      <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
      <translation>Ghostscript : EPS画像やPostScript印刷プレビューは使用できません</translation>
    </message>
    <message>
      <source>Ghostscript is missing : Postscript Print Preview is not available</source>
      <translation>Ghostscriptが見つかりません : Postscript印刷プレビューは利用できません</translation>
    </message>
    <message>
      <source>All</source>
      <translation>全て</translation>
    </message>
    <message>
      <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
      <translation>エラーを検知しました。 
これらを訂正するためにプリフライト検証を使用することを考えてください。</translation>
    </message>
    <message>
      <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
      <translation>エラーを検知しました。 
これらを訂正するためにプリフライト検証を使用することを考えてください。</translation>
    </message>
    <message>
      <source>-Page%1</source>
      <translation> ページ%1</translation>
    </message>
    <message>
      <source>Some objects are locked.</source>
      <translation>オブジェクトがロックされています</translation>
    </message>
    <message>
      <source>&amp;Lock All</source>
      <translation>全てロック(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Unlock All</source>
      <translation>全てロック解除(&amp;U)</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>情報</translation>
    </message>
    <message>
      <source>The program %1 is already running!</source>
      <translation>プログラム %1 はすでに実行しています!</translation>
    </message>
    <message>
      <source>The program %1 is missing!</source>
      <translation>プログラム %1 が見つかりません!</translation>
    </message>
    <message>
      <source>The selected color does not exist in the document's color set. Please enter a name for this new color.</source>
      <translation>選択された色はドキュメントのカラーセットの中に存在しません。この新しい色の名前を入力してください。</translation>
    </message>
    <message>
      <source>Color Not Found</source>
      <translation>色が見つかりません</translation>
    </message>
    <message>
      <source>The name you have selected already exists. Please enter a different name for this new color.</source>
      <translation>選択された名前はすでに使用されています。この新しい色に別の名前を入力してください。</translation>
    </message>
    <message>
      <source>&amp;Level</source>
      <translation>レベル(&amp;L)</translation>
    </message>
    <message>
      <source>Send to Layer</source>
      <translation>レイヤーに送る</translation>
    </message>
    <message>
      <source>Previe&amp;w Settings</source>
      <translation>プレビュー設定(&amp;W)</translation>
    </message>
    <message>
      <source>&amp;Tools</source>
      <translation>ツール(&amp;T)</translation>
    </message>
    <message>
      <source>X-Pos:</source>
      <translation>位置X:</translation>
    </message>
    <message>
      <source>Y-Pos:</source>
      <translation>位置Y:</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>目次</translation>
    </message>
    <message>
      <source>&amp;Character</source>
      <translation>文字(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Quote</source>
      <translation>引用符(&amp;Q)</translation>
    </message>
    <message>
      <source>S&amp;paces &amp;&amp; Breaks</source>
      <translation>スペースと改行(&amp;P)</translation>
    </message>
    <message>
      <source>Liga&amp;ture</source>
      <translation>リガチャ(&amp;T)</translation>
    </message>
    <message>
      <source>Do you really want to replace your existing image?</source>
      <translation>本当に既存の画像を置き換えますか?</translation>
    </message>
    <message>
      <source>Paste Recent</source>
      <translation>最近の貼り付け</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz);;All Files (*)</source>
      <translation>ドキュメント (*.sla *.sla.gz);;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>本当に全てのテキストを消去しますか?</translation>
    </message>
    <message>
      <source>Online &amp;Tutorials</source>
      <translation>オンラインチュートリアル(&amp;T)</translation>
    </message>
    <message>
      <source>Some color profiles used by this document are not installed:</source>
      <translation>このドキュメントで使用されているいくつかのカラープロファイルはインストールされていません:</translation>
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation>%1;;全てのファイル (*)</translation>
    </message>
    <message>
      <source>Applying User Shortcuts</source>
      <translation>ユーザショートカットを適用しています</translation>
    </message>
    <message>
      <source>Updating Images</source>
      <translation>画像をアップデート</translation>
    </message>
    <message>
      <source>Your document was saved to a temporary file and could not be moved: 
%1</source>
      <translation>ドキュメントは一時ファイルに保存されていて移動できません: %1</translation>
    </message>
    <message>
      <source>Print engine initialization failed</source>
      <translation>プリントエンジンの初期化に失敗しました。</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>名前(&amp;N):</translation>
    </message>
    <message>
      <source>New Entry</source>
      <translation>新規エントリ</translation>
    </message>
  </context>
  <context>
    <name>ScribusQApp</name>
    <message>
      <source>Invalid argument: </source>
      <translation>無効な引数: </translation>
    </message>
    <message>
      <source>File %1 does not exist, aborting.</source>
      <translation>ファイル %1 が存在しないので中断します</translation>
    </message>
    <message>
      <source>Usage: scribus [option ... ] [file]</source>
      <translation>使い方: scribus [オプション ... ] [ファイル]</translation>
    </message>
    <message>
      <source>Options:</source>
      <translation>オプション:</translation>
    </message>
    <message>
      <source>Print help (this message) and exit</source>
      <translation>ヘルプを表示(このメッセージ)して終了します</translation>
    </message>
    <message>
      <source>Uses xx as shortcut for a language, eg `en' or `de'</source>
      <translation>xxを言語のショートカットとして使用します。例 `en'もしくは`de'</translation>
    </message>
    <message>
      <source>List the currently installed interface languages</source>
      <translation>現在インストールされている言語を一覧にします</translation>
    </message>
    <message>
      <source>Show information on the console when fonts are being loaded</source>
      <translation>フォントが読み込まれたときにコンソール上に情報を表示</translation>
    </message>
    <message>
      <source>Do not show the splashscreen on startup</source>
      <translation>起動時にスプラッシュスクリーンを表示しない</translation>
    </message>
    <message>
      <source>Output version information and exit</source>
      <translation>バージョン情報を出力して終了</translation>
    </message>
    <message>
      <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>filename</source>
      <translation>ファイル名</translation>
    </message>
    <message>
      <source>Use filename as path for user given preferences</source>
      <translation>ファイル名をユーザ指定の設定パスとして使用する</translation>
    </message>
    <message>
      <source>Installed interface languages for Scribus are as follows:</source>
      <translation>インストールされているScribusの言語は以下の通り:</translation>
    </message>
    <message>
      <source>To override the default language choice:</source>
      <translation>標準の言語選択を上書き:</translation>
    </message>
    <message>
      <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
      <translation>scribus -l xx または scribus --lang xx で xx は選択した言語です。</translation>
    </message>
    <message>
      <source>Scribus Version</source>
      <translation>Scribusバージョン</translation>
    </message>
    <message>
      <source>Scribus, Open Source Desktop Publishing</source>
      <translation>Scribus, オープンソースデスクトップパブリッシング</translation>
    </message>
    <message>
      <source>Homepage</source>
      <translation>ホームページ</translation>
    </message>
    <message>
      <source>Documentation</source>
      <translation>ドキュメント</translation>
    </message>
    <message>
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>Issues</source>
      <translation>発行</translation>
    </message>
    <message>
      <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
      <translation>起動時にスプラッシュスクリーンを表示しません。~/.scribus中に.neversplashという空のファイルを書き込みます。</translation>
    </message>
    <message>
      <source>Download a file from the Scribus website and show the latest available version.</source>
      <translation>Scribusウェブサイトからファイルをダウンロードして、利用可能な最新バージョンを表示します</translation>
    </message>
    <message>
      <source>Display a console window</source>
      <translation>コンソールウィンドウを表示</translation>
    </message>
    <message>
      <source>Show location ICC profile information on console while starting</source>
      <translation>コンソールを開始する間にICCプロファイルの位置を表示</translation>
    </message>
    <message>
      <source>Invalid argument: %1</source>
      <translation>無効な引数: %1</translation>
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
      <translation>ここにコピー</translation>
    </message>
    <message>
      <source>Move Here</source>
      <translation>ここに移動</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>貼り付け(&amp;P)</translation>
    </message>
    <message>
      <source>File: </source>
      <translation>ファイル:</translation>
    </message>
    <message>
      <source>Original PPI: </source>
      <translation>オリジナルPPI: </translation>
    </message>
    <message>
      <source>Actual PPI: </source>
      <translation>実PPI: </translation>
    </message>
    <message>
      <source>Colorspace: </source>
      <translation>色空間: </translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>不明</translation>
    </message>
    <message>
      <source>Print: </source>
      <translation>印刷: </translation>
    </message>
    <message>
      <source>Enabled</source>
      <translation>有効</translation>
    </message>
    <message>
      <source>Disabled</source>
      <translation>無効</translation>
    </message>
    <message>
      <source>In&amp;fo</source>
      <translation>情報(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>PDFオプション(&amp;P)</translation>
    </message>
    <message>
      <source>Send to La&amp;yer</source>
      <translation>レイヤーに送る(&amp;Y)</translation>
    </message>
    <message>
      <source>Le&amp;vel</source>
      <translation>レベル(&amp;V)</translation>
    </message>
    <message>
      <source>Conve&amp;rt to</source>
      <translation>変換(&amp;R)</translation>
    </message>
    <message>
      <source>Linking Text Frames</source>
      <translation>テキストフレームを繋いでいます</translation>
    </message>
    <message>
      <source>Page %1 to %2</source>
      <translation>ページ %1 から %2</translation>
    </message>
    <message>
      <source>Cannot Convert In-Use Item</source>
      <translation>使用中のアイテムは変換できません</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Contents</source>
      <translation>目次</translation>
    </message>
    <message>
      <source>Paste Recent</source>
      <translation>最近の貼り付け</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
    <message>
      <source>Preview Mode</source>
      <translation>プレビューモード</translation>
    </message>
    <message>
      <source>CMS is active. Therefore the color display may not match the perception by visually impaired</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enter Object Size</source>
      <translation>オブジェクトのサイズを入力</translation>
    </message>
    <message>
      <source>No Image Loaded</source>
      <translation>画像が読み込まれていません</translation>
    </message>
    <message>
      <source>You are trying to link a frame to itself.</source>
      <translation>フレーム自身にリンクしようとしています。</translation>
    </message>
    <message>
      <source>You are trying to link a frame which is already linked.</source>
      <translation>既にリンクされているフレームにリンクしようとしています。</translation>
    </message>
    <message>
      <source>Page: </source>
      <translation>ページ: </translation>
    </message>
    <message>
      <source>Pages: </source>
      <translation>ページ: </translation>
    </message>
    <message>
      <source>Enable/disable Color Management</source>
      <translation>カラーマネージメントの有効/無効</translation>
    </message>
    <message>
      <source>Enable/disable the Preview Mode</source>
      <translation>プレビューモードの有効/無効</translation>
    </message>
    <message>
      <source>Select the visual appearance of the display. You can choose between normal and several color blindness forms</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Configure CMS...</source>
      <translation>CMSを設定...</translation>
    </message>
  </context>
  <context>
    <name>ScribusWin</name>
    <message>
      <source>Document:</source>
      <translation>ドキュメント:</translation>
    </message>
    <message>
      <source>has been changed since the last save.</source>
      <translation>最後に保存した時点から変更されています</translation>
    </message>
  </context>
  <context>
    <name>ScriptPlugin</name>
    <message>
      <source>Embedded Python scripting support.</source>
      <translation>埋め込みPythonスクリプトサポート</translation>
    </message>
    <message>
      <source>Scripter</source>
      <translation>スクリプタ</translation>
    </message>
  </context>
  <context>
    <name>ScripterCore</name>
    <message>
      <source>Script error</source>
      <translation>スクリプトエラー</translation>
    </message>
    <message>
      <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;>bugs.scribus.net&lt;/a> please.</source>
      <translation>もしオフィシャルのスクリプトを実行しているのであれば、 
&lt;a href=&quot;http://bugs.scribus.net&quot;>bugs.scribus.net&lt;/a> 
に報告してください。</translation>
    </message>
    <message>
      <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
      <translation>このメッセージはクリップボードにもあります。Ctrl+Vでバグトラックにペーストしてください。</translation>
    </message>
    <message>
      <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
      <translation>入力されたコマンドの実行中に内部エラーが発生しました。詳細は標準エラーに出力されています。</translation>
    </message>
    <message>
      <source>Examine Script</source>
      <translation>スクリプトを試す</translation>
    </message>
    <message>
      <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
      <translation>Pythonプラグインの設定に失敗しました。エラーの詳細は標準エラーに出力されています。 </translation>
    </message>
    <message>
      <source>Documentation for:</source>
      <translation>以下のドキュメント:</translation>
    </message>
    <message>
      <source>Script</source>
      <translation>スクリプト</translation>
    </message>
    <message>
      <source> doesn't contain any docstring!</source>
      <translation>何も文字列を含んでいません!</translation>
    </message>
    <message>
      <source>Python Scripts (*.py *.PY);;All Files (*)</source>
      <translation>Pythonスクリプト (*.py *.PY);;全てのファイル (*)</translation>
    </message>
  </context>
  <context>
    <name>ScripterPrefsGui</name>
    <message>
      <source>Extensions</source>
      <translation>拡張</translation>
    </message>
    <message>
      <source>Console</source>
      <translation>コンソール</translation>
    </message>
    <message>
      <source>Enable Extension Scripts</source>
      <translation>拡張スクリプトを有効に</translation>
    </message>
    <message>
      <source>Startup Script:</source>
      <translation>起動スクリプト:</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>変更...</translation>
    </message>
    <message>
      <source>Form</source>
      <translation>フォーム</translation>
    </message>
    <message>
      <source>Comments:</source>
      <translation>コメント:</translation>
    </message>
    <message>
      <source>Keywords:</source>
      <translation>キーワード:</translation>
    </message>
    <message>
      <source>Signs:</source>
      <translation>記号:</translation>
    </message>
    <message>
      <source>Strings:</source>
      <translation>文字列:</translation>
    </message>
    <message>
      <source>Numbers:</source>
      <translation>数字:</translation>
    </message>
    <message>
      <source>Errors:</source>
      <translation>エラー:</translation>
    </message>
    <message>
      <source>Base Texts:</source>
      <translation>ベーステキスト:</translation>
    </message>
  </context>
  <context>
    <name>SeList</name>
    <message>
      <source>Show Page Previews</source>
      <translation>ページプレビューを表示</translation>
    </message>
    <message>
      <source>Delete Master Page?</source>
      <translation>マスターページを削除しますか?</translation>
    </message>
    <message>
      <source>Are you sure you want to delete this master page?</source>
      <translation>本当にこのマスタページを削除しますか?</translation>
    </message>
  </context>
  <context>
    <name>SeView</name>
    <message>
      <source>Delete Page?</source>
      <translation>ページを削除?</translation>
    </message>
    <message>
      <source>Are you sure you want to delete this page?</source>
      <translation>本当にこのページを削除しますか?</translation>
    </message>
  </context>
  <context>
    <name>SearchReplace</name>
    <message>
      <source>Search/Replace</source>
      <translation>検索/置換</translation>
    </message>
    <message>
      <source>Search for:</source>
      <translation>以下で検索:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>テキスト</translation>
    </message>
    <message>
      <source>Font</source>
      <translation>フォント</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>フォントサイズ</translation>
    </message>
    <message>
      <source>Font Effects</source>
      <translation>フォントエフェクト</translation>
    </message>
    <message>
      <source>Fill Color</source>
      <translation>塗りつぶし色</translation>
    </message>
    <message>
      <source>Fill Shade</source>
      <translation>塗りつぶし色の濃さ</translation>
    </message>
    <message>
      <source>Stroke Color</source>
      <translation>輪郭色</translation>
    </message>
    <message>
      <source>Stroke Shade</source>
      <translation>輪郭色の濃さ</translation>
    </message>
    <message>
      <source>Left</source>
      <translation>左</translation>
    </message>
    <message>
      <source>Center</source>
      <translation>中央</translation>
    </message>
    <message>
      <source>Right</source>
      <translation>右</translation>
    </message>
    <message>
      <source>Block</source>
      <translation>ブロック</translation>
    </message>
    <message>
      <source>Forced</source>
      <translation>強制</translation>
    </message>
    <message>
      <source>Replace with:</source>
      <translation>次で置換:</translation>
    </message>
    <message>
      <source>&amp;Whole Word</source>
      <translation>単語全体(&amp;W)</translation>
    </message>
    <message>
      <source>&amp;Ignore Case</source>
      <translation>大文字小文字を区別しない(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>検索(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>置換(&amp;R)</translation>
    </message>
    <message>
      <source>Replace &amp;All</source>
      <translation>全て置換(&amp;A)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>閉じる(&amp;C)</translation>
    </message>
    <message>
      <source>Search finished</source>
      <translation>検索終了</translation>
    </message>
    <message>
      <source>Search finished, found %1 matches</source>
      <translation>検索終了。%1個マッチしました</translation>
    </message>
    <message>
      <source>Alignment</source>
      <translation>配置</translation>
    </message>
    <message>
      <source>Search for text or formatting in the current text</source>
      <translation>現在のテキストにあるテキストやフォーマットを検索します</translation>
    </message>
    <message>
      <source>Replace the searched for formatting with the replacement values</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Replace all found instances</source>
      <translation>見つかったものをすべて置換</translation>
    </message>
    <message>
      <source>Clear all search and replace options</source>
      <translation>すべての検索と置換オプションをクリア</translation>
    </message>
    <message>
      <source>Close search and replace</source>
      <translation>検索と置換を閉じる</translation>
    </message>
  </context>
  <context>
    <name>SelectFields</name>
    <message>
      <source>Select Fields</source>
      <translation>フィールドを選択</translation>
    </message>
    <message>
      <source>Available Fields</source>
      <translation>利用可能なフィールド</translation>
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
      <source>Selected Fields</source>
      <translation>選択されたフィールド</translation>
    </message>
  </context>
  <context>
    <name>ShadeButton</name>
    <message>
      <source>Other...</source>
      <translation>その他...</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>色の濃さ(&amp;S):</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>色の濃さ</translation>
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
      <translation>X オフセット</translation>
    </message>
    <message>
      <source>Y-Offset</source>
      <translation>Y オフセット</translation>
    </message>
  </context>
  <context>
    <name>ShortWordsPlugin</name>
    <message>
      <source>Short &amp;Words...</source>
      <comment>short words plugin</comment>
      <translation>ショートワード(&amp;W)...</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>ショートワード</translation>
    </message>
    <message>
      <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ShortcutWidget</name>
    <message>
      <source>&amp;No Key</source>
      <translation>キーなし(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;User Defined Key</source>
      <translation>ユーザ定義キー(&amp;U)</translation>
    </message>
    <message>
      <source>Set &amp;Key</source>
      <translation>キーを設定(&amp;K)</translation>
    </message>
  </context>
  <context>
    <name>SideBar</name>
    <message>
      <source>No Style</source>
      <translation>スタイルなし</translation>
    </message>
    <message>
      <source>Edit Styles...</source>
      <translation>スタイルを編集...</translation>
    </message>
  </context>
  <context>
    <name>StoryEditor</name>
    <message>
      <source>&amp;Reload Text from Frame</source>
      <translation>フレームからテキストを再読み込み(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Save to File...</source>
      <translation>ファイルに保存(&amp;S)...</translation>
    </message>
    <message>
      <source>&amp;Load from File...</source>
      <translation>ファイルから読み込み(&amp;L)...</translation>
    </message>
    <message>
      <source>Save &amp;Document</source>
      <translation>ドキュメントを保存(&amp;D)</translation>
    </message>
    <message>
      <source>&amp;Update Text Frame and Exit</source>
      <translation>テキストフレームを更新して終了(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Exit Without Updating Text Frame</source>
      <translation>テキストフレームを更新せずに終了(&amp;E)</translation>
    </message>
    <message>
      <source>Select &amp;All</source>
      <translation>全て選択(&amp;A)</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>切り取り(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>貼り付け(&amp;P)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Search/Replace...</source>
      <translation>検索/置換(&amp;S)...</translation>
    </message>
    <message>
      <source>&amp;Insert Glyph...</source>
      <translation>グリフを挿入(&amp;I)...</translation>
    </message>
    <message>
      <source>&amp;Edit Styles...</source>
      <translation>スタイルを編集(&amp;E)...</translation>
    </message>
    <message>
      <source>&amp;Fonts Preview...</source>
      <translation>フォントプレビュー(&amp;F)...</translation>
    </message>
    <message>
      <source>&amp;Update Text Frame</source>
      <translation>テキストフレームを更新(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Background...</source>
      <translation>背景(&amp;B)...</translation>
    </message>
    <message>
      <source>&amp;Display Font...</source>
      <translation>フォントを表示(&amp;D)...</translation>
    </message>
    <message>
      <source>&amp;Smart text selection</source>
      <translation>スマートテキスト選択(&amp;S)</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>編集(&amp;E)</translation>
    </message>
    <message>
      <source>&amp;Settings</source>
      <translation>設定(&amp;S)</translation>
    </message>
    <message>
      <source>Story Editor</source>
      <translation>ストーリエディタ</translation>
    </message>
    <message>
      <source>File</source>
      <translation>ファイル</translation>
    </message>
    <message>
      <source>Clear All Text</source>
      <translation>全てのテキストをクリア</translation>
    </message>
    <message>
      <source>Current Paragraph:</source>
      <translation>現在の段落:</translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>単語数: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>文字数: </translation>
    </message>
    <message>
      <source>Totals:</source>
      <translation>合計:</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>段落数: </translation>
    </message>
    <message>
      <source>Story Editor - %1</source>
      <translation>ストーリエディタ - %1</translation>
    </message>
    <message>
      <source>Do you want to save your changes?</source>
      <translation>変更点を保存しますか?</translation>
    </message>
    <message>
      <source>Do you really want to lose all your changes?</source>
      <translation>本当に全ての変更点を破棄してもよろしいですか?</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>本当に全てのテキストを消去しますか?</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>テキストファイル (*.txt);;全てのファイル(*)</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>名前を付けて保存</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>挿入(&amp;I)</translation>
    </message>
    <message>
      <source>Character</source>
      <translation>文字</translation>
    </message>
    <message>
      <source>Quote</source>
      <translation>引用符</translation>
    </message>
    <message>
      <source>Spaces &amp;&amp; Breaks</source>
      <translation>スペースと改行</translation>
    </message>
    <message>
      <source>Ligature</source>
      <translation>リガチャ</translation>
    </message>
    <message>
      <source>Space</source>
      <translation>空白</translation>
    </message>
  </context>
  <context>
    <name>StrikeValues</name>
    <message>
      <source>Auto</source>
      <translation>自動</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Displacement</source>
      <translation>位置ずれ</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>線幅</translation>
    </message>
  </context>
  <context>
    <name>StyleManager</name>
    <message>
      <source>Name:</source>
      <translation>名前:</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>リセット(&amp;R)</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>適用(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>新規(&amp;N)</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>インポート(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Clone</source>
      <translation>コピー(&amp;C)</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Shortcut</source>
      <translation>ショートカット</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Clone copies the style to make similar styles easily.</source>
      <translation>簡単に同じようなスタイルを作るためのスタイルのコピー</translation>
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
      <translation>スタイル名は重複しないものを選択してください</translation>
    </message>
    <message>
      <source>&lt;&lt; &amp;Done</source>
      <translation>&lt;&lt; 完了(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Style Manager</source>
      <translation>スタイルマネージャ</translation>
    </message>
  </context>
  <context>
    <name>StyleSelect</name>
    <message>
      <source>All Caps</source>
      <translation>全て大文字</translation>
    </message>
    <message>
      <source>Small Caps</source>
      <translation>スモールキャピタル</translation>
    </message>
    <message>
      <source>Subscript</source>
      <translation>下付き文字</translation>
    </message>
    <message>
      <source>Superscript</source>
      <translation>上付き文字</translation>
    </message>
    <message>
      <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>テキストに下線を引きます。線幅と位置ずれオプションを設定するために、ボタンをしばらく押したままにしてください。</translation>
    </message>
    <message>
      <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>取り消し線。少しボタンを押したままにして、線幅と位置ずれオプションを設定してください。</translation>
    </message>
    <message>
      <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>取り消し線。少しボタンを押したままにして、線幅と位置ずれオプションを設定してください。</translation>
    </message>
    <message>
      <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
      <comment>Text Style Selector</comment>
      <translation>アウトライン。少しボタンを押したままにして、アウトラインの線幅を変更してください。</translation>
    </message>
    <message>
      <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
      <translation>影つきテキスト。少しボタンを押したままにして、オフセットスペースを有効にしてください。</translation>
    </message>
  </context>
  <context>
    <name>SubdividePlugin</name>
    <message>
      <source>Subdivide Path</source>
      <translation>再分割パス</translation>
    </message>
    <message>
      <source>Path Tools</source>
      <translation>パスツール</translation>
    </message>
    <message>
      <source>Subdivide</source>
      <translation>再分割</translation>
    </message>
    <message>
      <source>Subdivide selected Path</source>
      <translation>選択されたパスの再分割</translation>
    </message>
  </context>
  <context>
    <name>SxwDialog</name>
    <message>
      <source>OpenOffice.org Writer Importer Options</source>
      <translation>OpenOffice.org Writerインポータオプション</translation>
    </message>
    <message>
      <source>Overwrite Paragraph Styles</source>
      <translation>段落スタイルを上書き</translation>
    </message>
    <message>
      <source>Enabling this will overwrite existing styles in the current Scribus document</source>
      <translation>これを有効にすると、現在のScribusドキュメント中の既存のスタイルを上書きします</translation>
    </message>
    <message>
      <source>Merge Paragraph Styles</source>
      <translation>段落スタイルをマージ</translation>
    </message>
    <message>
      <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use document name as a prefix for paragraph styles</source>
      <translation>ドキュメント名を段落スタイルのプレフィックスとして使用する</translation>
    </message>
    <message>
      <source>Prepend the document name to the paragraph style name in Scribus.</source>
      <translation>Scribusの段落スタイル名の先頭にドキュメント名を追加します。</translation>
    </message>
    <message>
      <source>Do not ask again</source>
      <translation>次回は尋ねない</translation>
    </message>
    <message>
      <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
      <translation>これらの設定をデフォルトにし、OpenOffice.org 1.x のドキュメントをインポートする際に再度尋ねません</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
  </context>
  <context>
    <name>TOCIndexPrefs</name>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>目次とインデックス</translation>
    </message>
    <message>
      <source>Table Of Contents</source>
      <translation>目次</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>追加(&amp;A)</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>The frame the table of contents will be placed into</source>
      <translation>目次を配置するフレーム</translation>
    </message>
    <message>
      <source>Item Attribute Name:</source>
      <translation>アイテム属性名:</translation>
    </message>
    <message>
      <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
      <translation>エントリの作成時の基礎として使われるフレームに設定されるアイテム属性</translation>
    </message>
    <message>
      <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
      <translation>エントリーのページ番号を、行の最初または最後に配置するか全く行いません</translation>
    </message>
    <message>
      <source>List Non-Printing Entries</source>
      <translation>非印字エントリをリストにする</translation>
    </message>
    <message>
      <source>Include frames that are set to not print as well</source>
      <translation>印刷なしに設定されるフレームも含む</translation>
    </message>
    <message>
      <source>The paragraph style used for the entry lines</source>
      <translation>入力行に使用される段落スタイル</translation>
    </message>
    <message>
      <source>Paragraph Style:</source>
      <translation>段落スタイル:</translation>
    </message>
    <message>
      <source>Destination Frame:</source>
      <translation>出力先フレーム:</translation>
    </message>
    <message>
      <source>Page Number Placement:</source>
      <translation>ページ番号の配置:</translation>
    </message>
  </context>
  <context>
    <name>TabCheckDoc</name>
    <message>
      <source>Ignore all errors</source>
      <translation>全てのエラーを無視</translation>
    </message>
    <message>
      <source>Automatic check before printing or exporting</source>
      <translation>印刷または出力前に自動チェック</translation>
    </message>
    <message>
      <source>Check for missing glyphs</source>
      <translation>見つからないグリフをチェック</translation>
    </message>
    <message>
      <source>Check for overflow in text frames</source>
      <translation>テキストフレームでのオーバーフローをチェック</translation>
    </message>
    <message>
      <source>Check for missing images</source>
      <translation>見つからない画像をチェック</translation>
    </message>
    <message>
      <source>Check image resolution</source>
      <translation>画像の解像度をチェック</translation>
    </message>
    <message>
      <source>Lowest allowed resolution</source>
      <translation>許可する最低解像度</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>Check for placed PDF Files</source>
      <translation>PDFファイルの配置をチェック</translation>
    </message>
    <message>
      <source>Check for PDF Annotations and Fields</source>
      <translation>PDF注釈とフィールドをチェック</translation>
    </message>
    <message>
      <source>Add Profile</source>
      <translation>プロファイルを追加</translation>
    </message>
    <message>
      <source>Remove Profile</source>
      <translation>プロファイルを削除</translation>
    </message>
    <message>
      <source>Highest allowed resolution</source>
      <translation>利用できる最高解像度</translation>
    </message>
    <message>
      <source>Check for GIF images</source>
      <translation>GIF画像をチェック</translation>
    </message>
    <message>
      <source>Ignore non-printable Layers</source>
      <translation>印刷できないレイヤーを無視</translation>
    </message>
    <message>
      <source>Check for items not on a page</source>
      <translation>ページ上にないアイテムをチェック</translation>
    </message>
    <message>
      <source>Check for used transparencies</source>
      <translation>使用されている透明度をチェック</translation>
    </message>
  </context>
  <context>
    <name>TabDisplay</name>
    <message>
      <source>TabDisplayBase</source>
      <translation>TabDisplayBase</translation>
    </message>
    <message>
      <source>General</source>
      <translation>全般</translation>
    </message>
    <message>
      <source>Adjust Display Size</source>
      <translation>表示サイズを調整</translation>
    </message>
    <message>
      <source>Scale%</source>
      <translation>倍率%</translation>
    </message>
    <message>
      <source>To adjust the display drag the ruler below with the slider.</source>
      <translation>表示を調整するには下の定規をスライドバーで動かしてください</translation>
    </message>
    <message>
      <source>Gaps Between Pages</source>
      <translation>ページ間のギャップ</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation>垂直:</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation>水平:</translation>
    </message>
    <message>
      <source>Scratch Space</source>
      <translation>スクラッチスペース</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>下(&amp;B):</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>上(&amp;T):</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>右(&amp;R):</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>左(&amp;L):</translation>
    </message>
    <message>
      <source>Page Display</source>
      <translation>ページ表示</translation>
    </message>
    <message>
      <source>Show Bleed Area</source>
      <translation>断ち切り領域を表示</translation>
    </message>
    <message>
      <source>Display &amp;Unprintable Area in Margin Color</source>
      <translation>印刷できない範囲をマージンの色で表示(&amp;U)</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Show Layer Indicators</source>
      <translation>レイヤー目印を表示</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation>フレームを表示</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation>テキストチェーンを表示</translation>
    </message>
    <message>
      <source>Rulers Relative to Page</source>
      <translation>ページに相対する定規</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation>テキストの制御文字を表示</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Pages:</source>
      <translation>ページ:</translation>
    </message>
    <message>
      <source>Selected Page Border:</source>
      <translation>選択されたページ枠:</translation>
    </message>
    <message>
      <source>Fill Color:</source>
      <translation>塗りつぶし色:</translation>
    </message>
    <message>
      <source>Frames</source>
      <translation>フレーム</translation>
    </message>
    <message>
      <source>Grouped:</source>
      <translation>グループ化:</translation>
    </message>
    <message>
      <source>Annotation:</source>
      <translation>注釈:</translation>
    </message>
    <message>
      <source>Selected:</source>
      <translation>選択済:</translation>
    </message>
    <message>
      <source>Linked:</source>
      <translation>リンク済み:</translation>
    </message>
    <message>
      <source>Locked:</source>
      <translation>ロック化:</translation>
    </message>
    <message>
      <source>Normal:</source>
      <translation>標準:</translation>
    </message>
    <message>
      <source>Text:</source>
      <translation>テキスト:</translation>
    </message>
    <message>
      <source>Control Characters:</source>
      <translation>制御文字:</translation>
    </message>
    <message>
      <source>Show Images</source>
      <translation>画像を表示</translation>
    </message>
    <message>
      <source>Resets the scale to the default dpi of your display</source>
      <translation>ディスプレイのデフォルトDPIに合わせるのをリセット</translation>
    </message>
  </context>
  <context>
    <name>TabDocument</name>
    <message>
      <source>Page Size</source>
      <translation>ページサイズ</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>サイズ(&amp;S):</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>縦方向</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>横方向</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>方向(&amp;N):</translation>
    </message>
    <message>
      <source>Units:</source>
      <translation>単位:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>幅(&amp;W):</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>高さ(&amp;H):</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>マージンガイド</translation>
    </message>
    <message>
      <source>Autosave</source>
      <translation>自動保存</translation>
    </message>
    <message>
      <source>min</source>
      <translation>分</translation>
    </message>
    <message>
      <source>&amp;Interval:</source>
      <translation>間隔(&amp;I):</translation>
    </message>
    <message>
      <source>Undo/Redo</source>
      <translation>元に戻す/やり直す</translation>
    </message>
    <message>
      <source>Action history length</source>
      <translation>アクション履歴の長さ</translation>
    </message>
    <message>
      <source>Width of document pages, editable if you have chosen a custom page size</source>
      <translation>ドキュメントのページ幅。カスタムページサイズを選べば編集が可能です</translation>
    </message>
    <message>
      <source>Height of document pages, editable if you have chosen a custom page size</source>
      <translation>ドキュメントのページの高さ。カスタムページサイズを選べば編集が可能です</translation>
    </message>
    <message>
      <source>Default page size, either a standard size or a custom size</source>
      <translation>デフォルトの標準サイズとカスタムサイズの両方のページサイズ</translation>
    </message>
    <message>
      <source>Default orientation of document pages</source>
      <translation>デフォルトのドキュメントページの方向</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>ドキュメントを編集する際のデフォルトの計測単位</translation>
    </message>
    <message>
      <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
      <translation>有効にすると、Scribusは一定時間たつごとに.bakという拡張子でファイルのバックアップコピーを保存します。</translation>
    </message>
    <message>
      <source>Time period between saving automatically</source>
      <translation>自動保存の間隔</translation>
    </message>
    <message>
      <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
      <translation>ステップ中のアクション履歴の長さを設定します。0に設定すれば、アクションは無限に保存されます。</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing pages in the document</source>
      <translation>ドキュメント中の全てのページにページサイズの変更を適用する</translation>
    </message>
    <message>
      <source>Apply settings to:</source>
      <translation>設定を適用:</translation>
    </message>
    <message>
      <source>All Document Pages</source>
      <translation>全てのドキュメントページ</translation>
    </message>
    <message>
      <source>All Master Pages</source>
      <translation>全てのマスターページ</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing master pages in the document</source>
      <translation>ドキュメント中の全てのマスタページにページサイズの変更を適用する</translation>
    </message>
  </context>
  <context>
    <name>TabExternalToolsWidget</name>
    <message>
      <source>External Tools</source>
      <translation>外部ツール</translation>
    </message>
    <message>
      <source>Web Browser to launch with links from the Help system</source>
      <translation>ヘルプシステムのリンクから起動されるWebブラウザ</translation>
    </message>
    <message>
      <source>Web Browser</source>
      <translation>Webブラウザ</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation>変更(&amp;C)...</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&lt;qt>File system location for your web browser. This is used for external links from the Help system.&lt;/qt></source>
      <translation>&lt;qt>webブラウザのファイルシステムの位置。これはヘルプシステムから外部へのリンクに使用されます。&lt;/qt></translation>
    </message>
    <message>
      <source>Name of &amp;Executable:</source>
      <translation>実行ファイル名(&amp;E):</translation>
    </message>
    <message>
      <source>Image Processing Tool</source>
      <translation>画像処理ツール</translation>
    </message>
    <message>
      <source>&lt;qt>File system location for graphics editor. If you use gimp and your distribution includes it, we recommend 'gimp-remote', as it allows you to edit the image in an already running instance of gimp.&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript Interpreter</source>
      <translation>Postscriptインタプリタ</translation>
    </message>
    <message>
      <source>Antialias text for EPS and PDF onscreen rendering</source>
      <translation>EPSとPDFのテキストの画面でのレンダリングをアンチエイリアスにする</translation>
    </message>
    <message>
      <source>Antialias &amp;Text</source>
      <translation>テキストのアンチエイリアス(&amp;T)</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Antialias graphics for EPS and PDF onscreen rendering</source>
      <translation>EPSとPDFの画像の画面でのレンダリングをアンチエイリアスにする</translation>
    </message>
    <message>
      <source>Antialias &amp;Graphics</source>
      <translation>画像のアンチエイリアス(&amp;G)</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>解像度:</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>&amp;Name of Executable:</source>
      <translation>実行ファイル名(&amp;N):</translation>
    </message>
    <message>
      <source>&lt;qt>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rescan for the external tools if they do not exist in the already specified location</source>
      <translation>指定された位置に存在しない場合は外部ツールを再スキャンする</translation>
    </message>
    <message>
      <source>&amp;Rescan</source>
      <translation>再スキャン(&amp;R)</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Start with empty frame</source>
      <translation>空フレームで開始</translation>
    </message>
    <message>
      <source>Always use the configured DPI setting for calculating the size, even if the image file reports something different.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Force DPI</source>
      <translation>DPIを強制</translation>
    </message>
    <message>
      <source>&lt;qt>Path to the editor executable.&lt;/qt></source>
      <translation>&lt;qt>エディタのパス&lt;/qt></translation>
    </message>
    <message>
      <source>Use Embedded Editor</source>
      <translation>埋め込みエディタを使用</translation>
    </message>
    <message>
      <source>Configurations:</source>
      <translation>設定:</translation>
    </message>
    <message>
      <source>Up</source>
      <translation>上</translation>
    </message>
    <message>
      <source>Down</source>
      <translation>下</translation>
    </message>
    <message>
      <source>Add</source>
      <translation>追加</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>Render Frames</source>
      <translation>フレームを描画</translation>
    </message>
    <message>
      <source>External Editor:</source>
      <translation>外部エディタ:</translation>
    </message>
  </context>
  <context>
    <name>TabGeneral</name>
    <message>
      <source>TabGeneralBase</source>
      <translation>TabGeneralBase</translation>
    </message>
    <message>
      <source>User Interface</source>
      <translation>ユーザインターフェース</translation>
    </message>
    <message>
      <source>&amp;Recent Documents:</source>
      <translation>最近開いたドキュメント(&amp;R):</translation>
    </message>
    <message>
      <source>&amp;Wheel Jump:</source>
      <translation>ホイールの移動量(&amp;W):</translation>
    </message>
    <message>
      <source>Show Splashscreen on Startup</source>
      <translation>起動時にスプラッシュスクリーンを表示</translation>
    </message>
    <message>
      <source>Show Startup Dialog</source>
      <translation>スタートアップダイアログを表示</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>&amp;Font Size (Menus):</source>
      <translation>フォントサイズ(メニュー)(&amp;F):</translation>
    </message>
    <message>
      <source>Font Size (&amp;Palettes):</source>
      <translation>フォントサイズ(パレット)(&amp;P):</translation>
    </message>
    <message>
      <source>Time before a Move or Resize starts:</source>
      <translation>移動もしくはリサイズを開始するまでの時間:</translation>
    </message>
    <message>
      <source> ms</source>
      <translation> ms</translation>
    </message>
    <message>
      <source>&amp;Theme:</source>
      <translation>テーマ(&amp;T):</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>言語(&amp;L):</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>パス</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation>変更(&amp;C)...</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>変更(&amp;H)...</translation>
    </message>
    <message>
      <source>Alt+H</source>
      <translation>Alt+H</translation>
    </message>
    <message>
      <source>&amp;Scripts:</source>
      <translation>スクリプト(&amp;S):</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>変更(&amp;N)...</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>&amp;ICC Profiles:</source>
      <translation>ICCプロファイル(&amp;I):</translation>
    </message>
    <message>
      <source>&amp;Documents:</source>
      <translation>ドキュメント(&amp;D):</translation>
    </message>
    <message>
      <source>Document &amp;Templates:</source>
      <translation>ドキュメントテンプレート(&amp;T):</translation>
    </message>
    <message>
      <source>Ch&amp;ange...</source>
      <translation>変更(&amp;A)...</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>Use Small Widgets in Palettes</source>
      <translation>パレットで小さなウィジェットを使用</translation>
    </message>
  </context>
  <context>
    <name>TabGuides</name>
    <message>
      <source>Common Settings</source>
      <translation>共通設定</translation>
    </message>
    <message>
      <source>Placing in Documents</source>
      <translation>ドキュメント中の配置</translation>
    </message>
    <message>
      <source>In the Background</source>
      <translation>背景に</translation>
    </message>
    <message>
      <source>In the Foreground</source>
      <translation>前景に</translation>
    </message>
    <message>
      <source>Snapping</source>
      <translation>スナップ</translation>
    </message>
    <message>
      <source>Snap Distance:</source>
      <translation>スナップ距離:</translation>
    </message>
    <message>
      <source>Grab Radius:</source>
      <translation>つかむ範囲:</translation>
    </message>
    <message>
      <source> px</source>
      <translation> px</translation>
    </message>
    <message>
      <source>Show Guides</source>
      <translation>ガイドの表示</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>色:</translation>
    </message>
    <message>
      <source>Show Margins</source>
      <translation>マージンの表示</translation>
    </message>
    <message>
      <source>Show Page Grid</source>
      <translation>ページグリッドの表示</translation>
    </message>
    <message>
      <source>Major Grid</source>
      <translation>メジャーグリッド</translation>
    </message>
    <message>
      <source>Spacing:</source>
      <translation>間隔:</translation>
    </message>
    <message>
      <source>Minor Grid</source>
      <translation>マイナーグリッド</translation>
    </message>
    <message>
      <source>Show Baseline Grid</source>
      <translation>ベースライングリッドの表示</translation>
    </message>
    <message>
      <source>Baseline Settings</source>
      <translation>ベースライン設定</translation>
    </message>
    <message>
      <source>Baseline &amp;Grid:</source>
      <translation>ベースライングリッド(&amp;G):</translation>
    </message>
    <message>
      <source>Baseline &amp;Offset:</source>
      <translation>ベースラインオフセット(&amp;O):</translation>
    </message>
    <message>
      <source>Guides are not visible through objects on the page</source>
      <translation>ガイドはページ上のオブジェクトよりも奥に表示されます</translation>
    </message>
    <message>
      <source>Guides are visible above all objects on the page</source>
      <translation>ガイドはページ上の全てのオブジェクトよりも手前に表示されます</translation>
    </message>
    <message>
      <source>Distance between the minor grid lines</source>
      <translation>マイナーグリッド線の間隔</translation>
    </message>
    <message>
      <source>Distance between the major grid lines</source>
      <translation>メジャーグリッド線の間隔</translation>
    </message>
    <message>
      <source>Color of the minor grid lines</source>
      <translation>マイナーグリッド線の色</translation>
    </message>
    <message>
      <source>Color of the major grid lines</source>
      <translation>メジャーグリッド線の色</translation>
    </message>
    <message>
      <source>Color of the guide lines you insert</source>
      <translation>挿入したガイドラインの色</translation>
    </message>
    <message>
      <source>Color for the margin lines</source>
      <translation>マージン線の色</translation>
    </message>
    <message>
      <source>Color for the baseline grid</source>
      <translation>ベースライングリッドの色</translation>
    </message>
    <message>
      <source>Turns the basegrid on or off</source>
      <translation>ベースグリッドをON/OFFする</translation>
    </message>
    <message>
      <source>Distance between the lines of the baseline grid</source>
      <translation>ベースライングリッドの線の間の間隔</translation>
    </message>
    <message>
      <source>Distance from the top of the page for the first baseline</source>
      <translation>ページの上から最初のベースラインまでの距離</translation>
    </message>
    <message>
      <source>Turns the gridlines on or off</source>
      <translation>グリッドラインをON/OFFする</translation>
    </message>
    <message>
      <source>Turns the guides on or off</source>
      <translation>ガイドをON/OFFする</translation>
    </message>
    <message>
      <source>Turns the margins on or off</source>
      <translation>マージンをON/OFFする</translation>
    </message>
    <message>
      <source>px</source>
      <translation>px</translation>
    </message>
    <message>
      <source>Distance within which an object will snap to your placed guides. After setting this you will need to restart Scribus to set this setting.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Radius of the area where Scribus will allow you to grab an objects handles.After setting this you will need to restart Scribus to set this setting.</source>
      <translation>オブジェクトを扱う際につかむことのできる範囲の半径。設定を有効にするために、設定後にScribusを再起動する必要があります。</translation>
    </message>
  </context>
  <context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
      <source>Keyboard Shortcuts</source>
      <translation>キーボードショートカット</translation>
    </message>
    <message>
      <source>Action</source>
      <translation>アクション</translation>
    </message>
    <message>
      <source>Shortcut</source>
      <translation>ショートカット</translation>
    </message>
    <message>
      <source>Search:</source>
      <translation>検索:</translation>
    </message>
    <message>
      <source>Shortcut for Selected Action</source>
      <translation>選択されたアクションのためのショートカット</translation>
    </message>
    <message>
      <source>CTRL+ALT+SHIFT+W</source>
      <translation>CTRL+ALT+SHIFT+W</translation>
    </message>
    <message>
      <source>Set &amp;Key</source>
      <translation>キーを設定(&amp;K)</translation>
    </message>
    <message>
      <source>Alt+K</source>
      <translation>Alt+K</translation>
    </message>
    <message>
      <source>&amp;User Defined Key</source>
      <translation>ユーザ定義キー(&amp;U)</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>&amp;No Key</source>
      <translation>キーなし(&amp;N)</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Loadable Shortcut Sets</source>
      <translation>読み込み可能なショートカットのセット</translation>
    </message>
    <message>
      <source>Reload the default Scribus shortcuts</source>
      <translation>デフォルトのScribusショートカットを再読み込みします</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>リセット(&amp;R)</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation>Alt+R</translation>
    </message>
    <message>
      <source>Export the current shortcuts into an importable file</source>
      <translation>現在のショートカットをインポート可能なファイルにエクスポートします</translation>
    </message>
    <message>
      <source>&amp;Export...</source>
      <translation>エクスポート(&amp;E)...</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>Import a shortcut set into the current configuration</source>
      <translation>ショートカットセットを現在の設定にインポートします</translation>
    </message>
    <message>
      <source>&amp;Import...</source>
      <translation>インポート(&amp;I)...</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation>Alt+I</translation>
    </message>
    <message>
      <source>Load the selected shortcut set</source>
      <translation>選択されたショートカットセットを読み込みます</translation>
    </message>
    <message>
      <source>&amp;Load</source>
      <translation>読み込み(&amp;L)</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Keyboard shortcut sets available to load</source>
      <translation>読み込み可能なキーボードショートカットのセットです</translation>
    </message>
  </context>
  <context>
    <name>TabManager</name>
    <message>
      <source>Manage Tabulators</source>
      <translation>タブの管理</translation>
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
      <translation>段落の数:</translation>
    </message>
    <message>
      <source>Always use standard Lorem Ipsum</source>
      <translation>常に標準のLorem Ipsumを使用する</translation>
    </message>
    <message>
      <source>Preview of current Paragraph Style visible when editing Styles</source>
      <translation>スタイル編集中に現在の段落スタイルのプレビューを表示</translation>
    </message>
    <message>
      <source>Always ask before fonts are replaced when loading a document</source>
      <translation>ドキュメント読み込み時にフォント置換をする場合は常にたずねる</translation>
    </message>
    <message>
      <source>Default number of paragraphs for sample text insertion</source>
      <translation>サンプルテキストを挿入するデフォルトの段落数</translation>
    </message>
    <message>
      <source>Always use the typical Latin-based Lorem Ipsum text for sample text</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show a preview by default when editing styles</source>
      <translation>スタイル編集時にデフォルトでプレビューを表示</translation>
    </message>
    <message>
      <source>Allow Scribus to automatically replace fonts when they are missing when opening a document</source>
      <translation>ドキュメントを開く際にScribusが不明なフォントを自動置換することを許可</translation>
    </message>
  </context>
  <context>
    <name>TabPDFOptions</name>
    <message>
      <source>Export Range</source>
      <translation>エクスポート範囲</translation>
    </message>
    <message>
      <source>&amp;All Pages</source>
      <translation>全てのページ(&amp;A)</translation>
    </message>
    <message>
      <source>C&amp;hoose Pages</source>
      <translation>ページを選択(&amp;H)</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>回転(&amp;R):</translation>
    </message>
    <message>
      <source>File Options</source>
      <translation>ファイルオプション</translation>
    </message>
    <message>
      <source>Compatibilit&amp;y:</source>
      <translation>互換性(&amp;Y):</translation>
    </message>
    <message>
      <source>&amp;Binding:</source>
      <translation>結合(&amp;B):</translation>
    </message>
    <message>
      <source>Left Margin</source>
      <translation>左マージン</translation>
    </message>
    <message>
      <source>Right Margin</source>
      <translation>右マージン</translation>
    </message>
    <message>
      <source>Generate &amp;Thumbnails</source>
      <translation>サムネイルを生成(&amp;T)</translation>
    </message>
    <message>
      <source>Save &amp;Linked Text Frames as PDF Articles</source>
      <translation>リンクされたテキストフレームをPDF記事として保存(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Include Bookmarks</source>
      <translation>ブックマークを取り込む(&amp;I)</translation>
    </message>
    <message>
      <source>Include La&amp;yers</source>
      <translation>レイヤーを取り込む(&amp;Y)</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>&amp;Resolution for EPS Graphics:</source>
      <translation>EPS画像の解像度(&amp;R):</translation>
    </message>
    <message>
      <source>Com&amp;press Text and Vector Graphics</source>
      <translation>テキストとベクトルグラフィックを圧縮(&amp;P)</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>自動</translation>
    </message>
    <message>
      <source>None</source>
      <translation>なし</translation>
    </message>
    <message>
      <source>Compression Metho&amp;d:</source>
      <translation>圧縮方法(&amp;D):</translation>
    </message>
    <message>
      <source>Maximum</source>
      <translation>最高</translation>
    </message>
    <message>
      <source>High</source>
      <translation>高</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>中</translation>
    </message>
    <message>
      <source>Low</source>
      <translation>低</translation>
    </message>
    <message>
      <source>Minimum</source>
      <translation>最低</translation>
    </message>
    <message>
      <source>Compression &amp;Quality:</source>
      <translation>圧縮の品質(&amp;Q):</translation>
    </message>
    <message>
      <source>&amp;General</source>
      <translation>全般(&amp;G)</translation>
    </message>
    <message>
      <source>Embedding</source>
      <translation>埋め込み</translation>
    </message>
    <message>
      <source>Available Fonts:</source>
      <translation>利用可能なフォント:</translation>
    </message>
    <message>
      <source>Fonts to embed:</source>
      <translation>埋め込むフォント:</translation>
    </message>
    <message>
      <source>&amp;Fonts</source>
      <translation>フォント(&amp;F)</translation>
    </message>
    <message>
      <source>Enable &amp;Presentation Effects</source>
      <translation>プレゼンテーションのエフェクトを有効に(&amp;P)</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>ページ</translation>
    </message>
    <message>
      <source>Show Page Pre&amp;views</source>
      <translation>ページプレビューを表示(&amp;V)</translation>
    </message>
    <message>
      <source>Effects</source>
      <translation>エフェクト</translation>
    </message>
    <message>
      <source>&amp;Display Duration:</source>
      <translation>表示する時間(&amp;D):</translation>
    </message>
    <message>
      <source>Effec&amp;t Duration:</source>
      <translation>エフェクトの持続時間(&amp;T):</translation>
    </message>
    <message>
      <source>Effect T&amp;ype:</source>
      <translation>エフェクトの種類(&amp;Y):</translation>
    </message>
    <message>
      <source>&amp;Moving Lines:</source>
      <translation>線の移動(&amp;M):</translation>
    </message>
    <message>
      <source>F&amp;rom the:</source>
      <translation>どこから(&amp;R):</translation>
    </message>
    <message>
      <source>D&amp;irection:</source>
      <translation>方向(&amp;I):</translation>
    </message>
    <message>
      <source> sec</source>
      <translation> 秒</translation>
    </message>
    <message>
      <source>No Effect</source>
      <translation>エフェクトなし</translation>
    </message>
    <message>
      <source>Blinds</source>
      <translation>ブラインド</translation>
    </message>
    <message>
      <source>Box</source>
      <translation>ボックス</translation>
    </message>
    <message>
      <source>Dissolve</source>
      <translation>ディソルブ</translation>
    </message>
    <message>
      <source>Glitter</source>
      <translation>輝き</translation>
    </message>
    <message>
      <source>Split</source>
      <translation>分割</translation>
    </message>
    <message>
      <source>Wipe</source>
      <translation>ワイプ</translation>
    </message>
    <message>
      <source>Horizontal</source>
      <translation>水平</translation>
    </message>
    <message>
      <source>Vertical</source>
      <translation>垂直</translation>
    </message>
    <message>
      <source>Inside</source>
      <translation>内側</translation>
    </message>
    <message>
      <source>Outside</source>
      <translation>外側</translation>
    </message>
    <message>
      <source>Left to Right</source>
      <translation>左から右に</translation>
    </message>
    <message>
      <source>Top to Bottom</source>
      <translation>上から下へ</translation>
    </message>
    <message>
      <source>Bottom to Top</source>
      <translation>下から上へ</translation>
    </message>
    <message>
      <source>Right to Left</source>
      <translation>右から左に</translation>
    </message>
    <message>
      <source>Top-left to Bottom-Right</source>
      <translation>左上から右下に</translation>
    </message>
    <message>
      <source>E&amp;xtras</source>
      <translation>拡張(&amp;X)</translation>
    </message>
    <message>
      <source>Display Settings</source>
      <translation>表示設定</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>単一ページ</translation>
    </message>
    <message>
      <source>Continuous</source>
      <translation>連続ページ</translation>
    </message>
    <message>
      <source>Double Page Left</source>
      <translation>左見開きページ</translation>
    </message>
    <message>
      <source>Double Page Right</source>
      <translation>右見開きページ</translation>
    </message>
    <message>
      <source>Visual Appearance</source>
      <translation>外観</translation>
    </message>
    <message>
      <source>Use Viewers Defaults</source>
      <translation>ビューアをデフォルトで使用する</translation>
    </message>
    <message>
      <source>Use Full Screen Mode</source>
      <translation>フルスクリーンモードで使用する</translation>
    </message>
    <message>
      <source>Display Bookmarks Tab</source>
      <translation>ブックマークタブを表示</translation>
    </message>
    <message>
      <source>Display Thumbnails</source>
      <translation>サムネイルを表示</translation>
    </message>
    <message>
      <source>Display Layers Tab</source>
      <translation>レイヤータブを表示</translation>
    </message>
    <message>
      <source>Hide Viewers Toolbar</source>
      <translation>ビューアのツールバーを隠す</translation>
    </message>
    <message>
      <source>Hide Viewers Menubar</source>
      <translation>ビューアのメニューバーを隠す</translation>
    </message>
    <message>
      <source>Zoom Pages to fit Viewer Window</source>
      <translation>ビューアのウィンドウにあわせてページをズーム</translation>
    </message>
    <message>
      <source>Special Actions</source>
      <translation>特殊なアクション</translation>
    </message>
    <message>
      <source>No Script</source>
      <translation>スクリプトなし</translation>
    </message>
    <message>
      <source>Viewer</source>
      <translation>ビューア</translation>
    </message>
    <message>
      <source>&amp;Use Encryption</source>
      <translation>暗号化を使用(&amp;U)</translation>
    </message>
    <message>
      <source>Passwords</source>
      <translation>パスワード</translation>
    </message>
    <message>
      <source>&amp;User:</source>
      <translation>ユーザ(&amp;U):</translation>
    </message>
    <message>
      <source>&amp;Owner:</source>
      <translation>所有者(&amp;O):</translation>
    </message>
    <message>
      <source>Settings</source>
      <translation>設定</translation>
    </message>
    <message>
      <source>Allow &amp;Printing the Document</source>
      <translation>ドキュメントの印刷を許可(&amp;P)</translation>
    </message>
    <message>
      <source>Allow &amp;Changing the Document</source>
      <translation>ドキュメントの変更を許可(&amp;C)</translation>
    </message>
    <message>
      <source>Allow Cop&amp;ying Text and Graphics</source>
      <translation>テキストとグラフィックのコピーを許可(&amp;Y)</translation>
    </message>
    <message>
      <source>Allow Adding &amp;Annotations and Fields</source>
      <translation>注釈とフィールドの追加を許可(&amp;A)</translation>
    </message>
    <message>
      <source>S&amp;ecurity</source>
      <translation>セキュリティ(&amp;E)</translation>
    </message>
    <message>
      <source>General</source>
      <translation>全般</translation>
    </message>
    <message>
      <source>Output &amp;Intended For:</source>
      <translation>出力の目的(&amp;I):</translation>
    </message>
    <message>
      <source>Screen / Web</source>
      <translation>画面 / ウェブ</translation>
    </message>
    <message>
      <source>Printer</source>
      <translation>プリンタ</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>グレースケール</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>スポットカラーをプロセスカラーに変換する</translation>
    </message>
    <message>
      <source>&amp;Use Custom Rendering Settings</source>
      <translation>カスタムのレンダリング設定を使用(&amp;U)</translation>
    </message>
    <message>
      <source>Rendering Settings</source>
      <translation>レンダリング設定</translation>
    </message>
    <message>
      <source>Fre&amp;quency:</source>
      <translation>周波数(&amp;Q):</translation>
    </message>
    <message>
      <source>&amp;Angle:</source>
      <translation>角度(&amp;A):</translation>
    </message>
    <message>
      <source>S&amp;pot Function:</source>
      <translation>スポット機能(&amp;P):</translation>
    </message>
    <message>
      <source>Simple Dot</source>
      <translation>単一点</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>線</translation>
    </message>
    <message>
      <source>Round</source>
      <translation>円</translation>
    </message>
    <message>
      <source>Ellipse</source>
      <translation>楕円</translation>
    </message>
    <message>
      <source>Solid Colors:</source>
      <translation>無色:</translation>
    </message>
    <message>
      <source>Profile:</source>
      <translation>プロファイル:</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>知覚</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>相対的な色彩</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>彩度</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>絶対的な色彩</translation>
    </message>
    <message>
      <source>Images:</source>
      <translation>画像:</translation>
    </message>
    <message>
      <source>C&amp;olor</source>
      <translation>色(&amp;O)</translation>
    </message>
    <message>
      <source>PDF/X-3 Output Intent</source>
      <translation>PDF/X-3 出力インテント</translation>
    </message>
    <message>
      <source>Output &amp;Profile:</source>
      <translation>プロファイルを出力(&amp;P):</translation>
    </message>
    <message>
      <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
      <translation>PDFにフォントを埋め込みます。フォントを埋め込むとドキュメントのレイアウトと見た目が保たれます。</translation>
    </message>
    <message>
      <source>Show page previews of each page listed above.</source>
      <translation>上のリストにある各ページのプレビューを表示</translation>
    </message>
    <message>
      <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
      <translation>エフェクトが有効になっている時間です。短い時間を指定するとエフェクトの速度が速くなり、長い時間だと遅くなります。</translation>
    </message>
    <message>
      <source>Type of the display effect.</source>
      <translation>表示エフェクトの種類</translation>
    </message>
    <message>
      <source>Direction of the effect of moving lines for the split and blind effects.</source>
      <translation>分割やブラインドエフェクトでのムービングラインのエフェクトの方向</translation>
    </message>
    <message>
      <source>Starting position for the box and split effects.</source>
      <translation>ボックスと分割エフェクトの開始位置</translation>
    </message>
    <message>
      <source>Direction of the glitter or wipe effects.</source>
      <translation>輝きもしくはワイプエフェクトの方向</translation>
    </message>
    <message>
      <source>Apply the selected effect to all pages.</source>
      <translation>選択されたエフェクトを全てのページに適用</translation>
    </message>
    <message>
      <source>Export all pages to PDF</source>
      <translation>全てのページをPDFにエクスポート</translation>
    </message>
    <message>
      <source>Export a range of pages to PDF</source>
      <translation>指定された範囲をPDFにエクスポート</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
      <translation>カンマで区切られたトークンのリストを入力してください。
トークンは*で全てのページを、1-5のようにしてページ範囲を、
もしくは単一のページ番号が利用できます。</translation>
    </message>
    <message>
      <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
      <translation>PDF中のページの結合を決定します。
変更する必要がなければ、デフォルトの選択 - 左マージン のままにしておいてください。</translation>
    </message>
    <message>
      <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
      <translation>PDF中にある各ページのサムネイルを生成します。
いくつかのビューワではサムネイルをナビゲーションとして利用できます。</translation>
    </message>
    <message>
      <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
      <translation>PDF中のリンクされた記事をナビゲートするのに役立つPDF記事を作成します。</translation>
    </message>
    <message>
      <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
      <translation>ドキュメント中で作成したブックマークを埋め込みます。長いPDFドキュメントをナビゲートするのに役にたちます。</translation>
    </message>
    <message>
      <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
      <translation>テキストとベクトルグラフィックの解像度を出力します。写真のように、ビットマップ画像の解像度には影響しません。</translation>
    </message>
    <message>
      <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
      <translation>テキストとグラフィックのロスレス圧縮を有効にします。理由がなければ、これをチェックしたままにしてください。これによりPDFのサイズが小さくなります。</translation>
    </message>
    <message>
      <source>DPI (Dots Per Inch) for image export.</source>
      <translation>出力画像のDPI (Dots Per Inch)</translation>
    </message>
    <message>
      <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
      <translation>出力されたPDFでセキュリティ機能を有効にします。PDF 1.3を選択した場合、PDFは40ビット暗号化で保護されます。PDF 1.4を選択した場合、PDFは128ビット暗号化で保護されます。
免責: PDF暗号化は、GPGやPGP暗号化ほどは信頼性が高くなく、いくつかの制限があります。</translation>
    </message>
    <message>
      <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
      <translation>出力するPDFで全てのセキュリティ機能を有効/無効にするマスターパスワードを選択</translation>
    </message>
    <message>
      <source>Choose a password for users to be able to read your PDF.</source>
      <translation>PDFが読めるユーザのパスワードを選択</translation>
    </message>
    <message>
      <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
      <translation>PDFの印刷を許可します。チェックされていなければ、印刷を禁止します。</translation>
    </message>
    <message>
      <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
      <translation>PDFの変更を許可します。チェックされていなければ、PDFの変更を禁止します。</translation>
    </message>
    <message>
      <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
      <translation>PDFからテキストやグラフィックのコピーを許可します。チェックされていなければ、テキストやグラフィックはコピーできません。</translation>
    </message>
    <message>
      <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
      <translation>PDFに注釈とフィールドの追加を許可します。チェックされていなければ、注釈とフィールドの編集を禁止します。</translation>
    </message>
    <message>
      <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
      <translation>これはデフォルトでは有効でない詳細設定です。プリンタから特に求められたり、正確な詳細が必要な場合にのみ有効にすべきです。そうでなければ、エクスポートしたPDFが適切に印刷されなかったり、システム間での真の互換性がなくなります。</translation>
    </message>
    <message>
      <source>Embed a color profile for solid colors</source>
      <translation>無色のカラープロファイルを埋め込む</translation>
    </message>
    <message>
      <source>Color profile for solid colors</source>
      <translation>無色のカラープロファイル</translation>
    </message>
    <message>
      <source>Rendering intent for solid colors</source>
      <translation>無色のレンダリングインテント</translation>
    </message>
    <message>
      <source>Embed a color profile for images</source>
      <translation>画像のカラープロファイルを埋め込む</translation>
    </message>
    <message>
      <source>Do not use color profiles that are embedded in source images</source>
      <translation>元の画像に埋め込まれたカラープロファイルを使用しない</translation>
    </message>
    <message>
      <source>Color profile for images</source>
      <translation>画像のカラープロファイル</translation>
    </message>
    <message>
      <source>Rendering intent for images</source>
      <translation>画像のレンダリングインテント</translation>
    </message>
    <message>
      <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
      <translation>印刷のための出力プロファイルです。できれば、プロファイル選択に関するプリンタの手引を入手してください。</translation>
    </message>
    <message>
      <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
      <translation>PDF/X-3またはPDFでの強制的な文字列はPDF/X-3の整合性を損ないます。ドキュメントのタイトルを使用することをお薦めします。</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation>物理ページの上から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation>物理ページの下から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation>物理ページの左から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation>物理ページの右から断ち切りまでの間隔</translation>
    </message>
    <message>
      <source>Mirror Page(s) horizontally</source>
      <translation>ページを水平方向に反転</translation>
    </message>
    <message>
      <source>Mirror Page(s) vertically</source>
      <translation>ページを垂直方向に反転</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>スポットカラーが合成色に変換されるのを有効にします。商業プリンタでスポットカラーを印刷するつもりでなければ、おそらく有効のままにしておくのが最適でしょう。</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>ページマージンで切り取る</translation>
    </message>
    <message>
      <source>Image Compression Method</source>
      <translation>画像圧縮法</translation>
    </message>
    <message>
      <source>Lossy - JPEG</source>
      <translation>ロスあり - JPEG</translation>
    </message>
    <message>
      <source>Lossless - Zip</source>
      <translation>ロスレス - Zip</translation>
    </message>
    <message>
      <source>Javascript to be executed
when PDF document is opened:</source>
      <translation>PDFドキュメントを開いた際にJavascriptを実行する:</translation>
    </message>
    <message>
      <source>Force Overprint Mode</source>
      <translation>重ね刷りモードを強制</translation>
    </message>
    <message>
      <source>&amp;Info String:</source>
      <translation>文字列の情報(&amp;I):</translation>
    </message>
    <message>
      <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
      <translation>フルスクリーンモードでAdobe&amp;#174; Reader&amp;#174;を使用した時にプレゼンテーションエフェクトを有効にします。</translation>
    </message>
    <message>
      <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
      <translation>選択されたページのプレゼンテーションを開始する前に、ページが表示されている時間。0に設定すると自動ページ遷移を行いません。</translation>
    </message>
    <message>
      <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
      <translation>ドキュメント中のレイヤーは、PDF 1.5を選択した場合のみPDFに出力されます。</translation>
    </message>
    <message>
      <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
      <translation>PDF出力のカラーモデルです。ディスプレイで使用するPDFや一般的なインクジェットで印刷する場合は 画面/Web を選択してください。4色CMYKプリンタで印刷する場合はプリンタを選択してください。グレースケールPDFにしたい場合は、グレースケールを選択してください。</translation>
    </message>
    <message>
      <source>Enables global Overprint Mode for this document, overrides object settings</source>
      <translation>このドキュメントで重ね刷りモードを有効にし、オブジェクト設定を上書きします</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins in the exported file</source>
      <translation>出力されたファイルでは、マージンの外側のオブジェクトを表示しません</translation>
    </message>
    <message>
      <source>Determines the PDF compatibility.&lt;br/>The default is &lt;b>PDF 1.3&lt;/b> which gives the widest compatibility.&lt;br/>Choose &lt;b>PDF 1.4&lt;/b> if your file uses features such as transparency or you require 128 bit encryption.&lt;br/>&lt;b>PDF 1.5&lt;/b> is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/>&lt;b>PDF/X-3&lt;/b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
      <translation>PDFの互換性を決定します。&lt;br/>デフォルトは幅広い互換性を持った&lt;b>PDF 1.3&lt;/b>です。&lt;br/>透明機能や128ビット暗号化のような機能をファイルで使用していれば、&lt;b>PDF 1.4&lt;/b>を選択してください。&lt;br/>&lt;b>PDF 1.5&lt;/b>は、PDF中の分割されたレイヤーに、オブジェクトを保持したいのであれば必要になります。&lt;br/>PDF/X-3は、商業印刷で色管理RGBを使いたい場合に、PDFを出力するのに向いており、カラーマネージメントを有効にすると選択可能になります。注意: PDF/X-3は全てのCMYK画像をICCベースのRGBカラーに変換します。プリンタで推奨されているか、一部の4色デジタルカラーレーザプリンタのときのみ使用してください。</translation>
    </message>
    <message>
      <source>&amp;Embed all</source>
      <translation>全て埋め込む(&amp;E)</translation>
    </message>
    <message>
      <source>Fonts to outline:</source>
      <translation>アウトライン化するフォント:</translation>
    </message>
    <message>
      <source>&amp;Outline all</source>
      <translation>全てアウトライン化(&amp;O)</translation>
    </message>
    <message>
      <source>Printer Marks</source>
      <translation>プリンタマーク</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>切り取りマーク</translation>
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>断ち切りマーク</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>登録マーク</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>カラーバー</translation>
    </message>
    <message>
      <source>Page Information</source>
      <translation>ページ情報</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>オフセット:</translation>
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation>断ち切り設定</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>上:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>下:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>左:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>右:</translation>
    </message>
    <message>
      <source>Use Document Bleeds</source>
      <translation>ドキュメントの断ち切りを使う</translation>
    </message>
    <message>
      <source>Pre-Press</source>
      <translation>製版</translation>
    </message>
    <message>
      <source>Convert all glyphs in the document to outlines.</source>
      <translation>ドキュメント中の全てのグリフをアウトラインに変換します。</translation>
    </message>
    <message>
      <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
      <translation>画像に対する圧縮法。自動にするとScribusが最適な方法を選びます。ZIPはロスレスで単色の画像に適しています。JPEGは多くの写真を含んでいる、小さなPDFファイルを作成するのに最適です(軽い画像品質の低下はありえますが)。特に圧縮オプションが必要でないのであれば、自動のままにしておいてください。</translation>
    </message>
    <message>
      <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
      <translation>ロスありの圧縮法を使ったときの、圧縮の品質レベル: 最低 (25%), 低 (50%), 中間 (75%), 高 (85%), 最高 (95%)です。品質レベルは、処理後の画像のサイズを直接決定するものではなく、指定したどの品質レベルでも、サイズと品質低下は画像によってまちまちであることに注意してくださ
い。最高を選択したとしても、JPEGでは常に品質のロスがあります。</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation>内側:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>外側:</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation>ドキュメントレイアウト</translation>
    </message>
    <message>
      <source>Maximum Image Resolution:</source>
      <translation>最大解像度:</translation>
    </message>
    <message>
      <source>Show the document in single page mode</source>
      <translation>単ページモードでドキュメントを表示</translation>
    </message>
    <message>
      <source>Show the document in single page mode with the pages displayed continuously end to end like a scroll</source>
      <translation>スクロールのように端から端まで連続してページを表示するような、単ページモードでドキュメントを表示</translation>
    </message>
    <message>
      <source>Show the document with facing pages, starting with the first page displayed on the left</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show the document with facing pages, starting with the first page displayed on the right</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the viewer's defaults or the user's preferences if set differently from the viewer defaults</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enables viewing the document in full screen</source>
      <translation>ドキュメントをフルスクリーンで閲覧する</translation>
    </message>
    <message>
      <source>Display the bookmarks upon opening</source>
      <translation>開く際にブックマークを表示</translation>
    </message>
    <message>
      <source>Display the page thumbnails upon opening</source>
      <translation>開く際にサムネイルページを表示</translation>
    </message>
    <message>
      <source>Forces the displaying of layers. Useful only for PDF 1.5+.</source>
      <translation>レイヤー表示を強制します。PDF 1.5+にのみ有効です。</translation>
    </message>
    <message>
      <source>Hides the Tool Bar which has selection and other editing capabilities</source>
      <translation>選択をしたり他の編集能力を持つツールバーを隠します</translation>
    </message>
    <message>
      <source>Hides the Menu Bar for the viewer, the PDF will display in a plain window. </source>
      <translation>ビューワのメニューバーを隠し、PDFをウィンドウ中に表示します。</translation>
    </message>
    <message>
      <source>Fit the document page or pages to the available space in the viewer window.</source>
      <translation>ドキュメントページをビューワウィンドウで有効なスペースに合わせます。</translation>
    </message>
    <message>
      <source>Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
      <translation>ビットマップ画像の解像度をを選択されたDPIに制限します。低解像度の画像では触らないままです。これをチェックしない場合、本来の解像度のままで描画します。これを有効にした場合、メモリ使用量が増えてエクスポートするのが遅くなります。</translation>
    </message>
    <message>
      <source>Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add registration marks to each separation</source>
      <translation>それぞれの版に追加される登録マークを追加します。</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation>カラーキャリブレーションバーを追加</translation>
    </message>
    <message>
      <source>Add document information which includes the document title and page numbers</source>
      <translation>ドキュメントタイトルとページ数を含んだドキュメント情報を追加します</translation>
    </message>
    <message>
      <source>Indicate the distance offset for the registration marks</source>
      <translation>登録マークへのオフセット距離を表示します</translation>
    </message>
    <message>
      <source>Use the existing bleed settings from the document preferences</source>
      <translation>ドキュメントの設定にある既存の断ち切り設定を使用します</translation>
    </message>
    <message>
      <source>Use Color Profile</source>
      <translation>カラープロファイルを使用</translation>
    </message>
    <message>
      <source>Do not use embedded color profiles</source>
      <translation>埋め込みカラープロファイルを使用しない</translation>
    </message>
    <message>
      <source>&amp;Apply Effect to all Pages</source>
      <translation>全てのページにエフェクトを適用(&amp;A)</translation>
    </message>
    <message>
      <source>Embed PDFs (EXPERIMENTAL)</source>
      <translation>PDF埋め込み (実験的)</translation>
    </message>
    <message>
      <source>Automatically rotate the exported pages</source>
      <translation>出力するページを自動的に回転する</translation>
    </message>
    <message>
      <source>Export PDFs in image frames as embedded PDFs. This does *not* yet take care of colorspaces, so you should know what you are doing before setting this to 'true'.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Push</source>
      <translation>押す</translation>
    </message>
    <message>
      <source>Cover</source>
      <translation>保護する</translation>
    </message>
    <message>
      <source>Uncover</source>
      <translation>保護しない</translation>
    </message>
    <message>
      <source>Fade</source>
      <translation>フェード</translation>
    </message>
    <message>
      <source>Rendering Intent:</source>
      <translation>レンダリングインテント:</translation>
    </message>
  </context>
  <context>
    <name>TabPrinter</name>
    <message>
      <source>TabPrinterBase</source>
      <translation>TabPrinterBase</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>オプション</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>ページ</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>ページを水平方向に反転</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>ページを垂直方向に反転</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation>メディアサイズを設定</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>ページマージンで切り取る</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>グレースケールで印刷</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation>可能ならカラーで印刷</translation>
    </message>
    <message>
      <source>Level 1</source>
      <translation>レベル 1</translation>
    </message>
    <message>
      <source>Level 2</source>
      <translation>レベル 2</translation>
    </message>
    <message>
      <source>Level 3</source>
      <translation>レベル 3</translation>
    </message>
    <message>
      <source>General</source>
      <translation>全般</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation>色分解して印刷</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>標準で印刷</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation>下色除去を適用</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>スポットカラーをプロセスカラーに変換する</translation>
    </message>
    <message>
      <source>Force Overprint Mode</source>
      <translation>重ね刷りモードを強制</translation>
    </message>
    <message>
      <source>Apply ICC Profiles</source>
      <translation>ICCプロファイルを適用</translation>
    </message>
    <message>
      <source>Marks &amp;&amp; Bleeds</source>
      <translation>マークと断ち切り</translation>
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation>断ち切り設定</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>上:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>下:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>左:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>右:</translation>
    </message>
    <message>
      <source>Printer Marks</source>
      <translation>プリンタマーク</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation>カラーキャリブレーションバーを追加</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>カラーバー</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>オフセット:</translation>
    </message>
    <message>
      <source>Add registration marks which are added to each separation</source>
      <translation>それぞれの版に追加される登録マークを追加します。</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>登録マーク</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>断ち切りマーク</translation>
    </message>
    <message>
      <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation>印刷後に紙を切断したり切り取ったりする目安となる切り取りマークをPDF中に作成します。</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>切り取りマーク</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation>印刷先</translation>
    </message>
    <message>
      <source>Alternative Printer Command</source>
      <translation>代わりのプリンタコマンド(&amp;L)</translation>
    </message>
    <message>
      <source>Command:</source>
      <translation>コマンド:</translation>
    </message>
    <message>
      <source>PostScript Options</source>
      <translation>PostScriptオプション</translation>
    </message>
  </context>
  <context>
    <name>TabScrapbook</name>
    <message>
      <source>This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Send Copied Items Automatically to Scrapbook</source>
      <translation>コピーされたアイテムを自動的にスクラップブックに送る</translation>
    </message>
    <message>
      <source>This enables copied items to be kept permanently in the scrapbook.</source>
      <translation>これを有効にすると、コピーされたアイテムをスクラップブック中に永久に保持します。</translation>
    </message>
    <message>
      <source>Keep Copied Items Permanently Across Sessions</source>
      <translation>セッションを越えて永久にコピーされたアイテムを保持します</translation>
    </message>
    <message>
      <source>The minimum number is 1; the maximum us 100.</source>
      <translation>最小の数は1で、最大の数は100です。</translation>
    </message>
    <message>
      <source>Number of Copied Items to Keep in Scrapbook:</source>
      <translation>スクラップブック中のコピーされたアイテム数:</translation>
    </message>
  </context>
  <context>
    <name>TabTools</name>
    <message>
      <source>Font:</source>
      <translation>フォント:</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Size:</source>
      <translation>サイズ:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>なし</translation>
    </message>
    <message>
      <source>Text Color:</source>
      <translation>テキスト色:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Shading:</source>
      <translation>色の濃さ:</translation>
    </message>
    <message>
      <source>Text Stroke:</source>
      <translation>テキストの輪郭:</translation>
    </message>
    <message>
      <source>Fill Color:</source>
      <translation>塗りつぶし色:</translation>
    </message>
    <message>
      <source>Stroke Color:</source>
      <translation>輪郭色:</translation>
    </message>
    <message>
      <source>Dot</source>
      <translation>ドット</translation>
    </message>
    <message>
      <source>Hyphen</source>
      <translation>ハイフン</translation>
    </message>
    <message>
      <source>Underscore</source>
      <translation>下線</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>カスタム</translation>
    </message>
    <message>
      <source>Tab Fill Character:</source>
      <translation>タブ埋め文字:</translation>
    </message>
    <message>
      <source>Tab Width:</source>
      <translation>タブ幅:</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>列数(&amp;M):</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>ギャップ(&amp;G):</translation>
    </message>
    <message>
      <source>Woven silk pyjamas exchanged for blue quartz</source>
      <translation>Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
      <source>&amp;Line Color:</source>
      <translation>線の色(&amp;L):</translation>
    </message>
    <message>
      <source>&amp;Shading:</source>
      <translation>色の濃さ(&amp;S):</translation>
    </message>
    <message>
      <source>&amp;Fill Color:</source>
      <translation>塗りつぶし色(&amp;F):</translation>
    </message>
    <message>
      <source>S&amp;hading:</source>
      <translation>色の濃さ(&amp;H):</translation>
    </message>
    <message>
      <source>Line Style:</source>
      <translation>線スタイル:</translation>
    </message>
    <message>
      <source>Line &amp;Width:</source>
      <translation>線幅(&amp;W):</translation>
    </message>
    <message>
      <source>Line S&amp;tyle:</source>
      <translation>線スタイル(&amp;T):</translation>
    </message>
    <message>
      <source>Arrows:</source>
      <translation>矢印:</translation>
    </message>
    <message>
      <source>Start:</source>
      <translation>開始:</translation>
    </message>
    <message>
      <source>End:</source>
      <translation>終了:</translation>
    </message>
    <message>
      <source>&amp;Free Scaling</source>
      <translation>自由倍率(&amp;F)</translation>
    </message>
    <message>
      <source>&amp;Horizontal Scaling:</source>
      <translation>水平方向の倍率(&amp;H):</translation>
    </message>
    <message>
      <source>&amp;Vertical Scaling:</source>
      <translation>垂直方向の倍率(&amp;V):</translation>
    </message>
    <message>
      <source>Keep Aspect &amp;Ratio</source>
      <translation>アスペクト比を保持(&amp;R)</translation>
    </message>
    <message>
      <source>F&amp;ill Color:</source>
      <translation>塗りつぶし色(&amp;I):</translation>
    </message>
    <message>
      <source>Use embedded Clipping Path</source>
      <translation>埋め込みクリッピングパスを使用</translation>
    </message>
    <message>
      <source>On Screen Preview</source>
      <translation>画面上のプレビュー</translation>
    </message>
    <message>
      <source>Full Resolution Preview</source>
      <translation>最大解像度のプレビュー</translation>
    </message>
    <message>
      <source>Normal Resolution Preview</source>
      <translation>通常解像度のプレビュー</translation>
    </message>
    <message>
      <source>Low Resolution Preview</source>
      <translation>低解像度のプレビュー</translation>
    </message>
    <message>
      <source>Mi&amp;nimum:</source>
      <translation>最小(&amp;N):</translation>
    </message>
    <message>
      <source>Ma&amp;ximum:</source>
      <translation>最大(&amp;X):</translation>
    </message>
    <message>
      <source>&amp;Stepping:</source>
      <translation>ステップ(&amp;S):</translation>
    </message>
    <message>
      <source>Text Frame Properties</source>
      <translation>テキストフレームのプロパティ</translation>
    </message>
    <message>
      <source>Shape Drawing Properties</source>
      <translation>形状の描画のプロパティ</translation>
    </message>
    <message>
      <source>Magnification Level Defaults</source>
      <translation>デフォルトの拡大率</translation>
    </message>
    <message>
      <source>Line Drawing Properties</source>
      <translation>線の描画プロパティ</translation>
    </message>
    <message>
      <source>Polygon Drawing Properties</source>
      <translation>多角形の描画プロパティ</translation>
    </message>
    <message>
      <source>Font for new text frames</source>
      <translation>新規テキストフレームのフォント</translation>
    </message>
    <message>
      <source>Size of font for new text frames</source>
      <translation>新規テキストフレームのフォントサイズ</translation>
    </message>
    <message>
      <source>Color of font</source>
      <translation>フォントの色</translation>
    </message>
    <message>
      <source>Number of columns in a text frame</source>
      <translation>テキストフレーム中の列数</translation>
    </message>
    <message>
      <source>Gap between text frame columns</source>
      <translation>テキストフレームの列の間のギャップ</translation>
    </message>
    <message>
      <source>Sample of your font</source>
      <translation>フォントのサンプル</translation>
    </message>
    <message>
      <source>Horizontal scaling of images</source>
      <translation>画像の水平方向の倍率</translation>
    </message>
    <message>
      <source>Vertical scaling of images</source>
      <translation>画像の垂直方向の倍率</translation>
    </message>
    <message>
      <source>Keep horizontal and vertical scaling the same</source>
      <translation>水平方向と垂直方向の倍率を同じに保ちます</translation>
    </message>
    <message>
      <source>Saturation of color of fill</source>
      <translation>塗りつぶし色の彩度</translation>
    </message>
    <message>
      <source>Line color of shapes</source>
      <translation>形状の線の色</translation>
    </message>
    <message>
      <source>Saturation of color of lines</source>
      <translation>線の色の彩度</translation>
    </message>
    <message>
      <source>Fill color of shapes</source>
      <translation>形状の塗りつぶし色</translation>
    </message>
    <message>
      <source>Line style of shapes</source>
      <translation>形状の線スタイル</translation>
    </message>
    <message>
      <source>Line width of shapes</source>
      <translation>形状の線幅</translation>
    </message>
    <message>
      <source>Minimum magnification allowed</source>
      <translation>利用できる最小の拡大率</translation>
    </message>
    <message>
      <source>Maximum magnification allowed</source>
      <translation>利用できる最大の拡大率</translation>
    </message>
    <message>
      <source>Change in magnification for each zoom operation</source>
      <translation>ズーム操作のたびに倍率が変化します</translation>
    </message>
    <message>
      <source>Color of lines</source>
      <translation>線の色</translation>
    </message>
    <message>
      <source>Saturation of color</source>
      <translation>色の彩度</translation>
    </message>
    <message>
      <source>Style of lines</source>
      <translation>線のスタイル</translation>
    </message>
    <message>
      <source>Width of lines</source>
      <translation>線幅</translation>
    </message>
    <message>
      <source>None</source>
      <comment>tab fill</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>テキスト</translation>
    </message>
    <message>
      <source>Shapes</source>
      <translation>形状</translation>
    </message>
    <message>
      <source>Lines</source>
      <translation>線</translation>
    </message>
    <message>
      <source>Images</source>
      <translation>画像</translation>
    </message>
    <message>
      <source>Regular Polygons</source>
      <translation>正多角形</translation>
    </message>
    <message>
      <source>Zoom</source>
      <translation>ズーム</translation>
    </message>
    <message>
      <source>Rotation Tool</source>
      <translation>回転ツール</translation>
    </message>
    <message>
      <source>Constrain to:</source>
      <translation>制約:</translation>
    </message>
    <message>
      <source>Other Properties</source>
      <translation>その他のプロパティ</translation>
    </message>
    <message>
      <source>Miscellaneous Settings</source>
      <translation>その他の設定</translation>
    </message>
    <message>
      <source>Item Duplicate</source>
      <translation>アイテムのコピー</translation>
    </message>
    <message>
      <source>X Displacement</source>
      <translation>X位置ずれ</translation>
    </message>
    <message>
      <source>Y Displacement</source>
      <translation>Y位置ずれ</translation>
    </message>
    <message>
      <source>Horizontal displacement of page items</source>
      <translation>ページアイテムの水平位置ずれ</translation>
    </message>
    <message>
      <source>Vertical displacement of page items</source>
      <translation>ページアイテムの垂直位置ずれ</translation>
    </message>
    <message>
      <source>Constrain value for the rotation tool when the Control key is pressed</source>
      <translation>コントロールキーを押したときに回転ツールの値を制限します</translation>
    </message>
    <message>
      <source>Degrees</source>
      <translation>度</translation>
    </message>
    <message>
      <source>Use the embedded clipping paths in images when importing them. JPEG, PSD and TIFF are the image formats which can embedded clipping paths.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Scale Image to Frame Size</source>
      <translation>フレームサイズに合わせて画像を拡大縮小(&amp;S)</translation>
    </message>
    <message>
      <source>Image Frame Properties</source>
      <translation>画像フレームのプロパティ</translation>
    </message>
    <message>
      <source>Image frames allow images to scale to any size</source>
      <translation>画像フレームは画像をどのサイズにも拡大縮小できます</translation>
    </message>
    <message>
      <source>Images in image frames are scaled to the size of the frame</source>
      <translation>画像フレーム中の画像はフレームのサイズに合わせて拡大縮小されます</translation>
    </message>
    <message>
      <source>Automatically scaled images keep their original proportions</source>
      <translation>自動的に拡大縮小された画像をオリジナルの比率に保ちます</translation>
    </message>
    <message>
      <source>Fill color of image frames</source>
      <translation>画像フレームの塗りつぶし色</translation>
    </message>
  </context>
  <context>
    <name>TabTypograpy</name>
    <message>
      <source>Subscript</source>
      <translation>下付き文字</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Displacement:</source>
      <translation>位置ずれ(&amp;D):</translation>
    </message>
    <message>
      <source>&amp;Scaling:</source>
      <translation>倍率(&amp;S):</translation>
    </message>
    <message>
      <source>Superscript</source>
      <translation>上付き文字</translation>
    </message>
    <message>
      <source>D&amp;isplacement:</source>
      <translation>位置ずれ(&amp;I):</translation>
    </message>
    <message>
      <source>S&amp;caling:</source>
      <translation>倍率(&amp;C):</translation>
    </message>
    <message>
      <source>Underline</source>
      <translation>下線</translation>
    </message>
    <message>
      <source>Displacement:</source>
      <translation>位置ずれ</translation>
    </message>
    <message>
      <source>Auto</source>
      <translation>自動</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>線幅:</translation>
    </message>
    <message>
      <source>Strikethru</source>
      <translation>取り消し線</translation>
    </message>
    <message>
      <source>Small Caps</source>
      <translation>スモールキャピタル</translation>
    </message>
    <message>
      <source>Sc&amp;aling:</source>
      <translation>倍率(&amp;A):</translation>
    </message>
    <message>
      <source>Automatic &amp;Line Spacing</source>
      <translation>自動行間隔(&amp;L)</translation>
    </message>
    <message>
      <source>Line Spacing:</source>
      <translation>行間隔:</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the font on a line</source>
      <translation>同じ高さのフォントのベースラインより上の位置ずれ</translation>
    </message>
    <message>
      <source>Relative size of the superscript compared to the normal font</source>
      <translation>通常フォントに対する上付き文字の相対サイズ</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font on a line</source>
      <translation>同じ高さの通常フォントのベースラインより下の位置ずれ</translation>
    </message>
    <message>
      <source>Relative size of the subscript compared to the normal font</source>
      <translation>通常フォントに対する下付き文字の相対サイズ</translation>
    </message>
    <message>
      <source>Relative size of the small caps font compared to the normal font</source>
      <translation>通常フォントに対するスモールキャピタルフォントの相対サイズ</translation>
    </message>
    <message>
      <source>Percentage increase over the font size for the line spacing</source>
      <translation>行間隔のフォントサイズで増加するパーセンテージ</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line width expressed as a percentage of the font size</source>
      <translation>フォントサイズのパーセンテージで表示される線幅</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
      <translation type="unfinished" />
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
      <source>Comma</source>
      <translation>コンマ</translation>
    </message>
    <message>
      <source>Center</source>
      <translation>中央</translation>
    </message>
    <message>
      <source>&amp;Position:</source>
      <translation>位置(&amp;P):</translation>
    </message>
    <message>
      <source>Dot</source>
      <translation>ドット</translation>
    </message>
    <message>
      <source>Hyphen</source>
      <translation>ハイフン</translation>
    </message>
    <message>
      <source>Underscore</source>
      <translation>下線</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>カスタム</translation>
    </message>
    <message>
      <source>Fill Char:</source>
      <translation>埋め文字:</translation>
    </message>
    <message>
      <source>Delete All</source>
      <translation>全て削除</translation>
    </message>
    <message>
      <source>Indentation for first line of the paragraph</source>
      <translation>段落の先頭行に対するインデント</translation>
    </message>
    <message>
      <source>Indentation from the left for the whole paragraph</source>
      <translation>段落全体に対する左側のインデント</translation>
    </message>
    <message>
      <source>Delete all Tabulators</source>
      <translation>全てのタブを削除</translation>
    </message>
    <message>
      <source>None</source>
      <comment>tab fill</comment>
      <translation>なし</translation>
    </message>
    <message>
      <source>Indentation from the right for the whole paragraph</source>
      <translation>段落全体に対する右側のインデント</translation>
    </message>
    <message>
      <source>Fill Character of Tab</source>
      <translation>タブ文字で埋める</translation>
    </message>
    <message>
      <source>Type/Orientation of Tab</source>
      <translation>タブの種類/方向</translation>
    </message>
    <message>
      <source>Position of Tab</source>
      <translation>タブの位置</translation>
    </message>
    <message>
      <source>Period</source>
      <translation>ピリオド</translation>
    </message>
  </context>
  <context>
    <name>TransformDialog</name>
    <message>
      <source>Scaling</source>
      <translation>倍率</translation>
    </message>
    <message>
      <source>Translation</source>
      <translation>変換</translation>
    </message>
    <message>
      <source>Rotation</source>
      <translation>回転</translation>
    </message>
    <message>
      <source>Skewing</source>
      <translation>歪曲</translation>
    </message>
    <message>
      <source>Scale</source>
      <translation>拡大</translation>
    </message>
    <message>
      <source>Scale H = %1 % V = %2 %</source>
      <translation>拡大 H = %1 % V = %2 %</translation>
    </message>
    <message>
      <source>Translate</source>
      <translation>変換</translation>
    </message>
    <message>
      <source>Translate H = %1%2 V = %3%4</source>
      <translation>変換 H = %1%2 V = %3%4</translation>
    </message>
    <message>
      <source>Rotate</source>
      <translation>回転</translation>
    </message>
    <message>
      <source>Rotate Angle = %1%2</source>
      <translation>回転角 - %1%2</translation>
    </message>
    <message>
      <source>Skew</source>
      <translation>歪曲</translation>
    </message>
    <message>
      <source>Skew H = %1%2 V = %3%4</source>
      <translation>歪曲 H = %1%2 V = %3%4</translation>
    </message>
  </context>
  <context>
    <name>TransformDialogBase</name>
    <message>
      <source>Transform</source>
      <translation>変形</translation>
    </message>
    <message>
      <source>Add</source>
      <translation>追加</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>u</source>
      <translation>u</translation>
    </message>
    <message>
      <source>d</source>
      <translation>d</translation>
    </message>
    <message>
      <source>Scaling</source>
      <translation>倍率</translation>
    </message>
    <message>
      <source>Horizontal</source>
      <translation>水平</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Vertical</source>
      <translation>垂直</translation>
    </message>
    <message>
      <source>Translation</source>
      <translation>変換</translation>
    </message>
    <message>
      <source>Rotate</source>
      <translation>回転</translation>
    </message>
    <message>
      <source>Angle</source>
      <translation>角度</translation>
    </message>
    <message>
      <source>Skew</source>
      <translation>歪曲</translation>
    </message>
    <message>
      <source>Origin</source>
      <translation>始点</translation>
    </message>
    <message>
      <source>Copies</source>
      <translation>部</translation>
    </message>
  </context>
  <context>
    <name>TransformEffectPlugin</name>
    <message>
      <source>Transform...</source>
      <translation>変形...</translation>
    </message>
    <message>
      <source>Transform Effect</source>
      <translation>変形効果</translation>
    </message>
    <message>
      <source>Apply multiple transformations at once</source>
      <translation>一度に複数の変形を適用</translation>
    </message>
  </context>
  <context>
    <name>UnderlineValues</name>
    <message>
      <source>Auto</source>
      <translation>自動</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Displacement</source>
      <translation>位置ずれ</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>線幅</translation>
    </message>
  </context>
  <context>
    <name>UndoManager</name>
    <message>
      <source>Add vertical guide</source>
      <translation>垂直ガイドを追加</translation>
    </message>
    <message>
      <source>Add horizontal guide</source>
      <translation>水平ガイドを追加</translation>
    </message>
    <message>
      <source>Remove vertical guide</source>
      <translation>垂直ガイドを削除</translation>
    </message>
    <message>
      <source>Remove horizontal guide</source>
      <translation>水平ガイドを削除</translation>
    </message>
    <message>
      <source>Move vertical guide</source>
      <translation>垂直ガイドを移動</translation>
    </message>
    <message>
      <source>Move horizontal guide</source>
      <translation>水平ガイドを移動</translation>
    </message>
    <message>
      <source>Lock guides</source>
      <translation>ガイドをロック</translation>
    </message>
    <message>
      <source>Unlock guides</source>
      <translation>ガイドをロック解除</translation>
    </message>
    <message>
      <source>Move</source>
      <translation>移動</translation>
    </message>
    <message>
      <source>Resize</source>
      <translation>サイズ変更</translation>
    </message>
    <message>
      <source>Rotate</source>
      <translation>回転</translation>
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
      <translation>選択</translation>
    </message>
    <message>
      <source>Group</source>
      <translation>グループ</translation>
    </message>
    <message>
      <source>Selection/Group</source>
      <translation>選択/グループ</translation>
    </message>
    <message>
      <source>Create</source>
      <translation>作成</translation>
    </message>
    <message>
      <source>X: %1, Y: %2
W: %3, H: %4</source>
      <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
      <source>Align/Distribute</source>
      <translation>整列/配置</translation>
    </message>
    <message>
      <source>Items involved</source>
      <translation>アイテムは含まれています</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>キャンセル</translation>
    </message>
    <message>
      <source>Set fill color</source>
      <translation>塗りつぶし色を設定</translation>
    </message>
    <message>
      <source>Color1: %1, Color2: %2</source>
      <translation>色1: %1, 色2: %2</translation>
    </message>
    <message>
      <source>Set fill color shade</source>
      <translation>塗りつぶし色の濃さを設定</translation>
    </message>
    <message>
      <source>Set line color</source>
      <translation>線の色を設定</translation>
    </message>
    <message>
      <source>Set line color shade</source>
      <translation>線の色の濃さを設定</translation>
    </message>
    <message>
      <source>Flip horizontally</source>
      <translation>水平方向に反転</translation>
    </message>
    <message>
      <source>Flip vertically</source>
      <translation>垂直方向に反転</translation>
    </message>
    <message>
      <source>Lock</source>
      <translation>ロック</translation>
    </message>
    <message>
      <source>Unlock</source>
      <translation>ロック解除</translation>
    </message>
    <message>
      <source>Lock size</source>
      <translation>サイズをロック</translation>
    </message>
    <message>
      <source>Unlock size</source>
      <translation>サイズをロック解除</translation>
    </message>
    <message>
      <source>Ungroup</source>
      <translation>グループ解除</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation>名前を変更</translation>
    </message>
    <message>
      <source>From %1
to %2</source>
      <translation>%1 から %2</translation>
    </message>
    <message>
      <source>Apply Master Page</source>
      <translation>マスターページに適用</translation>
    </message>
    <message>
      <source>Paste</source>
      <translation>貼り付け</translation>
    </message>
    <message>
      <source>Cut</source>
      <translation>切り取り</translation>
    </message>
    <message>
      <source>Set fill color transparency</source>
      <translation>塗りつぶし色の透明度を設定</translation>
    </message>
    <message>
      <source>Set line color transparency</source>
      <translation>線の色の透明度を設定</translation>
    </message>
    <message>
      <source>Set line style</source>
      <translation>線スタイルを設定</translation>
    </message>
    <message>
      <source>Set the style of line end</source>
      <translation>線の端のスタイルを設定</translation>
    </message>
    <message>
      <source>Set the style of line join</source>
      <translation>線の結合のスタイルを設定</translation>
    </message>
    <message>
      <source>Set line width</source>
      <translation>線幅を設定</translation>
    </message>
    <message>
      <source>No style</source>
      <translation>スタイルなし</translation>
    </message>
    <message>
      <source>Set custom line style</source>
      <translation>カスタム線スタイルを設定</translation>
    </message>
    <message>
      <source>Do not use custom line style</source>
      <translation>カスタム線スタイルを使用しない</translation>
    </message>
    <message>
      <source>Set start arrow</source>
      <translation>始点矢印の設定</translation>
    </message>
    <message>
      <source>Set end arrow</source>
      <translation>終端矢印の設定</translation>
    </message>
    <message>
      <source>Create table</source>
      <translation>テーブルを作成</translation>
    </message>
    <message>
      <source>Rows: %1, Cols: %2</source>
      <translation>行: %1, 列: %2</translation>
    </message>
    <message>
      <source>Set font</source>
      <translation>フォントを設定</translation>
    </message>
    <message>
      <source>Set font size</source>
      <translation>フォントサイズを設定</translation>
    </message>
    <message>
      <source>Set font width</source>
      <translation>フォント幅を設定</translation>
    </message>
    <message>
      <source>Set font height</source>
      <translation>フォント高さを設定</translation>
    </message>
    <message>
      <source>Set font fill color</source>
      <translation>フォントの塗りつぶし色を設定</translation>
    </message>
    <message>
      <source>Set font stroke color</source>
      <translation>フォントの輪郭色を設定</translation>
    </message>
    <message>
      <source>Set font fill color shade</source>
      <translation>フォントの塗りつぶし色の濃さを設定</translation>
    </message>
    <message>
      <source>Set font stroke color shade</source>
      <translation>フォントの輪郭色の濃さを設定</translation>
    </message>
    <message>
      <source>Set kerning</source>
      <translation>カーニングの設定</translation>
    </message>
    <message>
      <source>Set line spacing</source>
      <translation>行間隔の設定</translation>
    </message>
    <message>
      <source>Set paragraph style</source>
      <translation>段落スタイルを設定</translation>
    </message>
    <message>
      <source>Set language</source>
      <translation>言語を設定</translation>
    </message>
    <message>
      <source>Align text</source>
      <translation>テキストを整列</translation>
    </message>
    <message>
      <source>Set font effect</source>
      <translation>フォントエフェクトを設定</translation>
    </message>
    <message>
      <source>Image frame</source>
      <translation>画像フレーム</translation>
    </message>
    <message>
      <source>Text frame</source>
      <translation>テキストフレーム</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation>多角形</translation>
    </message>
    <message>
      <source>Bezier curve</source>
      <translation>ベジエ曲線</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>ポリライン</translation>
    </message>
    <message>
      <source>Text on a Path</source>
      <translation>パス上のテキスト</translation>
    </message>
    <message>
      <source>Convert to</source>
      <translation>変換</translation>
    </message>
    <message>
      <source>Import SVG image</source>
      <translation>SVG画像をインポート</translation>
    </message>
    <message>
      <source>Import EPS image</source>
      <translation>EPS画像をインポート</translation>
    </message>
    <message>
      <source>Import OpenOffice.org Draw image</source>
      <translation>OpenOffice.org Draw画像をインポート</translation>
    </message>
    <message>
      <source>Scratch space</source>
      <translation>スクラッチスペース</translation>
    </message>
    <message>
      <source>Text flows around the frame</source>
      <translation>フレーム周りのテキストの回り込み</translation>
    </message>
    <message>
      <source>Text flows around bounding box</source>
      <translation>バウンディングボックス周りのテキストの回り込み</translation>
    </message>
    <message>
      <source>Text flows around contour line</source>
      <translation>等高線周りのテキストの回り込み</translation>
    </message>
    <message>
      <source>No text flow</source>
      <translation>テキストの回り込みなし</translation>
    </message>
    <message>
      <source>No bounding box</source>
      <translation>バウンディングボックスなし</translation>
    </message>
    <message>
      <source>No contour line</source>
      <translation>等高線なし</translation>
    </message>
    <message>
      <source>Page %1</source>
      <translation>ページ %1</translation>
    </message>
    <message>
      <source>Set image scaling</source>
      <translation>画像倍率を設定</translation>
    </message>
    <message>
      <source>Frame size</source>
      <translation>フレームサイズ</translation>
    </message>
    <message>
      <source>Free scaling</source>
      <translation>自由倍率</translation>
    </message>
    <message>
      <source>Keep aspect ratio</source>
      <translation>アスペクト比を保持</translation>
    </message>
    <message>
      <source>Break aspect ratio</source>
      <translation>アスペクト比を保持しない</translation>
    </message>
    <message>
      <source>Edit contour line</source>
      <translation>等高線を編集</translation>
    </message>
    <message>
      <source>Edit shape</source>
      <translation>形状を編集</translation>
    </message>
    <message>
      <source>Reset contour line</source>
      <translation>等高線をリセット</translation>
    </message>
    <message>
      <source>Add page</source>
      <translation>ページを追加</translation>
    </message>
    <message>
      <source>Add pages</source>
      <translation>ページを追加</translation>
    </message>
    <message>
      <source>Delete page</source>
      <translation>ページを削除</translation>
    </message>
    <message>
      <source>Delete pages</source>
      <translation>ページを削除</translation>
    </message>
    <message>
      <source>Add layer</source>
      <translation>レイヤーを追加</translation>
    </message>
    <message>
      <source>Delete layer</source>
      <translation>レイヤーを削除</translation>
    </message>
    <message>
      <source>Rename layer</source>
      <translation>レイヤー名を変更</translation>
    </message>
    <message>
      <source>Raise layer</source>
      <translation>レイヤーを上げる</translation>
    </message>
    <message>
      <source>Lower layer</source>
      <translation>レイヤーを下げる</translation>
    </message>
    <message>
      <source>Send to layer</source>
      <translation>レイヤーに送る</translation>
    </message>
    <message>
      <source>Enable printing of layer</source>
      <translation>レイヤーの印刷を有効にする</translation>
    </message>
    <message>
      <source>Disable printing of layer</source>
      <translation>レイヤーの印刷を無効にする</translation>
    </message>
    <message>
      <source>Change name of the layer</source>
      <translation>レイヤーの名前を変更</translation>
    </message>
    <message>
      <source>Get image</source>
      <translation>画像を取得</translation>
    </message>
    <message>
      <source>Change Image Offset</source>
      <translation>画像オフセットを変更</translation>
    </message>
    <message>
      <source>Change Image Scale</source>
      <translation>画像倍率を変更</translation>
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
      <source>Enable Item Printing</source>
      <translation>アイテムの印刷を有効に</translation>
    </message>
    <message>
      <source>Disable Item Printing</source>
      <translation>アイテムの印刷を無効に</translation>
    </message>
    <message>
      <source>Multiple duplicate</source>
      <translation>複数コピー</translation>
    </message>
    <message>
      <source>Apply text style</source>
      <translation>テキストスタイルを適用</translation>
    </message>
    <message>
      <source>&amp;Undo: %1</source>
      <comment>f.e. Undo: Move</comment>
      <translation>元に戻す(&amp;U): %1</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>元に戻す(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Redo: %1</source>
      <comment>f.e. Redo: Move</comment>
      <translation>やり直し(&amp;R): %1</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>やり直し(&amp;R)</translation>
    </message>
    <message>
      <source>No object frame</source>
      <translation>オブジェクトフレームがありません</translation>
    </message>
    <message>
      <source>Reset control point</source>
      <translation>コントロールポイントをリセット</translation>
    </message>
    <message>
      <source>Reset control points</source>
      <translation>コントロールポイントをリセット</translation>
    </message>
    <message>
      <source>Apply image effects</source>
      <translation>画像エフェクトを適用</translation>
    </message>
    <message>
      <source>Insert frame</source>
      <translation>フレームを挿入</translation>
    </message>
    <message>
      <source>Adjust frame to the image size</source>
      <translation>画像サイズにフレームをあわせる</translation>
    </message>
    <message>
      <source>Set start and end arrows</source>
      <translation>初めと終わりの矢印を設定</translation>
    </message>
    <message>
      <source>Remove vertical auto guide</source>
      <translation>垂直自動ガイドを削除</translation>
    </message>
    <message>
      <source>Remove horizontal auto guide</source>
      <translation>水平自動ガイドを削除</translation>
    </message>
    <message>
      <source>Text flows around image clipping path</source>
      <translation>画像クリッピングパスのテキストの回りこみ</translation>
    </message>
    <message>
      <source>Remove all guides</source>
      <translation>すべてのガイドを削除</translation>
    </message>
    <message>
      <source>Remove page guides</source>
      <translation>ページガイドを削除</translation>
    </message>
    <message>
      <source>Copy</source>
      <translation>コピー</translation>
    </message>
    <message>
      <source>Copy page</source>
      <translation>ページをコピー</translation>
    </message>
    <message>
      <source>Convert to outlines</source>
      <translation>アウトラインに変換</translation>
    </message>
    <message>
      <source>Change formula</source>
      <translation>方式を変更</translation>
    </message>
    <message>
      <source>Import AI drawing</source>
      <translation>AI図面のインポート</translation>
    </message>
    <message>
      <source>Import XFig drawing</source>
      <translation>XFig図面のインポート</translation>
    </message>
    <message>
      <source>Render frame</source>
      <translation>フレームを描画</translation>
    </message>
    <message>
      <source>Import Barcode</source>
      <translation>バーコードをインポート</translation>
    </message>
    <message>
      <source>Duplicate layer %1</source>
      <translation>複製レイヤ %1</translation>
    </message>
  </context>
  <context>
    <name>UndoPalette</name>
    <message>
      <source>Initial State</source>
      <translation>初期状態</translation>
    </message>
    <message>
      <source>Action History</source>
      <translation>アクション履歴</translation>
    </message>
    <message>
      <source>Show selected object only</source>
      <translation>選択されたオブジェクトのみ表示</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>元に戻す(&amp;U)</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>やり直し(&amp;R)</translation>
    </message>
    <message>
      <source>Show the action history for the selected item only. This changes the effect of the undo/redo buttons to act on the object or document.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Undo the last action for either the current object or the document</source>
      <translation>現在のオブジェクトまたはドキュメントの最後のアクションを元に戻します</translation>
    </message>
    <message>
      <source>Redo the last action for either the current object or the document</source>
      <translation>現在のオブジェクトまたはドキュメントの最後のアクションをやり直します</translation>
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
    <name>UnicodeSearch</name>
    <message>
      <source>Unicode Search</source>
      <translation>Unicode検索</translation>
    </message>
    <message>
      <source>&amp;Search:</source>
      <translation>検索(&amp;S):</translation>
    </message>
  </context>
  <context>
    <name>UpgradeChecker</name>
    <message>
      <source>Attempting to get the Scribus version update file</source>
      <translation>Scribusのバージョンアップファイルを取得しています</translation>
    </message>
    <message>
      <source>(No data on your computer will be sent to an external location)</source>
      <translation>(あなたのコンピュータ上のデータを外部に送信することはありません)</translation>
    </message>
    <message>
      <source>Timed out when attempting to get update file.</source>
      <translation>アップデートファイルを取得中にタイムアウトしました</translation>
    </message>
    <message>
      <source>File not found on server</source>
      <translation>ファイルがサーバに見つかりません</translation>
    </message>
    <message>
      <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
      <translation>以下のバージョンファイルを開けません: %1 
エラー: %2 行: %3 列: %4</translation>
    </message>
    <message>
      <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
      <translation>Scribusのアップデートを検索中にエラーが起きました。インターネット接続を確認してください。</translation>
    </message>
    <message>
      <source>No updates are available for your version of Scribus %1</source>
      <translation>現在のバージョンのScribus %1 に関するアップデートはありません</translation>
    </message>
    <message>
      <source>One or more updates for your version of Scribus (%1) are available:</source>
      <translation>現在のバージョンのScribus (%1) に関するアップデートが利用可能です:</translation>
    </message>
    <message>
      <source>Please visit www.scribus.net for details.</source>
      <translation>詳細は www.scribus.net を参照してください</translation>
    </message>
    <message>
      <source>Finished</source>
      <translation>終了</translation>
    </message>
    <message>
      <source>Operation canceled</source>
      <translation>操作をキャンセルしました</translation>
    </message>
    <message>
      <source>This list may contain development/unstable versions.</source>
      <translation>このリストには開発/不安定版が含まれている可能性があります。</translation>
    </message>
    <message>
      <source>Error: %1</source>
      <translation>エラー: %1</translation>
    </message>
  </context>
  <context>
    <name>UrlLauncher</name>
    <message>
      <source>Locate your web browser</source>
      <translation>Webブラウザの位置</translation>
    </message>
    <message>
      <source>External Web Browser Failed to Start</source>
      <translation>外部ウェブブラウザの起動に失敗しました</translation>
    </message>
    <message>
      <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences.
Would you like to start the system's default browser instead?</source>
      <translation>Scribusは外部のウェブブラウザ %1 を開始することができませんでした。</translation>
    </message>
  </context>
  <context>
    <name>UsePrinterMarginsDialog</name>
    <message>
      <source>Use Printer Margins</source>
      <translation>プリンタマージンを使用</translation>
    </message>
    <message>
      <source>Select &amp;Printer:</source>
      <translation>プリンタを選択(&amp;P):</translation>
    </message>
    <message>
      <source>Margins</source>
      <translation>マージン</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>右:</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>上(&amp;T):</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>下(&amp;B):</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>左(&amp;L):</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>OK(&amp;O)</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>キャンセル(&amp;C)</translation>
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
      <translation>グループ%1</translation>
    </message>
  </context>
  <context>
    <name>WMFImportPlugin</name>
    <message>
      <source>Import &amp;WMF...</source>
      <translation>WMFをインポート(&amp;W)...</translation>
    </message>
    <message>
      <source>Imports WMF Files</source>
      <translation>WMFファイルをインポート</translation>
    </message>
    <message>
      <source>Imports most WMF files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>現在のドキュメントに、ベクトルデータをScribusオブジェクトに変換して、ほとんどのWMFファイルをインポートします。</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation>ファイルがインポートできません</translation>
    </message>
    <message>
      <source>WMF file contains some unsupported features</source>
      <translation>WMFファイルはサポートされていない機能を含んでいます</translation>
    </message>
  </context>
  <context>
    <name>XfigPlug</name>
    <message>
      <source>Importing: %1</source>
      <translation>インポート中: %1</translation>
    </message>
    <message>
      <source>Analyzing File:</source>
      <translation>ファイルを解析:</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>グループ%1</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>アイテムを生成</translation>
    </message>
  </context>
  <context>
    <name>gtFileDialog</name>
    <message>
      <source>Open</source>
      <translation>開く</translation>
    </message>
    <message>
      <source>&amp;Importer:</source>
      <translation>インポータ(&amp;I):</translation>
    </message>
    <message>
      <source>Import &amp;Text Only</source>
      <translation>テキストのみインポート(&amp;T)</translation>
    </message>
    <message>
      <source>&amp;Encoding:</source>
      <translation>エンコーディング(&amp;E):</translation>
    </message>
  </context>
  <context>
    <name>gtImporterDialog</name>
    <message>
      <source>Choose the importer to use</source>
      <translation>使用するインポータを選択</translation>
    </message>
    <message>
      <source>Remember association</source>
      <translation>関連付けを記憶</translation>
    </message>
    <message>
      <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
      <translation>ファイル拡張子を記憶する - インポータを関連付け、このタイプのファイルに対するインポータを選択すると再度尋ねません</translation>
    </message>
  </context>
  <context>
    <name>hysettingsBase</name>
    <message>
      <source>Form</source>
      <translation>フォーム</translation>
    </message>
    <message>
      <source>General Options</source>
      <translation>一般オプション</translation>
    </message>
    <message>
      <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
      <translation>拡張 - テキストをハイフンでつなぐ オプションを使用すると、それぞれの単語に対して、利用できる全てのハイフンを提案するダイアログボックスが表示されます。</translation>
    </message>
    <message>
      <source>&amp;Hyphenation Suggestions</source>
      <translation>ハイフネーションの提案(&amp;H)</translation>
    </message>
    <message>
      <source>Enables automatic hyphenation of your text while typing.</source>
      <translation>入力中にテキストの自動ハイフネーションを有効にする</translation>
    </message>
    <message>
      <source>Hyphenate Text Automatically &amp;During Typing</source>
      <translation>入力中に自動的にテキストをハイフンでつなぐ(&amp;D)</translation>
    </message>
    <message>
      <source>Behaviour</source>
      <translation>挙動</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>言語(&amp;L):</translation>
    </message>
    <message>
      <source>&amp;Smallest Word:</source>
      <translation>最小の単語数(&amp;S):</translation>
    </message>
    <message>
      <source>Length of the smallest word to be hyphenated.</source>
      <translation>単語をハイフンで結ぶ最小の長さ</translation>
    </message>
    <message>
      <source>Chars</source>
      <translation>文字</translation>
    </message>
    <message>
      <source>Consecutive Hyphenations &amp;Allowed:</source>
      <translation>連続したハイフネーションを許可(&amp;A):</translation>
    </message>
    <message>
      <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
      <translation>後ろに続く単語をお互いにハイフンで結ぶ最小の長さです。
値が0であれば、ハイフンの数に制限がないことを意味します。</translation>
    </message>
    <message>
      <source>Exceptions</source>
      <translation>例外</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>編集</translation>
    </message>
    <message>
      <source>Ignore List</source>
      <translation>無視リスト</translation>
    </message>
  </context>
  <context>
    <name>nftdialog</name>
    <message>
      <source>New From Template</source>
      <translation>テンプレートから新規作成</translation>
    </message>
    <message>
      <source>&amp;About</source>
      <translation>バージョン情報(&amp;A)</translation>
    </message>
    <message>
      <source>&amp;Image</source>
      <translation>画像(&amp;I)</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation>ヘルプ(&amp;H)</translation>
    </message>
  </context>
  <context>
    <name>replaceColorDialog</name>
    <message>
      <source>Replace Color</source>
      <translation>色を置換</translation>
    </message>
    <message>
      <source>Replace:</source>
      <translation>置換:</translation>
    </message>
    <message>
      <source>with:</source>
      <translation>以下で:</translation>
    </message>
  </context>
  <context>
    <name>replaceColorsDialog</name>
    <message>
      <source>Replace Colors</source>
      <translation>色を置換</translation>
    </message>
    <message>
      <source>Add ...</source>
      <translation>追加...</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>Edit...</source>
      <translation>編集...</translation>
    </message>
  </context>
  <context>
    <name>satdialog</name>
    <message>
      <source>Save as Template</source>
      <translation>テンプレートとして保存</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>名前</translation>
    </message>
    <message>
      <source>Category</source>
      <translation>カテゴリ</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>ページサイズ</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>色</translation>
    </message>
    <message>
      <source>Description</source>
      <translation>詳細</translation>
    </message>
    <message>
      <source>Usage</source>
      <translation>使い方</translation>
    </message>
    <message>
      <source>Author</source>
      <translation>作者</translation>
    </message>
    <message>
      <source>Email</source>
      <translation>Eメール</translation>
    </message>
    <message>
      <source>&amp;More Details</source>
      <translation>細かい詳細(&amp;M)</translation>
    </message>
  </context>
  <context>
    <name>selectDialog</name>
    <message>
      <source>Select Objects</source>
      <translation>オブジェクトを選択</translation>
    </message>
    <message>
      <source>on current Page</source>
      <translation>現在のページで</translation>
    </message>
    <message>
      <source>on current Layer</source>
      <translation>現在のレイヤーで</translation>
    </message>
    <message>
      <source>on the Scratch Space</source>
      <translation>スクラッチスペース上</translation>
    </message>
    <message>
      <source>Select by</source>
      <translation>以下の条件で選択</translation>
    </message>
    <message>
      <source>Object Type</source>
      <translation>オブジェクトタイプ</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation>テキストフレーム</translation>
    </message>
    <message>
      <source>Image Frame</source>
      <translation>画像フレーム</translation>
    </message>
    <message>
      <source>Shape</source>
      <translation>形状</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>ポリライン</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>線</translation>
    </message>
    <message>
      <source>Render Frame</source>
      <translation>フレームを描画</translation>
    </message>
    <message>
      <source>Fill Color</source>
      <translation>塗りつぶし色</translation>
    </message>
    <message>
      <source>Line Color</source>
      <translation>線の色</translation>
    </message>
    <message>
      <source>Line Width</source>
      <translation>線幅</translation>
    </message>
    <message>
      <source>Printable</source>
      <translation>印刷可能</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>はい</translation>
    </message>
    <message>
      <source>No</source>
      <translation>いいえ</translation>
    </message>
    <message>
      <source>Locked</source>
      <translation>ロック済み</translation>
    </message>
    <message>
      <source>Resizeable</source>
      <translation>リサイズ可能</translation>
    </message>
  </context>
  <context>
    <name>tfDia</name>
    <message>
      <source>Create filter</source>
      <translation>フィルタを作成</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>クリア(&amp;L)</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>削除(&amp;D)</translation>
    </message>
    <message>
      <source>Choose a previously saved filter</source>
      <translation>前に保存したフィルタを選択</translation>
    </message>
    <message>
      <source>Give a name to this filter for saving</source>
      <translation>このフィルタに保存する名前をつける</translation>
    </message>
    <message>
      <source>Give a name for saving</source>
      <translation>保存する名前を指定</translation>
    </message>
  </context>
  <context>
    <name>tfFilter</name>
    <message>
      <source>Disable or enable this filter row</source>
      <translation>このフィルタ行を無効/有効に</translation>
    </message>
    <message>
      <source>Remove this filter row</source>
      <translation>このフィルタ行を削除</translation>
    </message>
    <message>
      <source>Add a new filter row</source>
      <translation>新規フィルタ行を追加</translation>
    </message>
    <message>
      <source>to</source>
      <translation>から</translation>
    </message>
    <message>
      <source>and</source>
      <translation>と</translation>
    </message>
    <message>
      <source>remove match</source>
      <translation>マッチしたものを削除する</translation>
    </message>
    <message>
      <source>do not remove match</source>
      <translation>マッチしたものを削除しない</translation>
    </message>
    <message>
      <source>words</source>
      <translation>単語</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>削除</translation>
    </message>
    <message>
      <source>Replace</source>
      <translation>置換</translation>
    </message>
    <message>
      <source>Apply</source>
      <translation>適用</translation>
    </message>
    <message>
      <source>Value at the left is a regular expression</source>
      <translation>左の値は正規表現です</translation>
    </message>
    <message>
      <source>with</source>
      <translation>で</translation>
    </message>
    <message>
      <source>paragraph style</source>
      <translation>段落スタイル</translation>
    </message>
    <message>
      <source>all instances of</source>
      <translation>全てのインスタンス</translation>
    </message>
    <message>
      <source>all paragraphs</source>
      <translation>全ての段落</translation>
    </message>
    <message>
      <source>paragraphs starting with</source>
      <translation>以下で始まる段落</translation>
    </message>
    <message>
      <source>paragraphs with less than</source>
      <translation>未満の段落</translation>
    </message>
    <message>
      <source>paragraphs with more than</source>
      <translation>以上の段落</translation>
    </message>
  </context>
</TS>
