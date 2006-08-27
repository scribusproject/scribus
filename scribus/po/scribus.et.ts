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
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

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
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
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
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
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
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
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
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If printable is set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus %1</source>
        <translation>Scribus %1 info</translation>
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
        <translation>Ghostscripti versioon %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ghostscripti versioon pole teada</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribuse versioon %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Ehitamise ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Info</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Arendajate meeskond:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Kaasautorid:</translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Mac OS&amp;#174; X Aqua port:</translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation>Windows&amp;#174; port:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Ametlik dokumentatsioon:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Muu dokumentatsioon:</translation>
    </message>
    <message>
        <source>Tango Project Icons:</source>
        <translation>Tango projekti ikoonid:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utorid</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Ametlikud tõlked ja tõlkijad:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Varasemad tõlkijad:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Tõlked</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Kodulehekülg</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Dokumentatsioon internetis</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Veateated ja soovid</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Meililist</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>I&amp;nternet</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">See dialoog näitab Scribuse versiooni, ehitamise aega ja kompileeritud teekide toetust. Lühendis C-C-T-F tähendab C littlecms&apos;i, teine C CUPS-i, T TIFF-i ja F Fontconfig&apos;i tuge. Viimane täht näitab renderdajat: C=cairo või A=libart. Teegi toe puudumist näitab *. Samuti näeb siin Ghostscripti versiooni, mille Scribus leidis.</translation>
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
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Filename:</source>
        <translation>Failinimi:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Versioon:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Jah</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ei</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Lubatud:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Väljalaskeaeg:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Kirjeldus:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Autor(id):</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Autoriõigus:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Litsents:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: pluginate info</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
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
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ava...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Salvesta &amp;kui...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Taasta salvestatud versioon</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Ko&amp;gu väljundiks...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Hangi tekst...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Lisa &amp;tekst...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Hangi pilt...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Salvesta &amp;tekst...</translation>
    </message>
    <message>
        <source>Save as &amp;EPS...</source>
        <translation>Salvesta &amp;EPS-failina...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Salvesta P&amp;DF-failina...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokumendi &amp;seadistused...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Trüki...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>T&amp;rükkimise eelvaatlus</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Välju</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Võta tagasi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Tee uuesti</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>Val&amp;itud elemendi režiim</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Lõika</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Aseta</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Aseta (a&amp;bsoluutselt)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;Vali kõik</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Tühista kõik valikud</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Ot&amp;si/asenda...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Redigeeri pilti...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Vä&amp;rvid...</translation>
    </message>
    <message>
        <source>S&amp;tyles...</source>
        <translation>S&amp;tiilid...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Lõigu stiilid...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Joone stiilid...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>Le&amp;heküljetoorikud...</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScriptid...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Seadistused...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Muu...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Vasakule</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Keskele</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Paremale</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Rööpjoondus</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>Pü&amp;siv rööpjoondus</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Normaalne</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>Alla&amp;joonitud</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>&amp;Sõnade allajoonimine</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Läbikriipsutatud</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>S&amp;uurtähed</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapiteelkiri</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>Ü&amp;laindeks</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>Ala&amp;indeks</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>K&amp;ontuur</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Vari</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Pildiefektid</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabeldusmärgid...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Klooni</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Klooni &amp;mitmekordselt</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Rühmita</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Lõh&amp;u rühm</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>&amp;Lukustatud</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>&amp;Suurus lukustatud</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Trükkimine lubatud</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>Pee&amp;gelda rõhtsalt</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>Pee&amp;gelda püstiselt</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>V&amp;ii kõige taha</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Too &amp;kõige ette</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Vii tahapoole</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Too ettepoole</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Saada &amp;visandimappi</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Atribuudid...</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Rohkem infot...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Pilt &amp;nähtaval</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Uuenda pilti</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Sobita pilt kasti</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Pildi laiendatud omadused</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Madala kvaliteediga</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Normaalse kvaliteediga</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Täieliku kvaliteediga</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>On PDF-i &amp;järjehoidja</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>On PDF-i &amp;annotatsioon</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>A&amp;nnotatsiooni omadused</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Välja omadused</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Muuda kuju...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Lis&amp;a tekst trajektoorile</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Haagi tekst trajektoori küljest lahti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Ü&amp;henda hulknurgad</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Lah&amp;uta hulknurgad</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier&apos; kõver</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>P&amp;ildikast</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Kontuurid</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Hulknurk</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Tekstikast</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Glüüf...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Näidistekst</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>L&amp;isa...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>Im&amp;pordi...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Eemalda...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopeeri...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Liiguta...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Rakenda leheküljetoorikut...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Salvesta leheküljetoorikuna...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Halda &amp;juhtjooni...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Halda lehekülje omadusi...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>So&amp;bita aknasse</translation>
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
        <translation>&amp;Pisipildid</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;Veeriste näitamine</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>&amp;Kastide näitamine</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>P&amp;iltide näitamine</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>&amp;Alusvõrgu näitamine</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;Juhtjoonte näitamine</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Al&amp;usjoonte näitamine</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>&amp;Tekstiahela näitamine</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Mittetrükitavate märkide näitamine</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Joonlaudade näitamine</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Joonlauad on seotud leheküljega</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Tõmme &amp;alusvõrgule</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Tõmme &amp;juhtjoontele</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Omadused</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>&amp;Struktuur</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Visandimapp</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Kihid</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>L&amp;ehekülgede korraldamine</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Järjehoidjad</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Mõõdud</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>T&amp;oimingute ajalugu</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>Trük&amp;ieelne kontroll</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>Joo&amp;ndus ja jaotus</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Tööriistad</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF-i tööriistad</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Vali element</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abel</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Kujund</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Joon</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Vabakäejoon</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Pööra elementi</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Suurenda või vähenda</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Suurenda</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Vähenda</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Redigeeri kasti sisu</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Redigeeri teksti...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Lingi tekstikastid</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Eemalda tekstikastide link</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Värvivalija</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopeeri elemendi omadused</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Redigeeri teksti looredaktoris</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Lisa tekstikast</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Lisa pildikast</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Lisa tabel</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Lisa kujund</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Lisa hulknurk</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Lisa joon</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Lisa Bezier&apos; kõver</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Lisa vabakäejoon</translation>
    </message>
    <message>
        <source>Insert PDF Push Button</source>
        <translation>Lisa PDF-i kinnitusnupp</translation>
    </message>
    <message>
        <source>Insert PDF Text Field</source>
        <translation>Lisa PDF-i tekstiväli</translation>
    </message>
    <message>
        <source>Insert PDF Check Box</source>
        <translation>Lisa PDF-i märkekast</translation>
    </message>
    <message>
        <source>Insert PDF Combo Box</source>
        <translation>Lisa PDF-i liitkast</translation>
    </message>
    <message>
        <source>Insert PDF List Box</source>
        <translation>Lisa PDF-i nimekirjakast</translation>
    </message>
    <message>
        <source>Insert Text Annotation</source>
        <translation>Lisa teksti annotatsioon</translation>
    </message>
    <message>
        <source>Insert Link Annotation</source>
        <translation>Lisa lingi annotatsioon</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Piltide &amp;haldamine</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Poolitamise lubamine</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Poolitamise keelamine</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>Sisukorra &amp;genereerimine</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskaadi</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Paanidena</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Scribuse info</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;Pluginate info</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>&amp;Qt info</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Vihjed</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>S&amp;cribuse käsiraamat...</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Lülita palette</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Lülita juhtjooni</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Poolituskoht</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Sidekriips</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>&amp;Sisetühik</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Lehekülje&amp;number</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Uus rida</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Kasti katkestus</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Veeru katkestus</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Autoriõigus</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registreeritud kaubamärk</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Kaubamärk</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Kaldkriips</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Täpp</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Punkt keskel</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Pikk mõttekriips</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Lühike mõttekriips</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Arvuvahemiku kriips</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Tsitaadikriips</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Ülakoma</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Sirged topelt</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Ühekordne vasakpoolne</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Ühekordne parempoolne</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Kahekordne vasakpoolne</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Kahekordne parempoolne</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Ühekordne ümberpööratud</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Kahekordne ümberpööratud</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Ühekordne vasakpoolne prantsuse</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Ühekordne parempoolne prantsuse</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Kahekordne vasakpoolne prantsuse</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Kahekordne parempoolne prantsuse</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Ühekordne alumine koma</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Kahekordne alumine koma</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>CJK ühekordne vasakpoolne</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>CJK ühekordne parempoolne</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>CJK kahekordne vasakpoolne</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>CJK kahekordne parempoolne</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>Enn-tühik</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Emm-tühik</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Kitsas tühik</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Lai tühik</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Keskmine tühik</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Juuksekarvatühik</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Lisa poolituskoht</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Lisa sidekriips</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Lisa sisetühik</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Lisa leheküljenumber</translation>
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
        <source>Show Text Frame Columns</source>
        <translation>Tekstikasti veergude näitamine</translation>
    </message>
    <message>
        <source>Get Text 2...</source>
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
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Joondus ja jaotus</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Joondus</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Joondamise alus:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Esimene valitud</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Viimane valitud</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Lehekülg</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Veerised</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Juhtjoon</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Valik</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Objektide parema serva joondamine ankru vasaku serva järgi</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Objektide vasaku serva joondamine ankru parema serva järgi</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Joondamine alumise serva järgi</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Joondamine parema serva järgi</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Objektide ülemise serva joondamine ankru alumise serva järgi</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Tsentreerimine püstteljel</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Joondamine vasaku serva järgi</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Tsentreerimine rõhtteljel</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Objektide alumise serva joondamine ankru ülemise serva järgi</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Joondamine ülemise serva järgi</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Valitud juhtjoon:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Jaotus</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Objektide rõhtsate vahede võrdsustamine</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Objektide rõhtsate vahede võrdsustamine määratud väärtuse järgi</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Paremate servade võrdsed vahed</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Alumiste servade võrdsed vahed</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Rõhtsuunas keskmete võrdsed vahed</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Objektide püstiste vahede võrdsustamine</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Objektide püstiste vahede võrdsustamine määratud väärtuse järgi</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Vasakute servade võrdsed vahed</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Püstsuunas keskmete võrdsed vahed</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Ülemiste servade võrdsed vahed</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>Va&amp;hemaa:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Elementide jaotus määratud vahemaa järgi</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Valik puudub</translation>
    </message>
    <message>
        <source>Y: %1%2</source>
        <translation>Y: %1%2</translation>
    </message>
    <message>
        <source>X: %1%2</source>
        <translation>X: %1%2</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Joonda tekst vasakule</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Joonda tekst paremale</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Joonda tekst keskele</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Rööpjoondus</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Püsiv rööpjoondus</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Välja omadused</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tüüp:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Nupp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstiväli</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Märkekast</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Liitkast</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Nimekirjakast</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Omadused</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nimi:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Kohtspikker:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Font PDF 1.2 jaoks:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Piire</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Värv:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Laius:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Peen</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Lai</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stiil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Ühtlane</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Punktiir</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Allajoonitud</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Esilekerkiv</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Süvistatud</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Muu</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Ainult lugemiseks</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Nõutav</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Väärtust ei ekspordita</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Nähtavus:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Nähtav</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Peidetud</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ei trükita</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Ei näidata</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Välimus</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Button Down tekst</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Roll Over tekst</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikoonid</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Ikoonide kasutamine</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Eemalda</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Allavajutatud</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Pealeliikumisel</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Ikooni paigutus...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Esiletõstmine</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverteeritud</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Kontuuriga</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Vajutatav</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Mitmerealine</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Parool</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Piiratud</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>märgiga</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ei kerita</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Õigekirja ei kontrollita</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Märkimise stiil:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Linnuke</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Rist</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Romb</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Ring</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Täht</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Ruut</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Vaikimisi on märgitud</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Redigeeritav</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Valikud</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Liikumine</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Vormi saatmine</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Vormi lähtetamine</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Andmete import</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Sündmus:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Hiir alla</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Hiir üles</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Hiir peale</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Hiir ära</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Fookuses</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Pole fookuses</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Redigeeri...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Saatmine URL-ile:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Andmed saadetakse HTML-ina</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Andmed imporditakse asukohast:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Sihtkoht</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Fail:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Muuda...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Lehekülg:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-asukoht:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-asukoht:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Toiming</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Välja vorming:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Puhas</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Arv</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Protsent</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Kuupäev</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Kellaaeg</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Arvuvorming</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Kümnendkohti:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Rahasümboli kasutamine</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Rahasümbol ees</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Vorming</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Protsendivorming</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Kuupäevavorming</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Kellaajavorming</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Kohandatud skriptid</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Vorming:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Klahv:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Vorming</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Väärtuse sobivust ei kontrollita</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Väärtus peab olema suurem kui või võrdne:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>ja väiksem kui või võrdne:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Kohandatud sobivuse kontrollimise skript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Sobivuse kontroll</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Väärtust ei arvutata</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Väärtus on järgmiste väljade</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>summa</translation>
    </message>
    <message>
        <source>product</source>
        <translation>korrutis</translation>
    </message>
    <message>
        <source>average</source>
        <translation>keskmine</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>miinimum</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>maksimum</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation> :</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Vali...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Kohandatud arvutamise skript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Arvutamine</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Lippu ignoreeritakse PDF 1.3 korral</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Sisesta siia komadega eraldatult väljade nimekiri</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Nuppudel ikoonide kasutamiseks on vaja vähemalt normaalse nupu ikooni</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Pildid (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Näide:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Valiku muutmine</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-failid (*.pdf);;Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Annotatsiooni omadused</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Link</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Väline link</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Väline veebilink</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tüüp:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Sihtkoht</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Lehekülg:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-asukoht</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-asukoht:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-dokumendid (*.pdf);;Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Leheküljetooriku rakendamine</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Leheküljetoorik:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Rakendatakse</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>&amp;Aktiivsele leheküljele</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Paarislehekülgedele</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>Paa&amp;ritutele lehekülgedele</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Kõigile lehekülgedele</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;Vahemikule</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Valitud leheküljetooriku rakendamine paaris-, paaritutele või kõigile lehekülgedele määratud vahemikus</translation>
    </message>
    <message>
        <source>to</source>
        <translation>kuni</translation>
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
        <translation>Puudub</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>Ri&amp;bakoodi generaator...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Scribuse kasutajaliides puhtas PostScriptis loodud ribakoodi generaatorile</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 või 13 arvu</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 arvu</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 või 12 arvu</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 või 8 arvu</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 arvu</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 arvu</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation type="obsolete">9 või 10 arvu, mugavuse mõttes eraldatud kriipsudega</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Varieeruv arv tähti, arve ja sümboleid -. *$/+%</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Varieeruv arv ASCII sümboleid ja erifunktsioonide sümboleid. Algab esialgsele märgistikule vastava sobiva algustähega. UCC/EAN-128 nõuab, et algustähele järgneks tingimata FNC 1 sümbol.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Varieeruv arv arve ja sümboleid -$:/.+ABCD</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Varieeruv arv arve</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Varieeruv arv arve. ITF-14 koosneb 14 arvust ega sisalda kontrollarvu</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Varieeruv arv arve ja suurtähti</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Varieeruv arv kuueteistkümnendsüsteemi märke</translation>
    </message>
    <message>
        <source>Error opening file: %1</source>
        <translation>Viga faili avamisel: %1</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Ribakood pole täielik</translation>
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
        <translation>Ribakoodi generaator</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Ribakood</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tüüp:</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>Koo&amp;d:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Vali mõni saadaolevatest ribakoodi tüüpidest</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Kood ise arvuna. Vaata täpsemalt allolevat abiteadet.</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Lähtesta ribakoodi näited</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>Tekst&amp;i kaasamine ribakoodi</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Märkimise korral võib ribakoodis olla ka tähti</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>Tühimär&amp;gi kaitse</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Noolte joonistamine koodi järel tühiku tagamiseks</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>Ko&amp;ntrollsumma kaasamine</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Kontrollsumma genereerimine ja kaasamine ribakoodi</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Kontrolls&amp;umma arvu kaasamine</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Kontrollsumma arvu lisamine ribakoodi teksti</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Värvid</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Taust</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Taustavärv ribade taga</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Ribad</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Ribakoodi ribade värv</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Teksti ja arvude värv</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Siin näeb vihjeid ja abi</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Tulemuse eelvaatlus. 72dpi näide.</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Visandimapi kataloogi valik</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Visandimapp (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Imporditava visandimapifaili valik</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Kataloogi valik</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Nimeta ümber</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Kustuta</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Nimi &quot;%1&quot; ei ole unikaalne.
Palun vali mõni muu nimi.</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nimi:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Uus kirje</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Visandimapp</translation>
    </message>
    <message>
        <source>Create a new scrapbook page</source>
        <translation>Loo uus visandimapi lehekülg</translation>
    </message>
    <message>
        <source>Load an existing scrapbook</source>
        <translation>Ava olemasolev visandimapp</translation>
    </message>
    <message>
        <source>Save the selected scrapbook</source>
        <translation>Salvesta valitud visandimapp</translation>
    </message>
    <message>
        <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
        <translation>Impordi visandimapifail Scribuse versioonist &lt;=1.3.2</translation>
    </message>
    <message>
        <source>Close the selected scrapbook</source>
        <translation>Sulge valitud visandimapp</translation>
    </message>
    <message>
        <source>Copy To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move To:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Liiguta järjehoidjat</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Lisa järjehoidja</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Järjehoidjad</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Järjehoidjad</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Ikooni paigutus</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Välimus:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Ainult pealdis</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Ainult ikoon</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Pealdis ikooni all</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Pealdis ikooni kohal</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Pealdis ikoonist paremal</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Pealdis ikoonist vasakul</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Pealdis ikooni peal</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skaleerimine:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Alati</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Kui ikoon on liiga väike</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Kui ikoon on liiga suur</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Mitte kunagi</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skaleerimine viis:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proportsionaalne</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Mitteproportsionaalne</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikoon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Lähtesta</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Värvih&amp;alduse lubamine</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Süsteemsed profiilid</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB pildid:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK pildid:</translation>
    </message>
    <message>
        <source>&amp;RGB Solid Colors:</source>
        <translation>&amp;RGB ühtlased värvid:</translation>
    </message>
    <message>
        <source>&amp;CMYK Solid Colors:</source>
        <translation>&amp;CMYK ühtlased värvid:</translation>
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
        <source>Rendering Intents</source>
        <translation>Renderdamisviisid</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Pertseptuaalne</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Suhteline kolorimeetriline</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Küllastus</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absoluutne kolorimeetriline</translation>
    </message>
    <message>
        <source>Pictures:</source>
        <translation>Pildid:</translation>
    </message>
    <message>
        <source>Sol&amp;id Colors:</source>
        <translation>Ühtlased värv&amp;id:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Printeri sim&amp;uleerimine ekraanil</translation>
    </message>
    <message>
        <source>Convert all colors to printer space</source>
        <translation>Kõigi värvide teisendamine printeriruumi</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Värviulatusest väl&amp;jajäävate värvide märkimine</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Must&amp;punkti kompenseerimine</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Imporditud CMYK piltide vaikimisi värviprofiil</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Imporditud RGB piltide vaikimisi värviprofiil</translation>
    </message>
    <message>
        <source>Default color profile for solid RGB colors on the page</source>
        <translation>Ühtlaste RGB värvide vaikimisi värviprofiil leheküljel</translation>
    </message>
    <message>
        <source>Default color profile for solid CMYK colors on the page</source>
        <translation>Ühtlaste CMYK värvide vaikimisi värviprofiil leheküljel</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Värviprofiil, mille oled genereerinud või saanud tootjalt.
See profiil peab vastama monitorile ega tohi olla üldine profiil (s.t. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Printeri tootjapoolne värviprofiil.
See profiil peab vastama printerile ega tohi olla üldine profiil (s.t. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Ühtlaste värvide vaikimisi renderdamisviis. Kui sa ei tea, kuidas seda muuta,
tuleks valida suhteline kolorimeetriline või pertseptuaalne.</translation>
    </message>
    <message>
        <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Piltide vaikimisi renderdamisviis. Kui sa ei tea, kuidas seda muuta,
tuleks valida suhteline kolorimeetriline või pertseptuaalne.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Niinimetatud väljundvaate lubamine, mis näitab printeri profiili alusel,
millised näevad dokumendi värvid välja trükkimisel.</translation>
    </message>
    <message>
        <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
        <translation>Täieliku värvihaldusega keskkonna simuleerimine:
kõik värvid, nii RGB kui CMYK, teisendatakse printeri värviruumi.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Ekraanil näidatakse värve, mida ei pruugi olla võimalik korralikult trükkida.
See nõuab väga täpseid profiile ja on mõeldud lihtsalt hoiatuseks.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Mustpunkti kompenseerimisega saab parandada fotode kontrasti.
Kui dokumendis leidub fotosid, on soovitatav see valik sisse lülitada.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Värvi muutmine</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nimi:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Värvi&amp;mudel</translation>
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
        <source>Is Spot Color</source>
        <translation>Spot-värv</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Kokkutrükivärv</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Uus</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Vana</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-värvikaart</translation>
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
        <translation>Dünaamilised värviribad</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Staatilised värviribad</translation>
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
        <translation>Värvi nimega &quot;%1&quot; ei saa luua.
See nimi on reserveeritud läbipaistvale värvile.</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation type="obsolete">Värvi nimi ei ole unikaalne</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and flourescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
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
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Hiire paremat klahvi all hoides näeb pisipilti. Klahviga Insert saab glüüfi allolevasse valikukasti lisada, klahviga Delete viimati lisatud glüüfi eemaldada.</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Märgi valimine:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Font:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Märgiklass:</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation>L&amp;isatava märgi kood:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>L&amp;isa</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Lisab märgid teksti kursori asukohta</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Kustutab aktiivse valiku.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>Sulgeb dialoogi ja naaseb teksti redigeerimise juurde</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Siia saab otse kirjutada Unicode&apos;i neljakohalise arvväärtuse</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Täielik märgistik</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Ladina alusosa</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 täiendosa</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Ladina laiendatud A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Ladina laiendatud B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Üldised kirjavahemärgid</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Üla- ja alaindeksid</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Rahasümbolid</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Tähelaadsed sümbolid</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Arvuvormid</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Nooled</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matemaatilised märgid</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Joonekombinatsioonid</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Plokid</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geomeetrilised kujundid</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Mitmesugused sümbolid</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Piltmärgid</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Väikesed märgid</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligatuurid</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Erisümbolid</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Kreeka</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Kreeka laiendatud</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Kirillitsa</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Kirillitsa täiendosa</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Araabia</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Araabia laiendatud A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Araabia laiendatud B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Heebrea</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Probleeme ei esinenud</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Lehekülg</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Vabad objektid</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Esines probleeme</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Trükieelne kontroll</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Elemendid</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Probleemid</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Aktiivne profiil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignoreeri vigu</translation>
    </message>
    <message>
        <source>Check again</source>
        <translation>Kontrolli uuesti</translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation>Puuduvad glüüfid</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Teksti ülejooksmine</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objekt ei paikne leheküljel</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Puuduv pilt</translation>
    </message>
    <message>
        <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
        <translation>Pildi lahutus on alla %1 DPI, praegu %2 x %3 DPI-d</translation>
    </message>
    <message>
        <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
        <translation>Pildi lahutus on üle %1 DPI, praegu %2 x %3 DPI-d</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objekt on läbipaistev</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekt on PDF-i annotatsioon või väli</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objekt on põimitud PDF-fail</translation>
    </message>
    <message>
        <source>Image is GIF</source>
        <translation>Pilt on GIF</translation>
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
        <translation>Stiilide valik</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Saadaolevad stiilid</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Kataloogi valik</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Kogumine...</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Faili ei õnnestunud koguda:
%1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Hoiatus</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Ei õnnestunud koguda kõigi faile väljundiks failile:
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Värvid</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Impordi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeeri</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Klooni</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Eemalda kasutud</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Värvikomplektid</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktiivne värvikomplekt:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Salvesta värvikomplekt</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Vali avatav värvikomplekt</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Salvesta aktiivne värvikomplekt</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Eemalda kasutamata värvid dokumendi aktiivsest värvikomplektist</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Impordi olemasolevast dokumendist värvid aktiivsesse värvikomplekti</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Loo aktiivses värvikomplektis uus värv</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Redigeeri valitud värvi</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Kopeeri valitud värv</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Kustuta valitud värv</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Muuda aktiivne värvikomplekt vaikimisi värvikomplektiks</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nimi:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Nime valik</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumendid (*.sla *.sla.gz *.scd *.scd.gz);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumendid (*.sla *.scd);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>%1 koopia</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Uus värv</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Other Files (*.eps *.ps *.ai);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;Other Files (*.eps *.ps *.ai);;All Files (*)</source>
        <translation type="unfinished"></translation>
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
        <source>Cr&amp;eate color...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Välju</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Värv</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
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
        <translation>Värviratas</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Värv</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
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
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Viga: </translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Värv %1 on juba olemas!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Värv %1 lisatud.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Uus värv</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Värviratas...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Värviseadistuste abiline</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Värvivalija, mis arvestab värviteooriat.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Rakenda</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Loobu</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Hoiatus</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Üks lehekülg</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Kaks lehekülge</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>Kolm lehekülge</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4 lehekülge</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Vasakpoolne</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Keskmine</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>Vasak keskmine</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Parem keskmine</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Parempoolne</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation>Esmaspäev</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>Teisipäev</translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation>Kolmapäev</translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation>Neljapäev</translation>
    </message>
    <message>
        <source>Friday</source>
        <translation>Reede</translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation>Laupäev</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>Pühapäev</translation>
    </message>
    <message>
        <source>January</source>
        <translation>Jaanuar</translation>
    </message>
    <message>
        <source>February</source>
        <translation>Veebruar</translation>
    </message>
    <message>
        <source>March</source>
        <translation>Märts</translation>
    </message>
    <message>
        <source>April</source>
        <translation>Aprill</translation>
    </message>
    <message>
        <source>May</source>
        <translation>Mai</translation>
    </message>
    <message>
        <source>June</source>
        <translation>Juuni</translation>
    </message>
    <message>
        <source>July</source>
        <translation>Juuli</translation>
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
        <translation>Oktoober</translation>
    </message>
    <message>
        <source>November</source>
        <translation>November</translation>
    </message>
    <message>
        <source>December</source>
        <translation>Detsember</translation>
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
        <translation>Varjund:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Läbipaistmatus:</translation>
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
        <source>Move Vector</source>
        <translation>Liiguta vektorit</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Rõhtne üleminek</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Püstine üleminek</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonaalne üleminek</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Ristdiagonaalne üleminek</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Radiaalne üleminek</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Vaba lineaarne üleminek</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Vaba radiaalne üleminek</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Muuda joonevärvi omadusi</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Muuda täitevärvi omadusi</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Värviküllastus</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Normaalne või ülemineku täitmise meetod</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Määra valitud värvi läbipaistvus</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Liiguta üleminekuvektori algust hiire vasakut nuppu ja üleminekuvektori lõppu hiire paremat nuppu all hoides</translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Ühtesulamise režiim:</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">Tumendamine</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Helendamine</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Korrutamine</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Ekraan</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Ülekatmine</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Külm valgus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Soe valgus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Lahutamine</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="unfinished">Väljajätmine</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Värvi helestamine</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Värvi tumestamine</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Toon</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Valgsus</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>CSV importija valikud</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Väljade eraldaja:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Väärtuste eraldaja:</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Esimene rida on päis</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Printeri valikud</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Lehekülgede valim</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Kõik leheküljed</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Ainult paarisleheküljed</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Ainult paaritud leheküljed</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Peegeldamine</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Jah</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientatsioon</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Rõhtpaigutus</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Lehekülgi lehele</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>lehekülg lehe kohta</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>lehekülge lehe kohta</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Valik</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Väärtus</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>See dialoog näitab trükkimisel kasutatavaid CUPS-i valikuid. Täpsed parameetrid sõltuvad printeri draiverist. CUPS-i toetust saab kontrollida dialoogis Abi -&gt; Info. Seal tuleb otsida kirjet C-C-T, milles C=CUPS, teine C=littlecms ja T=TIFF. Puuduva toetuse korral seisab vastava teegi asemel *.</translation>
    </message>
</context>
<context>
    <name>CurveWidget</name>
    <message>
        <source>Open</source>
        <translation type="unfinished">Avamine</translation>
    </message>
    <message>
        <source>Curve Files (*.scu);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Salvestamine</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished">Ei õnnestunud kirjutada faili:
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
        <translation>&amp;Faili tihendamine</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>Fo&amp;ntide kaasamine</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodeering:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Liigub määratud dokumentide kataloogi.
Selle saab määrata seadistustes.</translation>
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
        <translation type="unfinished"></translation>
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
        <source>Set &amp;RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Kasutaja katkestas toimingu</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Värvi kustutamine</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Kustutatav värv:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Asendus:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Lehekülgede kustutamine</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Kustutatakse alates:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>kuni:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Stiili kustutamine</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Kustutatav stiil:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Asendus:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Stiil puudub</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Import ebaõnnestus</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Word&apos;i dokumendi import ebaõnnestus
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokumendi info</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Pealkiri:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>A&amp;utor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Võtmesõnad:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Kirjeldus:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Avaldaja:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Kaasautorid:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Kuupä&amp;ev:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Tüüp:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Vorming:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>I&amp;dentifikaator:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>All&amp;ikas:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Keel:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Seos:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Hõlmamine:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Õi&amp;gused:</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Täiendav &amp;info</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Isik või organisatsioon, kes vastutab dokumendi sisu eest tervikuna. Selle välja saab info huvides põimida nii Scribuse dokumenti kui ka PDF-i metaandmetesse.</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Dokumendi nimi. Selle välja saab info huvides põimida nii Scribuse dokumenti kui ka PDF-i metaandmetesse.</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Dokumendi sisu ülevaade. See väli sisaldab dokumendi lühikirjeldust või kokkuvõtet. See põimitakse eksportimisel PDF-i.</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Dokumendi sisu teema. See väli on mõeldud PDF-i põimitavate dokumendi võtmesõnade jaoks, mis aitavad PDF-failides otsida ja neid indekseerida.</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Isik või organisatsioon, kes vastutab dokumendi avaldamise eest</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Isik või organisatsioon, kes on andnud panuse dokumendi sisu loomisse</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Dokumendi loomisega seotud kuupäev vormingus YYYY-MM-DD (vastavalt ISO 8601 nõuetele)</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Dokumendi sisu iseloom või žanr, nt. kategooria, funktsioon, žanr vms</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Dokumendi füüsiline või digitaalne kuju. Mainida tasuks nii andmekandja tüüpi kui ka mõõtmeid. Siin saab ära kasutada MIME tüüpide jaoks mõeldud RFC2045 ja RFC2046.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Üheselt mõistetav viide dokumendile kindlas kontekstis, n. ISBN või URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Viide dokumendile, millest aktiivne dokument on tuletatud, nt. ISBN või URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Keel, milles on kirja pandud dokumendi sisu. Tavaliselt keelekood (ISO-639 järgi), millele võib lisanduda sidekriips ja riigikood (ISO-3166 järgi), nt. et, en-GB, fr-CH.</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Viide seotud dokumendile, võib olla formaalne identifikaator, nt. ISBN või URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Dokumendi sisu haare või ulatus, võib sisaldada asukohta, aega, jurisdiktsiooni</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Info dokumendiga seotud õiguste kohta, nt. autoriõigus, patent, kaubamärk</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Lisa dokumendile lehekülgede nummerdamise sektsioon. Uus sektsioon lisatakse valitud sektsiooni järele.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Kustuta valitud sektsioon</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation type="unfinished"></translation>
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
        <source>Page Number Out Of Bounds</source>
        <translation>Leheküljenumber väljub piiridest</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Sisestatud väärtus väljub aktiivse dokumendi leheküljenumbrite vahemikust (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Dokumendi sektsioonid</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Näitamine</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Alates</translation>
    </message>
    <message>
        <source>To</source>
        <translation>Kuni</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stiil</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Algus</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
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
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>On seotud elemendiga</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>On eellane elemendile</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>On järglane elemendile</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Tekstikastid</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Pildikastid</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Tõeväärtus</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Täisarv</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Reaalarv</translation>
    </message>
    <message>
        <source>String</source>
        <translation>String</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokumendi elemendi atribuudid</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tüüp</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Väärtus</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameeter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Seos</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Seos elemendiga</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
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
        <translation>Printeri seadistused</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Trükkimise sihtkoht</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fail</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Valikud...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fail:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Al&amp;ternatiivne trükkimiskäsk</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Kä&amp;sk:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Vahemik</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>&amp;Kõik</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>&amp;Aktiivne lehekülg</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>T&amp;rükkimisvahemik</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Koo&amp;piate arv:</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Normaalne trükk</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Trükkimine separeeritult</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Värvitrükk (kui võimalik)</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Halltoonis trükk</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Kõik</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Tsüaan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Kollane</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Must</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript tase 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript tase 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript tase 3</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Valikud</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Lehekülg</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Lehekülgede peegeldamine rõhtsalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Lehekülgede peegeldamine püstiselt</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Andmekandja suuruse määramine</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Kärpimine lehekülje veeriste juures</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Värv</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>UCR-i (Under Color Removal) rakendamine</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Spot-värvide teisendamine protsessivärvideks</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Ületrükirežiim</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>ICC profiilide rakendamine</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Muud valikud</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Eelvaatlus...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Trüki</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Trükitava lehekülje veeristest väljapoole jäävaid objekte ei näidata</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Sisesta komadega eradatud arvud või märgid:
* tähistab kõiki lehekülgi, 1-5 lehekülgede vahemikku,
konkreetsed leheküljenumbrid vastavaid lehekülgi.</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Alternatiivse trükkimishalduri (nt. kprinter või gtklp) kasutamine täiendavate trükkimisvalikute huvides</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>PostScripti taseme määramine.
Tase 1 või 2 võib tekitada hiiglaslikud failid.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Võimalus lülitada välja mõningad tsüaanist, kollasest ja magentast koosnevad halli toonid ning kasutada selle asemel musta. UCR mõjutab kõige rohkem piltide neid osi, kus valitsevad neutraalsed ja/või tumedad toonid, mis on suhteliselt lähedal hallile. Selle valiku kasutamisel võib paraneda mõningate piltide trükikvaliteet, kuid alati tuleb seda katse-eksituse meetodil ise järele proovida. UCR vähendab üleküllastuse võimalust CMY värvides.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Sisselülitamisel teisendatakse spot-värvid komposiitvärvideks. Kui sa ei kavatse just saata trükikotta spot-värve, on mõttekas see sisse lülitada.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Sisselülitamisel kehtestatakse dokumendile üldine ületrükirežiim, mis tühistab objektide määratlused</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Võimaldab värvihalduse korral põimida trükkimisandmetesse ICC profiilid</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Võimaldab konkreetselt määrata kindlaks PostScript-faili andmekandja (paberi, kile vms.) suuruse. Kui printer seda otseselt ei nõua, ei ole seda mõtet sisse lülitada.</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Printeri seadistuste hankimine ebaõnnestus</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvestamine</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript-failid (*.ps);;Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing PostScript</source>
        <translation>PostScripti import</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>PostScripti analüüsimine:</translation>
    </message>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Faili:
%1
import ebaõnnestus!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Saatuslik viga</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Elementide genereerimine</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>%1 pildi teisendamine ebaõnnestus!</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Viga</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Stiili redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nimi:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Märk</translation>
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
        <translation>Vahemaad</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fikseeritud reavahe</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automaatne reavahe</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Joondamine alusjoontele</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Süvisinitsiaalid</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Ridu:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Kaugus tekstist:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabeldusmärgid ja taandus</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Lõigustiili eelvaatlus</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Taust</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Vali heleda tekstistiili paremaks nägemiseks</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Lõigustiili nimi</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Valitud teksti või objekti font</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Fondi suurus</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Teksti täidise värv</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Teksti äärejoone värv</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Määrab ridades süvisinitsiaalide kogukõrguse</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Määrab vahe süvisinitsiaali ja teksti vahel</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Vahe enne lõiku</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Vahe pärast lõiku</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Märkide koondamine</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Nihe märkide alusjoone suhtes</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Reavahe</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation>Klõpsa reavahe režiimi valimiseks</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Näitab näidisteksti antud lõigustiiliga</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Stiili nimi pole unikaalne</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Redaktor</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ava...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Salvesta &amp;kui...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Salvesta ja välju</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Välju salvestamata</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Võta tagasi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Tee uuesti</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Lõika</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Aseta</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Hangi väljanimed</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fail</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeerimine</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScriptid (*.js);;Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Pildiefektid</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Valikud:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Värv:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Varjund:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Heledus:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Raadius:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Väärtus:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Poster:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Saadaolevad efektid</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Pehmendamine</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Heledus</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Toonimine</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Halltoonid</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverteerimine</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Poster</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Teravustamine</translation>
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
        <translation>Kasutatavad efektid</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
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
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Ekspordikataloogi valik</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Eksport piltidena</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Ekspordikataloog:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Valikud</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Pildi &amp;tüüp:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvaliteet:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Lahutus:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Suurus:</translation>
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
        <translation>Vahemik</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Aktiivne lehekülg</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Kõik leheküljed</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Vahemik</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Image size in Pixels</source>
        <translation>Pildi suurus pikslites</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Lehekülgede vahemiku eksport</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Sisesta komadega eradatud arvud või märgid:
* tähistab kõiki lehekülgi, 1-5 lehekülgede vahemikku,
konkreetsed leheküljenumbrid vastavaid lehekülgi.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Kõigi lehekülgede eksport</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Ainult aktiivse lehekülje eksport</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Piltide lahutus
Ekraanile mõeldud piltide korral võiks see olla 72 dpi</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Piltide suurus. 100% tähendab muudatusteta, 200% kaks korda suuremat jne.</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Piltide kvaliteet. 100% on parim, 1% halvim kvaliteet</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Saadaolevad ekspordivormingud</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Väljundkataloog, kuhu pildid salvestatakse.
Eksportfaili nimeks saab &apos;dokumendinimi-leheküljenumber.failitüüp&apos;.</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Muuda väljundkataloogi</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Laiendatud pildi omadused</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Tumendamine</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Helendamine</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Toon</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>KüllastusKüllastus</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>VärvVärv</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Valgsus</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Korrutamine</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Ekraan</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Hägunemine</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Ülekatmine</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Külm valgus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Soe valgus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Lahutamine</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Väljajätmine</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Värvi helestamine</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Värvi tumestamine</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Väljajätmine</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Ühtesulamise režiim:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Läbipaistmatus:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Taust</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Kihid</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Trajektoore ei kasutata</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Trajektoorid</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Suurus:</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Lahutus:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Teadmata</translation>
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
        <translation>Halltoonid</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Värviruum:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Pealkiri:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Pealkiri puudub</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribuse dokument</translation>
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
        <translation>Mõned selles dokumendis kasutatavad fondid on asendatud:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> asendati fondiga: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Saadaolevad fondid</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Fondiasendused</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Lisaasukohad</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Fondi nimi</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Kasutusel</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="obsolete">Põimimine:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Alamhulk</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Fondifaili asukoht</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Saadaolevad fondid</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Fondi nimi</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Asendus</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Fondiasendused</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>Lis&amp;a...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Eemalda</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation>Fontide asukohad saab määrata ainult seadistustes ja ainult siis, kui ükski dokument pole avatud. Sulge kõik avatud dokumendid ning muuda siis fontide asukohta menüükäsuga Redigeerimine-&gt;Seadistused.</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Lis&amp;aasukohad</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Kataloogi valik</translation>
    </message>
    <message>
        <source>Embed in PostScript</source>
        <comment>font preview</comment>
        <translation>Põimimine PostScripti</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Põdur Zagrebi tšellomängija-följetonist Ciqo külmetas kehvas garaažis</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Lisa valitud font stiili- ja fondimenüüsse</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Sulge eelvaatlus</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Siin saab kiiresti vajaliku fondi leida. Otsing on tõstutundetu. Otsingus võib kasutada metamärke (*, ?, [...]). Näide: t* leiab kõik fondid, mille alguses seisab t või T. *bold* leiab kõik fondid, mille nimes esineb sõna bold, bolder vms.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Alusta otsimist</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Valitud fondi suurus</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Näidet näeb pärast klahvi vabastamist</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Kasutaja</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Süsteem</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Fontide eelvaatlus</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Kiirotsing:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Ot&amp;si</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Fondi nimi</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Dok</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tüüp</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Alamhulk</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Õigused</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Fondi suurus:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Näitetekst</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>&amp;Määra</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Lähtesta tekst</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
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
        <translation>&amp;Fontide eelvaatlus...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Fontide eelvaatluse dialoog</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Saadaolevate fontide sortimine, otsimine ja sirvimine.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Fondiasendus</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Dokument sisaldab fonte, mida pole sinu süsteemi paigaldatut. Palun vali neile sobiv asendus. Loobumisel dokumenti ei avata.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Originaalfont</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Asendusfont</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Alati kasutatakse määratud asendusi</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Katkestab fontide asendamise ja dokumendi avamise</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Sisselülitamisel kasutab Scribus määratud asendusi puuduvate fontide jaoks ka edaspidi. Seda saab tühistada või muuta menüükäsuga Redigeerimine-&gt;Seadistused-&gt;Fondid.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Kui valida OK ja siis salvestada, kehtestatakse asendused dokumendis püsivalt.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Lisa, muuda või eemalda värvimarkereid</translation>
    </message>
    <message>
        <source>Position:</source>
        <translation>Asukoht:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Edit Guide</source>
        <translation>Juhtjoone redigeerimine</translation>
    </message>
    <message>
        <source>Enter a position:</source>
        <translation>Asukoht:</translation>
    </message>
    <message>
        <source>New Guide</source>
        <translation>Uus juhtjoon</translation>
    </message>
</context>
<context>
    <name>GuideManagerBase</name>
    <message>
        <source>Manage Guides</source>
        <translation>Juhtjoonte haldamine</translation>
    </message>
    <message>
        <source>&amp;Standard</source>
        <translation>&amp;Standard</translation>
    </message>
    <message>
        <source>Horizontals</source>
        <translation>Rõhtjooned</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Juhtjoon</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="obsolete">Ühik</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Verticals</source>
        <translation>Püstjooned</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>L&amp;isa</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Kustuta</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Juhtjoonte lukustamine</translation>
    </message>
    <message>
        <source>Appl&amp;y to All Pages</source>
        <translation>&amp;Rakendamine kõigile lehekülgedele</translation>
    </message>
    <message>
        <source>Alt+Y</source>
        <translation>Alt+Y</translation>
    </message>
    <message>
        <source>A&amp;utomatic</source>
        <translation>A&amp;utomaatne</translation>
    </message>
    <message>
        <source>&amp;Number:</source>
        <translation>&amp;Arv:</translation>
    </message>
    <message>
        <source>U&amp;se Gap:</source>
        <translation>&amp;Vahe:</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Nu&amp;mber:</source>
        <translation>A&amp;rv:</translation>
    </message>
    <message>
        <source>Use &amp;Gap:</source>
        <translation>Va&amp;he:</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Refer To</source>
        <translation>Sidumine</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Le&amp;hekülg</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>M&amp;argins</source>
        <translation>V&amp;eerised</translation>
    </message>
    <message>
        <source>S&amp;election</source>
        <translation>&amp;Valik</translation>
    </message>
    <message>
        <source>&amp;Misc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete &amp;Page Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all guides from the current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete &amp;Document guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all guides from the current document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation>Sisukord</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Link</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Sisukord</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Ot&amp;si</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Asjakohasus</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>O&amp;tsing</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>&amp;Kustuta kõik</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Järjehoidjad</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Trüki...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Välju</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fail</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>Ot&amp;si...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Otsi &amp;järgmine</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Otsi &amp;eelmine</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>Lis&amp;a järjehoidja</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>&amp;Kustuta kõik</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Järjehoidjad</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribuse abi</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Otsimine on tõstutundetu</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Vabandust, käsiraamatut pole!
Palun otsi saidilt http://docs.scribus.net värsket dokumentatsiooni
ja saidilt www.scribus.net allalaadimisvõimalusi.</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Otsimine</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Otsingusõna:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Uus järjehoidja</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Uue järjehoidja nimi:</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Võimalik poolitus</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Nõus</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Jäta vahele</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;Poolitamisettepanekud</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>&amp;Teksti automaatne poolitamine kirjutamise ajal</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Keel:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Lü&amp;him poolitatav sõna:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Poolitamisega lõppevaid ridu &amp;järjestikku:</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Sisselülitamise korral avatakse menüükäsu Ekstra-&gt;Teksti poolitamine valimisel dialoog, kus näidatakse iga sõna võimalikke poolitusi.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Lülitab sisse teksti automaatse poolitamise kirjutamise ajal.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Väikseima sõna pikkus, mida veel poolitatakse.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Järjestikuste poolitamisega lõppevate ridade maksimaalne arv.
0 tähendab, et piirang puudub.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Pildi info</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Üldine info</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Kuupäev/kellaaeg:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Põimitud profiil:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Jah</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ei</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Profiili nimi:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Põimitud asukohad:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Kihid:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF-info</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Kunstnik:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Kommentaar:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Kasutaja kommentaar:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kaamera mudel:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Kaamera tootja:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Kirjeldus:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Autoriõigus:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Skänneri mudel:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Skänneri tootja:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Impordi &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Impordib EPS-faile</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Impordib EPS-failid aktiivsesse dokumenti,
muutes nende vektorandmed Scribuse objektideks.</translation>
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
        <translation>Lehekülje lisamine</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>L&amp;isatakse</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>lehekülg(e)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>enne lehekülge</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>pärast lehekülge</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>lõppu</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Leheküljetoorikud</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Leheküljetoorik:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Lehekülje suurus</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Suurus:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntatsioon:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Rõhtpaigutus</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Objektide liigutamine koos leheküljega</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Right</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertAFrame</name>
    <message>
        <source>Open</source>
        <translation type="unfinished">Avamine</translation>
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
        <translation type="unfinished">&amp;Tekstikast</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished">P&amp;ildikast</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation type="unfinished">T&amp;abel</translation>
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
        <translation type="unfinished">Hulknurk</translation>
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
        <translation type="unfinished">Y:</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Laius:</translation>
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
        <translation type="unfinished">&amp;Loobu</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Tabeli lisamine</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Ridade arv:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Veergude arv:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>JavaScriptide redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Redigeeri...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>Lis&amp;a...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Lisab uue skripti, määrates samas kindlaks samanimelise funktsiooni. Kui soovid seda skripti kasuada avamistoimingu (Open Action) skriptina, ära kindlasti funktsiooni nime muuda.</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Uus skript:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Uus skript</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Kas tõesti see skript kustutada?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Jah</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ei</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>Toiming</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Aktiivne klahv</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Toimingu klahvi valimine</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Klahv puudub</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Kas&amp;utaja määratud klahv</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Määra klahv</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Laaditavad kiirklahvikomplektid</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Laadi</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Impordi...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Ekspordi...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Lä&amp;htesta</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Saadaolevad kiirklahvikomplektid</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Laadi valitud kiirklahvikomplekt</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Impordib kiirklahvikomplekti praegusse seadistusse</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Ekspordib aktiivsed kiirklahvid imporditavasse faili</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Laadib uuesti Scribuse vaikimisi kiirklahvid</translation>
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
        <translation>See klahvikombinatsioon on juba kasutusel</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Klahvikomplektide XML-failid (*.ksxml)</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Kihi kustutamine</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Kas kustutada ka kõik selle kihi objektid?</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Kihid</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Lisa uus kiht</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Kustuta kiht</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Too kiht ettepoole</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Vii kiht tahapoole</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Läbipaistmatus:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>(sp)%</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Ühtesulamise režiim:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Tumendamine</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Helendamine</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Korrutamine</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Ekraan</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Ülekatmine</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Külm valgus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Soe valgus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Lahutamine</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Väljajätmine</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Värvi helestamine</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Värvi tumestamine</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Väljajätmine</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Toon</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Valgsus</translation>
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
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Joonestiilide redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Impordi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeeri</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Klooni</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>%1 koopia</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Uus stiil</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Kas tõesti see stiil kustutada?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Jah</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ei</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.sla.gz *.scd *.scd.gz);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.scd);;Kõik failid (*)</translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Joone jämedus:</translation>
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
        <translation>Lameots</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Ruutots</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Ümarots</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Nurkühendus</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Längühendus</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Ümarühendus</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Ühtlane joon</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Punktiirjoon</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Punktjoon</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Kriipspunktjoon</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Kriipspunktpunktjoon</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Lorem Ipsumi valik</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Allikas:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML-fail:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Lõigud:</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Lehekülje omaduste haldamine</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Lehekülje suurus</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Suurus:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntatsioon:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Rõhtpaigutus</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Objektide liigutamine koos leheküljega</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tüüp:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Veerisejooned</translation>
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
        <translation type="unfinished">Normaalne</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>Preset Layouts:</source>
        <translation>Valmispaigutused:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;All:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>Ü&amp;lal:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Paremal:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vasakul:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Veeriseseadistusi rakendatakse kõigile lehekülgedele</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Veeriste muudatused rakendatakse kõigile dokumendi olemasolevatele lehekülgedele</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Printeri veerised...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Veeriste import valitud leheküljesuurusele saadaolevatelt printeritelt</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Vahemaa ülemise veerisejoone ja lehekülje serva vahel</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Vahemaa alumise veerisejoone ja lehekülje serva vahel</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Vahemaa vasaku veerisejoone ja lehekülje serva vahel. Kui valitud on kaks lehekülge kõrvuti, saab seda veeriste vahet kasutada köitmiseks vajalike veeriste korrigeerimiseks.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Vahemaa parema veerisejoone ja lehekülje serva vahel. Kui valitud on kaks lehekülge kõrvuti, saab seda veeriste vahet kasutada köitmiseks vajalike veeriste korrigeerimiseks.</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>S&amp;isemine:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Vä&amp;limine:</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Leheküljetoorikute redigeerimine</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Klooni valitud leheküljetoorik</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Kustuta valitud leheküljetoorik</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Lisa uus leheküljetoorik</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Impordi leheküljetoorikuid mõnest muust dokumendist</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Kas tõesti kustutada see leheküljetoorik?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Jah</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ei</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nimi:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Uus leheküljetoorik</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">%1 koopia</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nimi:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Uus leheküljetoorik</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Uus leheküljetoorik %1</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Koopia nr. %1 toorikust </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
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
        <translation>Mitmekordne kloonimine</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>Koo&amp;piate arv:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Ni&amp;he rõhtsuunas:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Nihe püstsuunas:</translation>
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
        <translation>Mõõtmine</translation>
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
        <translation>Nurk:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Pikkus:</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Master Page</source>
        <translation>Leheküljetooriku import</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Lehekülgede import</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Dokumendist:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Imporditakse leheküljed:</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Imporditakse leheküljetoorik</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Sisesta komadega eradatud arvud või märgid: * tähistab kõiki lehekülgi, 1-5 lehekülgede vahemikku, konkreetsed leheküljenumbrid vastavaid lehekülgi.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> 0 leheküljest</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Leheküljed luuakse</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>enne lehekülge</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>pärast lehekülge</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>lõppu</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Impordi</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.sla.gz *.scd *.scd.gz);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.scd);;Kõik failid (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> %1 leheküljest</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Puuduv font</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Font %1 ei ole paigaldatud.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ModeToolBar</name>
    <message>
        <source>Tools</source>
        <translation>Tööriistad</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Omadused...</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Lehekülgede liigutamine</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Lehekülje kopeerimine</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Lehekülgede liigutamine</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>leheküljele:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Koopiate arv:</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Enne lehekülge</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Pärast lehekülge</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Lõppu</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Lehekülgede liigutamine:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fikseeritud reavahe</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automaatne reavahe</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Joondamine alusjoontele</translation>
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
        <translation>&amp;X-asukoht:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-asukoht:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Veergude vahe</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Veeru laius</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Stiil puudub</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Nimi &quot;%1&quot; ei ole unikaalne.&lt;br/&gt;Palun vali mõni muu nimi.</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Omadused</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>P&amp;ilt</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Kujund</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Joon</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Värvid</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geomeetria</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>Pöö&amp;ramine:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Baaspunkt:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Tase</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Kujund:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Muuda kuju...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Ümarad
n&amp;urgad:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Teksti kaugus</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>V&amp;eergude arv:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">Va&amp;he:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>Ü&amp;lal:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;All:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vasakul:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Paremal:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabeldusmärgid...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Trajektoori teksti omadused</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Kõvera näitamine</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Alguse nihe:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Kaugus kõverast:</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Täitmise reegel</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Paaris-paaritu</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Mittenull</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="obsolete">Teksti&amp;voog ümber kasti</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Üm&amp;britsev kast</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Kont&amp;uurjoon</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;iil:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Keel:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Vaba skaleerimine</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Tegelik X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Tegelik Y-DPI:</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-sk&amp;aala:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-s&amp;kaala:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Skaleerimine kas&amp;ti suuruse järgi</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roportsionaalne</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Sisendprofiil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Renderdamisviis:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Pertseptuaalne</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Suhteline kolorimeetriline</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Küllastus</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absoluutne kolorimeetriline</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Vasak ots</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Otspunktid</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Baaspunkt:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Joone tüüp:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Algusnool:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Lõppnool:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>J&amp;oone jämedus:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Se&amp;rvad:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Nurkühendus</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Längühendus</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Ümarühendus</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Lameots</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Ruutots</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Ümarots</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Otsad:</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Lahtri jooned</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Joon üleval</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Joon vasakul</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Joon paremal </translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Joon all</translation>
    </message>
    <message>
        <source>Overprinting</source>
        <translation>Ületrükk</translation>
    </message>
    <message>
        <source>Knockout</source>
        <translation>Väljajätmine</translation>
    </message>
    <message>
        <source>Overprint</source>
        <translation>Ületrükk</translation>
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
        <translation>Valitud objekti nimi</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Aktiivse baaspunkti asukoht rõhtteljel</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Aktiivse baaspunkti asukoht püstteljel</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Laius</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Kõrgus</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Objekti pööramine aktiivse baaspunkti suhtes</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punkt, millest arvestatakse mõõte või pööramisnurka</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Vali baaspunktiks ülemine vasak punkt</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Vali baaspunktiks ülemine parem punkt</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Vali baaspunktiks alumine vasak punkt</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Vali baaspunktiks alumine parem punkt</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Vali baaspunktiks keskpunkt</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Peegelda rõhtsalt</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Peegelda püstiselt</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Liiguta üks tase ettepoole</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Liiguta üks tase tahapoole</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Liiguta kõige ette</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Liiguta kõige taha</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Tähistab taset, millel objekt asub. 0 tähendab, et objekt asub kõige all.</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Lukusta objekt või eemalda lukustus</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Lukusta objekti suurus või eemalda lukustus</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Luba või keela objekti trükkimine</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="obsolete">Sunnib allpool asuvate kastide teksti kulgema ümber objekti</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Tekstivoo määramiseks kasutatakse objektipiirete asemel neid ümbritsevat kasti</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Tekstivoo jaoks kasutatakse teist joont, mis tugineb kastipiiretele</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Valitud teksti või objekti font</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Fondi suurus</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Nihe märkide alusjoone suhtes</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Märkide skaleerimislaius</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Märkide skaleerimiskõrgus</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Teksti äärejoone ja/või varju värv (sõltuvalt valikust). Kui mõlemad on valitud, kasutatakse üht ja sama värvi.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Valitud teksti värv. Kui sisse on lülitatud kontuuriefekt, on see täidise värv. Kui sisse on lülitatud varjuefekt, on see kõige ülemine värv.</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Teksti äärejoone värvi küllastus</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Teksti täidise värvi küllastus</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Kirjutamine paremalt vasakule</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Märkide koondamine</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Reavahe</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation>Klõpsa reavahe režiimi valimiseks</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Aktiivse lõigu stiil</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Tekstikastis kasutatav keel</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Vasaku või otspunktide seadistuste muutmine</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Joone muster</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Joone jämedus</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Jooneühenduste tüüp</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Jooneotste tüüp</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Aktiivse objekti joonestiil</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Vali kasti kuju...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Muuda kasti kuju...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Määra nurkade ümardamise raadius</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Veergude arv tekstikastis</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Vahe või veerulaiuse lülitamine</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Teksti kaugus kasti ülaservast</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Teksti kaugus kasti alaservast</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Teksti kaugus kasti vasakust servast</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Teksti kaugus kasti paremast servast</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Muuda tekstikasti tabeldusmärke...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Võimaldab pildil olla kastist erineva suurusega</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Pildi rõhtsuunaline nihe kastis</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Pildi püstsuunaline nihe kastis</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Pildi suuruse muutmine rõhtsuunas</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Pildi suuruse muutmine püstsuunas</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>X- ja Y-skaala hoitakse võrdsena</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Säilitatakse proportsioon</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Pilt sobitatakse kasti suurusega</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Kasti proportsioonide asemel kasutatakse pildi proportsioone</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Pildi lähteprofiil</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Pildi renderdamisviis</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Laius:</translation>
    </message>
    <message>
        <source>Text &amp;Flow Around Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Keelatud</translation>
    </message>
    <message>
        <source>Use Frame &amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Effects</source>
        <translation type="unfinished">Pildiefektid</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
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
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Stiili redigeerimine</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Lameots</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Ruutots</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Ümarots</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Nurkühendus</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Längühendus</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Ümarühendus</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Joone jämedus:</translation>
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
        <translation>Ühtlane joon</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Punktiirjoon</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Punktjoon</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Kriipspunktjoon</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Kriipspunktpunktjoon</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Nimi &quot;%1&quot; ei ole unikaalne.&lt;br/&gt;Palun vali mõni muu nimi.</translation>
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
        <translation>Edenemine</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Üldine edenemine:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Loobu</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Minu &amp;plugin</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Minu plugin</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Plugin töötas!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Uus dokument</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Uus dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Ol&amp;emasolev dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Viimati kasutatud &amp;dokument</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Seda dialoogi rohkem ei näidata</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Dokumendi lehekülje suurus: kas mõni standardne või kohandatud</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Dokumendi lehekülgede orientatsioon</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumendi lehekülgede laius. Kui valisid kohandatud suuruse, saab seda muuta.</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumendi lehekülgede kõrgus. Kui valisid kohandatud suuruse, saab seda muuta.</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Esialgne dokumendi lehekülgede arv</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Dokumendi redigeerimisel vaikimisi kasutatav mõõtühik</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Tekstikastide automaatne loomine uute lehekülgede lisamisel</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Veergude arv automaatselt loodud tekstikastides</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Automaatselt loodud veergude vahe</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Lehekülje suurus</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Suurus:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntatsioon:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Rõhtpaigutus</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Veerisejooned</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Valikud</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Le&amp;hekülgede arv:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Va&amp;ikimisi ühik:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Automaatsed tekstikastid</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Va&amp;he:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>V&amp;eergude arv:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Uus &amp;mallist...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Laadib valmispaigutusega dokumente</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Teiste või enda loodud dokumendipõhja avamine (tagab dokumentidele ühtse stiili).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Sõlmed</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absoluutsed koordinaadid</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-asukoht:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-asukoht:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>&amp;Kontuurjoone redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Lä&amp;htesta kontuurjoon</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Lõp&amp;eta redigeerimine</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Sõlmede liigutamine</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Kontrollpunktide liigutamine</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Sõlmede lisamine</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Sõlmede kustutamine</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Kontrollpunktide liigutamine ükshaaval</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Kontrollpunktide liigutamine sümmeetriliselt</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Kontrollpunktide lähtestamine</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Konkreetse kontrollpunkti lähtestamine</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Hulknurga avamine või Bezier&apos; kõvera lõikamine</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Bezier&apos; kõvera sulgemine</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Asukoha peegeldamine rõhtsuunas</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Asukoha peegeldamine püstsuunas</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Asukoha nihe rõhtsuunas paremale</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Asukoha nihe rõhtsuunas vasakule</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Asukoha nihe püstsuunas üles</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Asukoha nihe püstsuunas alla</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Asukoha pööramine vastupäeva</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Asukoha pööramine päripäeva</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Asukoha suuruse kärpimine määratud protsendi võrra</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Asukoha suuruse suurendamine määratud protsendi võrra</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Asukoha suuruse kärpimine määratud väärtuse võrra</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Asukoha suuruse suurendamine määratud väärtuse võrra</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Pööramisnurk</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>Suurendamise või vähendamise protsent</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Suurendamise või vähendamise väärtus</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Kontuurjoone redigeerimise lubamine</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Kontuurjoone lähtestamine kasti esialgsele kujule</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Sisselülitamisel kasutatakse leheküljega, vastasel juhul objektiga seotud koordinaate.</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>See dokument ei paista olevat OpenOffice Draw&apos; fail.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Impordi &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Impordib OpenOffice.org Draw&apos; faile</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Impordib OpenOffice.org Draw&apos; failid aktiivsesse dokumenti, muutes nende vektorandmed Scribuse objektideks.</translation>
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
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>OpenDocument&apos;i importija valikud</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Lõigustiilide ülekirjutamine</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Sisselülitamisel kirjutatakse aktiivses Scribuse dokumendis olemasolevad stiilid üle</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Lõigustiilide ühendamine</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Lõigustiilid ühendatakse atribuutide kaupa. See vähendab erinevate lõigustiilide hulka, kui originaaldokumendis kasutatakse teistsuguse nimega, aga samasuguste atribuutidega stiili.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Dokumendi nime kasutamine lõigustiili prefiksina</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Dokumendi nimi lisatakse Scribuses lõigustiilide ette.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Rohkem ei küsita</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Määratud seadistused muudetakse vaikeseadistusteks ning dialoogi OASIS OpenDocument&apos;i importimisel enam ei näidata.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribuse dokument</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x dokument</translation>
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
        <translation>Joone jämedus</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Salvestamine PDF-failina</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Välj&amp;undfail:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>I&amp;ga lehekülg salvestatakse eraldi failina</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Võimaldab iga dokumendi lehekülje eksportida omaette nimega PDF-failina. Leheküljenumbrid lisatakse automaatselt. See on eriti abiks PDF-failide järjestamisel trükikotta saatmiseks.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Salvestamisnupp ei ole aktiivne, kui püüad eksportida PDF/X-3 ning PDF/X-3 kaardil puudub infostring.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvestamine</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-failid (*.pdf);;Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>PDFToolBar</name>
    <message>
        <source>PDF Tools</source>
        <translation>PDF-i tööriistad</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>PDF-i salvestamine</translation>
    </message>
    <message>
        <source>Exporting Master Page:</source>
        <translation>Leheküljetooriku eksport:</translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation>Lehekülje eksport:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Aktiivse lehekülje elementide eksport:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Trükkimise eelvaatlus</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="obsolete">&amp;Teksti antialias</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="obsolete">&amp;Graafika antialias</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Lä&amp;bipaistvuse näitamine</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Under Color Removal</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>CMYK-i &amp;näitamine</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Ületrükirežiim</translation>
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
        <translation>Värvilahutuse nimi</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Tsüaan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Kollane</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Must</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Skaleerimine:</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Sulge</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Trüki...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Võimaldab tekstielementide ilusamat esitamist aknas, seda küll mõningase eelvaatluse aeglustumisega. See puudutab ainult Type 1 fonte.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Võimaldab TrueType fontide, OpenType fontide, EPS-i, PDF-i ja vektorgraafika ilusamat esitamist aknas, seda küll mõningase eelvaatluse aeglustumisega. See puudutab ainult Type 1 fonte.</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Näitab läbipaistvust ja dokumendi läbipaistvaid elemente. Vajalik on Ghostscript 7.07 või uuem.</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Trükkimise eelvaatlusel simuleeritakse RGB värvide asemel CMYK värve</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Võimalus lülitada välja mõningad tsüaanist, kollasest ja magentast koosnevad halli toonid ning kasutada selle asemel musta. UCR mõjutab kõige rohkem piltide neid osi, kus valitsevad neutraalsed ja/või tumedad toonid, mis on suhteliselt lähedal hallile. Selle valiku kasutamisel võib paraneda mõningate piltide trükikvaliteet, kuid alati tuleb seda katse-eksituse meetodil ise järele proovida. UCR vähendab üleküllastuse võimalust CMY värvides.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Lehekülje skaleerimise muutmine.</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>C värvi (tsüaan) lubamine/keelamine</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>M värvi (magenta) lubamine/keelamine</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Y värvi (kollane) lubamine/keelamine</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>K värvi (must) lubamine/keelamine</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Kõik</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fail</translation>
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
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Page:</source>
        <translation>Leheküljetooriku töötlemine:</translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation>Lehekülje eksport:</translation>
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
        <translation type="unfinished">Omadused</translation>
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
        <translation type="unfinished">Süvisinitsiaalid</translation>
    </message>
    <message>
        <source>Parent&apos;s Drop Cap Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabeldusmärgid ja taandus</translation>
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
        <translation>Pilt</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Joon</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Hulknurk</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Kompleksjoon</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Trajektooritekst</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Koopia -</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>On seotud elemendiga</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>On eellane elemendile</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>On järglane elemendile</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Lehekülje elemendi atribuudid</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tüüp</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Väärtus</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameeter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Seos</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Seos elemendiga</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
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
        <translation>&amp;Loobu</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Lehekülgede paigutus</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Esimene lehekülg on:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Kaks lehekülge</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Parem keskmine</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Lohista leheküljed või leheküljetoorikud kustutamiseks prügikasti</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Siin on näha kõik leheküljetoorikud. Uue lehekülje loomiseks lohista toorik allolevasse leheküljekasti.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Lehekülgede korraldamine</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Saadaolevad leheküljetoorikud:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokumendi leheküljed:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %1</source>
        <translation>%1 (kokku %1)</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 (kokku %2)</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation type="obsolete">Quarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="obsolete">Foolscap</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation type="obsolete">Government Letter</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Ledger</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Executive</translation>
    </message>
    <message>
        <source>Post</source>
        <translation type="obsolete">Post</translation>
    </message>
    <message>
        <source>Crown</source>
        <translation type="obsolete">Crown</translation>
    </message>
    <message>
        <source>Large Post</source>
        <translation type="obsolete">Large Post</translation>
    </message>
    <message>
        <source>Demy</source>
        <translation type="obsolete">Demy</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>Royal</source>
        <translation type="obsolete">Royal</translation>
    </message>
    <message>
        <source>Elephant</source>
        <translation type="obsolete">Elephant</translation>
    </message>
    <message>
        <source>Double Demy</source>
        <translation type="obsolete">Double Demy</translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation type="obsolete">Quad Demy</translation>
    </message>
    <message>
        <source>STMT</source>
        <translation type="obsolete">STMT</translation>
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
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Tulemus</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Otsingutulemused: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Eelvaatlus</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Vali</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Manage Pictures</source>
        <translation>Piltide haldamine</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Asukoht</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Lehekülg</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Trükitav</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Olek</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Jah</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Puuduv</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Otsi</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Katkesta otsing</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Liigu</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Otsingu baaskataloogi valik</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - pildiotsing</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Otsing ebaõnnestus: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Pilti nimega &quot;%1&quot; ei leitud.</translation>
    </message>
    <message>
        <source>Search Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Sulge</translation>
    </message>
    <message>
        <source>Set a new location for the selected items. Useful when you may have moved the document but not the images.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select a base directory for your selected rows</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Salvesta p&amp;ildina...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksport pildina</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Ekspordib valitud leheküljed bittrasterpiltidena.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Pluginat ei leitud</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>tundmatu viga</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Sümbolit ei leitud (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 laadimine</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>init ebaõnnestus</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>tundmatu plugina tüüp</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Plugin; %1 laaditud</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 laadimine ebaõnnestus: %2</translation>
    </message>
    <message>
        <source>Plugin: %1 initialized ok </source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 initsialiseerimine korras </translation>
    </message>
    <message>
        <source>Plugin: %1 failed post initialization</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 ebaõnnestus initsialiseerimise järel</translation>
    </message>
    <message>
        <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. IF you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Pluginahaldur</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Käivitamine</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tüüp</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Laaditav?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Plugina ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fail</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Jah</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ei</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Muudatuste rakendamiseks tuleb rakendus uuesti käivitada.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Hulknurga omadused</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Nurki:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>Pöö&amp;ramine:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Te&amp;guri rakendamine</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Te&amp;gur:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Hulknurga nurkade arv</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Hulknurga pööramine kraadides</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Nõgususe/kumeruse teguri rakendamine hulknurga kuju muutmiseks</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Näidishulknurk</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Negatiivne väärtus muudab hulknurga nõgusaks (tähekujuliseks), positiivne kumeraks</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Kohandatud</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Seadistused</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="obsolete">Kasutajaliides</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="obsolete">&amp;Keel:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="obsolete">&amp;Teema:</translation>
    </message>
    <message>
        <source>Show S&amp;plashscreen On Startup:</source>
        <translation type="obsolete">Käivitusekraani &amp;näitamine:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="obsolete">&amp;Fondi suurus (menüüs):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="obsolete">Fondi suurus (&amp;palettides):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="obsolete">&amp;Hiireratas kerib:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="obsolete">V&amp;iimati kasutatud dokumente:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="obsolete">Asukohad</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="obsolete">&amp;Dokumendid:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="obsolete">&amp;Muuda...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="obsolete">I&amp;CC profiilid:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="obsolete">M&amp;uuda...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="obsolete">&amp;Skriptid:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="obsolete">Muu&amp;da...</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="obsolete">Do&amp;kumendimallid:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">Muud&amp;a...</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Üldine</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Lehekülje suurus</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Suurus:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Rõhtpaigutus</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntatsioon:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="obsolete">Ühikud:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Veerisejooned</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Automaatne salvestamine</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Intervall:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="obsolete">Tagasivõtmine/uuestitegemine</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="obsolete">Toimingute ajaloo pikkus</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Juhtjooned</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tüpograafia</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Tööriistad</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Poolitaja</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Fondid</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Trükieelne kontroll</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Värvihaldus</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF-i eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokumendi elemendi atribuudid</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Sisukord ja registrid</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Kiirklahvid</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Lehekülje vaade</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Värv:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">&amp;Mittetrükitavat ala näidatakse veerise värviga</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Piltide näitamine</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Tekstiahela näitamine</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Mittetrükitavate märkide näitamine</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Kastide näitamine</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Joonlauad on seotud leheküljega</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="obsolete">Sodiala</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">Ü&amp;lal:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Vasakul:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;All:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Paremal:</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Lehekülgede vahe</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Rõhtsuunas:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Püstsuunas:</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="obsolete">V&amp;aate suuruse muutmine</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="obsolete">Vaate kohandamiseks lohista allpool joonlaual liugurit.</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vaade</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Välised tööriistad</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="obsolete">Dokumendi avamisel küsitakse alati, kas fonte asendada</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="obsolete">Stiili redigeerimisel näidatakse aktiivse lõigustiili eelvaatlust</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="obsolete">Käivitusdialoogi näitamine</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="obsolete">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="obsolete">Alati kasutatakse standardset Lorem Ipsumit</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="obsolete">Lõikude arv:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Muud</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Pluginad</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="obsolete">Seotud kastide näitamise lubamine või keelamine.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Mittetrükitavate märkide näitamine (nt. lõigutähised tekstikastis)</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">Kastide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Piltide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="obsolete">Scribuse vaikimisi töökeele valimine. Tühjaksjätmisel kasutatakse keskkonnamuutujatega määratud väärtust. Seda saab siiski tühistada, kui Scribust käsurealt käivitades vastav võti lisada.</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="obsolete">Akna dekoratsioonide ja välimuse vaikeväärtus. Scribus kasutab kõiki saadaolevaid KDE või Qt teemasid, kui Qt on seadistatud leidma KDE pluginaid.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="obsolete">Menüüde ja akende fondi vaikesuurus</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="obsolete">Tööriistaakende fondi vaikesuurus</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="obsolete">Vaikimisi mõõtühik dokumendi redigeerimisel</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="obsolete">Ridade arv, mille võrra Scribus kerib hiireratta liigutamisel</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="obsolete">Menüüs Fail näidatavate viimati kasutatud dokumentide arv</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="obsolete">Dokumentide vaikimisi kataloog</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="obsolete">ICC profiilide vaikimisi kataloog. Seda ei saa muuta, kui mõni dokument on avatud. Vaikimisi otsib Scribus neid Mac OS X ja Windowsi korral süsteemsetest kataloogidest. Linuxi ja Unixi korral otsib Scribus neid kataloogidest $home/.color/icc,/usr/share/color/icc ja /usr/local/share/color/icc.(sp)</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="obsolete">Skriptija vaikimisi skriptide kataloog</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="obsolete">Täiendav dokumendimallide kataloog</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="obsolete">Dokumendi lehekülje suurus: kas mõni standardne või kohandatud</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="obsolete">Dokumendi lehekülgede vaikimisi orientatsioon</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Dokumendi lehekülgede laius. Kui valisid kohandatud suuruse, saab seda muuta.</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Dokumendi lehekülgede kõrgus. Kui valisid kohandatud suuruse, saab seda muuta.</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="obsolete">Sisselülitamise korral salvestab Scribus määratud aja möödumisel alati varukoopia laiendiga .bak</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="obsolete">Automaatsete salvestamiste intervall</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Toimingute ajaloo pikkus sammudes. 0 tähendab, et salvestatakse piiramatu hulk toiminguid.</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Paberi värv</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Veeristest väljapoole jäävale alale antakse veerise värv</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="obsolete">Vaikimisi suurendustaseme määramine</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="obsolete">Liugurit lohistades saab kindlaks määrata vajaliku suurendustaseme, millega Scribus näitab lehekülgi ja objekte</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Määrab ruumi suuruse vasakul pool dokumendiala, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Määrab ruumi suuruse paremal pool dokumendiala, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Määrab ruumi suuruse dokumendiala kohal, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Määrab ruumi suuruse dokumendiala all, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Kataloogi valik</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">Visandimapp</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>Save...</source>
        <translation type="obsolete">Salvesta...</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Vaikeväärtused</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Seadistuste salvestamine</translation>
    </message>
    <message>
        <source>Export...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Rakenda</translation>
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
        <translation>Kas migreerida Scribuse vanad seadistused?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus tuvastas Scribus 1.2 seadistustefailide olemasolu.
Kas soovid need migreerida Scribuse uude versiooni?</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Seadistustefaili &quot;%1&quot; avamine kirjutamiseks ebaõnnestus: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Seadistustefaili &quot;%1&quot; kirjutamine ebaõnnestus: QIODevice olekukood %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Seadistustefaili &quot;%1&quot; avamine ebaõnnestus: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Seadistuste XML-i lugemine failist &quot;%1&quot; ebaõnnestus: %2 real %3, veerus %4</translation>
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
        <translation>Viga seadistuste salvestamisel</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus ei suutnud seadistusi salvestada:&lt;br&gt;%1&lt;br&gt;Palun kontrolli faili ja kataloogi õigusi ning kettaruumi.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Viga seadistuste laadimisel</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus ei suutnud seadistusi laadida:&lt;br&gt;%1&lt;br&gt;Laaditakse vaikeseadistused.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Ajakiri</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Kuldne keskmine</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Üheksa osa</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Siin saab valida lehekülje valmispaigutuse. &apos;Puudub&apos; jätab veerised endiseks, Gutenberg määratleb need klassikaliselt, &apos;Magazine&apos; aga kõik veerised võrdseks. Aluseks võetakse vasak/sisemise veerise väärtus.</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ava...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Salvesta &amp;kui...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Välju</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fail</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Käivita</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Kä&amp;ivita konsoolina</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Salvesta väljund...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribuse Pythoni konsool</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Selle aluseks on standardne Pythoni konsool, mistõttu selles on teatud piiranguid, eriti tühimärkide kasutamises. Täpsemalt räägib sellest Scribuse käsiraamat.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Skriptikonsool</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Kirjuta siia oma käsud. Valikut töödeldakse skriptina.</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Skripti väljund</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Pythoni skriptid (*.py)</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Pythoni skriptifaili avamine</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Pythoni käskude salvestamine failina</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Hoiatus</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Tekstifailid (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Aktiivse väljundi salvestamine</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>&amp;Toon:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Küll:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Väär:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Punane:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Roheline:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Sinine:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Al&amp;fakanal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Baasvärvid</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Kohandatud värvid</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Defineeri kohandatud värvid &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>Lis&amp;a kohandatud värvidesse</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Värvi valik</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Faili kopeerimine või liigutamine</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Lugeda: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Kirjutada: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Faili &amp;nimi:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Faili &amp;tüüp:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Üks kataloog ülespoole</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Kõik failid (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Suurus</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tüüp</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Kuupäev</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atribuudid</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>&amp;Asukoht:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Tagasi</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Loo uus kataloog</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Nimekirjavaade</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Detailne vaade</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Faili info eelvaatlus</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Faili sisu eelvaatlus</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lugeda/kirjutada</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Ainult lugemiseks</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Ainult kirjutamiseks</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Ligipääsmatu</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Nimeviit failile</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Nimeviit kataloogile</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Nimeviit spetsiaalsele failile</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fail</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Kataloog</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Spetsiaalne fail</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Salvestamine</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Ava</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Nimeta ümber</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Laadi uuesti</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>&amp;Nime järgi</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>&amp;Suuruse järgi</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>&amp;Kuupäeva järgi</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>S&amp;orteerimata</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sorteerimine</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Näita &amp;peidetud faile</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>fail</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>kataloog</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>nimeviit</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Kustutamine: %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Kas tõesti kustutada %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Jah</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ei</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Uus kataloog 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Uus kataloog</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Uus kataloog %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Kataloogi otsimine</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Kataloogid</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salvesta</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Viga</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Faili ei leitud.
Palun kontrolli asukohta ja nime.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Kõik failid (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Kataloogi valik</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Kataloog:</translation>
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
        <translation>Fondi st&amp;iil</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Suurus</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efektid</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Läbi &amp;kriipsutatud</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>Allajoonit&amp;ud</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Värv</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Näide</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>K&amp;iri</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Rakenda</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Sulge</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Fondi valik</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Puhasta</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Vali kõik</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Võta tagasi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Tee uuesti</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Lõika</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Aseta</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Joonda</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Kohanda...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Qt info&lt;/h3&gt;&lt;p&gt;See programm kasutab Qt versiooni %1.&lt;/p&gt;&lt;p&gt;Qt on C++ tööriistakomplekt multiplatvormse GUI &amp;amp; rakenduste arendamiseks.&lt;/p&gt;&lt;p&gt;Qt kasutamine kindlustab lähtekoodi porditavuse MS&amp;nbsp;Windowsi, Mac&amp;nbsp;OS&amp;nbsp;X, Linuxi ja kõigi peamiste Unixi platvormide vahel.&lt;br&gt;Qt on saadaval ka integreeritud seadmete tarbeks.&lt;/p&gt;&lt;p&gt;Qt on firma Trolltech toode. Kui soovid rohkem infot, vaata &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Koopia nr. %1  - </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Taust</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Teksti import</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Kõik toetatud vormingud</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Kõik failid (*)</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaani</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albaania</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baski</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgaaria</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasiilia</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalaani</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Hiina</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Horvaadi</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tšehhi</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Taani</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Hollandi</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglise</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Inglise (Briti)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Saksa</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Saksa (tradits.)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Soome</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Prantsuse</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galeegi</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Kreeka</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungari</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indoneesia</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Itaalia</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>Jaapani</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Korea</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Leedu</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Letseburgi</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation type="obsolete">Norra (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norra (Nynorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norra</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Poola</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugali</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugali (BR)</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Vene</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Rootsi</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Hispaania</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Hispaania (Lad-Am)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovaki</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Sloveeni</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbia</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Tai</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Türgi</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukraina</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Uelsi</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribuse krahh</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribust tabas krahh signaaliga #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribust tabas krahh järgmise erindi tõttu: %1</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Lehekülg</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Leheküljetoorik </translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
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
        <translation>Väljundfaili %1 avamine ebaõnnestus</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Väljundvoog ei ole kirjutamisõigusega</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Seadistuste kontroll ebaõnnestus: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Sisendfaili %1 avamine ebaõnnestus</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Seadistuste XML-i lugemine ebaõnnestus:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (rida %2 veerg %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Seadistuste XML-i lugemine ebaõnnestus: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>null juursõlm</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; vigane</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>leiti %1 &lt;%2&gt; sõlme, vaja on 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>ootamatu null &lt;%2&gt; sõlm</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>sõlm &lt;%1&gt; ei ole element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elemendil &lt;%1&gt; puudub atribuut `value&apos;</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>elemendi &lt;%1&gt; väärtus peab olema `true&apos; või `false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elemendil &lt;lpiSettingsEntry&gt; puudub atribuut `name&apos;</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Kõik</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>PostScript-faili eksport</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Faili trükkimine</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Must</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Tsüaan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Kollane</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Freetype2 teek pole kättesaadav</translation>
    </message>
    <message>
        <source>Font %1(%2) is broken</source>
        <translation>Font %1(%2) on vigane</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Font %1 on vigane (voo lugemine), ei põimita</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Fondis %1 on vigane glüüf %2 (kood %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="obsolete">Font %1 on vigane ja jäetakse kõrvale</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Fondi %1 ugemine ebaõnnestus, ei põimita</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Fondipuhvri loomine</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Font %1 on vigane, jäetakse kõrvale</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Fondi %1 laadimine ebaõnnestus - fondi tüüp on tundmatu</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Leiti uus font, kontrollitakse...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Leiti muudetud font, kontrollitakse...</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Font %1 laaditud failist %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Font %1(%2) on %3 kloon</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Fondi %1 laadimine (leiti fontconfig&apos;i abil)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Fondi laadimine ebaõnnestus - freetype2 ei leidnud fondifaili</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Fondipuhvri lugemine</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Uuendatud fondipuhvri kirjutamine</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Fontide otsimine</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="obsolete">Fondis %1 on vigane glüüf %2 (kood %3), jäetakse kõrvale</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>tüübi %1 ekstraktimine fondist %2 (sihtaadress=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>memcpy päis: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabel &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>memcpy tabel: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy suhtaadress: %1 %2 %3</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Scribuse arendusversioon</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="obsolete">Kasutad Scribus 1.3.x arendusversiooni. Redigeeritav dokument on loodud Scribuse versiooniga 1.2.3 või vanemaga. Salvestamisel ei saa seda enam Scribus 1.2.3 või vanemas kasutada, kui sa ei vali just Fail-&gt;Salvesta kui. Kas soovid kindlasti jätkata?</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Üritad importida rohkem lehekülgi, kui neid leidub aktiivses dokumendis alates aktiivsest leheküljest.&lt;/p&gt;Vali üks järgmistest võimalustest:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Loo&lt;/b&gt; puuduvad leheküljed&lt;/li&gt;&lt;li&gt;&lt;b&gt;Impordi&lt;/b&gt; leheküljed viimase leheküljeni&lt;/li&gt;&lt;li&gt;&lt;b&gt;Loobu&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Loo</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Impordi</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Dokumenti tehtud muudatused pole salvestatud, nüüd aga soovisid need tühistada. Kas soovid jätkata?</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Initsialiseerimine...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvestamine</translation>
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
        <translation>Punktid (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Millimeetrid (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Tollid (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Pikad (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Sentimeetrid (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Tsiitserod (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Fail on olemas</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Fail nimega &apos;%1&apos; on juba olemas.&lt;br/&gt;Kas asendada see failiga, mida soovisid salvestada?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Asenda</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>lk</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Ribakoodi generaator</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Värviratas</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Fontide eelvaatlus</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Minu plugin</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Uus mallist</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Dokumendimall: </translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Ajalehed</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brošüürid</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Almanahhid</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Lendlehed</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Sildid</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kaardid</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Kirjablanketid</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Ümbrikud</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visiitkaardid</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendrid</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Reklaamid</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiketid</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menüüd</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programmid</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF-vormid</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-esitlused</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Ajakirjad</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Postrid</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Kuulutused</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Tekstidokumendid</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Voldikud</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Ümbrised</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Enda mallid</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksport pildina</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Salvestamine pildina</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Viga väljundfaili kirjutamisel.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Eksport õnnestus.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Fail on olemas. Kas kirjutada üle?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>on juba olemas. Kas kirjutada üle?</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Jah</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ei</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Jah kõigile</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS-i importija</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Kõik toetatud vormingud (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Salvestamine mallina</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nimeta värvi hankimine ei ole võimalik.</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Värvi ei leitud.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nimeta värvi muutmine ei ole võimalik.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Dokumendist ei leitud värvi.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Värvi ei leitud vaikimisi värvide seast.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nimeta värvi ei ole võimalik luua.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nimeta värvi ei ole võimalik kustutada.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nimeta värvi ei ole võimalik asendada.</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder on suurem kui lubatud.</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Dokumendi avamine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Dokumendi salvestamine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Ühik väljaspool piire. Kasuta mõnda  scribus.UNIT_* konstanti.</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Värvi ei leitud - Pythoni viga</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argument peab olema lehekülje elemendi nimi või PyCObject&apos;i eksemplar</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Omadust ei leitud</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Järglast ei leitud</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Tulemuse tüübi &apos;%1&apos; teisendamine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Omaduse tüüp &apos;%1&apos; ei ole toetatud</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>&apos;%1&apos; teisendamine omaduse tüübiks &apos;%2&apos; ebaõnnestus</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Tüübid klapivad, aga omaduse määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Sihtmärk ei ole pildikast.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Määratud element ei ole pildikast.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Vähem kui kaht elementi ei saa rühmitada</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Vähem kui kaht elementi ei saa rühmitada</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Rühmitamiseks on vajalik valik või elementide nimekiri</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>0% võrra ei saa skaleerida.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Fonti ei leitud.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Tühja eksemplari ei saa renderdada.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Pikselrastri salvestamine ebaõnnestus</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Kihil ei saa nimi puududa.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Kihti ei leitud.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Viimast kihti ei saa eemaldada.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Nimeta kihti ei ole võimalik luua.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Soovitud nimega objekt on juba olemas.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Punktide nimekiri peab sisaldama vähemalt kaht punkti (neli väärtust).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Punktide nimekiri peab sisaldama paarisarvu väärtusi.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Punktide nimekiri peab sisaldama vähemalt kolme punkti (kuus väärtust).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Punktide nimekiri peab sisaldama vähemalt nelja punkti (kaheksa väärtust).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Punktide nimekiri peab sisaldama x korda kuut väärtust.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objekti ei leitud.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Stiili ei leitud.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Stiili saab määrata ainult tekstikastile.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>EPS-i salvestamine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Leheküljenumber väljub piiridest.</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Määratud leheküljetooriku nimele ei vasta ükski olemasolev toorik.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument ei ole nimekiri: peab olema murdarvväärtuste nimekiri.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument sisaldab mittearvulisi väärtusi: peab olema murdarvväärtuste nimekiri.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument sisaldab mittearvulisi väärtusi: peab olema murdarvväärtuste nimekiri.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Joone jämedus väljub piiridest, peab olema  0 &lt;= joone_jämedus &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Joone varjund väljub piiridest, peab olema 0 &lt;= varjund &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Täidise varjund väljub piiridest, peab olema 0 &lt;= varjund &lt;= 100.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Nurga raadius peab olema positiivne arv.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Joone stiili ei leitud.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti fondi suuruse hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti fondi hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti teksti suuruse hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti veergude arvu hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti reavahe hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti veergude vahe hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti teksti hankimine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti teksti määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Teksti lisamine mitte-tekstikasti ebaõnnestus.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Lisamise indeks väljub piiridest.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Joondus väljub piiridest. Kasutada mõnda  scribus.ALIGN* konstanti.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti teksti joonduse määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Fondi suurus väljub piiridest, peab olema 1 &lt;= suurus &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti fondi suuruse määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti fondi määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Reavahe väljub piiridest, peab olema &gt;=0,1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti reavahe määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Veergude vahe väljub piiridest, peab olema positiivne.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti veergude vahe määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Veergude arv väljub piiridest, peab olema &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti veergude arvu määramine ebaõnnestus.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Valiku indeks väljub piiridest</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Teksti valimine mitte-tekstikastis ebaõnnestus</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Teksti kustutamine mitte-tekstikastis ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Teksti täidise määramine mitte-tekstikastis ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Teksti äärejoone määramine mitte-tekstikastis ebaõnnestus.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Teksti varjundi määramine mitte-tekstikastis ebaõnnestus.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Linkida saab ainult tekstikaste.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Sihtkast peab olema tühi.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Sihtkast on seotud teise kastiga.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Sihtkast on seotud teise kasti külge.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Lähte- ja sihtkoht on üks ja sama objekt.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti linkimist ei saa eemaldada.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekt ei ole lingitud tekstikast, lingi eemaldamine ei ole võimalik.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objekt on viimane kast jadas, linki ei saa eemaldada. Selle asemel võib lingi eemaldada eelmiselt kastilt.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikasti ei saa kontuuriks teisendada.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Ülejooksmist saab kontrollida ainult tekstikastides</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikastile ei saa järjehoidjat seada</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Mitte-tekstikastist ei õnnestunud infot hankida</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Faili nimi peab olema string.</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Faili nimi ei tohi olla tühi string.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Pilditüübi seadistusi ei saa kustutada.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Pilditüüp peab olema string.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>Atribuut &apos;allTypes&apos; on AINULT LUGEMISEKS</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Pildi eksport ebaõnnestus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>Kä&amp;ivita skript...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Näita konsooli</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>Sk&amp;ripti info...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribuse skriptid</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Viimati kasutatud skriptid</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Skripti info</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skriptija</translation>
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
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Kohandatud (täiendav) seadistus: </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standardseadistus: </translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Lühendid</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Lühendite töötlemine. Palun oota...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Lühendite töötlemine. Tehtud.</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVG eksport</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG pildid (*.svg *.svgz);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG pildid (*.svg);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Hoiatus</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Kas tõesti kirjutada üle fail:
%1?</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>SVG import</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Vana .sla-vormingu toetus</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Draw&apos; importija</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>CSV-failid</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_andmed</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_päis</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word&apos;i dokumendid</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML-failid</translation>
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
Välised lingid
</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument tekstidokumendid</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB dokumendid</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB_andmed</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>PDB import</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Faili %1 avamine ebaõnnestus</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="obsolete">See fail ei paista olevat PDB dokument. Kui arvad, et see siiski on, anna palun veast teada.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer&apos;i dokumendid</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Tekstifiltrid</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Tekstifailid</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Araabia</translation>
    </message>
    <message>
        <source>font %1 </source>
        <translation>font %1(sp)</translation>
    </message>
    <message>
        <source>size %1 </source>
        <translation>suurus %1 </translation>
    </message>
    <message>
        <source>+style </source>
        <translation>+stiil </translation>
    </message>
    <message>
        <source>+color </source>
        <translation>+värv </translation>
    </message>
    <message>
        <source>+underline </source>
        <translation>+allajoonitud </translation>
    </message>
    <message>
        <source>-underline </source>
        <translation>-allajoonitud </translation>
    </message>
    <message>
        <source>+strikeout </source>
        <translation>+läbikriipsutatud </translation>
    </message>
    <message>
        <source>-strikeout </source>
        <translation>-läbikriipsutatud </translation>
    </message>
    <message>
        <source>+shadow </source>
        <translation>+vari </translation>
    </message>
    <message>
        <source>-shadow </source>
        <translation>-vari </translation>
    </message>
    <message>
        <source>+outline </source>
        <translation>+kontuur </translation>
    </message>
    <message>
        <source>-outline </source>
        <translation>-kontuur </translation>
    </message>
    <message>
        <source>+tracking %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-tracking </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+baseline %1</source>
        <translation>+alusjoon %1</translation>
    </message>
    <message>
        <source>+stretch </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>parent= %1</source>
        <translation>eellane= %1</translation>
    </message>
    <message>
        <source>unnamed</source>
        <translation>nimetu</translation>
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
        <source>Font %1 is broken (no Face), discarding it</source>
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
        <source>Govt. Letter</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the origial will be untouched. Are you sure you wish to proceed with this operation?</source>
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
        <source>Scribus 1.3.0-&gt;1.3.3.2 Support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Puhasta</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Vali kõik</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Võta tagasi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Tee uuesti</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Lõika</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Aseta</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Süsteemne menüü</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Keri kokku</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Keri lahti</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normaliseeri</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimeeri</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maksimeeri</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Sulge</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Taasta</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Liiguta</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>Muu&amp;da suurust</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimeeri</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ksimeeri</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Ala&amp;ti peal</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimeeri</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Alamaknaks</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Sulge</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>K&amp;eri kokku</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>Keri laht&amp;i</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Kohandatud</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Dokumendi seadistused</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Lehekülje suurus</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Suurus:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntatsioon:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Rõhtpaigutus</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="obsolete">Ü&amp;hik:</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="obsolete">Suuruseseadistusi rakendatakse kõigile lehekülgedele</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Veerisejooned</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Automaatne salvestamine</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Intervall:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokumendi info</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Juhtjooned</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Lehekülje vaade</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Värv:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">&amp;Mittetrükitavat ala näidatakse veerise värviga</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Piltide näitamine</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Tekstiahela näitamine</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Mittetrükitavate märkide näitamine</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Kastide näitamine</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Joonlauad on seotud leheküljega</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="obsolete">Minimaalne sodiala</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">Ü&amp;lal:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Vasakul:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;All:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Paremal:</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Lehekülgede vahe</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Rõhtsuunas:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Püstsuunas:</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vaade</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tüpograafia</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Tööriistad</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Poolitaja</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Fondid</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Trükieelne kontroll</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF-i eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokumendi elemendi atribuudid</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Sisukord ja registrid</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Sektsioonid</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Värvihaldus</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="obsolete">Seotud kastide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Mittetrükitavate märkide näitamine (nt. lõigutähised tekstikastis)</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">Kastide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Piltide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Paberi värv</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Veeristest väljapoole jäävale alale antakse veerise värv</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="obsolete">Lehekülje suuruse muudatused rakendatakse kõigile dokumendi olemasolevatele lehekülgedele</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Värvide kohandamine</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Pythoni skriptid (*.py);; Kõik failid (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Käivitamine laiendiskriptina</translation>
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
        <translation>Stiilihaldur</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation type="obsolete">Veerg 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation type="obsolete">K&amp;looni</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nimi:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Rakenda</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation type="obsolete">&amp;Loobu</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Impordi</translation>
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
        <translation type="unfinished">Lä&amp;htesta</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished">Alt+R</translation>
    </message>
</context>
<context>
    <name>SMCStylePage</name>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Parent</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMCharacterStyle</name>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Omadused</translation>
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
        <translation type="unfinished">Uus stiil</translation>
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
        <translation>Omadused</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="obsolete">Read</translation>
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
        <translation type="unfinished">Uus stiil</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished">Ühtlane joon</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">Punktiirjoon</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">Punktjoon</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">Kriipspunktjoon</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">Kriipspunktpunktjoon</translation>
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
        <translation type="unfinished">Fikseeritud reavahe</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished">Automaatne reavahe</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished">Joondamine alusjoontele</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Süvisinitsiaalid</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabeldusmärgid ja taandus</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Omadused</translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="unfinished">&amp;Ridu:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation type="unfinished">Kaugus tekstist:</translation>
    </message>
    <message>
        <source>Based on</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Parent&apos;s Character Style</source>
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
        <translation type="unfinished">Lõigustiil</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Uus stiil</translation>
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
        <translation type="unfinished">Eemalda</translation>
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
        <translation>Stiilide kustutamine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>&amp;Loobu</translation>
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
        <translation type="unfinished">Stiil puudub</translation>
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
        <translation>Stiiliseadistused</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Aktiivse lõigu stiil</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Täidise värvi seadistused</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Teksti täidise värv</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Teksti täidise värvi küllastus</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Äärejoone värvi seadistused</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Teksti äärejoone värv</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Teksti äärejoone värvi küllastus</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Fondiseadistused</translation>
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
        <translation>Valitud teksti font</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Fondi suurus</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Märkide skaleerimislaius</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Märkide skaleerimiskõrgus</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Märkide seadistused</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Märkide koondamine</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save as &amp;SVG...</source>
        <translation>Salvesta &amp;SVG-failina...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Ekspordib SVG-faile</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Ekspordib aktiivse lehekülje SVG-failina.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Impordi &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Impordib SVG-faile</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Impordib SVG-failid aktiivsesse dokumenti,
muutes nende vektorandmed Scribuse objektideks.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Skaleeritav vektorgraafika</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG-fail sisaldab mõningaid toetamata omadusi</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Lühendid</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Sisetühikute rakendamise koht:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>
&amp;Valitud kastid</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Aktiivne lehekülg</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Kõik elemendid</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Töödeldakse ainult valitud kaste</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Töödeldakse ainult aktiivset lehekülge</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Töödeldakse dokumendi kõiki elemente</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Kasutaja seadistus</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Süsteemne seadistus</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvesta</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Lä&amp;htesta</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Salvesta kasutaja seadistus</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Laadi uuesti süsteemne seadistus ja eemalda kasutaja seadistus</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Redigeeri etteantud seadistust. Salvestamisel kirjutab see üle süsteemse seadistuse.</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Lühendid</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Kasutaja seadistus on juba olemas. Kas tõesti see üle kirjutada?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Faili %1 kirjutamine ebaõnnestus.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Kasutaja seadistus salvestatud</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Süsteemne seadistus uuesti laaditud</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Faili %1 avamine ebaõnnestus</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Salvesta &amp;mallina...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Dokumendi salvestamine mallina</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Salvestab dokumendi mallina. See lihtsustab tunduvalt tööd selliste dokumentidega, millel peab olema ühtne välimus.</translation>
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
        <translation type="unfinished">&amp;Loobu</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Laadimine/Salvestamine/Import/Eksport</translation>
    </message>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Püsiv</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Toiming</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Teadmata</translation>
    </message>
</context>
<context>
    <name>ScProgressBar</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 (kokku %2)</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Üles</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Paremale</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Alla</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vasakule</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Dokkimine...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Rõhtne</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Püstine</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Lahtise akna orientatsioon...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Trükkimine...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Koopia nr. %1  - </translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Taust</translation>
    </message>
</context>
<context>
    <name>Scribus12Format</name>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished">Scribus 1.2.x dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Taust</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Scribus13Format</name>
    <message>
        <source>Scribus 1.3.0-&gt;1.3.3.2 Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation>Dokumendi värvid</translation>
    </message>
</context>
<context>
    <name>ScribusCore</name>
    <message>
        <source>Initializing Plugins</source>
        <translation>Pluginate initsialiseerimine</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Kiirklahvide initsialiseerimine</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Seadistuste lugemine</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>ICC profiilide lugemine</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Fontide otsimine</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Süsteemist ei leitud ühtegi fonti.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Lõpetatakse.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Saatuslik viga</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Fondisüsteem initsialiseeritud</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Taust</translation>
    </message>
    <message>
        <source>An error occurred while opening icc profiles, color management is not enabled.</source>
        <translation>ICC profiilide avamisel tekkis viga, värvihaldus ei ole sisse lülitatud.</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Uus kiht</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Kas tõesti puhastada kogu tekst?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Kasutuselolevat elementi ei saa kustutada</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Elementi %1 redigeeritakse parajasti looredaktoris. Kustutamisoperatsioon katkestatakse.</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Mõned objektid on lukustatud.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Eemalda kõigi l&amp;ukustus</translation>
    </message>
    <message>
        <source>&amp;Skip locked objects</source>
        <translation>&amp;Jäta lukustatud objektid vahele</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Kiirklahvide seadmine</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Looredaktori initsialiseerimine</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Poolitaja initsialiseerimine</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Visandimapi lugemine</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fail</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Redigeerimine</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fail</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Ava &amp;viimati kasutatud</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Impordi</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Ekspordi</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeerimine</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Sisu</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>St&amp;iil</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Värv</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Suurus</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Varjund</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Font</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efektid</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Element</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Eelvaatluse seadistused</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Tase</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Saatmine ki&amp;hile</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-i valikud</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Kuju</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>T&amp;eisendamine</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>L&amp;isamine</translation>
    </message>
    <message>
        <source>&amp;Character</source>
        <translation>&amp;Märk</translation>
    </message>
    <message>
        <source>&amp;Quote</source>
        <translation>&amp;Jutumärk</translation>
    </message>
    <message>
        <source>S&amp;paces &amp;&amp; Breaks</source>
        <translation>&amp;Tühikud ja katkestused</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>Li&amp;gatuur</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Lehekülg</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Vaade</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Aknad</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>A&amp;bi</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Joondus</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Valmis</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Lehekülgede import...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Lehekülgede import</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Import tehtud</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Ei leitud midagi importida</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Saatuslik viga</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Fail %1 ei ole sobivas vormingus</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Laadimine...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Mõned dokumendis kasutatud ICC profiilid ei ole paigaldatud:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> asendati profiiliga: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(teisendatud)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Kõik toetatud vormingud</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Kõik failid (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Ei õnnestunud kirjutada faili:
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.sla.gz *.scd *.scd.gz);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.scd);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Salvestamine</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Salvestamine...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus avastas vigu. Nende parandamiseks võiks kasutada trükieelset kontrolli.</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignoreeri</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Katkesta</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Trükkimine...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Trükkimine ebaõnnestus!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Kasutuselolevat elementi ei saa lõigata</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Elementi %1 redigeeritakse parajasti looredaktoris. Lõikamisoperatsioon katkestatakse.</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Qt info</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribuse käsiraamat</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvestamine</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstifailid (*.txt);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normaalne</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nimi:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Lehekülje salvestamine toorikuna</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Uus leheküljetoorik %1</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Suurus:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Suurus</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Varjund:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Varjund</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Stiil puudub</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Koopiate arv: %1
Nihe rõhtsuunas: %2
Nihe püstsuunas: %3</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Järgmised programmid puuduvad:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: ei saa kasutada EPS-pilte ega trükkimise eelvaatlust</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: ei saa kasutada EPS-pilte ega PostScript trükkimise eelvaatlust</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript puudub: ei saa kasutada PostScript trükkimise eelvaatlust</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Kõik</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus avastas vigu.
Nende parandamiseks võiks kasutada trükieelset kontrolli.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS-failid (*.eps);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Avastati mõned vead.
Nende parandamiseks võiks kasutada trükieelset kontrolli.</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-lk%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Mõned objektid on lukustatud.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Lukusta kõik</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Eemalda kõigi l&amp;ukustus</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Info</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Programm %1 juba töötab!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Programm %1 puudub!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Valitud värv puudub dokumendi värvikomplektis. Palun anna uuele värvile nimi.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Värvi ei leitud</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Valitud nimi on juba olemas. Palun anna uuele värvile mõni muu nimi.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Tase</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Saatmine kihile</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Eel&amp;vaatluse seadistused</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Märk</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Jutumärk</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Tühik</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Tööriistad</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-asukoht:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-asukoht:</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Kas tõesti asendada olemasolev pilt?</translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Vigane argument: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Faili %1 ei ole olemas, lõpetatakse.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Kasutamine: scribus [võti ...] [fail]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Võtmed:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Abi näitamine ja väljumine</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>xx kasutamine keele lühendina, näiteks &apos;et&apos; või &apos;en&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Kasutajaliidese paigaldatud keelte nimekiri</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Info näitamine konsoolis fontide laadimisel</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Käivitamisel ei näidata käivitusekraani</translation>
    </message>
    <message>
        <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
        <translation>Käivitamisel ei näidata enam käivitusekraani. Kirjutab kataloogi ~/.scribus tühja faili nimega .neversplash.</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Näitab versiooni ja väljub</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Vahetab dialooginuppude järjekorra (nt. Loobu/Ei/Jah tavalise Jah/Ei/Loobu asemel)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>failinimi</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Failinimi määrab kasutaja seadistuste asukoha</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Scribuse kasutajaliidese paigaldatud keeled on järgmised:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Vaikimisi valitud keele tühistamiseks anna käsk:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx või scribus --lang xx, kus xx on sinu valitud keel.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribuse versioon</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus - avatud lähtekoodiga küljendusprogramm</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Kodulehekülg</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Dokumentatsioon</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Vead</translation>
    </message>
    <message>
        <source>Download a file from the Scribus website and show the latest available version.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display a console window</source>
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
        <translation>Kiht</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopeeri siia</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Liiguta siia</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Aseta</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Pilt</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fail: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Originaali PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Tegelik PPI: </translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Värviruum: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Teadmata</translation>
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
        <translation>Halltoonid</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Lingitud tekst</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Tekstikast</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Trajektoori tekst</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Lõike: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Sõnu: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Märke: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Trükkimine: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Lubatud</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Keelatud</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Eelvaatluse seadistused</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-i valikud</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Saatmine ki&amp;hile</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>&amp;Tase</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>T&amp;eisendamine</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Sisu</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Tekstikastide linkimine</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Üritad linkida täidetud kastiga või sama kasti endaga.</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Lehekülg %1 kuni %2</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Kasutuselolevat elementi ei saa teisendada</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Elementi %1 redigeeritakse parajasti looredaktoris. Antud elemendi kontuuriks teisendamise operatsioon jäetakse vahele.</translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>Dokumenti:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>on pärast viimast salvestamist muudetud.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Unusta</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Põimitud Pythoni skriptide toetus.</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skriptija</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Skripti viga</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Kui kasutad ametlikku skripti, anna sellest palun teada aadressil &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>See teade leidub ka lõikepuhvris. Käsuga Ctrl+V saab selle veajälitusse asetada.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Sisestatud käsu puhul tekkis sisemine viga. Üksikasjad saadeti standardveaväljundisse. </translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Skripti uurimine</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="obsolete">Pythoni skriptid (*.py);; Kõik failid (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentatsioon:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Skript</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> ei sisalda ühtegi docstringi!</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Pythoni plugina seadistamine ebaõnnestus. Vead saadeti standardveaväljundisse. </translation>
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
        <translation>Laiendid</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Konsool</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Muuda...</translation>
    </message>
    <message>
        <source>Scripter Preferences</source>
        <translation>Skriptija seadistused</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Laiendiskriptide lubamine</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Käivitusskript:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Vead:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Kommentaarid:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Võtmesõnad:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Märgid:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Arvud:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Stringid:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Baastekstid:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Värvi valik</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Käivitusskripti asukoht</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Näita lehekülgede eelvaatlust</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Otsimine/asendamine</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Otsimine:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Lõigustiil</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Font</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Fondi suurus</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Fondi efektid</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Täidise värv</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Täidise varjund</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Äärejoone värv</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Äärejoone varjund</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vasakul</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Keskel</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Paremal</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Rööpjoondus</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Püsiv rööpjoondus</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Asendamine:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Terve sõna</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Tähesuurust &amp;ignoreeritakse</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Ot&amp;si</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Asenda</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Asenda &amp;kõik</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>S&amp;ulge</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Otsing lõpetatud</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Otsing lõpetatud, leiti %1 sobivust</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Väljade valik</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Saadaolevad väljad</translation>
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
        <translation>Valitud väljad</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Muu...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Varjund:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Varjund</translation>
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
        <translation>X-nihe</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-nihe</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>Lü&amp;hendid...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Lühendid</translation>
    </message>
</context>
<context>
    <name>ShortcutWidget</name>
    <message>
        <source>&amp;No Key</source>
        <translation type="unfinished">&amp;Klahv puudub</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">Kas&amp;utaja määratud klahv</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">&amp;Määra klahv</translation>
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
        <source>Meta</source>
        <translation type="unfinished">Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished">Meta+</translation>
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
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Stiil puudub</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Redigeeri stiile...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Stiil puudub</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Stiili redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Impordi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeeri</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Klooni</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>%1 koopia</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Uus stiil</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.sla.gz *.scd *.scd.gz);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumendid (*.sla *.scd);;Kõik failid (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fail</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigeerimine</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>L&amp;isamine</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Märk</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Jutumärk</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Tühikud ja katkestused</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatuur</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Seadistused</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Looredaktor</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Uus</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Puhasta kogu tekst</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Laadi tekst kastist uuesti</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Salvesta failina...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>L&amp;aadi failist...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Salvesta &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Uuenda tekstikasti ja välju</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Välju tekstikasti uuendamata</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>&amp;Vali kõik</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Lõika</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopeeri</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Aseta</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Ot&amp;si/asenda...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Redigeeri stiile...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Fontide eelvaatlus...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Uuenda tekstikasti</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Tühik</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>L&amp;isa glüüf...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Taust...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Kasutatav font...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Nutikas teksti valimine</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fail</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Aktiivne lõik:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Sõnu: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Märke: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Kokku:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Lõike: </translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Looredaktor - %1</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Kas salvestada tehtud muudatused?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Kas tõesti loobuda kõigist muudatustest?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Kas tõesti puhastada kogu tekst?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Avamine</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstifailid (*.txt);;Kõik failid (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvestamine</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Automaatne</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Nihe</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Joone jämedus</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation type="obsolete">Valitud on rohkem kui üks element</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Nimi:</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">Lä&amp;htesta</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Rakenda</translation>
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
        <translation type="unfinished">&amp;Uus</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Impordi</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">K&amp;ustuta</translation>
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
        <translation type="unfinished">Uus</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished">Kiirklahv</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Teksti allajoonimine. Nuppu all hoides saab määrata joone jämeduse ja nihke.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Ainult sõnade allajoonimine. Nuppu all hoides saab määrata joone jämeduse ja nihke.</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Suurtähed</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapiteelkiri</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Alaindeks</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Ülaindeks</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Läbikriipsutamine. Nuppu all hoides saab määrata joone jämeduse ja nihke.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <comment>Text Style Selector</comment>
        <translation>Kontuur. Nuppu all hoides saab määrata kontuuri äärejoone jämeduse.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Varjuga tekst. Nuppu all hoides saab määrata varju nihke.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Writer&apos; importija valikud</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Lõigustiilide ülekirjutamine</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Sisselülitamisel kirjutatakse aktiivses Scribuse dokumendis olemasolevad stiilid üle</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Lõigustiilide ühendamine</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Lõigustiilid ühendatakse atribuutide kaupa. See vähendab erinevate lõigustiilide hulka, kui originaaldokumendis kasutatakse teistsuguse nimega, aga samasuguste atribuutidega stiili.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Dokumendi nime kasutamine lõigustiili prefiksina</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Dokumendi nimi lisatakse Scribuses lõigustiilide ette.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Rohkem ei küsita</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Määratud seadistused muudetakse vaikeseadistusteks ning dialoogi OpenOffice.org 1.x dokumendi importimisel enam ei näidata.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobu</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Alguses</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Lõpus</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Ei näidata</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Sisukord ja registrid</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Sisukord</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Kast, kuhu sisukord paigutatakse</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Leheküljenumbrite asukoht:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Elemendi atribuudi nimi:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Elemendi atribuut, mis kehtestatakse kastidele kirjete loomise alusena</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Kirjete leheküljenumbrid võib asetada rea lõppu, algusesse või jätta üldse näitamata</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Mittetrükitavate kirjete kaasamine</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Kastide kaasamine, mida ei trükita</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Kirjeridade lõigustiil</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Lõigustiil:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Sihtraam:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Re&amp;gister</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Sisukord ja registrid</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Sisukord</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Lis&amp;a</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Kast, kuhu sisukord paigutatakse</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Leheküljenumbrite asukoht:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Elemendi atribuudi nimi:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Elemendi atribuut, mis kehtestatakse kastidele kirjete loomise alusena</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Kirjete leheküljenumbrid võib asetada rea lõppu, algusesse või jätta üldse näitamata</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Mittetrükitavate kirjete kaasamine</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Kastide kaasamine, mida ei trükita</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Kirjeridade lõigustiil</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Lõigustiil:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Sihtraam:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Re&amp;gister</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Vigu ignoreeritakse</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Automaatne kontroll enne trükkimist või eksportimist</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Puuduvate glüüfide kontroll</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Leheküljelt väljas asuvate objektide kontroll</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Tekstikastide ülejooksmise kontroll</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Läbipaistvuse kontroll</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Puuduvate piltide kontroll</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Piltide lahutuse kontroll</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Väikseim lubatud lahutus</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Highest allowed resolution</source>
        <translation>Suurim lubatud lahutus</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Põimitud PDF-failide kontroll</translation>
    </message>
    <message>
        <source>Check for GIF images</source>
        <translation>GIF-piltide kontroll</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>PDF-i annotatsioonide ja väljade kontroll</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Lisa profiil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Eemalda profiil</translation>
    </message>
</context>
<context>
    <name>TabDisplay</name>
    <message>
        <source>Color for paper</source>
        <translation type="unfinished">Paberi värv</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Veeristest väljapoole jäävale alale antakse veerise värv</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished">Seotud kastide näitamise lubamine või keelamine.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Mittetrükitavate märkide näitamine (nt. lõigutähised tekstikastis)</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">Kastide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Turns the display of layer indicators on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">Piltide näitamise lubamine või keelamine</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Määrab ruumi suuruse vasakul pool dokumendiala, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Määrab ruumi suuruse paremal pool dokumendiala, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Määrab ruumi suuruse dokumendiala kohal, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Määrab ruumi suuruse dokumendiala all, mida saab kasutada lõikelauana elementide loomiseks ja muutmiseks ja kust neid saab aktiivsele leheküljele lohistada</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="unfinished">Vaikimisi suurendustaseme määramine</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished">Liugurit lohistades saab kindlaks määrata vajaliku suurendustaseme, millega Scribus näitab lehekülgi ja objekte</translation>
    </message>
</context>
<context>
    <name>TabDisplayBase</name>
    <message>
        <source>Form2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished">Lehekülje vaade</translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Kastide näitamine</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Tekstiahela näitamine</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">&amp;Mittetrükitavat ala näidatakse veerise värviga</translation>
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
        <translation type="unfinished">Mittetrükitavate märkide näitamine</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Piltide näitamine</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Värv:</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished">Sodiala</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Vasakul:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Paremal:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;All:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">Ü&amp;lal:</translation>
    </message>
    <message>
        <source>Gaps Between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished">Rõhtsuunas:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished">Püstsuunas:</translation>
    </message>
    <message>
        <source>Adjust Display Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished">Vaate kohandamiseks lohista allpool joonlaual liugurit.</translation>
    </message>
    <message>
        <source>textLabel16</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabDocument</name>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Kohandatud</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Lehekülje suurus</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Suurus:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Püstpaigutus</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Rõhtpaigutus</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntatsioon:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished">Ühikud:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Laius:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Kõrgus:</translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Veerisejooned</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Automaatne salvestamine</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Intervall:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="unfinished">Tagasivõtmine/uuestitegemine</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished">Toimingute ajaloo pikkus</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Dokumendi lehekülgede laius. Kui valisid kohandatud suuruse, saab seda muuta.</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Dokumendi lehekülgede kõrgus. Kui valisid kohandatud suuruse, saab seda muuta.</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished">Dokumendi lehekülje suurus: kas mõni standardne või kohandatud</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="unfinished">Dokumendi lehekülgede vaikimisi orientatsioon</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished">Sisselülitamise korral salvestab Scribus määratud aja möödumisel alati varukoopia laiendiga .bak</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="unfinished">Automaatsete salvestamiste intervall</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished">Toimingute ajaloo pikkus sammudes. 0 tähendab, et salvestatakse piiramatu hulk toiminguid.</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished">Lehekülje suuruse muudatused rakendatakse kõigile dokumendi olemasolevatele lehekülgedele</translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidget</name>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Ghostscripti asukoht</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Pildiredaktori asukoht</translation>
    </message>
    <message>
        <source>Locate your web browser</source>
        <translation>Brauseri asukoht</translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidgetBase</name>
    <message>
        <source>External Tools</source>
        <translation>Välised tööriistad</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>PostScripti interpretaator</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>Käivitatava faili &amp;nimi:</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation type="obsolete">Ghostscripti interpretaatori asukoha lisamine. Windowsi korral on oluline tähele panna, et kasutada tuleb programmi nimega gswin32c.exe - MITTE AGA gswin32.exe. Viimane võib Scribuse käivitamisel kaasa tuua hangumise.</translation>
    </message>
    <message>
        <source>&amp;Change..</source>
        <translation>&amp;Muuda..</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>&amp;Teksti antialias</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Teksti antialias EPS- ja PDF-failide renderdamiseks ekraanil</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>&amp;Graafika antialias</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Graafika antialias EPS- ja PDF-failide renderdamiseks ekraanil</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Lahutus:</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Pilditöötlusrakendus</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Käivitatava faili n&amp;imi:</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distribution includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="obsolete">Graafikaredaktori asukoht failisüsteemis. Kui kasutad GIMP-i ja sinu distributsioon seda sisaldab, soovitame kasutada selle varianti &apos;gimp-remote&apos;, mis lubab redigeerida pilti ka juba töötavas GIMP-i eksemplaris.</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Muuda...</translation>
    </message>
    <message>
        <source>&amp;Rescan</source>
        <translation>Uu&amp;ri uuesti</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>Rescan for the external tools if they do not exist in the already specified location</source>
        <translation>Väliste tööriistade uus otsing, kui neid ei leidu juba määratud asukohas</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ghostscripti interpretaatori asukoha lisamine. Windowsi korral on oluline tähele panna, et kasutada tuleb programmi nimega gswin32c.exe - MITTE AGA gswin32.exe. Viimane võib Scribuse käivitamisel kaasa tuua hangumise.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for graphics editor. If you use gimp and your distribution includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Graafikaredaktori asukoht failisüsteemis. Kui kasutad GIMP-i ja sinu distributsioon seda sisaldab, soovitame kasutada selle varianti &apos;gimp-remote&apos;, mis lubab redigeerida pilti ka juba töötavas GIMP-i eksemplaris.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Web Browser</source>
        <translation>Brauser</translation>
    </message>
    <message>
        <source>Web Browser to launch with links from the Help system</source>
        <translation>Brauser abisüsteemi linkide avamiseks</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for your web browser. This is used for external links from the Help system.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Brauseri asukoht failisüsteemis. Seda kasutatakse abisüsteemi väliste linkide avamiseks.&lt;/qt&gt;</translation>
    </message>
</context>
<context>
    <name>TabGeneral</name>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished">Scribuse vaikimisi töökeele valimine. Tühjaksjätmisel kasutatakse keskkonnamuutujatega määratud väärtust. Seda saab siiski tühistada, kui Scribust käsurealt käivitades vastav võti lisada.</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished">Menüüs Fail näidatavate viimati kasutatud dokumentide arv</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished">Ridade arv, mille võrra Scribus kerib hiireratta liigutamisel</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished">Akna dekoratsioonide ja välimuse vaikeväärtus. Scribus kasutab kõiki saadaolevaid KDE või Qt teemasid, kui Qt on seadistatud leidma KDE pluginaid.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished">Menüüde ja akende fondi vaikesuurus</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished">Tööriistaakende fondi vaikesuurus</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="unfinished">Dokumentide vaikimisi kataloog</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished">ICC profiilide vaikimisi kataloog. Seda ei saa muuta, kui mõni dokument on avatud. Vaikimisi otsib Scribus neid Mac OS X ja Windowsi korral süsteemsetest kataloogidest. Linuxi ja Unixi korral otsib Scribus neid kataloogidest $home/.color/icc,/usr/share/color/icc ja /usr/local/share/color/icc.(sp)</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished">Skriptija vaikimisi skriptide kataloog</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished">Täiendav dokumendimallide kataloog</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Kataloogi valik</translation>
    </message>
</context>
<context>
    <name>TabGeneralBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="unfinished">Kasutajaliides</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Keel:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished">&amp;Teema:</translation>
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
        <translation type="unfinished">&amp;Fondi suurus (menüüs):</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished">Fondi suurus (&amp;palettides):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished">&amp;Hiireratas kerib:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished">V&amp;iimati kasutatud dokumente:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished">&amp;Dokumendid:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Muuda...</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished">I&amp;CC profiilid:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+H</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="unfinished">&amp;Skriptid:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished">Muu&amp;da...</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished">Alt+N</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Üldised seadistused</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Asetus dokumendis</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>Taustal</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>Esiplaanil</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Haaramine</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Haaramise kaugus:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Haaramise raadius:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Juhtjoonte näitamine</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Värv:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Veeriste näitamine</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Lehekülje alusvõrgu näitamine</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Põhivõrgustik</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Samm:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Lisavõrgustik</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Alusjoonte näitamine</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Alusjoonte seadistused</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Alusjoonte võr&amp;gustik:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Alusjoonte ni&amp;he:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Juhtjooned ei paista läbi lehekülje objektide</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Juhtjooned on näha lehekülje objektide peal</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Lisavõrgustiku joonte vahe</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Põhivõrgustiku joonte vahe</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Vahemaa, mille tagant objekt haaratakse juhtjoone külge</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Ala raadius, kus Scribus lubab haarata objekti pidemetest</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Lisavõrgustiku joonte värv</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Põhivõrgustiku joonte värv</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Lisatavate juhtjoonte värv</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Veerisejoonte värv</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Alusjoonte värv</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Lülitab alusjoonte näitamise sisse või välja</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Alusjoonte vahe</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Esimese alusjoone vahemaa lehekülje ülaservaga</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Lülitab alusvõrgu näitamise sisse või välja</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Lülitab juhtjoonte näitamise sisse või välja</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Lülitab veeriste näitamise sisse või välja</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Klahvikomplektide XML-failid (*.ksxml)</translation>
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
        <translation>See klahvikombinatsioon on juba kasutusel</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidgetBase</name>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Kiirklahvid</translation>
    </message>
    <message>
        <source>Search:</source>
        <translation>Otsing:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Toiming</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation>Kiirklahv</translation>
    </message>
    <message>
        <source>Shortcut for Selected Action</source>
        <translation>Valitud toimingu kiirklahv</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Klahv puudub</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Kas&amp;utaja määratud klahv</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Määra klahv</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>CTRL+ALT+SHIFT+W</source>
        <translation>CTRL+ALT+SHIFT+W</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Laaditavad kiirklahvikomplektid</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Saadaolevad kiirklahvikomplektid</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Laadi</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Laadi valitud kiirklahvikomplekt</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Impordi...</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Impordib kiirklahvikomplekti praegusse seadistusse</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Ekspordi...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Ekspordib aktiivsed kiirklahvid imporditavasse faili</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Lä&amp;htesta</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Laadib uuesti Scribuse vaikimisi kiirklahvid</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Tabeldusmärkide haldamine</translation>
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
        <translation type="unfinished">Dokumendi avamisel küsitakse alati, kas fonte asendada</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished">Stiili redigeerimisel näidatakse aktiivse lõigustiili eelvaatlust</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished">Käivitusdialoogi näitamine</translation>
    </message>
    <message>
        <source>Show Splashscreen on Startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished">Alati kasutatakse standardset Lorem Ipsumit</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished">Lõikude arv:</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Ekspordivahemik</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Kõik leheküljed</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Valitud leheküljed</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>Pöö&amp;ramine:</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Kärpimine lehekülje veeriste juures</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Faili valikud</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Üh&amp;ilduvus:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>Köi&amp;teserv:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Vasak veeris</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Parem veeris</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Pisipil&amp;tide genereerimine</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>&amp;Lingitud tekstikastide salvestamine PDF-i artiklitena</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Järjeho&amp;idjate kaasamine</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Ki&amp;htide kaasamine</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>EPS-g&amp;raafika lahutus:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Teksti &amp;ja vektorgraafika tihendamine</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Pilditihendamise viis</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automaatne</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Kadudega - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Kadudeta - Zip</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Tihen&amp;damisviis:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maksimaalne</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Suur</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Keskmine</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Väike</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimaalne</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Tihenduse k&amp;valiteet:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Pildilahutuse &amp;muutmine väärtusele:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>Ül&amp;dine</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Kõigi fontid&amp;e põimimine</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Kõigi fontide vajaliku o&amp;sa põimimine</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Põimimine</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Saadaolevad fondid:</translation>
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
        <source>Fonts to embed:</source>
        <translation>Põimitavad fondid:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Vajalikku osa sisaldavad fondid:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Fondid</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Esitlusefektide lu&amp;bamine</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Lehekülg</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Lehekülgede eel&amp;vaatluse näitamine</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efektid</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Näitamise kestus:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Efek&amp;ti kestus:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>E&amp;fekti tüüp:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Ridade liiguta&amp;mine:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>A&amp;lates:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>S&amp;uund:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Efekt puudub</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Segunemine</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Kast</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Hägunemine</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Sädelus</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Lõhkumine</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Pühkimine</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Rõhtsuunas</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Püstsuunas</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Seestpoolt</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Väljastpoolt</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Vasakult paremale</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Ülalt alla</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Alt üles</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Paremalt vasakule</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Ülalt vasakult alla paremale</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Efekti r&amp;akendamine kõigil lehekülgedel</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Vaateseadistused</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation>Lehekülgede paigutus</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Üks lehekülg</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Pidev</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Kaks lehekülge vasakult paremale</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Kaks lehekülge paremalt vasakule</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Välimus</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Näitaja vaikeväärtused</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Täisekraanirežiim</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Järjehoidjate saki näitamine</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Pisipiltide näitamine</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Kihtide saki näitamine</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Näitaja tööriistariba peitmine</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Näitaja menüüriba peitmine</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Lehekülgede suurenduse muutmine vastavalt näitaja aknale</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Eritoimingud</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>PDF-dokumendi avamisel
käivitatav JavaScript:</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Skript puudub</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Näitaja</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Krüptimise kas&amp;utamine</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Paroolid</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>Kas&amp;utaja:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Omanik:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Seadistused</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>&amp;Dokumendi trükkimise lubamine</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Dokumendi &amp;muutmise lubamine</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>&amp;Teksti ja graafika kopeerimise lubamine</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>&amp;Annotatsioonide ja väljade lisamise lubamine</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Turvalisus</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Üldine</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Väljund&amp;i värvimudel:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Ekraan / Veeb</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Printer</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Halltoonid</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Spot-värvide teisendamine protsessivärvideks</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Ületrükirežiim</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>Kohandat&amp;ud renderdamisseadistused</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Renderdamisseadistused</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Sa&amp;gedus:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Nurk:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>S&amp;pot-kuju:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Lihtne punkt</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Joon</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Ümar</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Ellips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Ühtlased värvid:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>ICC profiili kasutamine</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profiil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Renderdamisviis:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Pertseptuaalne</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Suhteline kolorimeetriline</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Küllastus</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absoluutne kolorimeetriline</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Pildid:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Põimitud ICC profiile ei kasutata</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Värv</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 väljundviis</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Infostring:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Väljund&amp;profiil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Trimmimiskast</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Fontide põimimine PDF-faili. Põimitud fondid aitavad säilitada dokumendi kujunduse ja välimuse.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Lülitab sisse esitlusefektid, mida saab kasutada Adobe&amp;#174; Reader&amp;#174; ja teiste PDF-näitajate korral, mis toetavad täisekraanirežiimi.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Kõigi ülalolevate lehekülgede eelvaatluse näitamine</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Aeg, mille jooksul lehekülge näidatakse enne esitluse alustamist. 0 tähendab lehekülgede automaatse ülemineku keelamist.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Efekti esitamise aeg. Lühem aeg kiirendab efekti esitamist, pikem aeglustab.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Efekti tüüp</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Ridade liigutamise suund segunemise ja lõhkumise efekti korral</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Alguspositsioon kasti ja lõhkumise efekti korral</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Sädeluse või pühkimise efekti suund</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Valitud efekti kasutatakse kõigil lehekülgedel</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Kõigi lehekülgede eksport PDF-failina</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Lehekülgede vahemiku eksport PDF-failina</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Sisesta komadega eradatud arvud või märgid: * tähistab kõiki lehekülgi, 1-5 lehekülgede vahemikku, konkreetsed leheküljenumbrid vastavaid lehekülgi.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">Määrab PDF-faili ühilduvuse. Vaikimisi on PDF 1.3, mis tagab suurima ühilduvuse. PDF 1.4 võimaldab kasutada läbipaistvust, 128-bitist krüptimist ja muid toredaid omadusi. PDF 1.5 on vajalik siis, kui soovid säilitada PDF-failis objekte erinevatel kihtidel. PDF/X-3 on mõeldud PDF-faili eksportimiseks juhul, kui soovid RGB värvihaldust trükikodades trükkimiseks. Seda saab valida juhul, kui värvihaldus on sisse lülitatud. Seda tasub kasutada ainult siis, kui see sobib sinu printeriga või kui näiteks trükid välja digitaalsel neljavärvi-laserprinteril.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Määrab lehekülgede sidumise külje PDF-is. Kui sa just ei pea seda kindlasti muutma, jäta kehtima vaikevalik (vasak).</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Genereerib PDF-failis iga lehekülje pisipildid. Mõned näitajad kasutavad pisipilte liikumise hõlbustamiseks.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Genereerib PDF-i artiklid, millest on abi lingitud artiklite vahel liikumisel PDF-is.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Dokumendi kihid eksporditakse PDF-faili ainult juhul, kui valitud on PDF 1.5.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Kaasab dokumendis loodud järjehoidjad. Neist on abi pikkades PDF-dokumentides liikumisel.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Teksti ja vektorkgraafika eksportimise lahutus. See ei mõjuta bittrasterpiltide, nt. fotode lahutust.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Lülitab sisse teksti ja graafika kadudeta tihendamise. See oleks mõttekas ära märkida, kui just pole teisiti nõutud. See vähendab PDF-faili suurust.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options. This only affects JPEG images</source>
        <translation>Piltide tihendamise viis. Automaatne laseb Scribusel endal valida parima viisi. ZIP on kadudeta ning sobib hästi ühtlaste värvidega piltidele. JPEG on parem väikeste, aga paljude fotodega PDF-failide loomisel (pildikvaliteedis on küll võimalikud kerged kaod). Mõttekas oleks jätta kehtima automaatne valik, kui sul pole just erinõudmisi. See puudutab ainult JPEG-pilte.</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation>Tihenduse kvaliteet kadudega tihendusviisi korral: minimaalne (25%), väike (50%), keskmine (75%), suur (85%), maksimaalne (95%). Pane tähele, et kvaliteet ei mõjuta otseselt pildi lõplikku suurust: nii suurus kui kaotus kvaliteedis sõltuvad iga taseme korral ikka pildist endast.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Bittrasterpiltide lahutuse muutmine määratud DPI-le. Märkimatajätmisel renderdatakse neid loomuliku lahutusega. Sisselülitamise korral läheb vaja rohkem mälu ning eksport muutub aeglasemaks.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (punkti tolli kohta) piltide eksportimiseks</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Turvavõimaluste sisselülitamine eksporditud PDF-failis. PDF 1.3 korral kaitstakse PDF-i 40-bitise krüptimisega. PDF 1.4 koral kaitstakse PDF-i 128-bitise krüptimisega. Lahtiütlus: PDF-i krüptimine ei ole nii usaldusväärne nagu GPG või PGP krüptimine ning sel on teatud piirangud.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Ülemparooli valimine, mis lubab või keelab kõik turvavõimalused eksporditud PDF-is</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Parooli valimine kasutajale PDF-i lugemiseks</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>PDF-i trükkimise lubamine. Märkimatajätmisel on trükkimine keelatud. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>PDF-i muutmise lubamine. Märkimatajätmisel on muutmine keelatud.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Teksti või graafika kopeerimise lubamine PDF-ist. Märkimatajätmisel ei saa teksti ega graafikat kopeerida.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Annotatsioonide ja väljade lisamise lubamine PDF-ile. Märkimatajätmisel on annotatsioonide ja väljade redigeerimine keelatud.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>PDF-i väljundi värvimudel. Ekraan / veeb sobib PDF-ile, mida kasutatakse ekraanil vaatamiseks ja trükkimiseks tavalistes tindiprinterites. Printer sobib trükkimiseks neljavärvi-CMYK-printeris. Halltoonid sobib halltoonides PDF-i loomiseks.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>See on täppisseadistus, mis ei ole vaikimisi sisse lülitatud. See tuleks sisse lülitada ainult siis, kui trükikoda seda konkreetselt nõuab ja sul on olemas kõik üksikasjad. Vastasel juhtul ei pruugi eksporditud PDF-i trükkimine korralikult välja tulla ning see ei pruugi olla päris porditav erinevates süsteemides.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Värviprofiili põimimine ühtlaste värvide tarbeks</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Värviprofiil ühtlaste värvide tarbeks</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Renderdamisviis ühtlaste värvide tarbeks</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Värviprofiili põimimine piltide tarbeks</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Keelab kasutada lähtepiltidesse põimitud värviprofiile</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Värviprofiil piltide tarbeks</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Renderdamisviis piltide tarbeks</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Väljundprofiil trükkimiseks. Võimaluse korral tuleks seda konsulteerida trükikojaga. </translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>PDF/X-3 kohustuslik string, mille puudumisel PDF/X-3 ei vasta nõuetele. Soovitatav on kasutada siin dokumendi pealkirja.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Lõikevaru füüsilise lehekülje ülaservas</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Lõikevaru füüsilise lehekülje allservas</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Lõikevaru füüsilise lehekülje vasakus servas</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Lõikevaru füüsilise lehekülje paremas servas</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Lehekülgede peegeldamine rõhtsalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Lehekülgede peegeldamine püstiselt</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Sisselülitamisel kehtestatakse dokumendile üldine ületrükirežiim, mis tühistab objektide määratlused</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Sisselülitamisel teisendatakse spot-värvid komposiitvärvideks. Kui sa ei kavatse just saata trükikotta spot-värve, on mõttekas see sisse lülitada.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Trükitava lehekülje veeristest väljapoole jäävaid objekte ei näidata</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.&lt;br/&gt;The default is &lt;b&gt;PDF 1.3&lt;/b&gt; which gives the widest compatibility.&lt;br/&gt;Choose &lt;b&gt;PDF 1.4&lt;/b&gt; if your file uses features such as transparency or you require 128 bit encryption.&lt;br/&gt;&lt;b&gt;PDF 1.5&lt;/b&gt; is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/&gt;&lt;b&gt;PDF/X-3&lt;/b&gt; is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabScrapbookBase</name>
    <message>
        <source>Form2</source>
        <translation type="unfinished"></translation>
    </message>
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
        <translation>Suurus:</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Teksti värv:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Varjund:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Teksti äärejoon:</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Täidise värv:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Äärejoone värv:</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Kriips</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Alakriips</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Tabeldusmärgi täitmise märk:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Tabeldusmärgi laius:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>V&amp;eergude arv:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Va&amp;he:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Põdur Zagrebi tšellomängija-följetonist Ciqo külmetas kehvas garaažis</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Joone värv:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Varjund:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Täidise värv:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Va&amp;rjund:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Joone stiil:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>J&amp;oone jämedus:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Joone &amp;stiil:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Nooled:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Algus:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Lõpp:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Vaba skaleerimine</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Rõ&amp;htsuunas skaleerimine:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Püstsuunas skaleerimine:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Pildi &amp;skaleerimine vastavalt kasti suurusele</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>P&amp;roportsiooni säilitamine</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Täidise värv:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Põimitud lõikejoone kasutamine</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Eelvaatlus ekraanil</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Täieliku kvaliteediga</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Normaalse kvaliteediga</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Madala kvaliteediga</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mi&amp;nimaalne:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ksimaalne:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Samm:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Tekstikasti omadused</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Pildikasti omadused</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Kujundi omadused</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Suurendustasemed</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Joone omadused</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Hulknurga omadused</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Uute tekstikastide font</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Uute tekstikastide fondi suurus</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Fondi värv</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Veergude arv tekstikastis</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Tekstikasti veergude vahe</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Fondi näide</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Pildikastid võimaldavad piltide skaleerimist iga väärtuse järgi</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Piltide skaleerimine rõhtsuunas</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Piltide skaleerimine püstsuunas</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Rõht- ja püstsuunas skaleerimine on võrdne</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Pildikasti pildid skaleeritakse kasti suuruse järgi</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automaatselt skaleeritud pildid säilitavad algsed proportsioonid</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Pildikastide täidise värv</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Täidise värvi küllastus</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Kujundite joone värv</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Joone värvi küllastus</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Kujundite täidise värv</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Kujundite joone stiil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Kujundite joone jämedus</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Lubatud minimaalne suurendus</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Lubatud maksimaalne suurendus</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Suurenduse muutmise samm iga suurendusoperatsiooniga</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Joonte värv</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Värviküllastus</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Joonte stiil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Joonte jämedus</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Kohandatud:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Kohandatud: </translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Alaindeks</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Nihe:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Skaleerimine:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Ülaindeks</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>N&amp;ihe:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>S&amp;kaleerimine:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Allajoonimine</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Nihe:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automaatne</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Joone jämedus:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Läbikriipsutamine</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapiteelkiri</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Sk&amp;aleerimine:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Auto&amp;maatne reavahe</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Reavahe:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Fondi nihe ülespoole rea alusjoone suhtes</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Ülaindeksi suurus normaalse fondi suhtes</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Fondi nihe allapoole rea alusjoone suhtes</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Alaindeksi suurus normaalse fondi suhtes</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Kapiteelkirja suurus normaalse fondi suhtes</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Reavahe suurendamise protsent sõltuvalt fondi suurusest</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Nihe allapoole rea alusjoone suhtes fondi alapikendi protsendina</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Joone jämedus protsendina fondi suuruse suhtes</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Nihe ülespoole rea alusjoone suhtes fondi ülapikendi protsendina</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Vasakul</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Paremal</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Koma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Keskel</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Asukoht:</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Puudub</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Kriips</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Alakriips</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Kohandatud</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Täitmise märk:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Kustuta kõik</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Lõigu esimese rea taandus</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Terve lõigu vasaktaandus</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Kustuta kõik tabulaatorid</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Kohandatud:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Kohandatud: </translation>
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
        <translation>Brauseri asukoht</translation>
    </message>
    <message>
        <source>External Web Browser Failed to Start</source>
        <translation>Välise brauseri käivitamine ebaõnnestus</translation>
    </message>
    <message>
        <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences</source>
        <translation>Scribusel ei õnnestunud käivitada välist brauserit %1. Palun kontrolli seadistustedialoogi määrangut.</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Free Objects</source>
        <translation>Vabad objektid</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grupp </translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Lehekülg </translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Struktuur</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation>Automaatne</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Nihe</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Joone jämedus</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Püstise juhtjoone lisamine</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Rõhtsa juhtjoone lisamine</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Püstise juhtjoone eemaldamine</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Rõhtsa juhtjoone eemaldamine</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Püstise juhtjoone liigutamine</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Rõhtsa juhtjoone liigutamine</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Juhtjoonte lukustamine</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Juhtjoonte lukustuse eemaldamine</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Liigutamine</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Suuruse muutmine</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Pööramine</translation>
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
        <translation>L1: %1, K1: %2
L2: %3, K2: %4</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Pildi nihke muutmine</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Pildi skaleerimise muutmine</translation>
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
        <source>Selection</source>
        <translation>Valik</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Rühmitamine</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Valik/rühmitamine</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Loomine</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
L: %3, K: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Joondamine/jaotamine</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Asjassepuutuvad elemendid</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Loobumine</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Täidise värvi määramine</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Värv1: %1, Värv2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Täidise värvi varjundi määramine</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Joone värvi määramine</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Joone värvi varjundi määramine</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Peegeldamine rõhtsalt</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Peegeldamine püstiselt</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Lukustamine</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Lukustuse eemaldamine</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Suuruse lukustamine</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Suuruse lukustuse eemaldamine</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Elemendi trükkimise lubamine</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Elemendi trükkimise keelamine</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Rühma lõhkumine</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Kustutamine</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Ümbernimetamine</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>%1
-&gt; %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Leheküljetooriku rakendamine</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Asetamine</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Lõikamine</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Täidise värvi läbipaistvuse määramine</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Joone värvi läbipaistvuse määramine</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Joone stiili määramine</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Jooneotsa stiili määramine</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Joonte ühenduse stiili määramine</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Joone jämeduse määramine</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Stiil puudub</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Kohandatud joone stiili määramine</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Kohandatud joonestiili mittekasutamine</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Algusnoole määramine</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Lõpunoole määramine</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Tabeli loomine</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Ridu: %1, Veerge: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Fondi määramine</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Fondi suuruse määramine</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Fondi laiuse määramine</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Fondi kõrguse määramine</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Fondi täidise värvi määramine</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Fondi äärejoone värvi määramine</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Fondi täidise värvi varjundi määramine</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Fondi äärejoone värvi varjundi määramine</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Tähemärkide koondamise määramine</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Reavahe määramine</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Lõigustiili määramine</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Keele määramine</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Teksti joondamine</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Fondefekti määramine</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Pildikast</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Tekstikast</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Hulknurk</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bezier&apos; kõver</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Kompleksjoon</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Trajektoori tekst</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Teisendamine</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>SVG-pildi import</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>EPS-pildi eksport</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>OpenOffice.org Draw&apos; pildi import</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Sodiala</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Tekstivoog ümber kasti</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Tekstivoog ümber ümbritseva kasti</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Tekstivoog ümber kontuurjoone</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Tekstivoog puudub</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Ümbritsev kast puudub</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Kontuurjoon puudub</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Lehekülg %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Pildi skaleerimise määramine</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Kasti suurus</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Vaba skaleerimine</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Proportsiooni säilitamine</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Proportsiooni mittesäilitamine</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Kontuurjoone redigeerimine</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Kuju redigeerimine</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Kontuurjoone lähtestamine</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Lehekülje lisamine</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Lehekülgede lisamine</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Lehekülje kustutamine</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Lehekülgede kustutamine</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Kihi lisamine</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Kihi kustutamine</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Kihi ümbernimetamine</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Kihi toomine ettepoole</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Kihi viimine tahapoole</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Saatmine kihile</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Kihi trükkimise lubamine</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Kihi trükkimise keelamine</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Kihi nime muutmine</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Pildi hankimine</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Mitmekordne kloonimine</translation>
    </message>
    <message>
        <source>Apply text style</source>
        <translation>Tekstistiili rakendamine</translation>
    </message>
    <message>
        <source>&amp;Undo: %1</source>
        <comment>f.e. Undo: Move</comment>
        <translation>&amp;Võta tagasi: %1</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Võta tagasi</translation>
    </message>
    <message>
        <source>&amp;Redo: %1</source>
        <comment>f.e. Redo: Move</comment>
        <translation>&amp;Tee uuesti: %1</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Tee uuesti</translation>
    </message>
    <message>
        <source>No object frame</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Esialgne olek</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Toimingute ajalugu</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Ainult valitud objekti näitamine</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Võta tagasi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Tee uuesti</translation>
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
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Leheküljesuuruse %1 minimaalsed veerised</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Printeri veeriste kasutamine</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>&amp;Printer:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Veerised</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Paremal:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>Ü&amp;lal:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;All:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vasakul:</translation>
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
        <translation>&amp;Loobu</translation>
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
        <translation>Väärtuse sisestamine</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Sisesta väärtus ja klõpsa OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Sisesta väärtus ja klõpsa OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Saada väärtus skriptile</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vali kasutatav importija</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automaatne</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Ainult teksti import</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Teksti import ilma vorminduseta</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importija:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodeering:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Kasutatava importija valik</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Seos jäetakse meelde</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Faili laiendi ja importija seos jäetakse meelde ning seda tüüpi failide puhul ei paluta enam importijat valida.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Uus mallist</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Eemalda</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Ava</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Kõik</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Lehekülje suurus</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Värvid</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Kirjeldus</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Kasutamine</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Loodud versiooniga</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Kuupäev</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Mallide allalaadimine</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Dokumendimalle leiab aadressil http://www.scribus.net/ sektsioonis Downloads.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Mallide paigaldamine</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Ekstrakti pakett mallikataloogi ~/.scribus/templates aktiivse kasutaja või PREFIX/share/scribus/templates kõigi süsteemi kasutajate jaoks.  </translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Malli ettevalmistamine</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Kontrolli, et kasutaud pilte ja fonte võib vabalt kasutada. Kui fonte ei ole lubatud jagada, ära neid malli salvestamisel kogu.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Malli looja peab ka kontrollima, et eespool mallide paigaldamises räägitu kehtib ka tema malli kohta. See tähendab, et kasutajal peab olema võimalus mallipakett alla laadida, pakkida see mallikataloogi lahti ja seejärel kasutama hakata.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Malli eemaldamine</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Malli eemaldamine dialoogis &apos;Uus mallist&apos; eemaldab ainult kirje failist template.xml, kuid ei kustuta dokumendifaili. Eemaldamist võimaldavat hüpikmenüüd näeb ainult siis, kui sul on faili template.xml kirjutamisõigus.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Faili template.xml tõlkimine</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopeeri olemasolev fail template.xml nimega template.KEEL.xml (kasuta sama keelekoodi, mis vastab sinu keele qm-failile), näiteks eesti keele puhul template.et.xml. Koopia peab asuma originaalse failiga template.xml samas kataloogis, et Scribus seda laadida saaks.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Salvestamine mallina</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nimi</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategooria</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Lehekülje suurus</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Värvid</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Kirjeldus</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Kasutamine</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-posti aadress</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Rohkem üksikasju</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Vähem üksikasju</translation>
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
        <translation>rõhtpaigutus</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>püstpaigutus</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>kohandatud</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Filtri loomine</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Puhasta</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>K&amp;ustuta</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Vali varem salvestatud filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Kirjuta siia nimi, millega filter salvestada</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Kirjuta siia salvestatav nimi</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Lülita see filtririda sisse või välja</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Eemalda see filtririda</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Lisa uus filtririda</translation>
    </message>
    <message>
        <source>to</source>
        <translation></translation>
    </message>
    <message>
        <source>and</source>
        <translation>ja</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>eemalda sobivus</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ära eemalda sobivust</translation>
    </message>
    <message>
        <source>words</source>
        <translation>sõnaga</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Eemalda</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Asenda</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Rakenda</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Vasakul paiknev väärtus on regulaaravaldis</translation>
    </message>
    <message>
        <source>with</source>
        <translation>järgnevaga</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>lõigustiil</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>kõikjalt</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>kõigile lõikudele</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>lõikudele, mille alguses on</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>lõikudele vähem kui</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>lõikudele rohkem kui</translation>
    </message>
</context>
</TS>
