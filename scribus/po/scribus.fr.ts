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
                        unit, pagesType, firstPageOrder, numPages) -&gt; bool

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
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation type="unfinished"></translation>
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
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
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
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
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

Returns the page margins as a (top, left, right, bottom) tuple in the current
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
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFillTransparency([&quot;name&quot;]) -&gt; float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFillBlendmode([&quot;name&quot;]) -&gt; integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineTransparency([&quot;name&quot;]) -&gt; float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineBlendmode([&quot;name&quot;]) -&gt; integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerOutlined&quot;layer&quot;, outline)

Sets the layer &quot;layer&quot; to be locked or not. If outline is set to
true the layer will be displayed outlined.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerFlow&quot;layer&quot;, flow)

Sets the layers &quot;layer&quot;  flowcontrol to flow. If flow is set to
true text in layers above this one will flow around objects on this layer.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerBlendmode&quot;layer&quot;, blend)

Sets the layers &quot;layer&quot;  blendmode to blend.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerTransparency&quot;layer&quot;, trans)

Sets the layers &quot;layer&quot;  transparency to trans.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerLocked(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerOutlined(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerFlow(&quot;layer&quot;) -&gt; bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLayerBlendmode(&quot;layer&quot;) -&gt; int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLayerTransparency(&quot;layer&quot;) -&gt; float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setFillBlendmode(blendmode, [&quot;name&quot;])

Sets the fill blendmode of the object &quot;name&quot; to blendmode
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineBlendmode(blendmode, [&quot;name&quot;])

Sets the line blendmode of the object &quot;name&quot; to blendmode
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPageType() -&gt; integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getTextLines([&quot;name&quot;]) -&gt; integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
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
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the
printable set to false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerVisible(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting text at position -1 appends it to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus %1</source>
        <translation>À propos de Scribus %1</translation>
    </message>
    <message>
        <source>March</source>
        <translation type="obsolete">Mars</translation>
    </message>
    <message>
        <source>%1 %2 %3</source>
        <translation>%1 %2 %3</translation>
    </message>
    <message>
        <source>%3-%2-%1 %4 %5</source>
        <translation>%3-%2-%1·%4·%5</translation>
    </message>
    <message>
        <source>Using Ghostscript version %1</source>
        <translation>Utilisation de GhostScript version %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Aucune version de GhostScript disponible</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Identifiant de compilation :</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>À &amp;propos</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Équipe de programmation :</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Contributions :</translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Portage vers Mac OS&amp;#174; X Aqua :</translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation>Portage vers Windows&amp;#174; :</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Documentation officielle :</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Autre documentation :</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;uteurs</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Traducteur(rice)s officiel(le)s :</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Ancien(ne)s traducteur(rice)s :</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Traductions</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Site officiel</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Documentation en ligne</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Rapports de bogues et demandes d&apos;améliorations</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Liste de diffusion</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;En ligne</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">Ce panneau montre la version, la date de compilation et les bibliothèques compilées dans Scribus. Le symbole C-C-T-F indique le support de C=littlecms C=CUPS T=TIFF F=Fontconfig. La dernière indique le moteurde rendu, C=cairo ou A=libart. Les composantes manquantes sont indiquées par une astérisque (*). La version de Ghostscript détectée par Scribus est également mentionnée.</translation>
    </message>
    <message>
        <source>Tango Project Icons:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Updates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for &amp;Updates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected. The Windows version does not use fontconfig or CUPS libraries.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for updates to Scribus. No data from your machine will be transferred off it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Filename:</source>
        <translation>Nom du fichier :</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Oui</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Non</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Activé :</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Date de publication :</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Auteur(s) :</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright :</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Licence :</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus : À propos des extensions</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ouvrir...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Enregistrer &amp;sous...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Re&amp;venir à la version précédente</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>R&amp;assembler les éléments pour la sortie...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Importer un texte...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Ajouter un &amp;texte...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Importer une image...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Enregistrer le &amp;texte...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Enregistrer en &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Enregistrer en P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Ré&amp;glage du document...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Im&amp;primer...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Aperç&amp;u avant impression</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Quitter</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>Mode act&amp;ion</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cou&amp;per</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Co&amp;ller</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;Tout sélectionner</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>Tout &amp;désélectionner</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Rechercher-&amp;Substituer...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Editer l&apos;image...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>C&amp;ouleurs...</translation>
    </message>
    <message>
        <source>S&amp;tyles...</source>
        <translation>S&amp;tyles...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Styles de p&amp;aragraphes...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Styles de &amp;filets...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Gabarits...</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;Javascripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>Préféren&amp;ces...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Autre...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Gauche</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Centré</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Droite</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Justifié</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Forcé</translation>
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
        <translation>&amp;Souligné</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Mots souli&amp;gnés</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Barré</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;Majuscules</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>Petites &amp;capitales</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>Ex&amp;posant</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>In&amp;dice</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>Co&amp;ntour</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Ombre</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Effets</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulations...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Dupliquer</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Dupliquer-Déplacer &amp;multiples</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Grouper</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Dégroup&amp;er</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>&amp;Verrouillé</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>T&amp;aille verrouillée</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>L&apos;im&amp;pression est activée</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>Retourner &amp;horizontalement</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>Retourner &amp;verticalement</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>De haut en &amp;bas</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Mettre au d&amp;essus</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Placer au-dess&amp;ous</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Placer au-dess&amp;us</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Mettre dans l&apos;al&amp;bum</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>A&amp;ttributs...</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Plus d&apos;informations...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>I&amp;mage visible</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>A&amp;ctualiser l&apos;image</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Adapter le cadre à l&apos;image</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Propriétés étendues de l&apos;image</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Basse résolution</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>Résolution &amp;normale</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Haute résolution :</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>&amp;Signet PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>A&amp;nnotation PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Propriétés de l&apos;&amp;annotation</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Propriétés du &amp;champ</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Modifier la forme...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Joindre le texte au tracé</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Détac&amp;her le texte du tracé</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Associer les pol&amp;ygones</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Dissocier les &amp;polygones</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Courbe de &amp;Bézier</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>Cadre d&apos;&amp;image</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Contours</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Polygone</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>Cadre de &amp;texte</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Caractère...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Insérer un faux texte</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Insérer...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>I&amp;mporter...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Effacer...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Copier...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Déplacer...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>Appli&amp;quer un gabarit...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Convertir en gabarit...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Gestion des repères...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Propriétés de la page...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation type="obsolete">Taille de la &amp;fenêtre</translation>
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
        <translation>1&amp;00%</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation>&amp;200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>Minia&amp;ture(s)</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Afficher les &amp;marges</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Afficher les &amp;cadres</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Afficher les &amp;images</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Afficher la &amp;grille</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Afficher les &amp;repères</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Afficher la grille t&amp;ypographique</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Afficher les &amp;liens entre cadres</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Afficher les caractères invisibles</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Afficher la règle</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Guides relatifs à la page</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Grille m&amp;agnétique</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Re&amp;pères magnétiques</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Propriétés</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>Pla&amp;n du document</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Album</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>Ca&amp;lques</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Organiser les pages</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Signets</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>M&amp;esurer</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Historique des actions</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Vérificateur</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>Espacer et &amp;aligner</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Ou&amp;tils</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Outils P&amp;DF</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Sélectionner un objet</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;ableau</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>Forme (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>Fi&amp;let</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Ligne à main levée (&amp;F)</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotation de l&apos;objet</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Agrandissement ou réduction</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zoom avant</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zoom arrière</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Éditer le contenu du cadre</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Modifier le texte...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Lier les cadres de texte</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Délier les cadres de texte</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>Pip&amp;ette à couleurs</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Copier les propriétés de l&apos;objet</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Modifier le texte avec l&apos;éditeur interne</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Insérer un cadre de texte</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Insérer un cadre d&apos;image</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Insérer un tableau</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Insérer une forme</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Insérer un polygone</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Insérer un filet</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Insérer une courbe de Bézier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Dessiner une ligne à main levée</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Gestion des images</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>Effectuer les &amp;césures</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Défaire les césures</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>Générer la &amp;table des matières</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Cascade</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Mosaïque</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>À &amp;propos de Scribus</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;A propos des extensions</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>À propos de &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Info&amp;bulles</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;Manuel de Scribus...</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Afficher les palettes</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Afficher les repères</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>Trait d&apos;&amp;union conditionnel</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Trait d&apos;union insécable</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Espace in&amp;sécable</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>&amp;Numéro de page</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Saut de ligne</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Saut de cadre</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Saut de colonne</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Symbole Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Marque déposée enregistrée</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Marque déposée</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Barre oblique</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Puce</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Point médian</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Tiret cadratin</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Tiret demi-cadratin</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Tiret numérique</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Barre horizontale</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Guillemet APL</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Guillemet dactylographique</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Apostrophe culbutée</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Apostrophe</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Guillemet anglais ouvrant</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Guillemet anglais fermant</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Apostrophe culbutée</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Guillemet-virgule double</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Guillemet simple vers la gauche</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Guillemet simple vers la droite</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Guillemet français ouvrant</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Guillemet français fermant</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Guillemet-virgule</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Guillemet-virgule double</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>Apostrophe gauche CJC</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>Apostrophe droit CJC</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>Guillemet gauche CJC</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>Guillemet droit CJC</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>Demi-cadratin</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Cadratin</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Espace fine</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Espace forte</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Espace moyenne</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Espace ultrafine</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Insérer un trait d&apos;union conditionnel</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Insérer un trait d&apos;union insécable</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Insérer une espace insécable</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Insérer le folio</translation>
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
        <source>Paste (&amp;Absolute)</source>
        <comment>Fred</comment>
        <translation type="obsolete">Coller (&amp;absolu)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <comment>Fred</comment>
        <translation type="obsolete">&amp;Effacer</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation type="unfinished">Coller (&amp;absolu)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Effacer</translation>
    </message>
    <message>
        <source>Insert PDF Push Button</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Text Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Check Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Combo Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF List Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Text Annotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Link Annotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as &amp;EPS...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Frame...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Patterns...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to Patterns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sticky Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fit to Height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fit to Width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Bleeds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Zero Width Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zero Width NB Space</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignDistributeBase</name>
    <message>
        <source>Align and Distribute</source>
        <translation type="unfinished">Espacer - Aligner</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Aligner</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished">Repère sélectionné</translation>
    </message>
    <message>
        <source>&amp;Relative To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation type="unfinished">Espacer</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation type="unfinished">&amp;Distance :</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Espacer - Aligner</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Aligner</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>En &amp;rapport avec :</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Premier objet sélectionné</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Dernier objet sélectionné</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Page</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marges</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Repère</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Sélection</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation type="obsolete">Aligner les côtés droit des objets au côté gauche de l&apos;ancre</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation type="obsolete">Aligner les côtés gauches des objets au côté droit de l&apos;ancre</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Aligner le bas des objets</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Aligner les côtés droits</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation type="obsolete">Aligner le haut des objets avec le bas de l&apos;ancre</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Centrer sur l&apos;axe vertical</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Aligner les côtés gauches</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Centrer sur l&apos;axe horizontal</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation type="obsolete">Aligner le bas des objets avec le haut de l&apos;ancre</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Aligner le haut des objets</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>Repère sélectionné</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation type="unfinished">Espacer</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation type="obsolete">Rendre égal l&apos;espacement horizontal entre les objets</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation type="obsolete">Fixer l&apos;espacement horizontal entre les objets à la valeur spécifiée</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Distribuer les côtés droits de manière équidistante</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Distribution régulière en bas</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Distribution régulière des centres</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation type="obsolete">Rendre égaux les espaces entre les objets</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation type="obsolete">Fixer les espacements verticaux entre les objets à la valeur spécifiée</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Distribution régulière des côtés gauches</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Distribution régulière verticale des centres</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Distribution régulière en haut</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Distance :</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Distribution des objets à la distance spécifiée</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Aucune sélection</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="obsolete">Un ou des objets sont verrouillés.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="obsolete">To&amp;ut déverrouiller</translation>
    </message>
    <message>
        <source>Y: %1%2</source>
        <translation>Y : %1 %2</translation>
    </message>
    <message>
        <source>X: %1%2</source>
        <translation>X : %1 %2</translation>
    </message>
    <message>
        <source>Align right sides of items to left side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align left sides of items to right side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align tops of items to bottom of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align bottoms of items to top of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between items equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between items equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between items equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between items equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between items and sides of page equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between items and the top and bottom of page margins equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between items and sides of page margins equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between items and the top and bottom of page equal</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Texte aligné à gauche</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Texte aligné à droite</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Texte centré</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Texte justifié</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Justification forcée</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Propriétés du champ</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Bouton</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Champ de texte</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Case à cocher</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Menu déroulant</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Liste</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation type="obsolete">Infobulle :</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Police à utiliser pour le format PDF 1.3 :</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Bord</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Couleur :</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Largeur :</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Mince</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Large</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Style :</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Plein</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Pointillé</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Biseauté</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Creux</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Autre</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Lecture seule</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Nécessaire</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation type="obsolete">Ne pas exporter la valeur</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Visibilité :</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Visible</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Caché</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Non imprimable</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Invisible</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Aspect</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Texte pour le bouton bas</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Texte pour l&apos;effet de survol (rollover)</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Icônes</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Utiliser les icônes</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Appuyé</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Effet de survol (rollover)</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Position des icônes...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Surligné</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverser</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Pousser</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multi-ligne</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Limite de</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Caractères</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ne pas dérouler</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ne pas vérifier l&apos;orthographe</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Type de case :</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Coche</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Croix</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Rond</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Étoile</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Carré</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Cochée par défaut</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Modifiable</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Aller à</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Envoyer le formulaire</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Formulaire vierge</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importer des données</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Événement :</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Bouton souris relâché</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Bouton souris enfoncé</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Entrée souris</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Sortie souris</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Au centre</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Autour</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script :</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Modifier...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>URL :</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Envoyer les données en HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importer les données depuis :</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destination</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Vers :</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Page :</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X :</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y :</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Action</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Le champ est formaté comme :</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Texte</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Pourcentage</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Date</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Sur mesure</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Format du nombre</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Décimales :</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Unité monétaire</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Symbole monétaire</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Mise en page</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Format du pourcentage</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Format de la date</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Format de l&apos;heure</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Scripts sur mesure</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format :</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Frappe :</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>La valeur n&apos;est pas validée</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>La valeur doit être supérieure ou égale à :</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>et inférieure ou égale à :</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Script de validation sur mesure :</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Valider</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>La valeur n&apos;est pas calculée</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>La valeur est</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>la somme</translation>
    </message>
    <message>
        <source>product</source>
        <translation>le produit</translation>
    </message>
    <message>
        <source>average</source>
        <translation>la moyenne</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>le minimum</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>le maximum</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>des champs suivants :</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Choisir...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Script de calcul sur mesure :</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Calculer</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Le drapeau est ignoré pour le format PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Saisissez ici une liste de champs séparés par des virgules</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Vous avez au moins besoin de l&apos;icône « Normal » pour pouvoir utiliser les icônes comme boutons</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Images (*.tif *.png *.jpg *.xpm);;PostScript encapsulé (*.eps);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Exemple :</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Sélection modifiée</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Fichiers PDF (*.pdf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Tooltip:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do Not Export Value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Propriétés de l&apos;annotation</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lien</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Lien externe</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>URL</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type :</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destination</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Page :</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y :</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Documents PDF (*.pdf);;Tous les fichiers (*)</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Appliquer un gabarit</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Gabarit :</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Appliquer à</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Page &amp;courante</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>Pages &amp;paires</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>Pages &amp;impaires</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Toutes les pages</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>Dans la &amp;plage</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Applique le gabarit sélectionné aux pages paires, impaires ou à toutes les pages de la plage suivante</translation>
    </message>
    <message>
        <source>to</source>
        <translation>à</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
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
        <translation>Aucune</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>C&amp;réateur de codes à barres...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Interface Scribus pour la création de codes à barres en pur PostScript</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 ou 13 chiffres</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 chiffres</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 ou 12 chiffres</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 ou 8 chiffres</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 chiffres</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 chiffres</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation type="obsolete">9 ou 10 chiffres séparés par des tirets</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Nombre variable de caractères, chiffres et des symboles -.·*$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Chaîne composée d&apos;un nombre variable de caractères ASCII et de symboles spéciaux, débutant par le caractère approprié.  Le code UCC/EAN 128 présente en première position après le caractère de début, un caractère non signifiant en terme de données : le caractère fonction 1 (FNC1).</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Nombre variable de chiffres et des symboles -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Nombre variable de chiffres</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Nombre variable de chiffres. Un symbole ITF 14 contient 14 caractères sans somme de contrôle.</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Nombre variables de chiffres et de lettres majuscules</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Nombre variable de caractères héxadécimaux</translation>
    </message>
    <message>
        <source>Error opening file: %1</source>
        <translation>Erreur à l&apos;ouverture du fichier %1</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Codes à barres incomplet</translation>
    </message>
    <message>
        <source>For ISBN-10 the data should contain 9 or 10 <byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>digits separated appropriately by dash characters.
For ISBN-13 the data should contain 12 or 13 <byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>digits separated appropriately by dash characters.
If the last digit is not given then the ISBN <byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>checkdigit is calculated automatically.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>Créateur de codes à barres</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Code à barres</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type :</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>C&amp;ode :</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Choisir un type de code à barres</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Représentation numérique du code lui-même. Voir Aide ci-dessous</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Réglages initiaux des échantillons de codes à barres</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Ajouter un texte au code à barres</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Si cochée, le code à barres contiendra aussi des chiffres</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Conserver le blanc</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Détermine la réserve autour du code</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>I&amp;nclure la somme de contrôle</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Calculer et inclure une somme de contrôle dans le code à barres</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Incl&amp;ure un nombre de contrôle</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Inclure le nombre de contrôle dans le texte du code à barres</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>Couleur de &amp;fond</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Couleur de fond, sous les barres du code</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Barres</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Couleur des barres du code</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Texte</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Couleur du texte et des chiffres</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Explications et assistance</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Prévisualisation du code à 72 ppp</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Sélectionnez un dossier pour l&apos;Album</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Sélectionnez un dossier</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Le nom &quot;%1&quot; existe déjà.
Choisissez-en un autre.</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nouvelle entrée</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Album</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Fichier</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="obsolete">&amp;Prévisualisation</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation type="obsolete">&amp;Charger...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Enregistrer &amp;sous...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Fermer</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create a new scrapbook page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load an existing scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the selected scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close the selected scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copied Items</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Déplacer signet</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Insérer signet</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Signets</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Signets</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Position des icônes</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Disposition :</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Légende seule</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Icône seul</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Légende en dessous de l&apos;icône</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Légende au-dessus de l&apos;icône</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Légende à droite de l&apos;icône</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Légende à gauche de l&apos;icône</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Légende masquant l&apos;icône</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Mise à l&apos;échelle :</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Toujours</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Quand l&apos;icône est trop petit</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Quand l&apos;icône est trop grand</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proportionnel</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Indépendant</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Icône</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Remise à zéro</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Activer la gestion des couleurs</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Profils</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>Images &amp;RVB :</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>Images &amp;CMJN :</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation type="obsolete">Couleur&amp;s d&apos;accompagnement :</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>Écra&amp;n :</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Imp&amp;rimante :</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Rendus</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptible</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrie relative</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturation</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrie absolue</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation type="obsolete">Écr&amp;an :</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation type="obsolete">Impr&amp;imante :</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulation du rendu de l&apos;imprimante à l&apos;écran</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Identifier les couleurs hors &amp;gamme</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Équili&amp;brer le point noir</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Profil de couleur par défaut pour les images CMJN importées</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Profil de couleur par défaut pour les images RVB importées</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation type="obsolete">Profil de couleur par défaut pour les couleurs en page</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Profil de couleur que vous avez créé ou reçu du constructeur.
Ce profil doit être propre à votre écran et non pas générique (i.e sRVB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Profil de couleur du constructeur pour votre imprimante.
Ce profil doit être propre à votre imprimante et non pas générique (i.e. sRVB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="obsolete">Méthode de rendu par défaut pour votre écran. À moins que vous ne sachiez
pourquoi en changer, choisissez Colorimétrie relative ou Perceptible.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="obsolete">Méthode de rendu par défaut pour votre imprimante. À moins que vous ne sachiez
pourquoi en changer, choisissez Colorimétrie relative ou Perceptible.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Active « l&apos;épreuvage à l&apos;écran », basé sur le profil imprimante sélectionné.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Méthode permettant de voir à l&apos;écran les couleurs qui pourraient ne pas s&apos;imprimer
correctement. Elle nécessite des profils précis et ne sert que d&apos;avertissement.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Sert à améliorer le contraste des photos. Il est recommandé de l&apos;utiliser si
vous avez des photos dans votre composition.</translation>
    </message>
    <message>
        <source>&amp;RGB Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;CMYK Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sol&amp;id Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert all colors to printer space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for solid RGB colors on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for solid CMYK colors on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Édition des couleurs</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Modèle colorimétrique</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMJN</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>RVB</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation>RVB pour le Web</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Couleur d&apos;accompagnement</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Couleur quadrichromique</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nouvelle</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Ancienne</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Modèle de couleur TSL</translation>
    </message>
    <message>
        <source>C:</source>
        <translation>C :</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>M:</source>
        <translation>M :</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>J :</translation>
    </message>
    <message>
        <source>K:</source>
        <translation>N :</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Barres de couleur dynamique</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Barres de couleur statique</translation>
    </message>
    <message>
        <source>R:</source>
        <translation>R :</translation>
    </message>
    <message>
        <source>G:</source>
        <translation>V :</translation>
    </message>
    <message>
        <source>B:</source>
        <translation>B :</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Vous ne pouvez pas créer une couleur nommée &quot;%1&quot;.
Ce nom est réservé aux couleurs transparentes</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation type="obsolete">Ce nom de couleur existe déjà</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The name of the color already exists,
please choose another one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CStylePBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CWDialog</name>
    <message>
        <source>Normal Vision</source>
        <translation type="obsolete">Vision normale</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Cécité au rouge</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Cécité au vert</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Cécité au bleu</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Daltonisme</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="unfinished">Fusion des couleurs</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="unfinished">Erreur :</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="unfinished">La couleur %1 existe déjà !</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="unfinished">Couleur %1 ajoutée.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="unfinished">Ouverture du gestionnaire de couleurs</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="unfinished">Fusion de couleurs</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished">Impossible de trouver la couleur demandée. Vous avez certainement choisi noir, gris ou blanc. Il est impossible de traiter cette couleur.</translation>
    </message>
</context>
<context>
    <name>CWDialogBase</name>
    <message>
        <source>Color Wheel</source>
        <translation type="unfinished">Cercle chromatique</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMJN</translation>
    </message>
    <message>
        <source>C:</source>
        <translation type="unfinished">C :</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="unfinished">M :</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">J :</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="unfinished">N :</translation>
    </message>
    <message>
        <source>RGB:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RVB</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="unfinished">R :</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="unfinished">V :</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="unfinished">B :</translation>
    </message>
    <message>
        <source>CMYK:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Document</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="unfinished">Choisir une méthode de création pour le schéma de couleur. Voir la documentation pour plus de détails</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Angle :</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished">Différence entre la valeur sélectionnée et celle comptée. Voir la documentation pour plus d&apos;informations</translation>
    </message>
    <message>
        <source>Preview:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vision Defect Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample color scheme.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Merge</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation type="unfinished">Fusionner les couleurs créées avec celles du document</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">Substitue&amp;r</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="unfinished">Remplacer les couleurs créées dans le document</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Annuler</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="unfinished">Laisser les couleurs inchangées</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished">Simulez des anomalies de la vision courantes. Choississez parmi les anomalies suivantes.</translation>
    </message>
    <message>
        <source>Color Scheme Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>HSV:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="unfinished">Modèle de couleurs HSV</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="unfinished">Teinte</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="unfinished">Saturation</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="unfinished">Luminosité</translation>
    </message>
    <message>
        <source>Result Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="obsolete">Vous pouvez voir une vignette si vous cliquez et maintenez enfoncé le bouton droit de la souris. La touche Inser insère un caractère dans la sélection ci-dessous et la touche Suppr supprime le dernier caractère inséré</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Choisissez le(s) caractère(s) :</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Classe de caractères :</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation type="obsolete">&amp;Insérer le code</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insérer</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Fermer</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Insérer le(s) caractère(s) à la position du curseur dans le texte</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Effacer la(les) sélection(s) courante(s).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation type="obsolete">Fermer cette fenêtre et retourner à l&apos;édition de texte.</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="obsolete">Inscrire ici un code à 4 chiffres Unicode</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Jeu de caractères complet</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Latin de base</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 supplémentaire</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latin étendu-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latin étendu-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Ponctuation générale</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Exposants et indices</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Symboles des monnaies</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Symboles alphabétiques</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Formes numériques</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Flèches</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Opérateurs mathématiques</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Tracé de rectangles</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Éléments de blocs</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Formes géométriques</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Symboles divers</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbats</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Petites variations de formes</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligatures</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Caractères spéciaux</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grec</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Grec étendu</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Cyrillique</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Cyrillique supplémentaire</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabe</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabe étendu A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabe étendu B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hébreu</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished">Vous pouvez voir une vignette si vous cliquez et maintenez enfoncé le bouton droit de la souris. La touche Inser insère un caractère dans la sélection ci-dessous et la touche Suppr supprime le dernier caractère inséré</translation>
    </message>
    <message>
        <source>Scribus Char Palette (*.ucp);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enhanced Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quick Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Enhanced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a filename to open</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error</source>
        <translation type="unfinished">Erreur</translation>
    </message>
    <message>
        <source>Error reading file %1 - file is corrupted propably.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a filename to save under</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clean the Palette?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You will clean all characters from this palette. Are you sure?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Character Palette</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharTable</name>
    <message>
        <source>Delete</source>
        <translation type="unfinished">Supprimer</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Caractères manquants</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Débordement de texte</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>L&apos;objet n&apos;est pas sur une page</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Image manquante</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation type="obsolete">L&apos;image a une résolution inférieure à %1 PPP</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>L&apos;objet possède une transparence</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>L&apos;objet est une annotation ou un champ PDF</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>L&apos;objet est un PDF placé</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Document</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Aucun problème trouvé</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Page </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Libérer les objets</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Problèmes trouvés </translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Vérificateur</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Objets</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problèmes</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Profil actuel :</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorer les erreurs</translation>
    </message>
    <message>
        <source>Check again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image is GIF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Transparency used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blendmode used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layer &quot;%1&quot;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Choisir les styles</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Styles disponibles</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Sélectionnez un dossier</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Rassemblement...</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Impossible de rassembler les éléments du fichier : 
%1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Attention</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Impossible de rassembler tous les éléments
pour la sortie du fichier %1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Dupliquer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Efface&amp;r les couleurs inutilisées</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Jeux de couleurs</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Jeu de couleurs actuel :</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Enregistrer le jeu de couleurs</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Choisissez un jeu de couleurs à charger</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Enregistrer le jeu de couleurs courant</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Effacer les couleurs inutilisées dans le document courant</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Ajouter au jeu actuel les couleurs d&apos;un autre document</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Créer une nouvelle couleur</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Modifier la couleur sélectionnée</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Faire un double de la couleur sélectionnée</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Supprimer la couleur sélectionnée</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Faire du jeu de couleurs courant celui par défaut</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Choisissez un nom</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Documents (*.sla *.sla.gz *.scd *.scd.gz);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Documents (*.sla *.scd);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copie de %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nouvelle couleur</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;Other Files (*.eps *.epsi *.ps *.ai);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Other Files (*.eps *.epsi *.ps *.ai);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Information</translation>
    </message>
    <message>
        <source>The file %1 does not contain any new colors.
If the file was an EPS try to import it with File -&gt; Import</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Monochromatique</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Analogue</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Complémentaire</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Complémentaire divisée</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triadique</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tétradique (complémentaire double)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Couleur de base</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Monochromatique clair</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Monochromatique sombre</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>1re analogue</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>2nd analogue</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>1re divisée</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>2e divisée</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>3e divisée</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>4e divisée</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>1re triadique</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>2e triadique</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>1re tétradique (diamétralement opposée)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>2e tétradique (angle)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>3e tétradique (angle opposé)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation type="obsolete">Cré&amp;er une couleur...</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation type="obsolete">C&amp;omposantes de couleurs...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation type="obsolete">&amp;Importer une couleur existante...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation type="obsolete">&amp;Fusion des couleurs</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation type="obsolete">Substitue&amp;r les couleurs</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="obsolete">&amp;Quitter</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">C&amp;ouleur</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation type="obsolete">Vision normale</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Cécité au rouge</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Cécité au vert</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Cécité au bleu</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Daltonisme</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation type="obsolete">Anomalie de la vision</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation type="obsolete">Cercle chromatique</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Couleur</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Nom</translation>
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
        <translation type="obsolete">J</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">N</translation>
    </message>
    <message>
        <source>Select Method:</source>
        <translation type="obsolete">Méthode de sélection :</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation type="obsolete">Angle·(0·-·90·degrés) :</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation type="obsolete">&amp;Fusionner les couleurs</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation type="obsolete">Substitue&amp;r les couleurs</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation type="obsolete">Fusionner les couleurs créées avec celles du document</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="obsolete">Remplacer les couleurs créées dans le document</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="obsolete">Laisser les couleurs inchangées</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="obsolete">Différence entre la valeur sélectionnée et celle comptée. Voir la documentation pour plus d&apos;informations</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="obsolete">Choisissez la couleur de base en cliquant sur le cercle chromatique. En mode HSV, vous choisissez la teinte.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation type="obsolete">Exemple de schéma de couleur</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="obsolete">Choisir une méthode de création pour le schéma de couleur. Voir la documentation pour plus de détails</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation type="obsolete">Couleurs du schéma de couleurs choisi</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="obsolete">Simulez des anomalies de la vision courantes. Choississez parmi les anomalies suivantes.</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="obsolete">Fusion des couleurs</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="obsolete">Erreur :</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="obsolete">La couleur %1 existe déjà !</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="obsolete">Couleur %1 ajoutée.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="obsolete">Ouverture du gestionnaire de couleurs</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="obsolete">Fusion de couleurs</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Nouvelle couleur</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="obsolete">Impossible de trouver la couleur demandée. Vous avez certainement choisi noir, gris ou blanc. Il est impossible de traiter cette couleur.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>Cercle chromatique</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Assistant au paramétrage des couleurs</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Nuancier de couleur. Théorie des couleurs inclus.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>Appli&amp;quer</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Attention</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Sur mesure</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Page simple</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Pages en regard</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3 volets</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4 volets</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Page gauche</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="obsolete">Centre</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="obsolete">Milieu gauche</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="obsolete">Milieu droit</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Page droite</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Friday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>January</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>February</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>March</source>
        <translation type="unfinished">Mars</translation>
    </message>
    <message>
        <source>April</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>May</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>June</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>July</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>August</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>September</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>October</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>November</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>December</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Oui</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Non</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Oui</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Non</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Left Page</source>
        <comment>Left page location</comment>
        <translation type="unfinished">Page gauche</translation>
    </message>
    <message>
        <source>Middle</source>
        <comment>Middle page location</comment>
        <translation type="unfinished">Centre</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <comment>Middle Left page location</comment>
        <translation type="unfinished">Milieu gauche</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <comment>Middle Right page location</comment>
        <translation type="unfinished">Milieu droit</translation>
    </message>
    <message>
        <source>Right Page</source>
        <comment>Right page location</comment>
        <translation type="unfinished">Page droite</translation>
    </message>
    <message>
        <source>Normal</source>
        <comment>Default single master page</comment>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <comment>Default left master page</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <comment>Default middle master page</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Right</source>
        <comment>Default right master page</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <comment>Color Blindness - Normal Vision</comment>
        <translation type="unfinished">Vision normale</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <comment>Color Blindness - Red Color Blind</comment>
        <translation type="unfinished">Cécité au rouge</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <comment>Color Blindness - Greed Color Blind</comment>
        <translation type="unfinished">Cécité au vert</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <comment>Color Blindness - Blue Color Blind</comment>
        <translation type="unfinished">Cécité au bleu</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <comment>Color Blindness - Full Color Blindness</comment>
        <translation type="unfinished">Daltonisme</translation>
    </message>
    <message>
        <source>Custom: </source>
        <comment>Custom Tab Fill Option</comment>
        <translation type="unfinished">Sur mesure :</translation>
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
        <translation>Teinte :</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacité :</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation>X1 :</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation>Y1 :</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation>X2 :</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation>Y2 :</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Déplacer le vecteur</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Dégradé horizontal</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Dégradé vertical</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Dégradé montant</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Dégradé descendant</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Dégradé circulaire</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Dégradé linéaire libre</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Dégradé circulaire libre</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Modifier la couleur des filets</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Modifier la couleur de fond</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturation de la couleur</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Méthode de remplissage par dégradé ou non</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Définir la transparence de la couleur sélectionnée</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Déplacer le point de départ du gradient avec le bouton gauche de la souris enfoncé, et le point d&apos;arrivée avec le bouton droit de la souris enfoncé</translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Mode de fusion :</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">Assombrir</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Eclaircir</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Multiplier</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Écran</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Superposer</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Lumière dure</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Lumière douce</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Différence</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Exclusion</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Fusion de couleurs</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Multiplier</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Teinte</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Saturation</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Couleur</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Luminosité</translation>
    </message>
    <message>
        <source>Offsets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">J :</translation>
    </message>
    <message>
        <source>Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pattern</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Exclusion</translation>
    </message>
    <message>
        <source>X-Scale:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y-Scale:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CreateRangeBase</name>
    <message>
        <source>Create Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of Pages in Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Doc Page Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basic Range Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a Range of Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Consecutive Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>From:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="unfinished">à :</translation>
    </message>
    <message>
        <source>Comma Separated List</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Even Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Odd Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add To Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>Range of Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move &amp;Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <source>Move &amp;Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="unfinished">Su&amp;pprimer</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <source>Advanced Reordering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Group Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Page Order:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Order</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Annuler</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Options d&apos;importations des fichiers CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Délimiteur de champ :</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>Tabulation</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Délimiteur de valeur :</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>La première ligne est une en-tête</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Options d&apos;impression</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Format de la page</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Toutes les pages</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Pages paires</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Pages impaires</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Miroir</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Non</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Oui</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientation</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Page(s) par feuille</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Page par feuille</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Pages par feuille</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Option</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Ce panneau montre les options d&apos;impression CUPS. Les paramètres disponibles dépendent de votre pilote d&apos;imprimante. Vous pouvez vérifier le support de CUPS en sélectionnant Aide &gt; À propos de Scribus.Dans l&apos;identifiant de compilation, le symbole C-C-T indique le support de C=CUPS C=littlecms T=TIFF. Les bibliothèques manquantes sont indiquées par une *</translation>
    </message>
</context>
<context>
    <name>CurveWidget</name>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ouvrir</translation>
    </message>
    <message>
        <source>Curve Files (*.scu);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Enregistrer sous</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished">Impossible d&apos;enregistrer le fichier : 
%1</translation>
    </message>
    <message>
        <source>Inverts the curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resets the curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Switches between linear and cubic interpolation of the curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loads a curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saves this curve</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Comprimer le fichier</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Inclure les polices</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codage :</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Revenir dans le dossier des documents. Celui-ci
peut être défini à partir du menu Préférences.</translation>
    </message>
    <message>
        <source>&amp;Include ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation type="obsolete">Définir les composantes de couleur</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="obsolete">CMJN</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="obsolete">RVB</translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="obsolete">Modèle de couleurs HSV</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="obsolete">Teinte</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="obsolete">Saturation</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="obsolete">Luminosité</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="obsolete">R :</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="obsolete">V :</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="obsolete">B :</translation>
    </message>
    <message>
        <source>C:</source>
        <translation type="obsolete">C :</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="obsolete">M :</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="obsolete">J :</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="obsolete">N :</translation>
    </message>
    <message>
        <source>Set &amp;RGB</source>
        <translation type="obsolete">Définir en &amp;RVB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation type="obsolete">Définir en C&amp;MJN</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation type="obsolete">Définir en &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Annulé par l&apos;utilisateur</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Supprimer la couleur</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Supprimer la couleur :</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Substituer par :</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Supprimer une(des) page(s)</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Supprimer de :</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>à :</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Détruire le style</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Détruire le style :</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Substituer par :</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Aucun style</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Echec de l&apos;importation</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Impossible d&apos;importer le document Word 
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Informations sur le document</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titre :</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>A&amp;uteur :</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Mots clés :</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Descri&amp;ption :</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>Édite&amp;ur :</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Contributeur(s) :</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Dat&amp;e :</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ype :</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>For&amp;mat :</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;fiant :</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Source :</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Langue :</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>En &amp;rapport :</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Domaine traité :</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Droi&amp;ts :</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Documen&amp;t</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Autres &amp;informations</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Personne ou organisation responsable du contenu du document.
Ce champ peut être inclus dans le document Scribus pour référence, en plus des métadonnées d&apos;un fichier PDF</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Nom donné au document. Ce champ peut être inclus dans le document Scribus pour référence, en plus des métadonnées d&apos;un fichier PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Ce champ est utilisé pour faire une brève description ou un résumé du document. Il est inclus dans le fichier PDF lors d&apos;une exportation</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Ce champ est utilisé pour définir des mots clés à inclure dans un fichier PDF, ce qui facilite les recherches et l&apos;indexation des fichiers de ce type</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Personne ou organisation responsable de la diffusion du document</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Personne(s) ou organisation(s) contribuant au contenu du document</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Date associée à des modifications du document, au format AAAA-MM-JJ, suivant la norme ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Nature du contenu du document</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Informations pratiques sur le document. Voir les RFC 2045 et 2046 pour les types MIME</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Référence précise du document, ainsi que son contexte. Par exemple ISBN ou URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Référence à un document à l&apos;origine de la création de la composition</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Langue utilisée dans le document, idéalement au format ISO-639, et éventuellement suivi du code pays au format ISO-3166 (par exemple fr-CH)</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Document(s) en rapport avec la composition</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Limite(s) d&apos;application du contenu du document, tels un espace géographique, une durée ou une juridiction</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informations sur les droits du document tels un copyright, un brevet ou une marque déposée</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Ajoute une section à pages numérotées dans le document. La nouvelle section sera ajoutée après la section actuellement sélectionnée.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Supprimer la section sélectionnée</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation>&lt;b&gt;Nom :&lt;/b&gt; nom optionnel pour la section. Par ex. Index&lt;br/&gt;&lt;b&gt;Visible :&lt;/b&gt; les numéros des pages seront affichés pour cette section s&apos;il y a au moins un cadre de texte configuré pour ce faire.&lt;br/&gt;&lt;b&gt;De :&lt;/b&gt; premier numéro de page de cette section.&lt;br/&gt;&lt;b&gt;A :&lt;/b&gt; dernier numéro de page de cette section.&lt;br/&gt;&lt;b&gt;Style :&lt;/b&gt; style de numéro de page à utiliser.&lt;br/&gt;&lt;b&gt;Départ :&lt;/b&gt; index de début de numérotation. Par ex, si Départ=2 etStyle=a,b,c,..., les numéros de page débuteront à b. Pour la première section du document, ceci remplace l&apos;ancien Premier numéro de page du dialogue Nouveau document.</translation>
    </message>
    <message>
        <source>1, 2, 3, ...</source>
        <translation>1,·2,·3,·...</translation>
    </message>
    <message>
        <source>i, ii, iii, ...</source>
        <translation>i,·ii,·iii,·...</translation>
    </message>
    <message>
        <source>I, II, III, ...</source>
        <translation>I,·II,·III,·...</translation>
    </message>
    <message>
        <source>a, b, c, ...</source>
        <translation>a,·b,·c,·...</translation>
    </message>
    <message>
        <source>A, B, C, ...</source>
        <translation>A,·B,·C,·...</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Numéro de page hors plage</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>La valeur que vous avez entrée est plus grande que le nombre de pages du présent document (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Sections du document</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Montré</translation>
    </message>
    <message>
        <source>From</source>
        <translation>De</translation>
    </message>
    <message>
        <source>To</source>
        <translation>à</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Style</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Départ</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Relatif à</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Est le parent de</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Est le fils de </translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Cadres de texte</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Cadres d&apos;image</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Booléen</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Entier</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Nombre réel</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Chaîne de caractères</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Réglage du document</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Paramètre</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>En rapport</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>En rapport à</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Ajouter automatiquement à</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
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
        <translation>Configuration de l&apos;impression</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Destination de l&apos;impression</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Options...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fichier :</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Imprimer via une &amp;commande externe</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Co&amp;mmande :</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Sélection</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Imprimer &amp;toutes les pages</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Imprimer la pa&amp;ge courante</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Imprim&amp;er la sélection de pages</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Nom&amp;bre d&apos;exemplaires :</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Impression normale</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Séparation</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Imprimer en couleur si possible</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Imprimer en niveaux de gris</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tout</translation>
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
        <translation>Jaune</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Noir</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript niveau 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript niveau 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript niveau 3</translation>
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
        <translation>Miroir horizontal des pages</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Miroir vertical des pages</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Définir le format de média</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>S&apos;arrêter aux marges</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Retrait des sous-couleurs (UCR)</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Convertir les couleurs d&apos;accompagnement en couleurs quadrichromiques</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Appliquer un(des) profil(s) ICC</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Autres options</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Aperçu...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>Im&amp;primer</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Ne pas afficher les objets en-dehors des marges lors de l&apos;impression</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Insérer une liste d&apos;éléments séparés par des virgules,
les éléments pouvant être « * » pour toutes les pages, 1-5 pour
une sélection de pages, ou bien un simple numéro de page.</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Utiliser un gestionnaire d&apos;impression tel que kprinter ou gtklp, afin de disposer d&apos;options d&apos;impression supplémentaires</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Définit le niveau PostScript.
Les niveaux 1 et 2 peuvent créer des fichiers volumineux.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>L&apos;UCR (Under Color Removal en anglais) consiste à n&apos;utiliser que de l&apos;encre noire pour le rendu des teintes grises composées par les trois couleurs primaires. L&apos;UCR est plus efficace sur les zones de l&apos;image dans des tons neutres ou sombres, proches du gris. Cette fonction peut permettre une meilleure qualité d&apos;impression, mais des tests au cas par cas sont nécessaires. L&apos;UCR réduit les risques de saturation avec les encres CMJ.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Cette fonction permet de convertir les couleurs d&apos;accompagnement en un équivalent quadrichromique. Cochez cette option à moins de savoir exactement ce que vous faites.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Permet d&apos;incorporer des profils colorimétriques ICC dans votre processus d&apos;impression lorsque la gestion des couleurs est active.</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Vous pouvez ici définir le format de média sur lequel votre fichier PostScript sera imprimé. Ceci n&apos;est pas recommandé à moins que votre imprimeur n&apos;en ait besoin.</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Impossible d&apos;obtenir les paramètres de votre imprimante</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Fichiers PS (*.ps);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printer Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Crop Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleed Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Registration Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleed Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="unfinished">Droite :</translation>
    </message>
    <message>
        <source>Use Document Bleeds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleeds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Fond perdu en haut de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Fond perdu en bas de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Fond perdu à gauche de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Fond perdu à droite de la page</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Impossible d&apos;importer le fichier suivant :
%1</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Erreur fatale</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <comment>Fred</comment>
        <translation type="obsolete">Import PostScript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <comment>Fred</comment>
        <translation type="obsolete">Analyse du fichier PostScript :</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <comment>Fred</comment>
        <translation type="obsolete">Création des objets</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <comment>Fred</comment>
        <translation type="obsolete">Echec de la conversion de %1 images !</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation type="obsolete">Import PostScript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation type="unfinished">Analyse du fichier PostScript :</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation type="unfinished">Création des objets</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation type="unfinished">Echec de la conversion de %1 images !</translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Importing: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Édition du style</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Caractère spécial</translation>
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
        <source>Distances</source>
        <translation>Distances</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Espacement fixe entre les lignes</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <comment>Fred</comment>
        <translation type="obsolete">Interlignage automatique</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Aligner le texte sur la grille typographique</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Lettrines</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Lignes :</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Distance au texte :</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulations et retrait</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Aperçu des styles de paragraphe</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fond de page</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="obsolete">Facilite la lecture d&apos;un texte de couleur pâle</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Nom du style de paragraphe</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Police du texte ou de l&apos;objet sélectionné</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Corps</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Couleur du texte</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Couleur des contours du texte</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Définit la hauteur des lettrines, en nombre de lignes</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Définit la distance entre les lettrines et le texte</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Mettre une espace au-dessus du paragraphe</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Mettre une espace en dessous du paragraphe</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Interlignage</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Afficher un exemple de ce style de paragraphe</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Ce nom de style existe déjà</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <comment>Fred</comment>
        <translation type="obsolete">Réglage de l&apos;approche</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <comment>Fred</comment>
        <translation type="obsolete">Décalage des caractères par rapport à la ligne de base</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation>Cliquer pour choisir le mode d&apos;interlignage</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished">Réglage de l&apos;approche</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished">Décalage des caractères par rapport à la ligne de base</translation>
    </message>
    <message>
        <source>Select for easier reading of light colored text styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation type="unfinished">Automatique</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Éditeur</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ouvrir...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Enregistrer &amp;sous...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Enregistrer et quitter</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Quitter sans enregistrer</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cou&amp;per</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Co&amp;ller</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>O&amp;btenir les noms des champs</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>Javascripts (*.js);;Tous les fichiers (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Effets</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Couleur :</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Teinte :</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Luminosité :</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Contraste :</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Rayon :</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Valeur :</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Postériser :</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Effets disponibles</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Flou</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Luminosité</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Coloriser</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Contraste</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Niveaux de gris</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverser</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Postériser</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Accentuer les contours</translation>
    </message>
    <message>
        <source>&gt;&gt;</source>
        <translation>&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation>&lt;&lt;</translation>
    </message>
    <message>
        <source>Effects in use</source>
        <translation>Effets utilisés</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Color 1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color 2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color 3:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color 4:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tritone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quadtone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Curves</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExportBitmap</name>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="unfinished">Fichier existant. Remplacer ?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished">existe déjà. Remplacer ?</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tout</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Sélectionnez un dossier pour l&apos;exportation</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportation en tant qu&apos;image(s)</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exporter vers :</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>T&amp;ype d&apos;image :</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualité :</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Résolution :</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Taille :</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppp</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Sélection</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Page &amp;courante</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Toutes les pages</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Sélection</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exporter une sélection de pages vers un PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Insérer une liste d&apos;éléments séparés par des virgules,
les éléments pouvant être « * » pour toutes les pages, 1-5 pour
une sélection de pages, ou bien un simple numéro de page.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exporter toutes les pages</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exporter uniquement la page courante</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Résolution des images
Utiliser 72 ppp pour des images destinées à la lecture sur écran</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Dimensions des images. 100% pour des dimensions identiques, 200% pour les doubler, etc.</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Qualité des images - 100% pour la plus haute, 1% pour la plus basse</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Formats disponibles pour l&apos;exportation</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Dossier de sortie - endroit où sont placées les images matricielles. Le nom
du fichier exporté est de la forme « nomdudocument-numérodepage.type »</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Changer de dossier de sortie</translation>
    </message>
    <message>
        <source>Image size in Pixels</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Propriétés étendues de l&apos;image</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Assombrir</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Eclaircir</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Teinte</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturation</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Couleur</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Luminosité</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Multiplier</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Écran</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Dissoudre</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Superposer</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Lumière dure</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Lumière douce</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Différence</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Exclusion</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Fusion de couleurs</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Multiplier</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Exclusion</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Mode de fusion :</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacité :</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Fond de page</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Calques</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>N&apos;utiliser aucun chemin</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Chemins</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Corps :</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Résolution :</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>PPP</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>RVB</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMJN</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Niveaux de gris</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Espace colorimétrique :</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titre :</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Pas de titre</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Auteur :</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Document Scribus</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Certaines polices utilisées dans ce document ont été substituées :</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>a été remplacé par :</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Polices disponibles</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Substitutions de polices</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Chemins additionnels</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Nom de la police</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Utiliser la police</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="obsolete">Incorporée :</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Jeu partiel</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Chemin</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Polices &amp;disponibles</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nom de la police</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Substitution</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Substitutions de polices</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>A&amp;jouter...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="obsolete">Les chemins d&apos;accès aux polices ne peuvent être définis que dans les préférences, et lorsque aucun document n&apos;est ouvert. Fermez tous les documents ouverts, et utilisez le menu Édition &gt; Préférences pour modifier le chemin d&apos;accès aux polices.</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Chemins additionnels</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Sélectionnez un dossier</translation>
    </message>
    <message>
        <source>Embed in PostScript</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font search paths can only be set in File &gt; Preferences, and only when there is no document currently open. Close any open documents, then use File &gt; Preferences &gt; Fonts to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>à l&apos;île exiguë où l&apos;obèse jury mûr fête l&apos;haï volapük,
âne ex æquo au whist, ôtez ce vœu déçu
À L&apos;ÎLE EXIGUË OÙ L&apos;OBÈSE JURY MÛR FÊTE L&apos;HAÏ VOLAPÜK,
ÂNE EX ÆQUO AU WHIST, ÔTEZ CE VŒU DÉÇU</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Ajouter la police sélectionnée dans le menu Style, Polices</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Fermer la prévisualisation</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Saisissez ici le texte de recherche rapide dans les noms de polices. La recherche est insensible à la casse. Les jokers habituels (*,·?,·[...]) sont disponibles. Parexemple, t* listera toutes les polices dont le nom débute par t ou T; *bold* affichera les polices dont le nom contient bold, bolder, etc.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Démarrer la recherche</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Corps de la police sélectionnée</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>L&apos;échantillon sera visible dès que la touche sera relâchée</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Utilisateur</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Système</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Aperçu des polices disponibles</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Recherche rapide :</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Re&amp;chercher</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nom de la police</translation>
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
        <translation>Jeu partiel</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Accès</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Corps :</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Exemple de texte</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>Ré&amp;gler</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Revenir aux réglages texte initiaux</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>A&amp;jouter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
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
        <translation>Aperçu des &amp;polices disponibles...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Dialogue d&apos;aperçu des polices disponibles</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Rechercher et trier les polices disponibles.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Substitutions de polices</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Ce document utilise des polices qui ne sont pas installées sur votre système. Veuillez leur choisir des polices de substitution. Annuler stoppera le chargement du document.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Police d&apos;origine</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Police de substitution</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Rendre ces substitutions permanentes</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Annule ces substitutions de polices et arrête le chargement du document</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Activer ceci force Scribus à utiliser ces polices-ci en remplacement permanent des polices manquantes dans tout nouveau document. Ce réglage peut être modifié dans Édition &gt; Préférences &gt; Polices.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Si vous sélectionnez OK, puis sauvegardez, ces substitutions seront rendues permanentes dans le document.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Position :</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <comment>Fred</comment>
        <translation type="obsolete">Ajoutez, modifiez ou supprimez les points de contrôle de couleur ici</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation type="unfinished">Ajoutez, modifiez ou supprimez les points de contrôle de couleur ici</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation type="obsolete">Gestion des repères</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation type="obsolete">Repères horizontaux</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="obsolete">Repère</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="obsolete">Unités</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="obsolete">&amp;Y :</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">Ajo&amp;uter</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="obsolete">&amp;Supprimer</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation type="obsolete">Repères verticaux</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="obsolete">&amp;X :</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="obsolete">Ajout&amp;er</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="obsolete">Su&amp;pprimer</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation type="obsolete">Lignes et colonnes - guides automatiques</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation type="obsolete">&amp;Lignes :</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation type="obsolete">C&amp;olonnes :</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation type="obsolete">Espace entre les li&amp;gnes</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation type="obsolete">Espace entre les colo&amp;nnes</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation type="obsolete">En rapport à :</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Page</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation type="obsolete">&amp;Marges</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation type="obsolete">&amp;Sélection</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Fermer</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation type="obsolete">Act&amp;ualiser</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Prévisualisation</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="obsolete">&amp;Repères fixes</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation type="obsolete">Appliquer à toutes les p&amp;ages :</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation type="obsolete">Positionne les repères dans le document. Le gestionnaire de repères est toujours ouvert mais les changements sont permanents</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation type="obsolete">Il y a déjà un guide vide (0.0)</translation>
    </message>
    <message>
        <source>Edit Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter a position:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Guide</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GuideManagerBase</name>
    <message>
        <source>Manage Guides</source>
        <translation type="unfinished">Gestion des repères</translation>
    </message>
    <message>
        <source>Horizontals</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished">Repère</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="obsolete">Unités</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="unfinished">&amp;Supprimer</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished">Alt+E</translation>
    </message>
    <message>
        <source>Verticals</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="unfinished">Ajout&amp;er</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="unfinished">Su&amp;pprimer</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="unfinished">&amp;Repères fixes</translation>
    </message>
    <message>
        <source>Appl&amp;y to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Number:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>U&amp;se Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>Nu&amp;mber:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use &amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished">Alt+G</translation>
    </message>
    <message>
        <source>Refer To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="unfinished">&amp;Page</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished">Alt+P</translation>
    </message>
    <message>
        <source>M&amp;argins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;election</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Misc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all guides from the current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all guides from the current document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Single</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Column/Row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete Guides from Current &amp;Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete Guides from &amp;All Pages</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation>Contenu</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lien</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Contenu</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Re&amp;chercher</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Pertinence</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>R&amp;echercher</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>&amp;Tout effacer</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>Si&amp;gnets</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Im&amp;primer...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Quitter</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Chercher...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Trouver le suiva&amp;nt</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Trouver le &amp;précédent</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Ajouter un signet</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>Tout &amp;effacer</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Signets</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Aide en ligne</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>La recherche est indépendante de la casse</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Le manuel n&apos;est pas installé. Allez sur le site http://docs.scribus.net pour le télécharger.</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Rechercher</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Rechercher :</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nouveau signet</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Titre du nouveau signet :</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Césure possible</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Approuver</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Sauter</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Suggestions de &amp;césure</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Insère les césures automatiquement pen&amp;dant la saisie</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Langue :</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Mot le plus &amp;court :</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Césures consécutives &amp;autorisées :</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Une boîte de dialogue affichant toutes les césures possibles apparaîtra pour chaque mot lorsque vous déciderez d&apos;effectuer les césures.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Effectue automatiquement les césures en cours de saisie.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Minimum de lettres qu&apos;un mot doit contenir pour être coupé.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Nombre maximum de césures consécutives. Une valeur
de 0 permet un nombre illimité de césures.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Informations sur l&apos;image</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Informations générales</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Date / heure :</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Possède le profil : </translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Oui</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Non</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Nom du profil :</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Possède un tracé de détourage incorporé</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Possède les calques :</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>Informations EXIF</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Artiste :</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Commentaire :</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Commentaire utilisateur :</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Modèle d&apos;appareil photo :</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Fabricant de l&apos;APN :</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright :</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Modèle de scanner :</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Fabricant du scanner :</translation>
    </message>
    <message>
        <source>Exposure time</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Aperture:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ISO equiv.:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importer un fichier (&amp;E)PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importe des fichiers EPS</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importe la plupart des fichiers EPS dans le document courant,
convertissant les données vectorielles en objets Scribus.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>PDF</source>
        <translation>PDF</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Insérer une(des) page(s)</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insérer</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>page(s)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>avant la page</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>après la page</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>à la fin</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Gabarits</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Gabarit :</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Format de la page</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Taille :</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntation :</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hauteur :</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Déplacer les objets avec leur page</translation>
    </message>
</context>
<context>
    <name>InsertAFrame</name>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ouvrir</translation>
    </message>
    <message>
        <source>&lt;b&gt;Insert a text frame&lt;/b&gt;&lt;br/&gt;A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable format from plain text to OpenOffice.org.&lt;br/&gt;Your text may be edited and formatted on the page directly or in the simple Story Editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Insert an image frame&lt;/b&gt;&lt;br/&gt;An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertAFrameBase</name>
    <message>
        <source>Insert A Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;ype</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">Cadre de &amp;texte</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished">Cadre d&apos;&amp;image</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation type="unfinished">T&amp;ableau</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Polygone</translation>
    </message>
    <message>
        <source>&amp;Location</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Placement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Current Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selecting this will place the frame only on the current page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Range of Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selecting this will place frame on the selected range. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Position of Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Top Left of Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selecting this puts the frame on the top left with postion 0,0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Top Left of Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selecting this places the frame in the upper left of the page margins defined in your doc setup.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom Position:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the dimensions wished below in the X: Y: dialog below.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">J :</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Corps</translation>
    </message>
    <message>
        <source>Same as the Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Same as the Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Largeur :</translation>
    </message>
    <message>
        <source>&amp;Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source Image:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Select File...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>There are no options for this type of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Columns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Annuler</translation>
    </message>
    <message>
        <source>Link Created Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="unfinished">Toutes les pages</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Top Left of Bleed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selecting this places the frame in the upper left of the page bleed defined in your doc setup.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Same as the Bleed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Same as the Imported Image</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Insérer un tableau</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Nombre de lignes :</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Nombre de colonnes :</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Modifier les JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Modifier...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>A&amp;jouter...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Ajoute un nouveau script et définit une fonction du même nom. Si vous souhaitez utiliser ce script comme &quot;action d&apos;ouverture&quot;, assurez vous de ne pas renommer cette fonction.</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nouveau script :</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nouveau script</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Effacer définitivement ce script ?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Oui</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Non</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation type="obsolete">Action</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation type="obsolete">Raccourci actuel</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation type="obsolete">Sélectionner un raccourci pour cette action</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="obsolete">&amp;Pas de raccourci</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="obsolete">Racco&amp;urci personnalisé</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="obsolete">Alt+Majuscule+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="obsolete">&amp;Définir le raccourci</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="obsolete">Jeux de raccourcis clavier à charger</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="obsolete">&amp;Charger</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="obsolete">&amp;Importer...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="obsolete">E&amp;xporter...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="obsolete">&amp;Remise à zéro</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="obsolete">Jeux de raccourcis claviers disponibles</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="obsolete">Charger le jeu de raccourcis clavier</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="obsolete">Importer un jeu de raccourcis clavier dans la configuration courante</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="obsolete">Exporter les raccourcis clavier actuel vers un fichier</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="obsolete">Recharger les raccourcis clavier par défaut</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation type="obsolete">Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="obsolete">Ctrl</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="obsolete">Majuscule</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="obsolete">Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="obsolete">Meta+</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="obsolete">Majuscule+</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation type="obsolete">Alt+</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation type="obsolete">Ctrl+</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="obsolete">Raccourci déjà utilisé</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="obsolete">Fichiers de raccourcis clavier XML (*.ksxml)</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Détruire le calque</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Supprimer également tous les objets de ce calque ?</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Calques</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Créer un nouveau calque</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Détruire le calque</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Placer le calque au-dessus</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Placer le calque au-dessous</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished">Opacité :</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Mode de fusion :</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">Assombrir</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Eclaircir</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Multiplier</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Écran</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Superposer</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Lumière dure</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Lumière douce</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Différence</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Exclusion</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Fusion de couleurs</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Multiplier</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Exclusion</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Teinte</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Saturation</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Couleur</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Luminosité</translation>
    </message>
    <message>
        <source>Color of the Layer Indicator - Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make Layer Visible - Uncheck to hide the layer from the display </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Layer - Uncheck to disable printing. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lock or Unlock Layer - Unchecked is unlocked </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outline Mode - Toggles the &apos;wireframe&apos; display of objects to speed the display of very complex objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the Layer - Double clicking on the name of a layer enabled editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Duplicates the current layer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Édition des styles de filets</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Dupliquer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copie de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nouveau style</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Effacer définitivement ce style ?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Oui</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Non</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documents (*.sla *.scd);;Tous les fichiers (*)</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>LineStyleWBase</source>
        <translation>LineStyleWBase</translation>
    </message>
    <message>
        <source>%</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Épaisseur :</translation>
    </message>
</context>
<context>
    <name>LineStyleWidget</name>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plates</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Carrées</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arrondies</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Pointus</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Biseautés</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arrondis</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="obsolete">ligne continue</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="obsolete">Ligne tiretée</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="obsolete">Ligne pointillée</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="obsolete">Ligne mixte trait-point</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="obsolete">Ligne mixte trait-point-point</translation>
    </message>
    <message>
        <source>Add a new line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Join style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line shade</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Tous les fichiers (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Sélectionner le faux texte</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Auteur :</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Plus :</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>Fichier XML :</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Faux texte</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Paragraphes :</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Propriétés de la page</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Format de la page</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Taille :</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntation :</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hauteur :</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Déplacer les objets avec leur page</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marges</translation>
    </message>
    <message>
        <source>Other Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>Preset Layouts:</source>
        <translation>Formats prédéfinis :</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bas :</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Haut :</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Droite :</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Gauche :</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="obsolete">Appliquer les marges à toutes les pages</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Appliquer les changements de marge à toutes les pages du document</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Marges de l&apos;imprimante...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Obtenir les marges du format spécifié à partir des zones non-imprimables des imprimantes disponibles</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Espacement entre la marge haute et le bord de la page</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Espacement entre la marge basse et le bord de la page</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Espacement entre la marge de gauche et le bord de la page. Si Pages en regard est sélectionné, cet espacement peut être utilisé pour  disposer de marges suffisantes pour la reliure</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Espacement entre la marge de droite et le bord de la page. Si Pages en regard est sélectionné, cet espacement peut être utilisé pour disposer de marges suffisantes pour la reliure</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Intérieur :</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Extérie&amp;ur :</translation>
    </message>
    <message>
        <source>Apply settings to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Document Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing master pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Marges</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Fond perdu en haut de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Fond perdu en bas de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Fond perdu à gauche de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Fond perdu à droite de la page</translation>
    </message>
    <message>
        <source>Bleeds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="unfinished">Droite :</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Editer les gabarits</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Dupliquer le gabarit sélectionné</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Supprimer le gabarit sélectionné</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Ajouter un nouveau gabarit</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Importer les gabarits d&apos;un autre document</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Effacer définitivement ce gabarit ?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Oui</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Non</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Nouveau gabarit</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Copie de %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Nouveau gabarit</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nouveau gabarit %1</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copie #%1 de</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Dupliquer-Déplacer multiples</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Nombre de copies :</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Déplacement &amp;horizontal :</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Déplacement &amp;vertical :</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Distances</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation>X1 :</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation>Y1 :</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation>X2 :</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation>Y2 :</translation>
    </message>
    <message>
        <source>DX:</source>
        <translation>DX :</translation>
    </message>
    <message>
        <source>DY:</source>
        <translation>DY :</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation>Angle :</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Longueur :</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Master Page</source>
        <translation>Importer un gabarit</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importer une(des) page(s)</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Depuis le document :</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importer la(les) page(s) :</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Importer un gabarit</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Insérer une liste d&apos;éléments séparés par des virgules, les éléments pouvant être « * » pour toutes les pages, 1-5 pour ne sélection de pages, ou bien un simple numéro de page.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> à partir de 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Créer une(des) page(s)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Avant la page</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Après la page</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>À la fin</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documents (*.sla *.scd);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> de %1</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Police manquante</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>La police %1 n&apos;est pas installée.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Utiliser</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>à la place</translation>
    </message>
</context>
<context>
    <name>ModeToolBar</name>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Outils</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="unfinished">Propriétés...</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Déplacer une(des) page(s)</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copier une page</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Déplacer la(es) page(s)</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>à :</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Nombre de copies :</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Avant la page</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Après la page</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>À la fin</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Déplacer la(es) page(s) :</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Espacement fixe entre les lignes</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Interlignage automatique :</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Aligner le texte sur la grille typographique</translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation>&amp;X1 :</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation>X&amp;2 :</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation>Y&amp;1 :</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation>&amp;Y2 :</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X :</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hauteur :</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Espacement entre les colonnes</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Largeur de colonne</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Aucun style</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Le nom « %1 » existe déjà.&lt;br/&gt;Choisissez-en un autre.</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Texte</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Image</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>Forme (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>Fi&amp;let</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Couleurs</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Géométrie</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation :</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Point de référence :</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Disposition</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forme :</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Modifier la forme...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Angles
arr&amp;ondis :</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distance au texte</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>No&amp;mbre de colonnes :</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">&amp;Gouttière :</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Haut :</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bas :</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Gauche :</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Droite :</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>Tab&amp;ulations...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Propriétés du tracé de texte</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Afficher les courbes</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Départ :</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distance à la courbe :</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Filet plein</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Pair-Impair</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Pas zéro</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="obsolete">H&amp;abillage</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Utilise&amp;r les limites du cadre</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Utiliser la ligne de contour</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;yle :</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation type="obsolete">L&amp;angue :</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Mise à l&apos;échelle &amp;libre</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Résolution horizontale actuelle : </translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Résolution verticale actuelle : </translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>Échelle &amp;horizontale :</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Échelle &amp;verticale :</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Mettre l&apos;ima&amp;ge aux dimensions du cadre</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proportionnel</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Profil source :</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Rendu :</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptible</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrie relative</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturation</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrie absolue</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Point de départ</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Extrémités</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>Point de &amp;référence :</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>T&amp;ype de filet :</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Flèche de début :</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Flèche de fin :</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>É&amp;paisseur :</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>An&amp;gles :</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Pointus</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Biseautés</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arrondis</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plates</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Carrées</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arrondies</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>T&amp;erminaisons :</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Filets de tableau</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Filet en haut</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Filet à la gauche</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Filet à la droite</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Filet en bas</translation>
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
        <translation>Nom de l&apos;objet sélectionné</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Position horizontale du point de référence courant</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Position verticale du point de référence courant</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Largeur</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Hauteur</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotation de l&apos;objet par rapport au point de référence courant</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Point à partir duquel les mesures et les rotations sont effectuées</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Définir le coin supérieur gauche comme point de référence</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Définir le coin supérieur droit comme point de référence</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Définir le coin inférieur gauche comme point de référence</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Définir le coin inférieur droit comme point de référence</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Définir le centre comme point de référence</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Retourner horizontalement</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Retourner verticalement</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Placer au-dessus</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Placer au-dessous</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Mettre devant</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Mettre au fond</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Plan de l&apos;objet, 0 indique qu&apos;il est au fond</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Verrouiller ou non l&apos;objet</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Verrouiller ou non les dimensions de l&apos;objet</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Autoriser ou non l&apos;impression de l&apos;objet</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="obsolete">La justification du texte situé au-dessous est soumise aux contours de l&apos;objet</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Utiliser les limites du cadre pour la justification du texte</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Utiliser une ligne basée sur les contours de l&apos;objet pour la justification du texte</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Police du texte ou de l&apos;objet sélectionné</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Corps</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Décalage des caractères par rapport à la ligne de base</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Mise à l&apos;échelle de la largeur des caractères</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Mise à l&apos;échelle de la hauteur des caractères</translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="obsolete">Couleur de contour du texte. Disponible seulement si l&apos;option Contour est appliquée.</translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="obsolete">Couleur du texte.</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturation de la couleur des contours du texte</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturation de la couleur du texte</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Écriture de droite à gauche</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Réglage de l&apos;approche</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Interlignage</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Style du paragraphe courant</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="obsolete">Langue de césure pour ce cadre</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Modifier les paramètres pour le point de départ ou les extrémités</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Modèle de filet</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Épaisseur de filet</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Type de raccord</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Type de terminaison</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Style de filet de l&apos;objet courant</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Choisissez la forme du cadre...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Modifier la forme du cadre...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Définir le rayon de l&apos;arrondi</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Nombre de colonnes dans un cadre de texte</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Alterne entre gouttière et largeur de colonne</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Distance au texte depuis le haut du cadre</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Distance au texte depuis le bas du cadre</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Distance au texte depuis la gauche du cadre</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Distance au texte depuis la droite du cadre</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Modifier les paramètres de tabulation du cadre de texte...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Autoriser l&apos;image à des dimensions autres que celles du cadre</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Décalage horizontal de l&apos;image par rapport au cadre</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Décalage vertical de l&apos;image par rapport au cadre</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Redimensionnement horizontal de l&apos;image</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Redimensionnement vertical de l&apos;image</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Garder les mêmes dimensions pour X et Y</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Conserver les proportions</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Donner les dimensions du cadre à l&apos;image</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Utiliser les proportions de l&apos;image plutôt que celles du cadre</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Profil source de l&apos;image</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Méthode de rendu pour l&apos;image</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <comment>Fred</comment>
        <translation type="obsolete">Cliquer pour choisir le mode d&apos;interlignage</translation>
    </message>
    <message>
        <source>Overprinting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Knockout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overprint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation type="obsolete">Cliquer pour choisir le mode d&apos;interlignage</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Largeur :</translation>
    </message>
    <message>
        <source>Text &amp;Flow Around Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Désactivé</translation>
    </message>
    <message>
        <source>Use Frame &amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Effects</source>
        <translation type="unfinished">Effets</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished">Propriétés étendues de l&apos;image</translation>
    </message>
    <message>
        <source>Disable text flow from lower frames around object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use the frame shape for text flow of text frames below the object.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">&amp;Grouper</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished">Opacité :</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Mode de fusion :</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">Assombrir</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Eclaircir</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Multiplier</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Écran</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Superposer</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Lumière dure</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Lumière douce</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Différence</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Exclusion</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Fusion de couleurs</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Multiplier</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Teinte</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Couleur</translation>
    </message>
    <message>
        <source>Group the selected objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Destroys the selected group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation type="unfinished">Automatique</translation>
    </message>
    <message>
        <source>When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame&apos;s shape for text flow of text frames below the object. T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Édition du style</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plates</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Carrées</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arrondies</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Pointus</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Biseautés</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arrondis</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Épaisseur :</translation>
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
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>ligne continue</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Ligne tiretée</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Ligne pointillée</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Ligne mixte trait-point</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Ligne mixte trait-point-point</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Le nom « %1 » existe déjà.&lt;br/&gt;Choisissez-en un autre.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Progression</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Progression totale : </translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
</context>
<context>
    <name>MultipleDuplicate</name>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished">Déplacement &amp;horizontal :</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">Déplacement &amp;vertical :</translation>
    </message>
    <message>
        <source>&amp;Horizontal Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Vertical Gap:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultipleDuplicateBase</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="unfinished">Dupliquer-Déplacer multiples</translation>
    </message>
    <message>
        <source>&amp;By Number of Copies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation type="unfinished">&amp;Nombre de copies :</translation>
    </message>
    <message>
        <source>Create &amp;Gap Between Items Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished">Alt+G</translation>
    </message>
    <message>
        <source>&amp;Shift Created Items By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished">Déplacement &amp;horizontal :</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">Déplacement &amp;vertical :</translation>
    </message>
    <message>
        <source>By &amp;Rows &amp;&amp; Columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Annuler</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Mon &amp;extension</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Mon extension</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>L&apos;extension a fonctionné !</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nouveau document</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nouveau document</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Ouvrir un document &amp;existant</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>&amp;Documents récents</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Ne plus demander</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Dimensions de page du document, soit standards soit personnalisées</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientation des pages du document</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Largeur des pages du document, modifiable si vous avez choisi des dimensions de page personnalisées</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Hauteur des pages du document, modifiable si vous avez choisi des dimensions de page personnalisées</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Nombre initial de pages du document</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unité de mesure par défaut pour l&apos;édition de document</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Créer automatiquement les cadres de texte quand de nouvelles pages sont ajoutées</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Nombre de colonnes à créer dans les cadres de texte créés automatiquement</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Espacement entre les colonnes créées automatiquement</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Format de la page</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Taille :</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntation :</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Hauteur :</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marges</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Nom&amp;bre de pages :</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Unité par dé&amp;faut :</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Cadres de texte automati&amp;ques</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Gouttière :</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>No&amp;mbre de colonnes :</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="obsolete">Disposition</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation type="unfinished">La première page est :</translation>
    </message>
    <message>
        <source>Show Document Settings After Creation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nouveau d&apos;après mo&amp;dèle...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Charger les documents à mise en page prédéfinie</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Crée un document d&apos;après un modèle créé par vous ou par d&apos;autres (par exemplepour des documents possédant un style constant).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Points</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Coordonnées &amp;absolues</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X :</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y :</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Modifier le &amp;contour</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Remise à zéro du contour</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Terminer</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Déplacer un point</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Déplacer les points de contrôle</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Ajouter un point</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Supprimer un point</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Déplacer les points de contrôle indépendamment</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Déplacer les points de contrôle de façon symétrique</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Remise à zéro des points de contrôle</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Remise à zéro du point de contrôle courant</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Ouvre un polygone ou découpe une courbe de Bézier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Ferme cette courbe de Bézier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Miroir horizontal des tracés</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Miroir vertical des tracés</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Étirer les tracés vers la droite</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Étirer les tracés vers la gauche</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Étirer les tracés vers le haut</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Étirer les tracés vers le bas</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotation du tracé sens antihoraire</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotation du tracé sens horaire</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Réduire la taille du tracé du pourcentage indiqué</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Augmenter la taille du tracé du pourcentage indiqué</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Réduire la taille du tracé de la valeur indiquée</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Augmenter la taille du tracé de la valeur indiquée</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Angle de rotation</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>% d&apos;agrandissement ou de réduction</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Valeur d&apos;agrandissement ou de réduction</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Activer le mode d&apos;édition des contours</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Mettre le contour à la forme du cadre</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Lorsque cochée, on utilise les coordonnées relatives à la page, sinon celles relatives à l&apos;objet.</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Ce document ne semble pas être un fichier OpenOffice Draw.</translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importer une illustration &amp;OpenOffice.org...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importe une illustration OpenOffice.org</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importe la plupart des fichiers OpenOffice Draw dans le document courant,convertissant les données vectorielles en objets Scribus.</translation>
    </message>
    <message>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation>Illustration OpenDocument·1.0</translation>
    </message>
    <message>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation>Illustration OpenOffice.org 1.x</translation>
    </message>
    <message>
        <source>This file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Options d&apos;importation des fichiers OpenDocument</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Écraser les Styles de paragraphe</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Activer ceci remplacera les styles présents dans le document courant</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Fusionner les styles de paragraphe</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Fusionner les styles de paragraphe par attributs. Le résultat est un plus faible nombre de styles de paragraphe semblables, tout en conservant les attributs des styles, même si les styles du document d&apos;origine sont nommés différement.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Utiliser le nom du document comme préfixe pour les styles de paragraphe</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Ajouter le nom du document devant le nom du style de paragraphe.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ne plus demander</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Faire de ces paramètres ceux par défaut et ne plus demander à la prochaine importation d&apos;un document OASIS OpenDocument</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Document Scribus</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Document Scribus 1.2.x</translation>
    </message>
</context>
<context>
    <name>OneClick</name>
    <message>
        <source>Origin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">Corps</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Largeur :</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation type="unfinished">Longueur :</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Angle :</translation>
    </message>
    <message>
        <source>Remember Values</source>
        <translation type="unfinished"></translation>
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
        <translation>Épaisseur</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Enregistrer en PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Sortie vers un fichier :</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Parcourir...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Générer un fichier pour c&amp;haque page</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Ceci permet d&apos;exporter un fichier PDF par page du document, nommés individuellement.Les numéros de page sont ajoutés automatiquement. Ceci est surtout utile pour l&apos;imposition ou l&apos;impression commerciale.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Le bouton &quot;enregistrer&quot; sera désactivé si vous tentez d&apos;exporter au format PDF/X-3 et que la chaîne d&apos;information de l&apos;onglet PDF/X-3 est vide.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Fichiers PDF (*.pdf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>%1 does not exists and will be created, continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot create directory: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDFToolBar</name>
    <message>
        <source>PDF Tools</source>
        <translation type="unfinished">Outils PDF</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Enregistrement en PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation type="obsolete">Export des gabarits :</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="obsolete">Export des pages :</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Exporter les objets de la page courante :</translation>
    </message>
    <message>
        <source>Exporting Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="unfinished">Page :</translation>
    </message>
    <message>
        <source>Date:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Aperçu avant impression</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="obsolete">Lisser les &amp;textes</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="obsolete">Lisser les objets &amp;graphiques</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Afficher les trans&amp;parences</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>Retrait des &amp;sous-couleurs (UCR)</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Afficher en CMJN</translation>
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
        <translation>&amp;J</translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation>&amp;N</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Nom de la séparation</translation>
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
        <translation>Jaune</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Noir</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Mise à l&apos;échelle :</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Imprimer...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Donne un affichage plus agréable des textes, avec pour effet un léger ralentissement lors d&apos;une prévisualisation. Fonctionne uniquement avec les polices Type 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Donne un affichage plus agréable des polices TrueType, OpenType, des fichiers EPS, PDF ainsi que des graphiques vectoriels lors d&apos;une prévisualisation, ceci occasionnant alors un léger ralentissement</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Affiche la transparence et les objets transparents dans votre document. Nécessite au minimum Ghostscript 7.07</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>L&apos;aperçu simule les encres CMJN, au lieu des couleurs RVB</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>L&apos;UCR (Under Color Removal en anglais) consiste à n&apos;utiliser que de l&apos;encre noire pour le rendu des teintes grises composées par les trois couleurs primaires. L&apos;UCR est plus efficace sur les zones de l&apos;image dans des tons neutres ou sombres, proches du gris. Cette fonction peut permettre une meilleure qualité d&apos;impression, mais des tests au cas par cas sont nécessaires.L&apos;UCR réduit les risques de saturation avec les encres CMJ.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Changer l&apos;échelle de la page</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Activer/désactiver la couche C (cyan)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Activer/désactiver la couche M (magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Activer/désactiver la couche J (jaune)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Activer/désactiver la couche N (noir)</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tout</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fit to Width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fit to Height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fit to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation type="unfinished">Paramètres d&apos;affichage</translation>
    </message>
    <message>
        <source>Print Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished">Miroir horizontal des pages</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished">Miroir vertical des pages</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">S&apos;arrêter aux marges</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished">Imprimer en niveaux de gris</translation>
    </message>
    <message>
        <source>Convert Spot Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished">Appliquer un(des) profil(s) ICC</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished">Cette fonction permet de convertir les couleurs d&apos;accompagnement en un équivalent quadrichromique. Cochez cette option à moins de savoir exactement ce que vous faites.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished">Permet d&apos;incorporer des profils colorimétriques ICC dans votre processus d&apos;impression lorsque la gestion des couleurs est active.</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation type="obsolete">Traitement des gabarits :</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="obsolete">Export des pages :</translation>
    </message>
    <message>
        <source>Processing Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PStyleWBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Propriétés</translation>
    </message>
    <message>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distances and Alignment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation type="unfinished">Lettrines</translation>
    </message>
    <message>
        <source>Parent&apos;s Drop Cap Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabulations et retrait</translation>
    </message>
    <message>
        <source>Ch&amp;aracter Style</source>
        <translation type="unfinished"></translation>
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
        <translation>Texte</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Filet</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygone</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Ligne complexe</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Tracé</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Copie de</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Relatif à</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Est le parent de</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Est le fils de </translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Attributs de l&apos;objet</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Paramètre</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>En rapport</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>En rapport à</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
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
        <translation>&amp;Annuler</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation type="obsolete">Disposition</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>La première page est :</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Pages en regard</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Milieu droit</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Déposez une page ou un gabarit sur la corbeille pour le supprimer.</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Voici la liste des gabarits disponibles ; pour créer une nouvelle page, déposez un gabarit dans la fenêtre ci-dessous.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Organiser les pages</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Gabarits disponibles :</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Pages du document :</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %1</source>
        <translation>%1 sur %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 sur %2</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Lettre</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Légal</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Moyenne</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="obsolete">A</translation>
    </message>
    <message>
        <source>B</source>
        <translation type="obsolete">B</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">C</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="obsolete">D</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="obsolete">E</translation>
    </message>
</context>
<context>
    <name>PatternDialog</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Sélectionnez un dossier</translation>
    </message>
    <message>
        <source>Loading Patterns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ouvrir</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation type="obsolete">Résultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="obsolete">Résultats de la recherche pour : </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Prévisualisation</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="obsolete">Sélectionner</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Annuler</translation>
    </message>
</context>
<context>
    <name>PicSearchBase</name>
    <message>
        <source>Result</source>
        <translation type="unfinished">Résultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="unfinished">Résultats de la recherche pour : </translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="unfinished">&amp;Prévisualisation</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished">Alt+P</translation>
    </message>
    <message>
        <source>&amp;Select</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Manage Pictures</source>
        <translation>Gestion des images</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Chemin</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Page</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Imprimer</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>État</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Oui</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Manquante</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Rechercher</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Annuler la recherche</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Aller à</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Sélectionnez un dossier de base pour la recherche</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - Recherche d&apos;image</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>La recherche a échoué : %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Impossible de trouver des images nommées &quot;%1&quot;</translation>
    </message>
    <message>
        <source>Search Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Fermer</translation>
    </message>
    <message>
        <source>Set a new location for the selected items. Useful when you may have moved the document but not the images.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select a base directory for your selected rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Case insensitive search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show/hide image thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Enregistrer en tant qu&apos;&amp;image...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exporter en tant qu&apos;image</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exporte les pages sélectionnées comme images bitmap.</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="unfinished">Enregistrer en tant qu&apos;image</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished">Erreur d&apos;écriture du(des) fichier(s) de sortie.</translation>
    </message>
    <message>
        <source>Export successful</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Impossible de trouver l&apos;extension</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>erreur inconnue</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Impossible de trouver le symbole (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Extensions : chargement de %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>Échec de l&apos;initialisation</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>Extension de type inconnu</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Extension %1 chargée</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Impossible de charger l&apos;extension %1 : %2</translation>
    </message>
    <message>
        <source>Plugin: %1 initialized ok </source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin: %1 failed post initialization</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Gestionnaire d&apos;extensions</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Extension</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Comment lancer</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Charger ?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Identification de l&apos;extension</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Oui</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Non</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Vous devez redémarrer l&apos;application pour que les changements prennent effet</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Propriétés du polygone</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Nombr&amp;e de côtés :</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation :</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>&amp;Polygone convexe-concave</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Intensité :</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Nombre d&apos;angles pour les polygones</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Degrés de rotation pour les polygones</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Autoriser la déformation des côtés du polygone</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Exemple de polygone</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Une valeur négative correspond à une forme concave (en forme d&apos;étoile), une valeur positive à une forme convexe</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Préférences</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="obsolete">Interface graphique</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="obsolete">&amp;Langue :</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="obsolete">Appar&amp;ence :</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="obsolete">&amp;Corps (Menus) :</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="obsolete">Corps (&amp;Palettes) :</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="obsolete">Dé&amp;filement :</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="obsolete">Docu&amp;ments récents :</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="obsolete">Chemins</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="obsolete">&amp;Documents :</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="obsolete">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="obsolete">Profils &amp;ICC :</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="obsolete">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="obsolete">&amp;Scripts :</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="obsolete">Parco&amp;urir...</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="obsolete">Modèl&amp;es de documents :</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Format de la page</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Taille :</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntation :</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="obsolete">Unités :</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Hauteur :</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Marges</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Enregistrement automatique</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Fréquence :</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="obsolete">Défaire/Refaire</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="obsolete">Longueur de l&apos;historique des actions</translation>
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
        <translation>Typographie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Outils</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Césure</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Polices</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Vérificateur</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Gestion des couleurs</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Exporter au format PDF</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Réglage du document</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Table des matières et index</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Raccourcis clavier</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Affichage de la page</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Couleur :</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">Afficher les zones &amp;non imprimables dans la couleur des marges</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Afficher les images</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Afficher les liens de cadres</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Afficher les caractères de contrôle du texte</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Afficher les cadres</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Guides relatifs à la page</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="obsolete">Espace de travail</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Haut :</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Gauche :</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Bas :</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Droite :</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Espacement entre les pages</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Horizontal : </translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Vertical :</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="obsolete">&amp;Réglage</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="obsolete">Pour régler l&apos;affichage, ajustez la règle ci-dessous à l&apos;aide du curseur.</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="obsolete">Interpréteur PostScript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="obsolete">&amp;Nom de l&apos;exécutable :</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="obsolete">Lisser les &amp;textes</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="obsolete">Lisser les objets &amp;graphiques</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation type="obsolete">ppp</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="obsolete">Résolution :</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="obsolete">Outil de retouche d&apos;image</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="obsolete">Nom de l&apos;&amp;exécutable :</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Outils externes</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="obsolete">Toujours demander avant de remplacer les polices à l&apos;ouverture d&apos;un document</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="obsolete">Aperçu du style de paragraphe courant lors de l&apos;édition des styles</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="obsolete">Afficher l&apos;écran de démarrage</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="obsolete">Faux texte</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="obsolete">Toujours utiliser le faux texte Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="obsolete">Nombre de paragraphes :</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Divers</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="obsolete">(Dés)activer l&apos;affichage des liens entre les cadres de texte</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Affiche les caractères invisibles tels que le symbole de paragraphe dans les cadres de texte</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">(Dés)activer l&apos;affichage des contours des cadres</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">(Dés)activer l&apos;affichage des images</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="obsolete">Sélectionne la langue utilisée par défaut par Scribus. Laisser vide pour choisir la langue définie par les variables d&apos;environnement. Il est toujours possible de modifier la langue avec les options de ligne de commande au démarrage de Scribus</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="obsolete">Choisissez l&apos;apparence générale de Scribus. Scribus utilise les thèmes KDE ou Qt installés, si Qt est configuré pour chercher les modules KDE.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="obsolete">Corps de la police pour les menus et les fenêtres</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="obsolete">Corps de la police pour les fenêtres d&apos;outils</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="obsolete">Unité de mesure par défaut pour l&apos;édition de document</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="obsolete">Nombre de lignes qui défilent pour chaque cran de molette de la souris</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="obsolete">Nombre de documents récemment ouverts à afficher dans le menu Fichier</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="obsolete">Dossier des documents</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="obsolete">Dossier par défaut des profils ICC. Ne peut être changé lorsqu&apos;un document estouvert. Par défaut, Scribus cherchera dans les dossiers systèmes sous Mac OS X et Windows. Sous Linux et Unix, Scribus cherchera dans les dossiers $home/.color/icc,/usr/share/color/icc·et·/usr/local/share/color/icc·</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="obsolete">Dossier par défaut pour les scripts</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="obsolete">Dossier alternatif pour le stockage des modèles</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="obsolete">Dimensions de page par défaut, soit standards soit personnalisées</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="obsolete">Orientation par défaut des pages du document</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Largeur des pages du document, modifiable si vous avez choisi des dimensions de page personnalisées</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Hauteur des pages du document, modifiable si vous avez choisi des dimensions de page personnalisées</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="obsolete">Si activé, Scribus effectue à intervalles réguliers une sauvegarde de sécurité de votre fichier avec l&apos;extension « .bak »</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="obsolete">Intervalle entre chaque sauvegarde automatique</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Définit la longueur de l&apos;historique d&apos;actions. Si égal à 0 un nombre infini d&apos;actions sera stocké.</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Couleur du papier</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Masquer la zone en-dehors des marges avec la couleur de celles-ci</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="obsolete">Définir le niveau de zoom par défaut</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="obsolete">Placez une règle sur votre écran et déplacez le curseur afin de définir le niveau de zoom permettant à Scribus d&apos;afficher les pages et les objets qu&apos;elles contiennent à la bonne dimension</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="obsolete">Lisser les textes pour l&apos;affichage EPS et PDF</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="obsolete">Lisser les objets graphiques pour l&apos;affichage EPS et PDF</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="obsolete">Chemins pour les éditeurs graphiques. Si vous utilisez GIMP et que votre distribution l&apos;inclut, nous vous recommandons « gimp-remote », qui permet d&apos;ouvrir les images dans votre session GIMP courante.</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Définit la quantité d&apos;espace disponible à gauche du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Définit la quantité d&apos;espace disponible à droite du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Définit la quantité d&apos;espace disponible en haut du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Définit la quantité d&apos;espace disponible en bas du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Sélectionnez un dossier</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation type="obsolete">Localisez GhostScript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="obsolete">Localisez votre éditeur d&apos;image</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter.</source>
        <comment>Fred</comment>
        <translation type="obsolete">Emplacement de l&apos;interpréteur GhostScript.</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">Album</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Imprimante</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>Save...</source>
        <translation type="obsolete">Enregistrer...</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>Dé&amp;fauts</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Enregistrer les préférences</translation>
    </message>
    <message>
        <source>Export...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">Appli&amp;quer</translation>
    </message>
    <message>
        <source>All preferences can be reset here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply all changes without closing the dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export current preferences into file</source>
        <translation type="unfinished"></translation>
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
        <translation>Migrer les anciens paramètres de Scribus ?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus a détecté un fichier de préférences pour la version 1.2.
Voulez-vous migrer les paramètres vers la nouvelle version ?</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Impossible d&apos;ouvrir le fichier de préférences &quot;%1&quot; en écriture : %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Echec de l&apos;écriture du fichier de préférences %1 : QIO Device status code %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Impossible d&apos;ouvrir le fichier de préférences &quot;%1&quot; : %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Impossible de lire les préférences XML de &quot;%1&quot; : %2 à la ligne %3, colonne %4</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>PostScript</translation>
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
        <source>Error Writing Preferences</source>
        <translation>Erreur d&apos;enregistrement des préférences</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus n&apos;a pas pu sauvegarder ses préférences : &lt;br&gt; %1&lt;br&gt;Vérifier les permissions du fichier et du dossier ainsi que la place disque disponible.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Erreur de chargement des préférences</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus n&apos;a pas pu charger ses préférences :&lt;br&gt;%1&lt;br&gt;Les paramètres par défaut vont être utilisés.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Périodique</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Nombre d&apos;or</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Neuf parties</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Vous pouvez choisir une maquette prédéfinie ici. « Aucune » laisse les marges telles quelles. « Gutenberg » règle des marges classiques. « Périodique » donne la même valeur à toutes les marges.</translation>
    </message>
    <message>
        <source>You can select a predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ouvrir...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Enregistrer &amp;sous...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Quitter</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Exécuter</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Lancer dans la &amp;console</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Enregistrer la sortie...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Console de scripts python</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Ceci est dérivé de la console Python standard et est de ce fait quelque peu limité. En particulier dans le cas d&apos;espaces blancs. Veuillez consulter le manuel de Scribus pour plus d&apos;information.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Console de script</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Taper les commandes ici. Une sélection est traitée comme un script</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Sortie de votre script </translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Scripts Python (*.py);;</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Ouvrir un fichier de script Python</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Sauvegarder les commandes Python dans un fichier</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Attention</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Fichiers texte (*.txt);;</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Enregistrer la sortie courante</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line: %1 Column: %2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Teint&amp;e :</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Saturation :</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>Va&amp;leur :</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rouge :</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Vert :</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Bleu :</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Canal a&amp;lpha (transparence) :</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Co&amp;uleurs de base</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Couleurs sur mesure</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Définir de nouvelles couleurs &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>A&amp;jouter aux couleurs</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Sélectionner la couleur</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copier ou déplacer un fichier</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Lecture : %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Écriture : %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Type :</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Dossier parent</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Corps</translation>
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
        <translation>Attributs</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Regarder &amp;dans :</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Retour</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Créer un nouveau dossier</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Liste</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Vue détaillée</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Aperçu des informations</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Aperçu du contenu</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lecture-écriture</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Lecture seule</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Écriture seule</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Inaccessible</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Lien symbolique vers un fichier</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Lien symbolique vers un dossier</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Lien symbolique vers un fichier spécial</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Dossier</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Caractère spécial</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Ouvrir</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Renommer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>R&amp;echarger</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Trier par &amp;nom</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Trier par &amp;taille</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Trier par &amp;date</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Non trié</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Trier</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Afficher les fichiers &amp;cachés</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>le fichier</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>le dossier</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>le lien symbolique</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Supprimer %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Voulez-vous vraiment supprimer %1 « %2 » ?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Oui</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Non</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nouveau dossier 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nouveau dossier</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nouveau dossier %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Rechercher un dossier</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Dossiers</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Fichier introuvable.
Vérifiez le chemin et le nom du fichier.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Sélectionnez un dossier</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Dossier :</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Polices</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>St&amp;yle de police</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Corps</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effets</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>&amp;Barré</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Souligné</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Couleur</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Exemple</translation>
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
        <translation>Appliquer</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Sélectionnez une police</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cou&amp;per</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Co&amp;ller</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Réorganiser</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Personnaliser...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;À propos de Qt&lt;/h3&gt;&lt;p&gt;Ce programme utilise Qt version %1.&lt;/p&gt;&lt;p&gt;Qt est un toolkit C++ pour le développement multi-plateformes d&amp;apos;interfaces graphiques et d&amp;apos;applications.&lt;/p&gt;&lt;p&gt;Qt fournit une compatibilité des sources entre les systèmes MS&amp;amp;nbsp;Windows, Mac&amp;amp;nbsp;OS&amp;amp;nbsp;X, Linux, ainsi que pour les principales variantes commerciales d&amp;apos;Unix.&lt;br&gt;Qt est également disponible pour les systèmes embarqués.&lt;/p&gt;&lt;p&gt;Qt est un produit Trolltech. Consultez &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; pour plus d&amp;apos;information.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copie #%1 de</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fond de page</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importation de texte</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Tous les formats pris en charge</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikans</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanais</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Basque</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgare</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation type="obsolete">Brésilien</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalan</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Chinois</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Croate</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tchèque</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danois</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Hollandais</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Anglais</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Anglais britannique</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Allemand</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Allemand (Trad.)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finnois</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Français</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galicien</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grec</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hongrois</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonésien</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italien</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Coréen</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lithuanien</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Luxembourgeois</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation type="obsolete">Norvégien bokmål</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norvégien nynorsk</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norvégien</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polonais</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugais</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugais (BR)</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russe</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Suédois</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Espagnol</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Espagnol (hispano-américain)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovaque</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovène</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbe</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Thaïlandais</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turc</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrainien</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Gallois</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Plantage de Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Plantage de Scribus à cause du signal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Plantage de Scribus à cause de l&apos;exception %1</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Page</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Gabarit</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
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
        <translation>DLE (enveloppe)</translation>
    </message>
    <message>
        <source>Could not open output file %1</source>
        <translation>Impossible d&apos;ouvrir le fichier : %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Le flux de sortie n&apos;est pas accessible en écriture</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>La vérification des paramètres a échoué : %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Impossible d&apos;ouvrir le fichier d&apos;entrée %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Impossible de lire les paramètres XML :</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1·(ligne·%2·colonne·%3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Impossible de lire les paramètres XML : %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>noeud racine vide</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt;·invalide</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>%1 &lt;%2&gt; noeuds trouvés, 1 nécessaire.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>noeud vide &lt;%2&gt; inattendu</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>le noeud &lt;%1&gt; n&apos;est pas un élément</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>l&apos;élément &lt;%1&gt; n&apos;a pas d&apos;attribut &apos;valeur&apos;</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>la valeur de l&apos;élément &lt;%1&gt; doit être &apos;vrai&apos; ou &apos;faux&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>l&apos;élement &lt;lpiSettingsEntry&gt; n&apos;a pas d&apos;attribut &apos;nom&apos;</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tout</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Export du fichier PostScript</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Impression du fichier</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Noir</translation>
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
        <translation>Jaune</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Bibliothèque Freetype2 non disponible</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation type="obsolete">La police %1 est défectueuse, elle sera ignorée</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>La police %1 est défectueuse (flux de lecture), elle sera ignorée</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation type="obsolete">La police %1 est défectueuse (Freetype2), elle sera ignorée</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation type="unfinished">La police %1 est défectueuse, elle sera ignorée</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>La police %1 possède un caractère défectueux, %2 (code de caractère %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="obsolete">La police %1 est défectueuse, elle sera ignorée</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Impossible de lire la police %1. Elle ne sera pas incorporée</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>La police %1 est défectueuse, elle sera ignorée</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Création du cache de polices</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Impossible de charger la police %1 - format de police inconnu</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Nouvelle police trouvée, vérification...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Police modifiée trouvée, vérification...</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Police %1 chargée depuis %2 (%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>La police %1 (%2) est un doublon de %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Chargement de la police %1 (trouvée via fontconfig)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Impossible de charger une police - freetype2 est incapable de trouver le fichier de police</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Lecture du cache de polices</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Enregistrement du cache de polices mis à jour</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Recherche des polices</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="obsolete">La police %1 est défectueuse (Freetype), elle sera ignorée</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="obsolete">La police %1 possède un caractère défectueux, %2 (code de caractère %3), elle sera ignorée</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>extraction du caractère %1 de la police %2 (décalage=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>entête memcpy : %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tableau &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>tableau memcpy : %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>décalage memcpy : %1 %2 %3</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Initialisation...</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Version de développement de Scribus</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="obsolete">Vous utilisez actuellement une version de développement de Scribus 1.3.x Le document sur lequel vous travaillez a été créé avec Scribus 1.2.3 ou antérieur. La sauvegarde de ce document va le rendre incompatible avec Scribus 1.2.3 à moins d&apos;utiliser le menu Fichier-&gt;Enregistrer sous. Êtes-vous sûr de vouloir continuer cette opération ?</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Vous essayez d&apos;importer plus de pages que disponibles dans le document, de la page courante à la fin.&lt;/p&gt;Les choix possibles sont :&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Créer&lt;/b&gt; les pages manquantes&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importer&lt;/b&gt; les pages jusqu&apos;à la dernière page du document&lt;/li&gt;&lt;li&gt;&lt;b&gt;Annuler&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>C&amp;réer</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Les changements dans votre document n&apos;ont pas été enregistrés et vous souhaitez revenir à la version enregistrée. Souhaitez-vous continuer ?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
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
        <translation> po</translation>
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
        <source> c</source>
        <translation> c</translation>
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
        <translation>po</translation>
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
        <translation>Points (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Millimètres (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Pouces (po)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimètres (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicéro (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Fichier existant</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Le fichier « %1 » existe déjà. &lt;br/&gt;Voulez-vous vraiment le remplacer par le fichier courant ?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>Substitue&amp;r</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>page</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Créateur de codes à barres</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Cercle chromatique</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Aperçu des polices disponibles</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Mon extension</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Nouveau d&apos;après modèle</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Modèle de document :</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Lettres d&apos;information</translation>
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
        <translation>Prospectus</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Enseignes</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Cartes</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>En-têtes de lettre</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Enveloppes</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Cartes de visite</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Calendriers</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Publicités</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Étiquettes</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menus</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programmes</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Formulaires PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Présentations PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Périodiques</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Affiches</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Faire-parts</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Documents texte</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Dépliants</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Jaquettes</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Modèles personnels</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation type="obsolete">Exporter en tant qu&apos;image</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="obsolete">Enregistrer en tant qu&apos;image</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="obsolete">Erreur d&apos;écriture du(des) fichier(s) de sortie.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="obsolete">Exportation réussie.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="obsolete">Fichier existant. Remplacer ?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="obsolete">existe déjà. Remplacer ?</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Oui</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Non</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="obsolete">Oui, tous</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>Import PS/EPS</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Tous les formats pris en charge (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Enregistrer en tant que modèle</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;obtenir une couleur sans son nom.</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Couleur introuvable.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Impossible de modifier une couleur sans nom.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Couleur introuvable dans le document.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Couleur introuvable dans les couleurs d&apos;origine.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Impossible de créer une couleur sans nom.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;effacer une couleur sans nom.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Impossible de remplacer une couleur sans nom.</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder est plus gros que ce qui est permis.</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;ouvrir le document</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;enregistrer le document</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Unité hors plage. Utiliser une des constantes scribus.UNIT_*.</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Couleur introuvable - erreur Python</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>L&apos;argument doit être un nom d&apos;objet ou une instance PyCObject</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Propriété introuvable</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Enfant introuvable</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Impossible de convertir le résultat de type &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Le type de propriété &apos;%1&apos; n&apos;est pas pris en charge</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Impossible de convertir &apos;%1&apos; en type de propriété &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Les types correspondent, mais impossible de positionner la propriété.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>La cible n&apos;est pas un cadre d&apos;image.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>L&apos;objet spécifié n&apos;est pas un cadre d&apos;image.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Impossible de grouper moins de deux objets</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Impossible de grouper moins de deux objets</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Une sélection ou une liste d&apos;objets est requise pour grouper</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Impossible de redimensionner de 0%.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Police introuvable.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;afficher le rendu d&apos;un échantillon vide.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Impossible d&apos;enregistrer l&apos;image pixmap.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;avoir un calque sans nom.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Calque introuvable.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;effacer le dernier calque.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Impossible de créer un calque sans nom.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Un objet de même nom existe déjà.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>La liste de points doit contenir au moins deux points (quatre valeurs).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>La liste de points doit contenir un nombre pair de valeurs.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>La liste de points doit contenir au moins trois points (six valeurs).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>La liste de points doit contenir au moins quatre points (huit valeurs).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>La liste de points doit être composée d&apos;un multiple de six valeurs.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objet introuvable.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Style introuvable.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Un style ne peut être affecté qu&apos;à un cadre de texte.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;enregistrer le fichier EPS</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Numéro de page hors plage</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>L&apos;argument n&apos;est pas une liste : il doit être une liste de nombres décimaux</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>L&apos;argument contient des valeurs non-numériques : il doit être une liste de nombres décimaux.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>L&apos;argument contient des valeurs non-numériques : il doit être une liste de nombres décimaux.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Épaisseur de filet hors limites. La valeur doit être comprise entre 0 et 12 inclus.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Teinte de contour hors limites. La valeur doit être comprise entre 0 et 100 inclus.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Teinte de remplissage hors limites. La valeur doit être comprise entre 0 et 100 inclus.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Le rayon d&apos;angle doit être un nombre positif.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Style de filet introuvable.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un corps de police ne peut être obtenu que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un nom de police ne peut être obtenu que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Une taille de texte ne peut être obtenue que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un nombre de colonnes ne peut être obtenu que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un interlignage ne peut être obtenu que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un espacement de colonnes ne peut être obtenu que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un texte ne peut être obtenu que d&apos;un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Un texte ne peut être défini que dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Un texte ne peut être inséré que dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Insertion d&apos;index hors limites.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Alignement hors plage. Utiliser une des constantes scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>L&apos;alignement d&apos;un texte ne peut être effectué que dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Corps de police hors limites. La valeur doit être comprise entre 1 et 512 inclus.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Un corps de police ne peut être affecté qu&apos;à un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Une police ne peut être affectée qu&apos;à un cadre de texte.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Interlignage hors limites. La valeur doit être supérieure ou égale à 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Un interlignage ne peut être défini que dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Espacement de colonnes hors limites. La valeur doit être positive.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Un espacement de colonnes ne peut être défini que dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Nombre de colonnes hors limites. La valeur doit être supérieure à 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Un nombre de colonnes ne peut être défini que dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Index de la sélection hors limites</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Un texte peut être sélectionné seulement dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Un texte peut être effacé seulement dans un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Une couleur de fond peut être affectée seulement à un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Une couleur de contour peut être affectée seulement à un cadre de texte.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Une teinte de texte peut être affectée seulement à un cadre de texte.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Seuls les cadres de texte peuvent être liés.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Le cadre ciblé doit être vide.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Le cadre ciblé est lié à un autre cadre.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Le cadre ciblé est lié par un autre cadre.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Source et cible sont les mêmes.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Seul un cadre de texte peut être délié.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>L&apos;objet n&apos;est pas un cadre de texte lié, impossible de le délier.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>L&apos;objet est le dernier d&apos;une série de cadres liés, impossible de le délier. Déliez plutôt le cadre précédent.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Seul un cadre de texte peut être converti en contours.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>La vérification du débordement de texte ne peut avoir lieu que dans un cadre de texte</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Un signet ne peut être défini que dans un cadre de texte</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Des informations ne peuvent être obtenues que d&apos;un cadre de texte</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>La nom de fichier doit être une chaîne de caractères.</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>La nom de fichier ne peut être vide.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;effacer les paramètres de type d&apos;image.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Le type d&apos;image doit être une chaîne de caractères.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>L&apos;attribut &apos;allTypes&apos; est LECTURE SEULE.</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Impossible d&apos;exporter l&apos;image</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Démarrer un script...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Afficher la console</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>À propos du scr&amp;ipt...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scripts Scribus</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Scripts &amp;récents</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>À propos du script</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Moteur de script</translation>
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
        <translation>Module d&apos;interfaçage Python pour Scribus
Ce module est l&apos;interface Python pour Scribus. Il fournit des fonctions
permettant de contrôler Scribus et de manipuler des objets sur le canevas.
Chaque fonction est documentée individuellement ci-dessous.

Quelques éléments sont communs à la plupart des fonctions.

La plupart des fonctions opèrent sur des cadres. Ceux-ci sont identifiés par
leur nom ou une chaîne, et ne sont pas de réels objets Python. De nombreuses
fonctions acceptent comme argument facultatif le nom d&apos;un cadre.
Certaines exceptions sont également communes à une majorité de fonctions,
et ne sont pas encore documentées via docstring.

- De nombreuses fonctions entraîneront une erreur NoDocOpenError si elles sont
appelées sans un document sur lequel opérer.
- Si vous ne passez pas de nom de cadre à une fonction qui en attend un,
celle-ci utilisera le cadre actuellement sélectionné, s&apos;il y en a un. Sinon,
elle renverra une erreur NoValidObjectError si elle ne trouve rien sur
quoi opérer.
- De nombreuses fonctions renverront une erreur WrongFrameTypeError si vous
tentez de les utiliser sur un type de cadre non adapté. Par exemple,
changer la couleur du texte sur un cadre d&apos;image est un non-sens, et par
conséquent déclenchera cette erreur.
- Les erreurs résultant d&apos;appels à des fonctions de l&apos;API Python seront
remontées telles quelles. De fait, la liste des exceptions renvoyées par les
fonctions fournies ici et dans le docstring est incomplète.

Le détail des exceptions que chaque fonction est susceptible de déclencher
est fourni dans la documentation de ladite fonction.
</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Configuration personnalisée (facultatif) :</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Configuration standard :</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Espaces insécables</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Traitement des espaces insécables en cours...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Traitement des espaces insécables terminé.</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>Exporter en SVG</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Fichiers SVG (*.svg *.svgz);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Fichiers SVG (*.svg);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Attention</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Voulez-vous vraiment remplacer le fichier :
%1 ?</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>Import SVG</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Support de l&apos;ancien format .sla</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Filtre d&apos;import OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>Illustrations OpenOffice.org (*.sxd);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Fichiers CSV</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>Données CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>En-tête CSV</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Documents Word</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Fichiers HTML</translation>
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
Liens externes
</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Documents texte OpenDocument</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Documents Palm PDB</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>Données PDB</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>Import PDB</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Impossible d&apos;ouvrir le fichier %1</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="obsolete">Ce fichier n&apos;est probablement reconnu comme étant un document PDB. Veuillez s.v.p. rapporter ceci comme un bogue si vous êtes sûr que c&apos;en est un.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Documents OpenOffice.org</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Fichiers texte avec filtrage</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Fichiers texte</translation>
    </message>
    <message>
        <source>Japanese</source>
        <comment>Fred</comment>
        <translation type="obsolete">Japonais</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation type="unfinished">Japonais</translation>
    </message>
    <message>
        <source>Font %1(%2) is broken</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation type="unfinished">Arabe</translation>
    </message>
    <message>
        <source>Dzongkha</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>font %1 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>size %1 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+style </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+color </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+underline </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-underline </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+strikeout </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-strikeout </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+shadow </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-shadow </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+outline </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-outline </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-tracking </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+stretch </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>parent= %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian (Bokm&#xc3;&#xa5;l)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Romanian</source>
        <translation type="unfinished"></translation>
    </message>
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
        <translation type="unfinished">Lettre</translation>
    </message>
    <message>
        <source>Govt. Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Légal</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Moyenne</translation>
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
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blendmode out of bounds, must be 0 &lt;= blendmode &lt;= 15.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus 1.2.x Support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus 1.3.4 Support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get number of lines of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+tracking %1 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+baseline %1 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Breton</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>English (American)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Font(s):
%1 are not available.
They have been replaced by &quot;Courier&quot;
Therefore the image may be not correct</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>English (Australian)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.epsi *.EPSI *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation type="unfinished">Hébreu</translation>
    </message>
    <message>
        <source>Scribus 1.3.0-&gt;1.3.3.7 Support</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cou&amp;per</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Co&amp;ller</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menu système</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Teinte</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Dérouler</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normaliser</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimiser</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximiser</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurer</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Déplacer</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Corps</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimiser</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximiser</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Toujours au premier plan</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimiser</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Réduire</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Enrouler</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Dérouler</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Réglage du document</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Format de la page</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Taille :</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntation :</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Hauteur :</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="obsolete">&amp;Unité :</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="obsolete">Appliquer les dimensions à toutes les pages</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Marges</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Enregistrement automatique</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Fréquence :</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Document</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Informations sur le document</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guides</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Affichage de la page</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Couleur :</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">Afficher les zones &amp;non imprimables dans la couleur des marges</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Afficher les images</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Afficher les liens de cadres</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Afficher les caractères de contrôle du texte</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Afficher les cadres</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Guides relatifs à la page</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="obsolete">Espace de travail minimum</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Haut :</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Gauche :</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Bas :</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Droite :</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Espacement entre les pages</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Horizontal : </translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Vertical :</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typographie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Outils</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Césure</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Polices</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Vérificateur</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Exporter au format PDF</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Réglage du document</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Table des matières et index</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Sections</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Gestion des couleurs</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="obsolete">(Dés)activer l&apos;affichage des liens entre cadres de texte</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Affiche les caractères invisibles tels que le symbole de paragraphe dans les cadres de texte</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">(Dés)activer l&apos;affichage des contours des cadres</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">(Dés)activer l&apos;affichage des images</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Couleur du papier</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Masquer la zone en-dehors des marges avec la couleur de celles-ci</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="obsolete">Appliquer les changements de dimensions à toutes les pages du document</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Ajustement des couleurs</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Scripts Python (*.py);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Exécuter comme script d&apos;extension</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY);; All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMAlignSelect</name>
    <message>
        <source>P</source>
        <comment>P as in Parent</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use parent style&apos;s alignment instead of overriding it</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Gestionnaire de styles</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation type="obsolete">Colonne 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation type="obsolete">C&amp;loner</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation type="obsolete">O&amp;K</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation type="obsolete">A&amp;ppliquer</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="obsolete">Alt+P</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation type="obsolete">An&amp;nuler</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">Appli&amp;quer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importer</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>Please select a unique name for the style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;&lt; &amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Remise à zéro</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMCStylePage</name>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Parent style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font face</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tracking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Stroke color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Stroke shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Teinte</translation>
    </message>
</context>
<context>
    <name>SMCharacterStyle</name>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Propriétés</translation>
    </message>
    <message>
        <source>Character Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Nouveau style</translation>
    </message>
    <message>
        <source>Clone of %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 (%2)</source>
        <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMColorCombo</name>
    <message>
        <source>Use Parent Value</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMFontComboH</name>
    <message>
        <source>Use Parent Font</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="obsolete">Barres</translation>
    </message>
    <message>
        <source>Line Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Nouveau style</translation>
    </message>
    <message>
        <source>Clone of %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 (%2)</source>
        <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished">ligne continue</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">Ligne tiretée</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">Ligne pointillée</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">Ligne mixte trait-point</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">Ligne mixte trait-point-point</translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="unfinished"> pt </translation>
    </message>
</context>
<context>
    <name>SMPStyleWidget</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished">Espacement fixe entre les lignes</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished">Aligner le texte sur la grille typographique</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distances and Alignment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation type="unfinished">Lettrines</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabulations et retrait</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Propriétés</translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="unfinished">&amp;Lignes :</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation type="unfinished">Distance au texte :</translation>
    </message>
    <message>
        <source>Based on</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Parent style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line spacing mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space above</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable or disable drop cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drop cap lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drop cap offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alignment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First line indent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left indent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right indent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabulators</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMParagraphStyle</name>
    <message>
        <source>Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation type="unfinished">Style de paragraphe</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Nouveau style</translation>
    </message>
    <message>
        <source>Clone of %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 (%2)</source>
        <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMReplaceDia</name>
    <message>
        <source>Remove</source>
        <translation type="unfinished">Effacer</translation>
    </message>
    <message>
        <source>Replace with</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMReplaceDiaBase</name>
    <message>
        <source>Delete Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation type="unfinished">An&amp;nuler</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished">Alt+N</translation>
    </message>
</context>
<context>
    <name>SMRowWidget</name>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Aucun style</translation>
    </message>
</context>
<context>
    <name>SMScComboBox</name>
    <message>
        <source>Use Parent Value</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMShadeButton</name>
    <message>
        <source>Use Parent Value</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMStyleSelect</name>
    <message>
        <source>P</source>
        <comment>P as in Parent</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use parent style&apos;s effects instead of overriding them</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMTabruler</name>
    <message>
        <source> Parent Tabs </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Paramètres de paragraphe</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Style du paragraphe courant</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Paramètres de couleur du texte</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Couleur du texte</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturation de la couleur du texte</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Paramètres de couleur des contours du texte</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Couleur des contours du texte</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturation de la couleur des contours du texte</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Paramètres des polices</translation>
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
        <source>Font of selected text</source>
        <translation>Police du texte sélectionné</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Corps</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Mise à l&apos;échelle de la largeur des caractères</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Mise à l&apos;échelle de la hauteur des caractères</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Paramètres des caractères</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Réglage de l&apos;approche</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Enregistrer la page en &amp;SVG...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Exporte des fichiers SVG</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Exporte la page courante dans un fichier SVG.</translation>
    </message>
    <message>
        <source>Save as &amp;SVG...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importer un fichier &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importe les fichiers SVG</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importe la plupart des fichiers SVG dans le document courant,
convertissant les données vectorielles en objets Scribus.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>Le fichier SVG contient des fonctionnalités qui ne sont pas prises en charge</translation>
    </message>
</context>
<context>
    <name>SVGPlug</name>
    <message>
        <source>Group%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Espaces insécables</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Appliquer les espaces insécables sur :</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>Cadres &amp;sélectionnés</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Page courante</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Tous les objets</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Seuls les cadres sélectionnés ont été traités.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Seul la page courante a été traitée.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Tous les objets du document ont été traités.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Paramètres utilisateur</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Configuration système</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Enregistrer</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Remise à zéro</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Enregistrer la configuration utilisateur</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Recharge les paramètres par défaut et supprime ceux définis par l&apos;utilisateur</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Modifiez selon vos besoin. Si vous sauvegardez vos paramètres, ceux-ci auront préséance sur les paramètres par défaut du système</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Espaces insécables</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Des paramètres sur mesure existent déjà. Désirez-vous les remplacer ?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Impossible d&apos;enregistrer le fichier %1</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Paramètres utilisateur enregistrés</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Configuration système rechargée</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Impossible d&apos;ouvrir le fichier %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Enregistrer en tant que &amp;modèle...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Enregistrer un document en tant que modèle</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Sauve un document comme modèle. C&apos;est un bon moyen de faciliter la création de documents avec un style constant</translation>
    </message>
</context>
<context>
    <name>ScGTFileDialog</name>
    <message>
        <source>Select a file to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Append</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show options</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScInputDialog</name>
    <message>
        <source>Input Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>InputDialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Annuler</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Charger/Enregistrer/Importer/Exporter</translation>
    </message>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Persistant</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Action</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
</context>
<context>
    <name>ScProgressBar</name>
    <message>
        <source>%1 of %2</source>
        <translation type="unfinished">%1 sur %2</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Haut</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Droite</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Bas</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Gauche</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Permettre l&apos;arrimage à...</translation>
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
        <source>Floating Orientation...</source>
        <translation>Orientation...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Impression en cours...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Copie #%1 de</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Fond de page</translation>
    </message>
</context>
<context>
    <name>Scribus12Format</name>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished">Document Scribus 1.2.x</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Fond de page</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Copie #%1 de</translation>
    </message>
</context>
<context>
    <name>Scribus134Format</name>
    <message>
        <source>Scribus 1.3.4 Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Copie #%1 de</translation>
    </message>
</context>
<context>
    <name>Scribus13Format</name>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Copie #%1 de</translation>
    </message>
    <message>
        <source>Scribus 1.3.0-&gt;1.3.3.7 Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation type="obsolete">Couleurs du document</translation>
    </message>
</context>
<context>
    <name>ScribusCore</name>
    <message>
        <source>Initializing Plugins</source>
        <translation type="unfinished">Recherche des extensions (plug-ins)</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished">Configuration des raccourcis clavier</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="unfinished">Lecture des préférences</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished">Chargement des profils ICC</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Recherche des polices</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished">Il n&apos;y a pas de polices convenablement installées sur votre système</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="unfinished">Quitter maintenant</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="unfinished">Erreur fatale</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished">Gestion des polices activée</translation>
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
        <translation>Fond de page</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nouveau calque</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Effacer l&apos;intégralité du texte ?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Impossible de supprimer un objet en cours d&apos;utilisation</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>L&apos;objet %1 est en cours d&apos;édition dans l&apos;éditeur. L&apos;opération de suppression est annulée</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished">Un ou des objets sont verrouillés.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished">To&amp;ut déverrouiller</translation>
    </message>
    <message>
        <source>&amp;Skip locked objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished">Nombre d&apos;exemplaires : %1
Décalage horizontal : %2
Décalage vertical : %3</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Ajustement des couleurs</translation>
    </message>
    <message>
        <source>Default Paragraph Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default Character Style</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Recherche des extensions (plug-ins)</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="obsolete">Configuration des raccourcis clavier</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Lecture des préférences</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Initialisation de l&apos;éditeur interne</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">Chargement des profils ICC</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Initialisation du module de césure</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Lecture de l&apos;album</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Activation des raccourcis clavier</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Recherche des polices</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="obsolete">Il n&apos;y a pas de polices convenablement installées sur votre système</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="obsolete">Quitter maintenant</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Erreur fatale</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Gestion des polices activée</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Documents &amp;récents</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>E&amp;xporter</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>St&amp;yle</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Couleur</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Corps</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Tei&amp;nte</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Polices</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effets</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objet</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Paramètres de l&apos;aperçu</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Disposition</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Mettre sur le cal&amp;que</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Options PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>Forme (&amp;S)</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>Conve&amp;rtir en</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Insérer</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Caractère spécial</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Citation</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="obsolete">Espaces et sauts</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation type="obsolete">Ligatures</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Page</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Affichage</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Fe&amp;nêtres</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>A&amp;ide</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Alignement</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Prêt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importation des pages...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importer une(des) page(s)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importation réussie</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Aucun objet trouvé pour l&apos;importation</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Le fichier %1 n&apos;est pas au format Scribus</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Certains profils ICC utilisés dans ce document ne sont pas installés :</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>a été remplacé par :</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(converti)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Tous les formats pris en charge</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Impossible d&apos;enregistrer le fichier : 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Documents (*.sla *.sla.gz *.scd *.scd.gz);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Documents (*.sla *.scd);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Enregistrement...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Des erreurs ont été trouvées. Utilisez le vérificateur afin de les corriger</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorer</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Impression en cours...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Document</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Impossible d&apos;imprimer !</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Impossible de couper un objet utilisé</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>L&apos;objet %1 est en cours d&apos;édition dans l&apos;éditeur interne. L&apos;opération &apos;couper&apos; va être annulée</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>À propos de Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Manuel de Scribus</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Fichiers texte (*.txt);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Convertir la page en gabarit</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nouveau gabarit %1</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Taille :</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Corps</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Teinte :</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Teinte</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Aucun style</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="obsolete">Nombre d&apos;exemplaires : %1
Décalage horizontal : %2
Décalage vertical : %3</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Le programme suivant est absent :</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript : vous ne pouvez pas utiliser d&apos;images EPS ni l&apos;aperçu</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript : vous ne pouvez pas utiliser d&apos;images EPS ni l&apos;aperçu d&apos;impression PostScript</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript est introuvable : l&apos;aperçu d&apos;impression PostScript n&apos;est pas disponible.</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tout</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Des erreurs ont été trouvées.
Utilisez le vérificateur afin de les corriger</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>Fichiers EPS (*.eps);;</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Des erreurs ont été trouvées.
Utilisez le vérificateur afin de les corriger</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>- Page %1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Un ou des objets sont verrouillés.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>Tout &amp;verrouiller</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>To&amp;ut déverrouiller</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Le programme %1 est déjà en cours de fonctionnement !</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Le programme %1 est absent !</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>La couleur choisie n&apos;existe pas dans le jeu de couleurs du document. Entrez un nom pour cette nouvelle couleur</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Couleur introuvable</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Le nom que vous avez choisi existe déjà. Entrez un nouveau nom pour cette nouvelle couleur</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>Disposition</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Mettre sur le calque</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Paramètres de l&apos;aperçu</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Espaces et sauts</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Ou&amp;tils</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X :</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y :</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Voulez-vous vraiment remplacer l&apos;image actuelle ?</translation>
    </message>
    <message>
        <source>Contents</source>
        <comment>Fred</comment>
        <translation type="obsolete">Contenu</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation type="unfinished">Contenu</translation>
    </message>
    <message>
        <source>&amp;Character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;paces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Updating Pictures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished">Effacer l&apos;intégralité du texte ?</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Argument invalide :</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Le fichier %1 n&apos;existe pas, annulation.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Usage:·scribus·[option·...·]·[fichier]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Affiche l&apos;aide (ce message) et quitte</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Utilise xx comme raccourci pour une langue, par ex. `en&apos; ou `fr&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Affiche les langues disponibles pour l&apos;interface</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Afficher les informations sur la console lors du chargement des polices</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Ne pas afficher l&apos;écran de démarrage</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Affiche la version et quitte</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Organise les boutons de dialogues de droite à gauche (par ex. Annuler/Non/Oui au lieu de Oui/Non/Annuler)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>Nom du fichier</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Utiliser le nom du fichier comme chemin pour les préférences données d&apos;un utilisateur</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Les langues disponibles pour l&apos;interface de Scribus sont les suivantes :</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Pour changer la langue par défaut :</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus·-l·xx·ou scribus·--lang·xx,·où xx est la langue choisie</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Version de Scribus</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus : la PAO à code source libre</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Site officiel</translation>
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
        <translation>Problèmes</translation>
    </message>
    <message>
        <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Download a file from the Scribus website and show the latest available version.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show location ICC profile information on console while starting</source>
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
        <source>Layer</source>
        <translation type="obsolete">Calque</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Copier ici</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Déplacer ici</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Co&amp;ller</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Image</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fichier : </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Résolution d&apos;origine : </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Résolution actuelle : </translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Espace colorimétrique :</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>RVB</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMJN</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Niveaux de gris</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Texte lié</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Cadre de texte</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Joindre le texte au tracé</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragraphes : </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Mots : </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Lettres et espaces : </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Imprimer : </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Paramètres de l&apos;aperçu</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Options PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Mettre sur le cal&amp;que</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Disposi&amp;tion</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Conve&amp;rtir en</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Lier les cadres de texte</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Vous tentez de créer un lien vers un cadre contenant déjà du texte ou de lier un cadre à lui-même.</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Pages %1 à %2</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Impossible de convertir un objet en cours d&apos;utilisation</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>L&apos;objet %1 est en cours d&apos;édition dans l&apos;éditeur interne. L&apos;opération de conversion en contours va être annulée</translation>
    </message>
    <message>
        <source>Contents</source>
        <comment>Fred</comment>
        <translation type="obsolete">Contenu</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation type="unfinished">Contenu</translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lines: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables the Preview Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here you can select the visual appearance of the display
You can choose between normal and several color blindness forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation type="obsolete">Vision normale</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Cécité au rouge</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Cécité au vert</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Cécité au bleu</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Daltonisme</translation>
    </message>
    <message>
        <source>Switches Color Management on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CMS is active. Therefore the color display may not match the perception by visually impaired</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter Object Size</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>Document :</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>a été modifié depuis la dernière sauvegarde.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Fermer sans sauvegarder</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Support incorporé des scripts Python </translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Moteur de script</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Erreur d&apos;exécution du script</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Si vous utilisez un script officiel, merci de le signaler à &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Ce message est présent dans le presse-papiers. Utiliser Ctrl+V pour le copier dans l&apos;outil de suivi des bogues.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Erreur interne lors du traitement de la commande entrée. Les détails sontaffichés sur stderr.</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Examiner le script</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="obsolete">Scripts Python (*.py);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Documentation pour :</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Script :</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> n&apos;est pas documenté!</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>L&apos;extension Python ne s&apos;est pas chargée correctement. Plus de précisions dans stderr. </translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Extensions</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Console</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <source>Scripter Preferences</source>
        <translation>Préférences de l&apos;outil de script</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Activer les scripts d&apos;extension</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Script de démarrage :</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Erreurs :</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Commentaires :</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Mots clés :</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Signes :</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Nombres :</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Chaîne de caractères :</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Textes de base :</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Sélectionner la couleur</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Localisez le Script de démarrage :</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Aperçu de la page</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Rechercher-Substituer</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Rechercher :</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Style de paragraphe</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Police</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Corps</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Graisses et styles</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Couleur de fond</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Teinte de fond</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Couleur de contour</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Teinte de contour</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Gauche</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centré</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Droite</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Justifié</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Forcé</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Substituer par :</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Mot entier</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorer la casse</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Re&amp;chercher</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>Substitue&amp;r</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>&amp;Tout substituer</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Recherche terminée</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Recherche terminée, trouvé %1 occurrences</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Sélectionner les champs</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Champs disponibles</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;Incorporer</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation>E&amp;xclure</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Champs sélectionnés</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Autre...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Teinte :</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Teinte</translation>
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
        <translation>Décalage horizontal</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Décalage vertical</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>Espaces &amp;insécables...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Espaces insécables</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortcutWidget</name>
    <message>
        <source>&amp;No Key</source>
        <translation type="unfinished">&amp;Pas de raccourci</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">Racco&amp;urci personnalisé</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished">Alt+Majuscule+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">&amp;Définir le raccourci</translation>
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
        <translation type="unfinished">Majuscule</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="unfinished">Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished">Meta+</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="unfinished">Majuscule+</translation>
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
        <source>No shortcut for the style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style has user defined shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Assign a shortcut for the style</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Aucun style</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Édition des styles...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Aucun style</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Édition des styles</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Dupliquer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copie de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nouveau style</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documents (*.sla *.scd);;Tous les fichiers (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>É&amp;dition</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insérer</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Caractère spécial</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Citation</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Espaces et sauts</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatures</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Configuration</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Éditeur interne</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Effacer tout le texte</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Recharger le texte depuis le cadre</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Enregistrer vers...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Ouvrir un fichier...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Enregistrer le &amp;document</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Actualiser le cadre de texte et fermer</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Fermer sans actualiser le cadre de texte</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;Tout sélectionner</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cou&amp;per</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Co&amp;ller</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Rechercher-&amp;Substituer...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Édition des st&amp;yles...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Aperçu des &amp;polices disponibles...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Mettre à jour le cadre de texte</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Espaces et sauts</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Insérer un caractère...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Fond de page...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Police d&apos;affichage...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Sélection rapide</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Paragraphe courant :</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Mots : </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Lettres et espaces : </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Total :</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragraphes : </translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Éditeur interne - %1</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Enregistrer les modifications ?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Abandonner l&apos;intégralité des modifications ?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Effacer l&apos;intégralité du texte ?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Fichiers texte (*.txt);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Automatique</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Déplacement</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Épaisseur</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation type="obsolete">Plus d&apos;un objet sélectionné</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Nom :</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Remise à zéro</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">Appli&amp;quer</translation>
    </message>
    <message>
        <source>&lt;&lt; &amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit &gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Nouveau</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">S&amp;upprimer</translation>
    </message>
    <message>
        <source>Reset all changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply all changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply all changes and exit edit mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create a new style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import styles from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clone selected style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete selected styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New</source>
        <translation type="unfinished">Nouvelle</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to Scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="unfinished">Supprimer</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">É&amp;dition</translation>
    </message>
    <message>
        <source>&amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nom</translation>
    </message>
    <message>
        <source>Edit styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the selected style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="unfinished">Modifier</translation>
    </message>
    <message>
        <source>New %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished">Raccourci déjà utilisé</translation>
    </message>
    <message>
        <source>More than one style selected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Souligné</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation type="obsolete">Mots Soulignés</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Majuscules</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Petites capitales</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Indice</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Exposant</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation type="obsolete">Barré</translation>
    </message>
    <message>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation type="obsolete">Contour</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation type="obsolete">Ombré</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <comment>Text Style Selector</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Options d&apos;importation des fichiers OpenOffice.org</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Écraser les Styles de paragraphe</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Activer ceci remplacera les styles présents dans le document courant</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Fusionner les styles de paragraphe</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Fusionner les styles de paragraphe par attributs. Le résultat est un plus faible nombre de styles de paragraphe semblables, tout en conservant les attributs des styles, même si les styles du document d&apos;origine sont nommés différement.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Utiliser le nom du document comme préfixe pour les styles de paragraphe</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Ajouter le nom du document devant le nom du style de paragraphe.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ne plus demander</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Faire de ces paramètres ceux par défaut et ne plus demander à la prochaine importation d&apos;un document au format OpenOffice.org 1.x</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Au début</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>A la fin</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Non affiché</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Table des matières et index</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Table des matières</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Le cadre dans lequel la table des matières doit être placée</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Numéros de pages situés :</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Nom de l&apos;attribut :</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Cet attribut sera appliqué aux cadres que vous utiliserez comme base pour la création des entrées</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Positionne les numéros de pages des entrées au début ou à la fin de la ligne, ou pas du tout</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Affiche les entrées non imprimables</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Inclure également les cadres non imprimables</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Le style de paragraphe qui sera appliqué aux entrées</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Style de paragraphe :</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Cadre de destination :</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="obsolete">Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Table des matières et index</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Table des matières</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Ajo&amp;uter</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Le cadre dans lequel la table des matières doit être placée</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Numéros de pages situés :</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Nom de l&apos;attribut :</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Cet attribut sera appliqué aux cadres que vous utiliserez comme base pour la création des entrées</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Positionne les numéros de pages des entrées au début ou à la fin de la ligne, ou pas du tout</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Affiche les entrées non imprimables</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Inclure également les cadres non imprimables</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Le style de paragraphe qui sera appliqué aux entrées</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Style de paragraphe :</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Cadre de destination :</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="obsolete">Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Ignorer toutes les erreurs</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Vérification automatique avant impression ou exportation</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Vérification des caractères manquants</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation type="obsolete">Vérification des objets placés hors d&apos;une page</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Vérification du débordement de texte dans les cadres de texte</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation type="obsolete">Vérification des transparences</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Vérification des images manquantes</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Vérification de la résolution des images</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Résolution la plus faible autorisée</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppp</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Vérification des fichiers PDF inclus</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Vérification des annotations et des champs PDF</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Ajouter un profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Supprimer un profil</translation>
    </message>
    <message>
        <source>Highest allowed resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for GIF images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ignore non-printable Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for items not on a page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for used transparencies</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabDisplay</name>
    <message>
        <source>Color for paper</source>
        <translation type="unfinished">Couleur du papier</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Masquer la zone en-dehors des marges avec la couleur de celles-ci</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished">(Dés)activer l&apos;affichage des liens entre les cadres de texte</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Affiche les caractères invisibles tels que le symbole de paragraphe dans les cadres de texte</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">(Dés)activer l&apos;affichage des contours des cadres</translation>
    </message>
    <message>
        <source>Turns the display of layer indicators on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">(Dés)activer l&apos;affichage des images</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Définit la quantité d&apos;espace disponible à gauche du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Définit la quantité d&apos;espace disponible à droite du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Définit la quantité d&apos;espace disponible en haut du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Définit la quantité d&apos;espace disponible en bas du document comme zone de travail permettant de créer et modifier des objets avant de les déplacer sur la page active</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="unfinished">Définir le niveau de zoom par défaut</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished">Placez une règle sur votre écran et déplacez le curseur afin de définir le niveau de zoom permettant à Scribus d&apos;afficher les pages et les objets qu&apos;elles contiennent à la bonne dimension</translation>
    </message>
</context>
<context>
    <name>TabDisplayBase</name>
    <message>
        <source>Page Display</source>
        <translation type="unfinished">Affichage de la page</translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Afficher les cadres</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Afficher les liens de cadres</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Afficher les zones &amp;non imprimables dans la couleur des marges</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <source>Rulers Relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished">Afficher les caractères de contrôle du texte</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Afficher les images</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Couleur :</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished">Espace de travail</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Gauche :</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Droite :</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Bas :</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Haut :</translation>
    </message>
    <message>
        <source>Gaps Between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished">Horizontal : </translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished">Vertical :</translation>
    </message>
    <message>
        <source>Adjust Display Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished">Pour régler l&apos;affichage, ajustez la règle ci-dessous à l&apos;aide du curseur.</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Général</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Couleurs</translation>
    </message>
    <message>
        <source>Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="unfinished">Couleur de fond :</translation>
    </message>
    <message>
        <source>Selected Page Border:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Locked:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selected:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linked:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grouped:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Annotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Control Characters:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Bleed Area</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TabDisplayBase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scale%</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabDocument</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Sur mesure</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Format de la page</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Taille :</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portrait (à la française)</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntation :</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished">Unités :</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Largeur :</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Hauteur :</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Marges</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Enregistrement automatique</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Fréquence :</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="unfinished">Défaire/Refaire</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished">Longueur de l&apos;historique des actions</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Largeur des pages du document, modifiable si vous avez choisi des dimensions de page personnalisées</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Hauteur des pages du document, modifiable si vous avez choisi des dimensions de page personnalisées</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished">Dimensions de page par défaut, soit standards soit personnalisées</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="unfinished">Orientation par défaut des pages du document</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished">Unité de mesure par défaut pour l&apos;édition de document</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished">Si activé, Scribus effectue à intervalles réguliers une sauvegarde de sécurité de votre fichier avec l&apos;extension « .bak »</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="unfinished">Intervalle entre chaque sauvegarde automatique</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished">Définit la longueur de l&apos;historique d&apos;actions. Si égal à 0 un nombre infini d&apos;actions sera stocké.</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished">Appliquer les changements de dimensions à toutes les pages du document</translation>
    </message>
    <message>
        <source>Apply settings to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Document Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing master pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidget</name>
    <message>
        <source>Locate Ghostscript</source>
        <translation type="unfinished">Localisez GhostScript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished">Localisez votre éditeur d&apos;image</translation>
    </message>
    <message>
        <source>Locate your web browser</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidgetBase</name>
    <message>
        <source>External Tools</source>
        <translation type="unfinished">Outils externes</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished">Interpréteur PostScript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished">&amp;Nom de l&apos;exécutable :</translation>
    </message>
    <message>
        <source>&amp;Change..</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished">Lisser les &amp;textes</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="unfinished">Lisser les textes pour l&apos;affichage EPS et PDF</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished">Lisser les objets &amp;graphiques</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished">Alt+G</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="unfinished">Lisser les objets graphiques pour l&apos;affichage EPS et PDF</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished">Résolution :</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> ppp</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="unfinished">Outil de retouche d&apos;image</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished">Nom de l&apos;&amp;exécutable :</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>&amp;Rescan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for graphics editor. If you use gimp and your distribution includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Web Browser</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Web Browser to launch with links from the Help system</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for your web browser. This is used for external links from the Help system.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rescan for the external tools if they do not exist in the already specified location</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGeneral</name>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished">Sélectionne la langue utilisée par défaut par Scribus. Laisser vide pour choisir la langue définie par les variables d&apos;environnement. Il est toujours possible de modifier la langue avec les options de ligne de commande au démarrage de Scribus</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished">Nombre de documents récemment ouverts à afficher dans le menu Fichier</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished">Nombre de lignes qui défilent pour chaque cran de molette de la souris</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished">Choisissez l&apos;apparence générale de Scribus. Scribus utilise les thèmes KDE ou Qt installés, si Qt est configuré pour chercher les modules KDE.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished">Corps de la police pour les menus et les fenêtres</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished">Corps de la police pour les fenêtres d&apos;outils</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="unfinished">Dossier des documents</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished">Dossier par défaut des profils ICC. Ne peut être changé lorsqu&apos;un document estouvert. Par défaut, Scribus cherchera dans les dossiers systèmes sous Mac OS X et Windows. Sous Linux et Unix, Scribus cherchera dans les dossiers $home/.color/icc,/usr/share/color/icc·et·/usr/local/share/color/icc·</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished">Dossier par défaut pour les scripts</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished">Dossier alternatif pour le stockage des modèles</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Sélectionnez un dossier</translation>
    </message>
</context>
<context>
    <name>TabGeneralBase</name>
    <message>
        <source>GUI</source>
        <translation type="obsolete">Interface graphique</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Langue :</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished">Appar&amp;ence :</translation>
    </message>
    <message>
        <source>Time before a Move or Resize starts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> ms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished">&amp;Corps (Menus) :</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished">Corps (&amp;Palettes) :</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished">Dé&amp;filement :</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished">Docu&amp;ments récents :</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Chemins</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished">&amp;Documents :</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished">Profils &amp;ICC :</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>Alt+H</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="unfinished">&amp;Scripts :</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished">Parco&amp;urir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>Document &amp;Templates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">&amp;Parcourir...</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished">Alt+N</translation>
    </message>
    <message>
        <source>User Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished">Afficher l&apos;écran de démarrage</translation>
    </message>
    <message>
        <source>Show Splashscreen on Startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TabGeneralBase</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Paramètres communs</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Positionnement dans les documents :</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>En arrière-plan</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>Au premier plan</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Magnétisation</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Distance de magnétisation :</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Rayon d&apos;action :</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Afficher les repères</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Couleur :</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Afficher les marges</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Grand quadrillage</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Espacement :</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Petit quadrillage</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Afficher la grille typographique</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Paramètres de la grille typographique</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Espace entre deux lignes de la &amp;grille typographique :</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Point de départ de la grille typographique :</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Les repères ne sont pas visibles au travers des objets sur la page</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Les repères sont visible au travers des objets sur la page</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Espacement entre 2 lignes du petit quadrillage</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Espacement entre 2 lignes du grand quadrillage</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Distance à partir de laquelle un objet est attiré par la grille ou par un repère</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Rayon d&apos;action pour attraper les objets</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Couleur du petit quadrillage</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Couleur du grand quadrillage</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Couleur des repères</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Couleur des marges</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Couleur de la grille typographique</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>(Dés)active l&apos;affichage de la grille typographique</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Espacement entre les lignes typographiques</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Distance entre le haut de la page et la première ligne typographique</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>(Dés)active l&apos;affichage de la grille</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>(Dés)active l&apos;affichage des repères</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>(Dés)active l&apos;affichage des marges</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="unfinished">Fichiers de raccourcis clavier XML (*.ksxml)</translation>
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
        <translation type="unfinished">Majuscule</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="unfinished">Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished">Meta+</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="unfinished">Majuscule+</translation>
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
        <source>This key sequence is already in use</source>
        <translation type="unfinished">Raccourci déjà utilisé</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidgetBase</name>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation type="unfinished">Raccourcis clavier</translation>
    </message>
    <message>
        <source>Search:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action</source>
        <translation type="unfinished">Action</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut for Selected Action</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="unfinished">&amp;Pas de raccourci</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished">Alt+N</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">Racco&amp;urci personnalisé</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">&amp;Définir le raccourci</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation type="unfinished">Alt+K</translation>
    </message>
    <message>
        <source>CTRL+ALT+SHIFT+W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="unfinished">Jeux de raccourcis clavier à charger</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="unfinished">Jeux de raccourcis claviers disponibles</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="unfinished">&amp;Charger</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="unfinished">Charger le jeu de raccourcis clavier</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="unfinished">&amp;Importer...</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="unfinished">Importer un jeu de raccourcis clavier dans la configuration courante</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="unfinished">E&amp;xporter...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished">Alt+E</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="unfinished">Exporter les raccourcis clavier actuel vers un fichier</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Remise à zéro</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="unfinished">Recharger les raccourcis clavier par défaut</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Gestion des tabulations</translation>
    </message>
</context>
<context>
    <name>TabMiscellaneousBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished">Toujours demander avant de remplacer les polices à l&apos;ouverture d&apos;un document</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished">Aperçu du style de paragraphe courant lors de l&apos;édition des styles</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="obsolete">Afficher l&apos;écran de démarrage</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">Faux texte</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished">Toujours utiliser le faux texte Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished">Nombre de paragraphes :</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Plage d&apos;exportation</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Toutes les pages</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Les pages suivantes</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation :</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>S&apos;arrêter aux marges</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Options de fichier</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Compati&amp;bilité :</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>Reli&amp;ure :</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>À gauche</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>À droite</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Gé&amp;nérer les vignettes</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Enregistrer les ca&amp;dres de texte liés comme des articles PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Inclure les signets</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Inclure les cal&amp;ques</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppp</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Résolution des graphiques EPS :</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Comprimer le texte et les graphiques &amp;vectoriels</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Méthode de compression</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatique</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Avec perte - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Sans perte - Zip</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Métho&amp;de de compression :</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maximale</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Haute</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Moyenne</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Basse</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimale</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>&amp;Qualité de compression :</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Rééchantillonner les i&amp;mages à :</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Général</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">Incorporer &amp;toutes les polices</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">Incorporer des &amp;jeux partiels de toutes les polices</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Incorporation</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Polices disponibles :</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;Incorporer</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation>E&amp;xclure</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Polices à incorporer :</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Jeux partiels de polices à incorporer :</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>Poli&amp;ces</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Acti&amp;ver les effets de présentation</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Page</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Aperç&amp;u des pages</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effets</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Affichage de la &amp;durée :</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Durée de l&apos;effe&amp;t :</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>T&amp;ype d&apos;effet :</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Lignes &amp;mobiles :</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>Depui&amp;s :</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>D&amp;irection :</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> seconde(s)</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Pas d&apos;effet</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Masques</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Boîte</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Dissoudre</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Scintillement</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Rupture</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Effacer</translation>
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
        <translation>Interne</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Externe</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>De gauche à droite</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>De haut en bas</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>De bas en haut</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>De droite à gauche</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>D&apos;en haut à gauche vers en bas à droite</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Appli&amp;quer le(s) effet(s) à toutes les pages</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Paramètres d&apos;affichage</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="obsolete">Disposition</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Page simple</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Continu</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Double page gauche</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Double page droite</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Aspect</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Utiliser les paramètres par défaut du visualiseur</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Mode plein écran</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Afficher l&apos;onglet de signets</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Afficher les miniatures</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Afficher l&apos;onglet de calques</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Masquer la barre d&apos;outils du visualiseur</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Masquer la barre des menus du visualiseur</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Zoomer les pages en fonction de la fenêtre du visualiseur</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Actions spéciales</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript à exécuter
à l&apos;ouverture du document PDF :</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Aucun script</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Visualiseur</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>C&amp;hiffrement</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Mots de passe</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Utilisateur :</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>Proprié&amp;taire :</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Autoriser l&apos;&amp;impression du document</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Autoriser la &amp;modification du document</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Autori&amp;ser la copie des textes et des objets graphiques</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Autoriser l&apos;a&amp;jout d&apos;annotations et de champs</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>P&amp;rotection</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>&amp;Sortie sur :</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Écran / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Imprimante</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Niveaux de gris</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Convertir les couleurs d&apos;accompagnement en couleurs quadrichromiques</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Utiliser des options de rendu personnalisées</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Rendus</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Fré&amp;quence :</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Angle :</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Point de trame :</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Point</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Filet</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Rond</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Ovale</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Couleurs d&apos;accompagnement :</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Utiliser un profil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil :</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Rendu :</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptible</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrie relative</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturation</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrie absolue</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Images :</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Ne pas utiliser les profils ICC intégrés</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>C&amp;ouleur</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Sortie PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>Ligne d&apos;&amp;information :</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Profil de &amp;sortie :</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Cadre de découpe</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PD&amp;F/X-3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Inclure les polices dans le fichier PDF. Ceci permet de conserver l&apos;apparence intégrale de votre document.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Active les effets de présentation en mode plein écran lors de l&apos;utilisation d&apos;Adobe&amp;#174; Reader&amp;#174; ou des autres visionneuses PDF le supportant.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Afficher les aperçus de chaque page dans la liste ci-dessus.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Temps d&apos;exécution de l&apos;effet. Un temps court accélérera l&apos;effet, un temps plus long le ralentira.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Type de l&apos;effet.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Direction des lignes mobiles pour les effets rupture et masques.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Position de départ pour les effets boîte et rupture.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Direction pour les effets scintillement et effacer.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Appliquer l&apos;effet sélectionné sur toutes les pages.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exporter toutes les pages vers un fichier PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exporter une sélection de pages vers un fichier PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Insérer une liste d&apos;éléments séparés par des virgules, les éléments pouvant être « * » pour toutes les pages, 1-5 pour une sélection de pages, ou bien un simple numéro de page.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">Compatibilité PDF. PDF 1.3 est l&apos;option par défaut et offre la meilleure compatibilité possible. Choisir PDF 1.4 si vous avez besoin des fonctionnalités telles que la transparence ou le chiffrement 128 bits. Le PDF 1.5 permet de préserver les objets sur différents calques dans le fichier PDF. PDF/X-3 permet l&apos;exportation de fichiers PDF pour l&apos;impression professionnelle et est disponible si vous avez activé la gestion des couleurs. A n&apos;utiliser qu&apos;à la demande d&apos;un imprimeur ou dans quelques cas d&apos;impression sur une imprimante numérique laser 4 couleurs.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Position de la reliure dans le fichier PDF. Sauf raison particulière, laisser la valeur par défaut : à gauche</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Générer les vignettes pour chaque page du fichier PDF. Certaines visionneuses les utilisent pour faciliter la navigation dans le document.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Générer les articles PDF, pratique pour la navigation entre les articles liés dans un fichier PDF.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Les calques de votre document sont exportés dans le fichier PDF. Disponible uniquement si le format PDF 1.5 est sélectionné.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Inclure les signets que vous avez créés dans le document. Pratique pour la navigation dans les longs fichiers PDF.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Exporter les résolutions du texte et des objets vectoriels. Ceci n&apos;a aucun impact sur la résolution des objets matriciels comme les photos.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Compression sans perte des textes et des objets graphiques. Sauf raison particulière, à laisser activé. Cela permet de réduire la taille du fichier PDF.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options. This only affects JPEG images</source>
        <translation type="obsolete">Type de compression pour les images. Automatique laisse Scribus choisir la meilleure méthode. ZIP pour une compression sans pertes. JPEG permet de réduire la taille des fichiers PDF qui ont de nombreuses photos (avec une possible petite perte de qualité d&apos;image). Laisser sur Automatique, sauf si vous avez des besoins spéciaux de compression. Cette option ne concerne que les images au format JPEG</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="obsolete">Niveaux de qualité de compression pour les méthodes de compression avec pertes : minimum (25%), moyen (75%), haute (85%), maximum (95%). Notez qu&apos;un niveau de qualité ne détermine pas directement la taille des fichiers résultants. La taille et la perte de qualité varient d&apos;une image à l&apos;autre à niveau constant.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Réduire la résolution de vos images matricielles à celle sélectionnée. Si cette option est désactivée, les images seront rendues dans leur résolution native. Activer cette option peut augmenter l&apos;utilisation mémoire et ralentir l&apos;exportation.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>PPP (Points Par Pouce) pour l&apos;exportation d&apos;image.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Activation des fonctions de sécurité dans le fichier PDF. Avec la version 1.3, le PDF utilise un chiffrement sur 40 bits. En version 1.4, le PDF utilise un chiffrement sur 128 bits. Attention : le chiffrement PDF n&apos;est pas aussi robuste que celui de GPG ou PGP.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Choisissez un mot de passe qui permet d&apos;activer ou désactiver les fonctionnalités de sécurité dans le fichier PDF généré.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Mot de passe nécessaire pour lire le fichier PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Si cochée, l&apos;impression du fichier PDF est autorisée.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Si cochée, la modification du fichier PDF est autorisée.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Si cochée, la copie des textes et des objets graphiques du fichier PDF est autorisée.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Si cochée, l&apos;ajout et la modification d&apos;annotations et de champs dans le fichier PDF est autorisé.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Modèle de couleur pour la sortie du fichier PDF. Choisir Écran / Web pour une utilisation standard ou une impression sur une imprimante à jet d&apos;encre. Choisir Imprimante pour un fichier PDF destiné à une impression CMJN. Choisiréchelle de gris pour créer un fichier PDF en niveaux de gris.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Ceci est un réglage avancé non activé par défaut. Il ne devrait être activé que lorsque demandé par votre imprimeur et que celui-ci vous en a communiqué tous les détails. Dans le cas contraire, votre PDF risque de ne pas s&apos;imprimer correctement et ne sera pas portable d&apos;un système à un autre.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Inclure un profil de couleur pour les couleurs d&apos;accompagnement</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Profil de couleur pour les couleurs d&apos;accompagnement</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Méthode de rendu pour les couleurs d&apos;accompagnement</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Inclure un profil de couleur pour les images</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Ne pas utiliser les profils de couleur inclus dans les images sources</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Profil de couleur pour les images</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Méthode de rendu pour les images</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Profil de sortie pour l&apos;impression. À ce sujet, consultez votre imprimeur, si possible.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Chaîne obligatoire pour PDF/X-3 sinon la conformité PDF/X-3 ne sera pas assurée. Il est recommandé d&apos;utiliser le titre du document.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Fond perdu en haut de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Fond perdu en bas de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Fond perdu à gauche de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Fond perdu à droite de la page</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Miroir horizontal des pages</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Miroir vertical des pages</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Cette fonction permet de convertir les couleurs d&apos;accompagnement en un équivalent quadrichromique. Cochez cette option à moins de savoir exactement ce que vous faites.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Ne pas afficher les objets en-dehors des marges dans le fichier exporté</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <comment>Fred</comment>
        <translation type="obsolete">Durée pendant laquelle la page est affichée avant le début de la présentation sur la page sélectionnée. Une valeur de 0 désactive la transition automatique entre les pages.</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation type="unfinished">Durée pendant laquelle la page est affichée avant le début de la présentation sur la page sélectionnée. Une valeur de 0 désactive la transition automatique entre les pages.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.&lt;br/&gt;The default is &lt;b&gt;PDF 1.3&lt;/b&gt; which gives the widest compatibility.&lt;br/&gt;Choose &lt;b&gt;PDF 1.4&lt;/b&gt; if your file uses features such as transparency or you require 128 bit encryption.&lt;br/&gt;&lt;b&gt;PDF 1.5&lt;/b&gt; is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/&gt;&lt;b&gt;PDF/X-3&lt;/b&gt; is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Embed all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outline all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printer Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Crop Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleed Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Registration Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleed Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="unfinished">Droite :</translation>
    </message>
    <message>
        <source>Use Document Bleeds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pre-Press</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert all glyphs in the document to outlines.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabPrinter</name>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Fond perdu en haut de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Fond perdu en bas de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Fond perdu à gauche de la page</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Fond perdu à droite de la page</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation type="unfinished">Ne pas afficher les objets en-dehors des marges lors de l&apos;impression</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished">Utiliser un gestionnaire d&apos;impression tel que kprinter ou gtklp, afin de disposer d&apos;options d&apos;impression supplémentaires</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished">Définit le niveau PostScript.
Les niveaux 1 et 2 peuvent créer des fichiers volumineux.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">L&apos;UCR (Under Color Removal en anglais) consiste à n&apos;utiliser que de l&apos;encre noire pour le rendu des teintes grises composées par les trois couleurs primaires. L&apos;UCR est plus efficace sur les zones de l&apos;image dans des tons neutres ou sombres, proches du gris. Cette fonction peut permettre une meilleure qualité d&apos;impression, mais des tests au cas par cas sont nécessaires. L&apos;UCR réduit les risques de saturation avec les encres CMJ.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished">Cette fonction permet de convertir les couleurs d&apos;accompagnement en un équivalent quadrichromique. Cochez cette option à moins de savoir exactement ce que vous faites.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished">Permet d&apos;incorporer des profils colorimétriques ICC dans votre processus d&apos;impression lorsque la gestion des couleurs est active.</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished">Vous pouvez ici définir le format de média sur lequel votre fichier PostScript sera imprimé. Ceci n&apos;est pas recommandé à moins que votre imprimeur n&apos;en ait besoin.</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Fichier</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tout</translation>
    </message>
</context>
<context>
    <name>TabPrinterBase</name>
    <message>
        <source>Print Destination</source>
        <translation type="unfinished">Destination de l&apos;impression</translation>
    </message>
    <message>
        <source>Alternative Printer Command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Command:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Options</translation>
    </message>
    <message>
        <source>Postscript Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level 2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level 3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation type="unfinished">Imprimer en couleur si possible</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished">Imprimer en niveaux de gris</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Page</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished">Miroir horizontal des pages</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished">Miroir vertical des pages</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished">Définir le format de média</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">S&apos;arrêter aux marges</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Couleur</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished">Retrait des sous-couleurs (UCR)</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished">Convertir les couleurs d&apos;accompagnement en couleurs quadrichromiques</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished">Appliquer un(des) profil(s) ICC</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Général</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation type="unfinished">Impression normale</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation type="unfinished">Séparation</translation>
    </message>
    <message>
        <source>Marks &amp;&amp; Bleeds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printer Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Crop Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleed Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Registration Marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bleed Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="unfinished">Droite :</translation>
    </message>
    <message>
        <source>TabPrinterBase</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabScrapbookBase</name>
    <message>
        <source>Send Copied Items Automatically to Scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keep Copied Items Permanently Across Sessions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables copied items to be kept permanently in the scrapbook.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of Copied Items to Keep in Scrapbook:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The minimum number is 1; the maximum us 100.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>TabScrapbookBase</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Corps :</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Couleur du texte : </translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Teinte :</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Contour des caractères :</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Couleur de fond :</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Couleur des contours :</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Point</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Trait d&apos;union</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Soulignement</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Sur mesure</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Points de conduite :</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Largeur de tabulation :</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>No&amp;mbre de colonnes :</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Gouttière :</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>à l&apos;île exiguë où l&apos;obèse jury mûr fête l&apos;haï volapük,
âne ex æquo au whist, ôtez ce vœu déçu
À L&apos;ÎLE EXIGUË OÙ L&apos;OBÈSE JURY MÛR FÊTE L&apos;HAÏ VOLAPÜK,
ÂNE EX ÆQUO AU WHIST, ÔTEZ CE VŒU DÉÇU</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Couleur du &amp;filet :</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Teinte :</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Couleur de fond :</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>T&amp;einte :</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Styles de filets :</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>É&amp;paisseur :</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Styles de &amp;filets :</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Flèches :</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Départ :</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Fin :</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Mise à l&apos;échelle &amp;libre</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Échelle &amp;horizontale :</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Échelle &amp;verticale :</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Mettre l&apos;image aux &amp;dimensions du cadre</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Conserver les p&amp;roportions</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Couleur de fond :</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Utiliser le tracé de détourage incorporé</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Aperçu avant impression</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Aperçu en haute résolution</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Aperçu standard</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Aperçu en basse résolution</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mi&amp;nimum :</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ximum :</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>Pro&amp;gression :</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Propriétés des cadres de texte</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Propriétés des cadres d&apos;image</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Propriétés des formes</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Propriétés de visualisation</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Propriétés des filets</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Propriétés des polygones</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Police pour les nouveaux cadres de texte</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Corps pour les nouveaux cadres de texte</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Couleur de la police</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Nombre de colonnes dans un cadre de texte</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Espacement entre les colonnes d&apos;un cadre de texte</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Échantillon de votre police</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Les cadres d&apos;image permettent une mise à l&apos;échelle libre des images</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Échelle horizontale des images</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Échelle verticale des images</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Appliquer la même valeur de mise à l&apos;échelle pour l&apos;horizontale et la verticale</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Les images sont mises aux dimensions du cadre</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Les images redimensionnées automatiquement conservent leurs proportions</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Couleur de fond des cadres d&apos;image</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Saturation de la couleur</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Couleur des contours des formes</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Saturation de la couleur des filets</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Couleur de fond des formes</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Style du contour des formes</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Épaisseur des contours des formes</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Taille minimale de visualisation</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Taille maximale de visualisation</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Pas du zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Couleur des filets</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturation de la couleur</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Style des filets</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Épaisseur des filets</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="obsolete">Sur mesure :</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="obsolete">Sur mesure :</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished">Aucune</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Texte</translation>
    </message>
    <message>
        <source>Shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="unfinished">Barres</translation>
    </message>
    <message>
        <source>Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Regular Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zoom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotation Tool</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Constrain to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Other Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Miscellaneous Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Item Duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal displacement of page items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical displacement of page items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Constrain value for the rotation tool when the Control key is pressed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Indice</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Décalage :</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Mise à l&apos;échelle :</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Exposant</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Décalag&amp;e :</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Mise à l&apos;é&amp;chelle :</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Décalage :</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automatique</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Épaisseur :</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Barré</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Petites capitales</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Mise à l&apos;éc&amp;helle :</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Inter&amp;lignage automatique</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Interlignage :</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Décalage au-dessus de la ligne de base de la police</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Corps relatif de l&apos;exposant par rapport à celui de la police</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Décalage en dessous de la ligne de base de la police</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Corps relatif de l&apos;indice par rapport à celui de la police</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Corps relatif des petites capitales par rapport à celui de la police</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>L&apos;interlignage équivaut au corps de la police plus ce pourcentage</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Décalage sous la ligne de base de la police, exprimé en pourcentage du jambage des lettres</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Largeur de filet exprimé en pourcentage du corps de la police</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Décalage au dessus de la ligne de base de la police, exprimé en pourcentage de la hampe des lettres</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Gauche</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Droite</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Arrêt complet</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Virgule</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centré</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Position :</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Aucune</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Point</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Trait d&apos;union</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Soulignement</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Sur mesure</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Points de conduite :</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Retrait de la première ligne du paragraphe</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Retrait gauche du paragraphe</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Effacer toutes les tabulations</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="obsolete">Sur mesure :</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="obsolete">Sur mesure :</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished">Aucune</translation>
    </message>
    <message>
        <source>Indentation from the right for the whole paragraph</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TextBrowser</name>
    <message>
        <source>Locate your web browser</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>External Web Browser Failed to Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Free Objects</source>
        <translation>Libérer les objets</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grouper</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Page </translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Plan du document</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Élément</translation>
    </message>
    <message>
        <source>Free items</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation>Automatique</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Déplacement</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Épaisseur</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Ajouter un repère vertical</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Ajouter un repère horizontal</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Supprimer le repère vertical</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Supprimer le repère horizontal</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Déplacer le repère vertical</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Déplacer le repère horizontal</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Verrouiller les repères</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Déverrouiller les repères</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Déplacer</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Redimensionner</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Rotation</translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation>X1 :·%1,·Y1 :·%2,·%3
X2 :·%4,·Y2 :·%5,·%6</translation>
    </message>
    <message>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation>W1 :·%1,·H1 :·%2
W2 :·%3,·H2 :·%4</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Changer le décalage de l&apos;image</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Changer l&apos;échelle de l&apos;image</translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
        <translation>X1 :·%1,·Y1 :·%2
X2 :·%4,·Y2 :·%5</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
X: %4, Y: %5</source>
        <translation>X :·%1,·Y :·%2
X :·%4,· y:·%5</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Sélection</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Grouper</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Sélectionner/Grouper</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X :·%1,·Y :·%2
L :·%3,·H :·%4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Espacer-Aligner</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Objets concernés</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Sélectionner la couleur de fond</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Couleur1 :·%1,·Couleur2 :·%2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Sélectionner la teinte de fond</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Sélectionner la couleur de filet</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Couleur des contours des formes</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Retourner horizontalement</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Retourner verticalement</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Verrouiller</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Déverrouiller</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Verrouiller la taille</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Déverrouiller la taille</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Autoriser l&apos;impression de l&apos;objet</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Désactiver l&apos;impression de l&apos;objet</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Dégrouper</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>De %1
à %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Appliquer un gabarit</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Coller</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Couper</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Opacité de la couleur de fond</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Opacité de la couleur de filet</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Édition des styles de filets</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Type de terminaison</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Type de raccord</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Épaisseur</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Aucun style</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Définir un style de filet sur mesure</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Ne pas utiliser de style de filet sur mesure</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Flèche de début</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Flèche de fin</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Créer un tableau</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Lignes : %1, Colonnes : %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Sélectionnez une police</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Corps</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Largeur de police</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Hauteur de police</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Couleur de remplissage de la police</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Couleur de contour de la police</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Teinte de fond des polices</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Teinte de contour des polices</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Crénage</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Espacement entre les lignes</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Style de paragraphe</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Aligner le texte</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Graisses et styles</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Cadre d&apos;image</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Cadre de texte</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygone</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Courbe de Bézier</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Ligne complexe</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Joindre le texte au tracé</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Convertir en</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Importer un fichier SVG</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importer un fichier EPS</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Importer une illustration OpenOffice.org</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Espace de travail</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Habillage autour du cadre</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Habillage autour de la boîte englobante</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Habillage autour du contour</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Pas d&apos;habillage</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Pas de boîte englobante</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Pas de ligne de contour</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Page %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Échelle de l&apos;image</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Taille du cadre</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Mise à l&apos;échelle libre</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Conserver les proportions</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Modifier les proportions</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Modifier le contour</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Modifier la forme</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Remise à zéro du contour</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Ajouter une page</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Ajouter des pages</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Supprimer une page</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Supprimer des pages</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Créer un nouveau calque</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Détruire le calque</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Renommer le calque</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Placer le calque au-dessus</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Placer le calque au-dessous</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Mettre sur le calque</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Autoriser ou non l&apos;impression du calque</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Désactiver l&apos;impression du calque</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Changer le nom du calque</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Charger une image</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Dupliquer-Déplacer multiples</translation>
    </message>
    <message>
        <source>Apply text style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo: %1</source>
        <comment>f.e. Undo: Move</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">Ann&amp;uler</translation>
    </message>
    <message>
        <source>&amp;Redo: %1</source>
        <comment>f.e. Redo: Move</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Rétablir</translation>
    </message>
    <message>
        <source>No object frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset control point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply image effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjust frame to the image size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set start and end arrows</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Etat initial</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Historique des actions</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Afficher uniquement l&apos;objet sélectionné</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation>%1 : %2</translation>
    </message>
</context>
<context>
    <name>UnicodeSearchBase</name>
    <message>
        <source>Unicode Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meaning</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UpgradeChecker</name>
    <message>
        <source>Attempting to get the Scribus version update file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Timed out when attempting to get update file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No updates are available for your version of Scribus %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This list may contain development versions.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Marges minimum pour format %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Marges de votre imprimante</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Choisir l&apos;im&amp;primante</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marges</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Droite :</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Haut :</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bas :</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Gauche :</translation>
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
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Insérer une valeur</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Saisir une valeur et appuyer sur OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Saisir une valeur et appuyer sur OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Envoyer la valeur au script</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Outils</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="obsolete">Propriétés...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation type="obsolete">Outils PDF</translation>
    </message>
    <message>
        <source>Button</source>
        <translation type="obsolete">Bouton</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation type="obsolete">Champ de texte</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation type="obsolete">Case à cocher</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation type="obsolete">Menu déroulant</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation type="obsolete">Liste</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="obsolete">Insérer un champ (PDF)</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation type="obsolete">Insérer une annotation (PDF)</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Texte</translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="obsolete">Lien</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Choisissez le filtre d&apos;importation à utiliser</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatique</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importer le texte seul</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importer le texte sans aucune mise en forme</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Filtre d&apos;importation :</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codage :</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ouvrir</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Choisissez le filtre d&apos;importation à utiliser</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Mémoriser l&apos;association</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Mémoriser l&apos;association extension de fichier - filtre d&apos;importation et ne pas demander à nouveau de sélectionner un filtre d&apos;importation pour des fichiers de ce type.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nouveau d&apos;après modèle</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Ouvrir</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tout</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Format de la page</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Description</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Utilisation</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Réalisé avec</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Date</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Auteur</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Télécharger des modèles</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Des modèles sont disponibles sur le site http://www.scribus.net/ dans la rubrique « Downloads ».</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Installation des modèles</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Extraire l&apos;archive dans le dossier ~/.scribus/templates pour l&apos;utilisateur courant ou dans le dossier PREFIX/share/scribus/templates pour que le modèle soit accessible à tous les utilisateurs du système.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Préparation d&apos;un modèle</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Assurez-vous que les images et les polices contenues dans votre document peuvent être redistribuées librement.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Le créateur de modèles doit veiller à ce que chacun puisse installer et utiliser librement les modèles qu&apos;il a créés.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Effacer un modèle</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>L&apos;effacement d&apos;un modèle de cette liste supprimera uniquement son entrée dans le fichier template.xml. Les fichiers ne seront pas effacés. Vous ne pouvez effacer ces entrées que si vous avez le droit d&apos;écrire sur le fichier template.xml.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Traduction de template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Copiez un fichier template.xml existant vers une fichier template.CODEPAYS.xml, par exemple template.fr.xml pour un fichier en français. Le nouveau fichier doit se trouver dans le même dossier que l&apos;original pour pouvoir être ouvert.</translation>
    </message>
</context>
<context>
    <name>patternDialogBase</name>
    <message>
        <source>Patterns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished">Effacer</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Annuler</translation>
    </message>
    <message>
        <source>Remove All</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Enregistrer en tant que modèle</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Catégorie</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Format de la page</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Description</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Utilisation</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Auteur</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Courriel</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Plus de détails</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Moins de détails</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Légal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Lettre</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloïd</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>paysage (à l&apos;italienne)</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>portrait (à la française)</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>sur mesure</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Créer un filtre</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Effacer</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;upprimer</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Sélectionner un filtre existant</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Nom d&apos;enregistrement de ce filtre</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Nom d&apos;enregistrement</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Activer ou désactiver ce filtre</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Effacer ce filtre</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Ajouter un nouveau filtre</translation>
    </message>
    <message>
        <source>to</source>
        <translation>à</translation>
    </message>
    <message>
        <source>and</source>
        <translation>et</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>effacer l&apos;occurence</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ne pas effacer l&apos;occurence</translation>
    </message>
    <message>
        <source>words</source>
        <translation>mots</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Substituer</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Appliquer</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>La valeur à gauche est une expression régulière</translation>
    </message>
    <message>
        <source>with</source>
        <translation>avec</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>style de paragraphe</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>toutes les occurences de</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>tous les paragraphes</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>paragraphes débutant avec</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>paragraphes avec moins de</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>paragraphes avec plus de</translation>
    </message>
</context>
</TS>
